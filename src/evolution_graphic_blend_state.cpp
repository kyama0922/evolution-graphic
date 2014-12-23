#include "evolution_graphic_blend_state.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//-----------------------
//RasterrizerProperty
//-----------------------
//IUnknown
u32 BlendStateProperty::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT BlendStateProperty::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_BlendStateProperty))
    {
        *ppvObject = static_cast<BlendStateProperty *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 BlendStateProperty::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

BlendStateProperty::BlendStateProperty() :mp_blend_state(nullptr){


}

BlendStateProperty::~BlendStateProperty(){
    EVOLUTION_RELEASE(this->mp_blend_state);
}

GraphicResult::_RESULT BlendStateProperty::Create(GraphicManager* graphic_manager, const D3D11_BLEND_DESC& desc){
    HRESULT hr;
    hr = graphic_manager->GetID3D11Device()->CreateBlendState(&desc, &this->mp_blend_state);
    if (FAILED(hr))
    {
        return GraphicResult::CREATE_FAILED;
    }
    return GraphicResult::RESULT_OK;
}

ID3D11BlendState* BlendStateProperty::GetBlendState()const{
    return this->mp_blend_state;
}


//-----------------------
//BlendState
//-----------------------
//IUnknown
u32 BlendState::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT BlendState::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IBlendState))
    {
        *ppvObject = static_cast<IBlendState *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_BlendState))
    {
        *ppvObject = static_cast<BlendState *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 BlendState::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle BlendState::GetGraphicHandle(){
    return this->m_handle;
}


BlendState::BlendState(GraphicManager* graphic_manager) :mp_property(nullptr), mp_graphic_manager(nullptr){
    graphic_manager->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicManager, (void**)&mp_graphic_manager);

    m_desc.AlphaToCoverageEnable = false;
    m_desc.IndependentBlendEnable = false;
    m_desc.RenderTarget[0].BlendEnable = false;
    m_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    m_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    m_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    m_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    m_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    m_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    m_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

BlendState::~BlendState(){
    EVOLUTION_RELEASE(this->mp_graphic_manager);
    EVOLUTION_RELEASE(this->mp_property);
}

GraphicResult::_RESULT BlendState::Create()
{
    EVOLUTION::CORE::Hash* hash = EVOLUTION::CORE::Hash::GetInstance();
    u32 crc32 = hash->CRC32((u8*)&this->m_desc, sizeof(this->m_desc));

    EVOLUTION::UTILITY::Collection& correction = this->mp_graphic_manager->GetCorrection();
    if (correction.Find(crc32))
    {
        IUnknown* unknown = correction.QueryItem(crc32);
        RESULT result = unknown->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_BlendStateProperty, (void**)&this->mp_property);
        if (EVOLUTION::FUNCTION::IsFailed(result))
        {
            return GraphicResult::CREATE_FAILED;
        }
    }

    this->mp_property = NEW BlendStateProperty();
    this->mp_property->Create(this->mp_graphic_manager, this->m_desc);
    correction.InsertItem(crc32, this->mp_property);

    return GraphicResult::RESULT_OK;
}

BLEND::_TYPE BlendState::GetSrcBlend(u8 number)const{
    return this->GetType(this->m_desc.RenderTarget[number].SrcBlend);
}

BLEND::_TYPE BlendState::GetDestBlend(u8 number)const{
    return this->GetType(this->m_desc.RenderTarget[number].DestBlend);
}

BLEND_OP::_OP BlendState::GetBlendOp(u8 number)const{
    return this->GetType(this->m_desc.RenderTarget[number].BlendOp);
}

BLEND::_TYPE BlendState::GetSrcBlendAlpha(u8 number)const{
    return this->GetType(this->m_desc.RenderTarget[number].SrcBlendAlpha);
}

BLEND::_TYPE BlendState::GetDestBlendAlpha(u8 number)const{
    return this->GetType(this->m_desc.RenderTarget[number].DestBlendAlpha);
}

BLEND_OP::_OP BlendState::GetBlendOpAlpha(u8 number)const{
    return this->GetType(this->m_desc.RenderTarget[number].BlendOpAlpha);
}

COLOR_WRITE_ENABLE::_TYPE BlendState::GetRenderTargetWriteMask(u8 number)const{
    return (COLOR_WRITE_ENABLE::_TYPE)this->m_desc.RenderTarget[number].RenderTargetWriteMask;
}

u32 BlendState::GetMask()const{
    return this->m_mask;
}
const f32* BlendState::GetBlendFactor(f32(&blend_factor)[4])const{
    blend_factor[0] = this->m_blend_factor[0];
    blend_factor[1] = this->m_blend_factor[1];
    blend_factor[2] = this->m_blend_factor[2];
    blend_factor[3] = this->m_blend_factor[3];
    return blend_factor;
}

