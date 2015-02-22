#include "evolution_graphic_input_layout.h"


using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 InputLayout::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT InputLayout::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_InputLayout))
    {
        *ppvObject = static_cast<InputLayout*>(this);
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

u32 InputLayout::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


//グラフィックハンドルの取得
GraphicHandle InputLayout::GetGraphicHandle(){
    return this->m_graphic_handle;
}

InputLayout::InputLayout(ID3D11Device* device, ID3DBlob* vertex_blob, const SHADER_INPUT_LAYOUT* input, u32 input_size){
    //インプットレイアウトの作成
    HRESULT hr = device->CreateInputLayout((D3D11_INPUT_ELEMENT_DESC*)input, input_size, vertex_blob->GetBufferPointer(), vertex_blob->GetBufferSize(), &this->mp_inputlayout);
    if (FAILED(hr))
    {
        throw GraphicException::CREATE_INPUT_LAYOUT;
    }
}

InputLayout::~InputLayout(){
    EVOLUTION_RELEASE(this->mp_inputlayout);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT InputLayout::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->IASetInputLayout(this->mp_inputlayout);
    return GraphicResult::RESULT_OK;
}

