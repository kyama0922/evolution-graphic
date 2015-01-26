#include "evolution_graphic_rasterrizer.h"
#include "evolution_graphic_command.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//-----------------------
//RasterrizerProperty
//-----------------------

//IUnknown
u32 RasterrizerStateProperty::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT RasterrizerStateProperty::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_RasterrizerStateProperty))
    {
        *ppvObject = static_cast<RasterrizerStateProperty *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 RasterrizerStateProperty::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

RasterrizerStateProperty::RasterrizerStateProperty() :mp_rasterizer_state(nullptr){

}

RasterrizerStateProperty::~RasterrizerStateProperty(){
    EVOLUTION_RELEASE(this->mp_rasterizer_state);
}

GraphicResult::_RESULT  RasterrizerStateProperty::Create(GraphicManager* graphic_manager, const D3D11_RASTERIZER_DESC& desc){
    HRESULT hr;
    hr = graphic_manager->GetID3D11Device()->CreateRasterizerState(&desc, &this->mp_rasterizer_state);
    if (FAILED(hr))
    {
        return   GraphicResult::CREATE_FAILED;
    }
   return  GraphicResult::RESULT_OK;
}

ID3D11RasterizerState*  RasterrizerStateProperty::GetRasterizerState()const{
    return this->mp_rasterizer_state;
}

//-----------------------
//Rasterrizer
//-----------------------

//IUnknown
u32 RasterrizerState::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT RasterrizerState::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IRasterrizerState))
    {
        *ppvObject = static_cast<IRasterrizerState *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_RasterrizerState))
    {
        *ppvObject = static_cast<RasterrizerState *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 RasterrizerState::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle RasterrizerState::GetGraphicHandle(){
    return this->m_handle;
}


RasterrizerState::RasterrizerState(GraphicManager* graphic_manager) :mp_property(nullptr), mp_graphic_manager(nullptr){

    graphic_manager->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicManager, (void**)&mp_graphic_manager);

    m_desc.FillMode = D3D11_FILL_SOLID;
    m_desc.CullMode = D3D11_CULL_BACK;
    m_desc.FrontCounterClockwise = false;
    m_desc.DepthBias = 0;
    m_desc.SlopeScaledDepthBias = 0.0f;
    m_desc.DepthBiasClamp = 0.0f;
    m_desc.DepthClipEnable = true;
    m_desc.ScissorEnable = false;
    m_desc.MultisampleEnable = false;
    m_desc.AntialiasedLineEnable = false;

    for (s32 i = 0; i < D3D11CONST_VALUE::RENDER_TARGET_COUNT; i++)
    {
        VIEW_PORT view_port = { 0 };
        this->SetViewPort(i, view_port);

        EVOLUTION::GRAPHIC::SCISSOR_RECT rect{ 0 };
        this->SetScissorRect(i, rect);

    }
}

RasterrizerState::~RasterrizerState(){
    EVOLUTION_RELEASE(this->mp_property);
    EVOLUTION_RELEASE(this->mp_graphic_manager);
}

GraphicResult::_RESULT RasterrizerState::Create(){
    EVOLUTION::CORE::Hash* hash = EVOLUTION::CORE::Hash::GetInstance();
    u32 crc32 = hash->CRC32((u8*)&this->m_desc, sizeof(this->m_desc));

    EVOLUTION::UTILITY::Collection& correction = this->mp_graphic_manager->GetCorrection();
    if (correction.Find(crc32))
    {
        IUnknown* unknown = correction.QueryItem(crc32);
        RESULT result = unknown->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_RasterrizerStateProperty, (void**)&this->mp_property);
        if (EVOLUTION::FUNCTION::IsFailed(result))
        {
            return GraphicResult::CREATE_FAILED;
        }
    }

    this->mp_property = NEW RasterrizerStateProperty();
    this->mp_property->Create(this->mp_graphic_manager, this->m_desc);
    correction.InsertItem(crc32, this->mp_property);

    return GraphicResult::RESULT_OK;
}