void BlendState::SetProperty(BLEND_STATE_PROPERTY::_TYPE type, bool blend){
    switch (type)
    {
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::ALPHATOCOVERAGE:
        this->m_desc.AlphaToCoverageEnable = blend;
        break;
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::INDEPENDENTBLEND:
        this->m_desc.IndependentBlendEnable = blend;
        break;
    }
}

void BlendState::SetProperty(BLEND_STATE_PROPERTY::_OP operation, u8 number, BLEND_OP::_OP op){
    D3D11_BLEND_OP d3d11_blend_op;
    switch (op)
    {
    case EVOLUTION::GRAPHIC::BLEND_OP::ADD:
        d3d11_blend_op = D3D11_BLEND_OP_ADD;
        break;
    case EVOLUTION::GRAPHIC::BLEND_OP::SUBTRACT:
        d3d11_blend_op = D3D11_BLEND_OP_SUBTRACT;
        break;
    case EVOLUTION::GRAPHIC::BLEND_OP::REV_SUBTRACT:
        d3d11_blend_op = D3D11_BLEND_OP_REV_SUBTRACT;
        break;
    case EVOLUTION::GRAPHIC::BLEND_OP::MIN:
        d3d11_blend_op = D3D11_BLEND_OP_MIN;
        break;
    case EVOLUTION::GRAPHIC::BLEND_OP::MAX:
        d3d11_blend_op = D3D11_BLEND_OP_MAX;
        break;
    }

    switch (operation)
    {
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::BLEND_OP:
        this->m_desc.RenderTarget[number].BlendOp = d3d11_blend_op;
        break;
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::BLEND_OP_ALPHA:
        this->m_desc.RenderTarget[number].BlendOpAlpha = d3d11_blend_op;
        break;
    }
}

void BlendState::SetProperty(BLEND_STATE_PROPERTY::_OBJECT object, u8 number, BLEND::_TYPE type){

    D3D11_BLEND d3d11_blend_type;
    switch (type)
    {
    case EVOLUTION::GRAPHIC::BLEND::ZERO:
        d3d11_blend_type = D3D11_BLEND_ZERO;
        break;
    case EVOLUTION::GRAPHIC::BLEND::ONE:
        d3d11_blend_type = D3D11_BLEND_ONE;
        break;
    case EVOLUTION::GRAPHIC::BLEND::SRC_COLOR:
        d3d11_blend_type = D3D11_BLEND_SRC_COLOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_SRC_COLOR:
        d3d11_blend_type = D3D11_BLEND_INV_SRC_COLOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::SRC_ALPHA:
        d3d11_blend_type = D3D11_BLEND_SRC_ALPHA;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_SRC_ALPHA:
        d3d11_blend_type = D3D11_BLEND_INV_SRC_ALPHA;
        break;
    case EVOLUTION::GRAPHIC::BLEND::DEST_ALPHA:
        d3d11_blend_type = D3D11_BLEND_DEST_ALPHA;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_DEST_ALPHA:
        d3d11_blend_type = D3D11_BLEND_INV_DEST_ALPHA;
        break;
    case EVOLUTION::GRAPHIC::BLEND::DEST_COLOR:
        d3d11_blend_type = D3D11_BLEND_DEST_COLOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_DEST_COLOR:
        d3d11_blend_type = D3D11_BLEND_INV_DEST_COLOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::SRC_ALPHA_SAT:
        d3d11_blend_type = D3D11_BLEND_SRC_ALPHA_SAT;
        break;
    case EVOLUTION::GRAPHIC::BLEND::BLEND_FACTOR:
        d3d11_blend_type = D3D11_BLEND_BLEND_FACTOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_BLEND_FACTOR:
        d3d11_blend_type = D3D11_BLEND_INV_BLEND_FACTOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::SRC1_COLOR:
        d3d11_blend_type = D3D11_BLEND_SRC1_COLOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_SRC1_COLOR:
        d3d11_blend_type = D3D11_BLEND_INV_SRC1_COLOR;
        break;
    case EVOLUTION::GRAPHIC::BLEND::SRC1_ALPHA:
        d3d11_blend_type = D3D11_BLEND_SRC1_ALPHA;
        break;
    case EVOLUTION::GRAPHIC::BLEND::INV_SRC1_ALPHA:
        d3d11_blend_type = D3D11_BLEND_INV_SRC1_ALPHA;
        break;
    }

    switch (object)
    {
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::SRC:
        this->m_desc.RenderTarget[number].SrcBlend = d3d11_blend_type;
        break;
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::DEST:
        this->m_desc.RenderTarget[number].DestBlend = d3d11_blend_type;
        break;
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::SRCALPHA:
        this->m_desc.RenderTarget[number].SrcBlendAlpha = d3d11_blend_type;
        break;
    case EVOLUTION::GRAPHIC::BLEND_STATE_PROPERTY::DESTALPHA:
        this->m_desc.RenderTarget[number].DestBlendAlpha = d3d11_blend_type;
        break;
    }
}

