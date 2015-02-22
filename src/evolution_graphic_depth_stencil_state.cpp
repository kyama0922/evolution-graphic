#include "evolution_graphic_depth_stencil_state.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//-----------------------
//DepthStencilProperty
//-----------------------
//IUnknown
u32 DepthStencilProperty::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT DepthStencilProperty::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_DepthStencilProperty))
    {
        *ppvObject = static_cast<DepthStencilProperty *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 DepthStencilProperty::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

DepthStencilProperty::DepthStencilProperty() :mp_rendertarget_view(nullptr) {

}

DepthStencilProperty::~DepthStencilProperty(){
    EVOLUTION_RELEASE(this->mp_rendertarget_view);
}

GraphicResult::_RESULT DepthStencilProperty::Create(GraphicManager* graphic_manager, const D3D11_DEPTH_STENCIL_DESC& desc){

    HRESULT hr;
    hr = graphic_manager->GetID3D11Device()->CreateDepthStencilState(&desc, &this->mp_rendertarget_view);
    if (FAILED(hr))
    {
        return GraphicResult::CREATE_FAILED;
    }
    return GraphicResult::RESULT_OK;
}

ID3D11DepthStencilState* DepthStencilProperty::GetRasterizerState()const{
    return this->mp_rendertarget_view;
}


//-----------------------
//DepthStencilState
//-----------------------
//IUnknown
u32 DepthStencilState::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT DepthStencilState::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IDepthStencilState))
    {
        *ppvObject = static_cast<IDepthStencilState *>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_DepthStencilState))
    {
        *ppvObject = static_cast<DepthStencilState *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 DepthStencilState::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle DepthStencilState::GetGraphicHandle(){
    return this->m_handle;
}


DepthStencilState::DepthStencilState(GraphicManager* graphic_manager) : mp_graphic_manager(nullptr), m_stencil_ref(0){
    graphic_manager->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicManager, (void**)&mp_graphic_manager);

    m_desc.DepthEnable = true;
    m_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_desc.DepthFunc = D3D11_COMPARISON_LESS;
    m_desc.StencilEnable = false;
    m_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    m_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    m_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    m_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    m_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    m_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    m_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    m_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

}
DepthStencilState::~DepthStencilState(){
    EVOLUTION_RELEASE(this->mp_graphic_manager);
    EVOLUTION_RELEASE(this->mp_property);
}

GraphicResult::_RESULT DepthStencilState::Create(){
    EVOLUTION::CORE::Hash* hash = EVOLUTION::CORE::Hash::GetInstance();
    u32 crc32 = hash->CRC32((u8*)&this->m_desc, sizeof(this->m_desc));

    EVOLUTION::UTILITY::Collection& correction = this->mp_graphic_manager->GetCorrection();
    if (correction.Find(crc32))
    {
        IUnknown* unknown = correction.QueryItem(crc32);
        RESULT result = unknown->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IDepthBuffer, (void**)&this->mp_property);
        if (EVOLUTION::FUNCTION::IsFailed(result))
        {
            return GraphicResult::CREATE_FAILED;
        }
    }

    this->mp_property = NEW DepthStencilProperty();
    this->mp_property->Create(this->mp_graphic_manager, this->m_desc);
    correction.InsertItem(crc32, this->mp_property);

    return GraphicResult::RESULT_OK;
}

bool DepthStencilState::IsDepth()const{
    return this->m_desc.DepthEnable == TRUE;
}

DEPTH_WRITE_MASK::_MASK DepthStencilState::GetDepthWriteMask()const{
    DEPTH_WRITE_MASK::_MASK mask;
    switch (this->m_desc.DepthWriteMask)
    {
    case D3D11_DEPTH_WRITE_MASK_ZERO:
        mask = DEPTH_WRITE_MASK::ZERO;
        break;
    case D3D11_DEPTH_WRITE_MASK_ALL:
        mask = DEPTH_WRITE_MASK::ALL;
        break;

    }
    return  mask;
}

COMPARISON_FUNC::_FUNC DepthStencilState::GetDepthFunc()const{
    return  this->GetType(this->m_desc.DepthFunc);
}

bool DepthStencilState::IsStencil()const{
    return this->m_desc.StencilEnable == TRUE;
}

u8 DepthStencilState::GetStencilReadMask()const{
    return this->m_desc.StencilReadMask;
}

u8 DepthStencilState::GetStencilWriteMask()const{
    return this->m_desc.StencilWriteMask;
}

DEPTH_STENCILOP_DESC DepthStencilState::GetFrontFace()const{
    DEPTH_STENCILOP_DESC out_desc;
    out_desc.StencilDepthFailOp = this->GetType(this->m_desc.FrontFace.StencilDepthFailOp);
    out_desc.StencilFailOp = this->GetType(this->m_desc.FrontFace.StencilFailOp);
    out_desc.StencilFunc = this->GetType(this->m_desc.FrontFace.StencilFunc);
    out_desc.StencilPassOp = this->GetType(this->m_desc.FrontFace.StencilPassOp);
    return out_desc;
}