FILL_MODE::_MODE RasterrizerState::GetFillMode()const{
    FILL_MODE::_MODE mode;
    switch (this->m_desc.FillMode)
    {
    case D3D11_FILL_WIREFRAME:
        mode = FILL_MODE::WIREFRAME;
        break;
    case D3D11_FILL_SOLID:
        mode = FILL_MODE::SOLID;
        break;
    }
    return mode;
}

CULL_MODE::_MODE RasterrizerState::GetCullMode()const{
    CULL_MODE::_MODE mode;
    switch (m_desc.CullMode)
    {
    case D3D11_CULL_NONE:
        mode = CULL_MODE::NONE;
        break;
    case D3D11_CULL_FRONT:
        mode = CULL_MODE::FRONT;
        break;
    case D3D11_CULL_BACK:
        mode = CULL_MODE::BACK;
        break;
    }
    return mode;
}

bool RasterrizerState::IsFrontCounterClockwise()const{
    return this->m_desc.FrontCounterClockwise == TRUE;
}
s32 RasterrizerState::GetDepthBias()const{
    return this->m_desc.DepthBias;
}
f32 RasterrizerState::GetDepthBiasClamp()const{
    return this->m_desc.DepthBiasClamp;
}
f32 RasterrizerState::GetSlopeScaledDepthBias()const{
    return this->m_desc.SlopeScaledDepthBias;
}
bool RasterrizerState::IsDepthClip()const{
    return this->m_desc.DepthClipEnable == TRUE;
}
bool RasterrizerState::IsScissor()const{
    return this->m_desc.ScissorEnable == TRUE;
}
bool RasterrizerState::IsMultisample()const{
    return this->m_desc.MultisampleEnable == TRUE;
}
bool RasterrizerState::IsAntialiasedLine()const{
    return this->m_desc.AntialiasedLineEnable == TRUE;
}

VIEW_PORT*  RasterrizerState::GetViewPort(u32 slot_number, VIEW_PORT* view_port)const{
    view_port->Width = this->m_view_port[slot_number].Width;
    view_port->Height = this->m_view_port[slot_number].Height;
    view_port->MinDepth = this->m_view_port[slot_number].MinDepth;
    view_port->MaxDepth = this->m_view_port[slot_number].MaxDepth;
    view_port->TopLeftX = this->m_view_port[slot_number].TopLeftX;
    view_port->TopLeftY = this->m_view_port[slot_number].TopLeftY;
    return view_port;
}

SCISSOR_RECT*  RasterrizerState::GetScissorRect(u32 slot_number, SCISSOR_RECT* scissor_rect)const{
    scissor_rect->Bottom = this->m_scissor_rect[slot_number].bottom;
    scissor_rect->Left = this->m_scissor_rect[slot_number].left;
    scissor_rect->Right = this->m_scissor_rect[slot_number].right;
    scissor_rect->Top = this->m_scissor_rect[slot_number].top;
    return scissor_rect;
}

void RasterrizerState::SetProperty(FILL_MODE::_MODE type){
    switch (type)
    {
    case EVOLUTION::GRAPHIC::FILL_MODE::WIREFRAME:
        this->m_desc.FillMode = D3D11_FILL_WIREFRAME;
        break;
    case EVOLUTION::GRAPHIC::FILL_MODE::SOLID:
        this->m_desc.FillMode = D3D11_FILL_SOLID;
        break;
    }
}
void RasterrizerState::SetProperty(CULL_MODE::_MODE type){
    switch (type)
    {
    case EVOLUTION::GRAPHIC::CULL_MODE::NONE:
        this->m_desc.CullMode = D3D11_CULL_NONE;
        break;
    case EVOLUTION::GRAPHIC::CULL_MODE::FRONT:
        this->m_desc.CullMode = D3D11_CULL_FRONT;
        break;
    case EVOLUTION::GRAPHIC::CULL_MODE::BACK:
        this->m_desc.CullMode = D3D11_CULL_BACK;
        break;
    }
}