void BlendState::SetProperty(u8 number, COLOR_WRITE_ENABLE::_TYPE type){
    this->m_desc.RenderTarget[number].RenderTargetWriteMask = type;
}

void BlendState::SetProperty(u8 number, bool blend_enable){
    this->m_desc.RenderTarget[number].BlendEnable = blend_enable;
}

void BlendState::SetMask(u32 mask){
    this->m_mask = mask;
}

void BlendState::SetBlendFactor(const f32(&blend_factor)[4]){
    this->m_blend_factor[0] = blend_factor[0];
    this->m_blend_factor[1] = blend_factor[1];
    this->m_blend_factor[2] = blend_factor[2];
    this->m_blend_factor[3] = blend_factor[3];
}

GraphicResult::_RESULT BlendState::ChangeProperty(){
    EVOLUTION_RELEASE(this->mp_property);

    EVOLUTION::CORE::Hash* hash = EVOLUTION::CORE::Hash::GetInstance();
    u32 crc32 = hash->CRC32((u8*)&this->m_desc, sizeof(this->m_desc));

    EVOLUTION::UTILITY::Collection& correction = this->mp_graphic_manager->GetCorrection();
    if (correction.Find(crc32))
    {
        IUnknown* unknown = correction.QueryItem(crc32);
        RESULT result = unknown->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_BlendStateProperty, (void**)&this->mp_property);
        if (EVOLUTION::FUNCTION::IsFailed(result))
        {
            return GraphicResult::CREATE_FAILED;
        }
        return GraphicResult::RESULT_OK;
    }

    this->mp_property = NEW BlendStateProperty();
    this->mp_property->Create(this->mp_graphic_manager, this->m_desc);
    correction.InsertItem(crc32, this->mp_property);

    return GraphicResult::RESULT_OK;
}

BlendStateProperty* BlendState::GetBlendStateProperty()const{
    return this->mp_property;
}


BLEND_OP::_OP BlendState::GetType(D3D11_BLEND_OP in_op)const{
    BLEND_OP::_OP op;
    switch (in_op)
    {
    case D3D11_BLEND_OP_ADD:
        op = BLEND_OP::ADD;
        break;
    case D3D11_BLEND_OP_SUBTRACT:
        op = BLEND_OP::SUBTRACT;
        break;
    case D3D11_BLEND_OP_REV_SUBTRACT:
        op = BLEND_OP::REV_SUBTRACT;
        break;
    case D3D11_BLEND_OP_MIN:
        op = BLEND_OP::MIN;
        break;
    case D3D11_BLEND_OP_MAX:
        op = BLEND_OP::MAX;
        break;
    }
    return op;
}

BLEND::_TYPE BlendState::GetType(D3D11_BLEND in_type)const{
    BLEND::_TYPE type;
    switch (in_type)
    {
    case D3D11_BLEND_ZERO:
        type = BLEND::ZERO;
        break;
    case D3D11_BLEND_ONE:
        type = BLEND::ONE;
        break;
    case D3D11_BLEND_SRC_COLOR:
        type = BLEND::SRC_COLOR;
        break;
    case D3D11_BLEND_INV_SRC_COLOR:
        type = BLEND::INV_SRC_COLOR;
        break;
    case D3D11_BLEND_SRC_ALPHA:
        type = BLEND::SRC_ALPHA;
        break;
    case D3D11_BLEND_INV_SRC_ALPHA:
        type = BLEND::INV_SRC_ALPHA;
        break;
    case D3D11_BLEND_DEST_ALPHA:
        type = BLEND::DEST_ALPHA;
        break;
    case D3D11_BLEND_INV_DEST_ALPHA:
        type = BLEND::INV_DEST_ALPHA;
        break;
    case D3D11_BLEND_DEST_COLOR:
        type = BLEND::DEST_COLOR;
        break;
    case D3D11_BLEND_INV_DEST_COLOR:
        type = BLEND::INV_DEST_COLOR;
        break;
    case D3D11_BLEND_SRC_ALPHA_SAT:
        type = BLEND::SRC_ALPHA_SAT;
        break;
    case D3D11_BLEND_BLEND_FACTOR:
        type = BLEND::BLEND_FACTOR;
        break;
    case D3D11_BLEND_INV_BLEND_FACTOR:
        type = BLEND::INV_BLEND_FACTOR;
        break;
    case D3D11_BLEND_SRC1_COLOR:
        type = BLEND::SRC1_COLOR;
        break;
    case D3D11_BLEND_INV_SRC1_COLOR:
        type = BLEND::INV_SRC1_COLOR;
        break;
    case D3D11_BLEND_SRC1_ALPHA:
        type = BLEND::SRC1_ALPHA;
        break;
    case D3D11_BLEND_INV_SRC1_ALPHA:
        type = BLEND::INV_SRC1_ALPHA;
        break;
    }
    return type;
}