DEPTH_STENCILOP_DESC DepthStencilState::GetBackFace()const{
    DEPTH_STENCILOP_DESC out_desc;
    out_desc.StencilDepthFailOp = this->GetType(this->m_desc.BackFace.StencilDepthFailOp);
    out_desc.StencilFailOp = this->GetType(this->m_desc.BackFace.StencilFailOp);
    out_desc.StencilFunc = this->GetType(this->m_desc.BackFace.StencilFunc);
    out_desc.StencilPassOp = this->GetType(this->m_desc.BackFace.StencilPassOp);
    return out_desc;
}

u32 DepthStencilState::GetStencilRef()const{
    return this->m_stencil_ref;
}

void DepthStencilState::SetProperty(DEPTHSTENCILSTATE_PROPERTY::_TYPE type, bool _property){
    switch (type)
    {
    case EVOLUTION::GRAPHIC::DEPTHSTENCILSTATE_PROPERTY::DEPTH:
        this->m_desc.DepthEnable = _property;
        break;
    case EVOLUTION::GRAPHIC::DEPTHSTENCILSTATE_PROPERTY::STENCIL:
        this->m_desc.StencilEnable = _property;
        break;
    }
}

void DepthStencilState::SetProperty(COMPARISON_FUNC::_FUNC func){

    switch (func)
    {
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::NEVER:
        this->m_desc.DepthFunc = D3D11_COMPARISON_NEVER;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::LESS:
        this->m_desc.DepthFunc = D3D11_COMPARISON_LESS;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::EQUAL:
        this->m_desc.DepthFunc = D3D11_COMPARISON_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::LESS_EQUAL:
        this->m_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::GREATER:
        this->m_desc.DepthFunc = D3D11_COMPARISON_GREATER;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::NOT_EQUAL:
        this->m_desc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::GREATER_EQUAL:
        this->m_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::ALWAYS:
        this->m_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
        break;
    }
}

void DepthStencilState::SetProperty(DEPTH_WRITE_MASK::_MASK mask){
    switch (mask)
    {
    case EVOLUTION::GRAPHIC::DEPTH_WRITE_MASK::ZERO:
        this->m_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        break;
    case EVOLUTION::GRAPHIC::DEPTH_WRITE_MASK::ALL:
        this->m_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        break;
    }
}

void DepthStencilState::SetProperty(DEPTHSTENCILSTATE_PROPERTY::_MASK mask, u8 value){
    switch (mask)
    {
    case EVOLUTION::GRAPHIC::DEPTHSTENCILSTATE_PROPERTY::READ:
        this->m_desc.StencilReadMask = value;
        break;
    case EVOLUTION::GRAPHIC::DEPTHSTENCILSTATE_PROPERTY::WRITE:
        this->m_desc.StencilWriteMask = value;
        break;
    }
}

void DepthStencilState::SetProperty(DEPTHSTENCILSTATE_PROPERTY::_FORCE force, DEPTH_STENCILOP_DESC desc){
    D3D11_COMPARISON_FUNC func;
    switch (desc.StencilFunc)
    {
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::NEVER:
        func = D3D11_COMPARISON_NEVER;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::LESS:
        func = D3D11_COMPARISON_LESS;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::EQUAL:
        func = D3D11_COMPARISON_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::LESS_EQUAL:
        func = D3D11_COMPARISON_LESS_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::GREATER:
        func = D3D11_COMPARISON_GREATER;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::NOT_EQUAL:
        func = D3D11_COMPARISON_NOT_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::GREATER_EQUAL:
        func = D3D11_COMPARISON_GREATER_EQUAL;
        break;
    case EVOLUTION::GRAPHIC::COMPARISON_FUNC::ALWAYS:
        func = D3D11_COMPARISON_ALWAYS;
        break;
    }

    switch (force)
    {
    case EVOLUTION::GRAPHIC::DEPTHSTENCILSTATE_PROPERTY::Front:
        this->m_desc.FrontFace.StencilDepthFailOp = this->GetType(desc.StencilDepthFailOp);
        this->m_desc.FrontFace.StencilFailOp = this->GetType(desc.StencilFailOp);
        this->m_desc.FrontFace.StencilFunc = func;
        this->m_desc.FrontFace.StencilPassOp = this->GetType(desc.StencilPassOp);
        break;
    case EVOLUTION::GRAPHIC::DEPTHSTENCILSTATE_PROPERTY::Back:
        this->m_desc.BackFace.StencilDepthFailOp = this->GetType(desc.StencilDepthFailOp);
        this->m_desc.BackFace.StencilFailOp = this->GetType(desc.StencilFailOp);
        this->m_desc.BackFace.StencilFunc = func;
        this->m_desc.BackFace.StencilPassOp = this->GetType(desc.StencilPassOp);
        break;
    }
}

