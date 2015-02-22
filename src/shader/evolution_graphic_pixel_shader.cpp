#include "evolution_graphic_pixel_shader.h"


using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 PixelShader::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT PixelShader::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_PixelShader))
    {
        *ppvObject = static_cast<PixelShader*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
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

u32 PixelShader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle PixelShader::GetGraphicHandle(){
    return this->m_graphic_handle;
}

PixelShader::PixelShader(ID3D11Device* device, ID3DBlob* blob){
    HRESULT hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &this->mp_pixel_shader);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_PIXEL_SHADER;
    }
}

PixelShader::~PixelShader(){
    EVOLUTION_RELEASE(this->mp_pixel_shader);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT PixelShader::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->PSSetShader(this->mp_pixel_shader, nullptr, 0);
    return GraphicResult::RESULT_OK;
}

