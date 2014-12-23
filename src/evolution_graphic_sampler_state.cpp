#include "evolution_graphic_sampler_state.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//サンプラーステートのDefault設定関数
void EVOLUTION::FUNCTION::SetDefaultSamplerState(GRAPHIC::SAMPLER_DESC& desc){
    desc.Filter = FILTER::MIN_MAG_MIP_LINEAR;
    desc.AddressU = TEXTURE_ADDRESS::CLAMP;
    desc.AddressV = TEXTURE_ADDRESS::CLAMP;
    desc.AddressW = TEXTURE_ADDRESS::CLAMP;
    desc.MinLOD = -3.402823466e+38F;
    desc.MaxLOD = 3.402823466e+38F;
    desc.MipLODBias= 0.0f;
    desc.MaxAnisotropy= 1;
    desc.ComparisonFunc = COMPARISON_FUNC::NEVER;
    desc.BorderColor[0] = 1.0f;
    desc.BorderColor[1] = 1.0f;
    desc.BorderColor[2] = 1.0f;
    desc.BorderColor[3] = 1.0f;
}

u32 SamplerState::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT SamplerState::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_ISamplerState))
    {
        *ppvObject = static_cast<ISamplerState *>(this);
        this->AddRef();
    }
    else if(IsEqualGUID(riid, EVOLUTION_GUID::IID_SamplerState))
    {
        *ppvObject = static_cast<SamplerState *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 SamplerState::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

GraphicHandle SamplerState::GetGraphicHandle(){
    return this->m_handle;
}

SamplerState::SamplerState() :mp_sampler_state(nullptr){

}
SamplerState::~SamplerState(){
    EVOLUTION_RELEASE(this->mp_sampler_state);
}

GraphicException::_EXCEPTION SamplerState::Create(GraphicManager* graphic_manager , const SAMPLER_DESC& desc){
    this->m_desc = desc;
    D3D11_SAMPLER_DESC _11desc;
    _11desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)desc.AddressU;
    _11desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)desc.AddressV;
    _11desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)desc.AddressW;
    _11desc.BorderColor[0] = desc.BorderColor[0];
    _11desc.BorderColor[1] = desc.BorderColor[1];
    _11desc.BorderColor[2] = desc.BorderColor[2];
    _11desc.BorderColor[3] = desc.BorderColor[3];
    _11desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)desc.ComparisonFunc;
    _11desc.Filter = (D3D11_FILTER)desc.Filter;
    _11desc.MaxAnisotropy = desc.MaxAnisotropy;
    _11desc.MaxLOD = desc.MaxLOD;
    _11desc.MinLOD = desc.MinLOD;
    _11desc.MipLODBias = desc.MipLODBias;

    HRESULT hr = graphic_manager->GetID3D11Device()->CreateSamplerState(&_11desc, &this->mp_sampler_state);
    //作成失敗
    if (FAILED(hr))
    {
        GraphicException::CREATE_SAMPLER_STATE;
    }
    return  GraphicException::NONE_EXCEPTION;
}

ID3D11SamplerState* SamplerState::GetSamplerState()const{
    return this->mp_sampler_state;
}

const SAMPLER_DESC& SamplerState::GetSampleDesc()const{
    return this->m_desc;
}