void RasterrizerState::SetProperty(s32 depth_bias){
    this->m_desc.DepthBias = depth_bias;
}

void RasterrizerState::SetProperty(RASTERRIZER_PROPERTY_TYPE::_TYPE1 type, f32 value){
    switch (type)
    {
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::DEPTHBIASCLAMP:
        this->m_desc.DepthBiasClamp = value;
        break;
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::SLOPESCALEDDEPTHBIAS:
        this->m_desc.SlopeScaledDepthBias = value;
        break;
    }
}
void RasterrizerState::SetProperty(RASTERRIZER_PROPERTY_TYPE::_TYPE2 type, bool flg){
    switch (type)
    {
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::FRONTCOUNTERCLOCKWISE:
        this->m_desc.FrontCounterClockwise = flg;
        break;
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::DEPTHCLIP:
        this->m_desc.DepthClipEnable = flg;
        break;
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::SCISSOR:
        this->m_desc.ScissorEnable = flg;
        break;
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::MULTISAMPLE:
        this->m_desc.MultisampleEnable = flg;
        break;
    case EVOLUTION::GRAPHIC::RASTERRIZER_PROPERTY_TYPE::ANTIALIASEDLINE:
        this->m_desc.AntialiasedLineEnable = flg;
        break;
    }
}

void RasterrizerState::SetViewPort(u32 slot_number, const VIEW_PORT& view_port){
    this->m_view_port[slot_number].Width = view_port.Width;
    this->m_view_port[slot_number].Height = view_port.Height;
    this->m_view_port[slot_number].MinDepth = view_port.MinDepth;
    this->m_view_port[slot_number].MaxDepth = view_port.MaxDepth;
    this->m_view_port[slot_number].TopLeftX = view_port.TopLeftX;
    this->m_view_port[slot_number].TopLeftY = view_port.TopLeftY;
}

void RasterrizerState::SetScissorRect(u32 slot_number,const SCISSOR_RECT& scissor_rect){
    this->m_scissor_rect[slot_number].bottom = scissor_rect.Bottom;
    this->m_scissor_rect[slot_number].left = scissor_rect.Left;
    this->m_scissor_rect[slot_number].right = scissor_rect.Right;
    this->m_scissor_rect[slot_number].top = scissor_rect.Top;
}
//設定したPropertyの反映
GraphicResult::_RESULT RasterrizerState::ChangeProperty(){
    EVOLUTION_RELEASE(this->mp_property);

    EVOLUTION::CORE::Hash* hash = EVOLUTION::CORE::Hash::GetInstance();
    u32 crc32 = hash->CRC32((u8*)&this->m_desc, sizeof(this->m_desc));

    EVOLUTION::UTILITY::Collection& correction = this->mp_graphic_manager->GetCorrection();
    if (correction.Find(crc32))
    {
        IUnknown* unknown = correction.QueryItem(crc32);
        RESULT result = unknown->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_RasterrizerStateProperty, (void**)&this->mp_property);
        if (EVOLUTION::FUNCTION::IsFailed(result))
        {
            return GraphicResult::CREATE_FAILED;
        }
    }
    else
    {
        this->mp_property = NEW RasterrizerStateProperty();
        this->mp_property->Create(this->mp_graphic_manager, this->m_desc);
        correction.InsertItem(crc32, this->mp_property);
    }

    return GraphicResult::RESULT_OK;
}

D3D11_VIEWPORT* RasterrizerState::GetViewPort(){
    return this->m_view_port;
}

D3D11_RECT*  RasterrizerState::GetScissorRect(){
    return this->m_scissor_rect;
}

RasterrizerStateProperty* RasterrizerState::GetRasterrizerStateProperty()const{
    return  this->mp_property;
}