void DepthStencilState::SetStencilRef(u32 stencil_ref){
    this->m_stencil_ref = stencil_ref;
}


GraphicResult::_RESULT DepthStencilState::ChangeProperty(){
    EVOLUTION_RELEASE(this->mp_property);

    EVOLUTION::CORE::Hash* hash = EVOLUTION::CORE::Hash::GetInstance();
    u32 crc32 = hash->CRC32((u8*)&this->m_desc, sizeof(this->m_desc));

    EVOLUTION::UTILITY::Collection& correction = this->mp_graphic_manager->GetCorrection();
    if (correction.Find(crc32))
    {
        IUnknown* unknown = correction.QueryItem(crc32);
        RESULT result = unknown->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_DepthStencilProperty, (void**)&this->mp_property);
        if (EVOLUTION::FUNCTION::IsFailed(result))
        {
            return GraphicResult::CREATE_FAILED;
        }
        return GraphicResult::RESULT_OK;
    }

    this->mp_property = NEW DepthStencilProperty();
    this->mp_property->Create(this->mp_graphic_manager, this->m_desc);
    correction.InsertItem(crc32, this->mp_property);

    return GraphicResult::RESULT_OK;
}

DepthStencilProperty* DepthStencilState::GetDepthStencilProperty()const{
    return this->mp_property;
}

STENCIL_OP::_OP DepthStencilState::GetType(D3D11_STENCIL_OP in_type)const{
    STENCIL_OP::_OP out_op;
    switch (in_type)
    {
    case D3D11_STENCIL_OP_KEEP:
        out_op = STENCIL_OP::KEEP;
        break;
    case D3D11_STENCIL_OP_ZERO:
        out_op = STENCIL_OP::ZERO;
        break;
    case D3D11_STENCIL_OP_REPLACE:
        out_op = STENCIL_OP::REPLACE;
        break;
    case D3D11_STENCIL_OP_INCR_SAT:
        out_op = STENCIL_OP::INCR_SAT;
        break;
    case D3D11_STENCIL_OP_DECR_SAT:
        out_op = STENCIL_OP::DECR_SAT;
        break;
    case D3D11_STENCIL_OP_INVERT:
        out_op = STENCIL_OP::INVERT;
        break;
    case D3D11_STENCIL_OP_INCR:
        out_op = STENCIL_OP::INCR;
        break;
    case D3D11_STENCIL_OP_DECR:
        out_op = STENCIL_OP::DECR;
        break;
    }
    return out_op;
}

COMPARISON_FUNC::_FUNC DepthStencilState::GetType(D3D11_COMPARISON_FUNC in_func)const{
    COMPARISON_FUNC::_FUNC out_func;
    switch (in_func)
    {
    case D3D11_COMPARISON_NEVER:
        out_func = COMPARISON_FUNC::NEVER;
        break;
    case D3D11_COMPARISON_LESS:
        out_func = COMPARISON_FUNC::LESS;
        break;
    case D3D11_COMPARISON_EQUAL:
        out_func = COMPARISON_FUNC::EQUAL;
        break;
    case D3D11_COMPARISON_LESS_EQUAL:
        out_func = COMPARISON_FUNC::LESS_EQUAL;
        break;
    case D3D11_COMPARISON_GREATER:
        out_func = COMPARISON_FUNC::GREATER;
        break;
    case D3D11_COMPARISON_NOT_EQUAL:
        out_func = COMPARISON_FUNC::NOT_EQUAL;
        break;
    case D3D11_COMPARISON_GREATER_EQUAL:
        out_func = COMPARISON_FUNC::GREATER_EQUAL;
        break;
    case D3D11_COMPARISON_ALWAYS:
        out_func = COMPARISON_FUNC::ALWAYS;
        break;
    }
    return out_func;
}

D3D11_STENCIL_OP DepthStencilState::GetType(STENCIL_OP::_OP in_op)const{
    D3D11_STENCIL_OP out_op;
    switch (in_op)
    {
    case EVOLUTION::GRAPHIC::STENCIL_OP::KEEP:
        out_op = D3D11_STENCIL_OP_KEEP;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::ZERO:
        out_op = D3D11_STENCIL_OP_ZERO;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::REPLACE:
        out_op = D3D11_STENCIL_OP_REPLACE;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::INCR_SAT:
        out_op = D3D11_STENCIL_OP_INCR_SAT;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::DECR_SAT:
        out_op = D3D11_STENCIL_OP_DECR_SAT;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::INVERT:
        out_op = D3D11_STENCIL_OP_INVERT;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::INCR:
        out_op = D3D11_STENCIL_OP_INCR;
        break;
    case EVOLUTION::GRAPHIC::STENCIL_OP::DECR:
        out_op = D3D11_STENCIL_OP_DECR;
        break;
    }

    return out_op;
}