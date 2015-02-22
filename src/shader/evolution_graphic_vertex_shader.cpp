#include "evolution_graphic_vertex_shader.h"
#include "evolution_graphic_input_layout.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 VertexShader::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT VertexShader::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_VertexShader))
    {
        *ppvObject = static_cast<VertexShader*>(this);
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

u32 VertexShader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle VertexShader::GetGraphicHandle(){
    return this->m_graphic_handle;
}

VertexShader::VertexShader(ID3D11Device* device, ID3DBlob* blob, const SHADER_INPUT_LAYOUT* input, u32 input_size){
    mp_input_layout = NEW InputLayout(device, blob, input, input_size);

    HRESULT hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &this->mp_vertex_shader);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_VERTEX_SHADER;
    }
}

VertexShader::~VertexShader(){
    EVOLUTION_RELEASE(this->mp_vertex_shader);
    EVOLUTION_RELEASE(this->mp_input_layout);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT VertexShader::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    this->mp_input_layout->Execute(command_data, context, swapchain);
    context->VSSetShader(this->mp_vertex_shader, nullptr, 0);
    return GraphicResult::RESULT_OK;
}

