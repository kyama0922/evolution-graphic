#include "evolution_graphic_hull_shader.h"


using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 HullShader::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT HullShader::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_HullShader))
    {
        *ppvObject = static_cast<HullShader*>(this);
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

u32 HullShader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle HullShader::GetGraphicHandle(){
    return this->m_graphic_handle;
}

HullShader::HullShader(ID3D11Device* device, ID3DBlob* blob){
    HRESULT hr = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &this->mp_hull_shader);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_HULL_SHADER;
    }
}

HullShader::~HullShader(){
    EVOLUTION_RELEASE(this->mp_hull_shader);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT HullShader::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->HSSetShader(this->mp_hull_shader, nullptr, 0);
    return GraphicResult::RESULT_OK;
}

