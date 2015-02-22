#include "evolution_graphic_depthbuffer.h"

#include "evolution_graphic_command.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 DepthBuffer::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT DepthBuffer::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
    {
        *ppvObject = static_cast<IGraphicD3DExecute*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_ITexture2D))
    {
        *ppvObject = static_cast<ITexture2D*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IDepthBuffer))
    {
        *ppvObject = static_cast<IDepthBuffer*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_DepthBuffer))
    {
        *ppvObject = static_cast<DepthBuffer*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 DepthBuffer::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle DepthBuffer::GetGraphicHandle(){
    return this->m_handle;
}


//テクスチャフォーマット
FORMAT_TYPE::_FORMAT_TYPE DepthBuffer::GetFormat(){
    return this->m_format_type;
}

//幅
u32 DepthBuffer::GetWidth(){
    return this->m_width;
}
//高さ
u32 DepthBuffer::GetHeigth(){
    return this->m_heigth;
}

DepthBuffer::DepthBuffer(IGraphicCommand* command_queue) :
m_width(0),
m_heigth(0),
mp_graphic_command(nullptr)
{
    command_queue->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&mp_graphic_command);
}

DepthBuffer::~DepthBuffer(){
    EVOLUTION_RELEASE(this->mp_depth_stencil_view);
    EVOLUTION_RELEASE(this->mp_texture2d);
    EVOLUTION_RELEASE(mp_graphic_command);
}

GraphicResult::_RESULT DepthBuffer::Create(GraphicManager* graphic_manager, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format){
    ID3D11Device* device = graphic_manager->GetID3D11Device();

    HRESULT hr = S_OK;

    D3D11_TEXTURE2D_DESC depthTextureDesc;
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width = width;
    depthTextureDesc.Height = heigth;
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = (DXGI_FORMAT)format;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.SampleDesc.Quality = 0;
    depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    depthTextureDesc.CPUAccessFlags = 0;
    depthTextureDesc.MiscFlags = 0;

    hr = device->CreateTexture2D(&depthTextureDesc, NULL, &this->mp_texture2d);
    if (FAILED(hr))
    {
        EVOLUTION_RELEASE(this->mp_texture2d);
        EVOLUTION_RELEASE(this->mp_depth_stencil_view);
        return GraphicResult::CREATE_FAILED;
    }

    //ステンシルターゲット作成
    D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
    DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    DepthStencilViewDesc.Flags = 0;

    hr = device->CreateDepthStencilView(this->mp_texture2d, &DepthStencilViewDesc, &this->mp_depth_stencil_view);

    if (FAILED(hr))
    {
        EVOLUTION_RELEASE(this->mp_texture2d);
        EVOLUTION_RELEASE(this->mp_depth_stencil_view);
        return GraphicResult::CREATE_FAILED;
    }
    return GraphicResult::RESULT_OK;

}
//デプスバッファのクリア
void DepthBuffer::Clear(DEPTH_CLEAR_FLAG::_FLAG flag, f32 depth, u8 stencil){
    this->m_clear_property.clear_flag = flag;
    this->m_clear_property.depth = depth;
    this->m_clear_property.stencil = stencil;

    IGraphicD3DExecute* cmd;
    this->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&cmd);
    this->mp_graphic_command->SetCommand(cmd, (ptr_t)&this->m_clear_property, sizeof(m_clear_property));
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT DepthBuffer::Execute(const CommandProperty& command_property, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    const ClearProperty* clear_property = (ClearProperty*)command_property.val_ptr;
    context->ClearDepthStencilView(this->mp_depth_stencil_view, clear_property->clear_flag, clear_property->depth, clear_property->stencil);
    return GraphicResult::RESULT_OK;
}

ID3D11DepthStencilView* DepthBuffer::GetID3D11DepthStencilView()const{
    return this->mp_depth_stencil_view;
}