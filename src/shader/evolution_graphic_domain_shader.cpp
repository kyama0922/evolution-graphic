#include "evolution_graphic_domain_shader.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 DomainShader::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT DomainShader::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_DomainShader))
    {
        *ppvObject = static_cast<DomainShader*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
    {
        *ppvObject = static_cast<IGraphicD3DExecute*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 DomainShader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle DomainShader::GetGraphicHandle(){
    return this->m_graphic_handle;
}

DomainShader::DomainShader(ID3D11Device* device, ID3DBlob* blob){
    HRESULT hr = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &this->mp_domain_shader);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_DOMAIN_SHADER;
    }
}

DomainShader::~DomainShader(){
    EVOLUTION_RELEASE(this->mp_domain_shader);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT DomainShader::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->DSSetShader(this->mp_domain_shader, nullptr, 0);
    return GraphicResult::RESULT_OK;
}