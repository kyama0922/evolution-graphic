#include "evolution_graphic_compute_shader.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 ComputeShader::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT ComputeShader::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_ComputeShader))
    {
        *ppvObject = static_cast<ComputeShader*>(this);
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

u32 ComputeShader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle ComputeShader::GetGraphicHandle(){
    return this->m_graphic_handle;
}

ComputeShader::ComputeShader(ID3D11Device* device, ID3DBlob* blob){
    HRESULT hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &this->mp_compute_shader);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_COMPUTE_SHADER;
    }
}

ComputeShader::~ComputeShader(){
    EVOLUTION_RELEASE(this->mp_compute_shader);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT ComputeShader::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->CSSetShader(this->mp_compute_shader, nullptr, 0);
    return GraphicResult::RESULT_OK;
}

