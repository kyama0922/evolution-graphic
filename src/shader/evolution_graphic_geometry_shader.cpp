#include "evolution_graphic_geometry_shader.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 GeometryShader::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT GeometryShader::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_GeometryShader))
    {
        *ppvObject = static_cast<GeometryShader*>(this);
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

u32 GeometryShader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle GeometryShader::GetGraphicHandle(){
    return this->m_graphic_handle;
}

GeometryShader::GeometryShader(ID3D11Device* device, ID3DBlob* blob){
    HRESULT hr = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &this->mp_geometry_shader);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_GEOMETRY_SHADER;
    }
}

GeometryShader::~GeometryShader(){
    EVOLUTION_RELEASE(this->mp_geometry_shader);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT GeometryShader::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->GSSetShader(this->mp_geometry_shader, nullptr, 0);
    return GraphicResult::RESULT_OK;
}

