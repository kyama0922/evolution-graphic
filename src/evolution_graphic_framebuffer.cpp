#include "evolution_graphic_framebuffer.h"
#include "evolution_graphic_manager.h"
#include "evolution_graphic_command.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//IUnknown
u32 FrameBuffer::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT FrameBuffer::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){

    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IFrameBuffer))
    {
        *ppvObject = static_cast<IFrameBuffer*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
    {
        *ppvObject = static_cast<IGraphicD3DExecute*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_FrameBuffer))
    {
        *ppvObject = static_cast<FrameBuffer*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 FrameBuffer::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle FrameBuffer::GetGraphicHandle(){
    return this->m_handle;
}

//テクスチャフォーマット
FORMAT_TYPE::_FORMAT_TYPE FrameBuffer::GetFormat(){
    return this->m_format_type;
}

//幅
u32 FrameBuffer::GetWidth(){
    return this->m_width;
}

//高さ
u32 FrameBuffer::GetHeigth(){
    return this->m_heigth;
}

FrameBuffer::FrameBuffer(GraphicManager* graphic_manager, IGraphicCommand* graphic_command, ID3D11Texture2D* p_texture) :
mp_texture2d(p_texture),
mp_rendertarget_view(nullptr),
mp_shader_resource_view(nullptr),
m_clear_color(0, 0, 0, 0)
{
    graphic_command->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&this->mp_graphic_command);

    HRESULT hr = S_OK;
    try{
        D3D11_TEXTURE2D_DESC desc;
        p_texture->GetDesc(&desc);
        this->m_width = desc.Width;
        this->m_heigth = desc.Height;
        this->m_format_type = (FORMAT_TYPE::_FORMAT_TYPE)desc.Format;

        hr = graphic_manager->GetID3D11Device()->CreateRenderTargetView(this->mp_texture2d, nullptr, &this->mp_rendertarget_view);
        if (FAILED(hr)){
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_RENDERTARGETVIEW;
        }
    }
    catch (EVOLUTION::GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_RELEASE(this->mp_graphic_command);
        EVOLUTION_RELEASE(this->mp_texture2d);
        EVOLUTION_RELEASE(this->mp_rendertarget_view);
        EVOLUTION_RELEASE(this->mp_shader_resource_view);

        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
        throw EVOLUTION::GRAPHIC::GraphicResult::CREATE_FAILED;
    }

}

FrameBuffer::FrameBuffer(GraphicManager* graphic_manager, IGraphicCommand* command_queue, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format) :
mp_texture2d(nullptr),
mp_rendertarget_view(nullptr),
mp_shader_resource_view(nullptr),
m_width(width), 
m_heigth(heigth),
m_format_type(format), 
m_clear_color(0, 0, 0, 0)
{
    command_queue->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&this->mp_graphic_command);

    HRESULT hr = S_OK;
    try{

        D3D11_TEXTURE2D_DESC desc;
        memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));
        desc.Width = this->m_width;
        desc.Height = this->m_heigth;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = (DXGI_FORMAT)format;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        //ShaderResourceViewの作成
        D3D11_RENDER_TARGET_VIEW_DESC renderdesc;
        ZeroMemory(&renderdesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        renderdesc.Format = desc.Format;
        renderdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        //ShaderResourceViewの作成
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;

        hr = graphic_manager->GetID3D11Device()->CreateTexture2D(&desc, nullptr, &this->mp_texture2d);
        if (FAILED(hr)){
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_TEXTURE2D;
        }

        hr = graphic_manager->GetID3D11Device()->CreateRenderTargetView(this->mp_texture2d, &renderdesc, &this->mp_rendertarget_view);
        if (FAILED(hr)){
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_RENDERTARGETVIEW;
        }

        hr = graphic_manager->GetID3D11Device()->CreateShaderResourceView(this->mp_texture2d, &srvDesc, &this->mp_shader_resource_view);
        if (FAILED(hr)){
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_SHADERRESOURCEVIEW;
        }
    }
    catch (EVOLUTION::GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_RELEASE(this->mp_graphic_command);
        EVOLUTION_RELEASE(this->mp_texture2d);
        EVOLUTION_RELEASE(this->mp_rendertarget_view);
        EVOLUTION_RELEASE(this->mp_shader_resource_view);

        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
    }
}

FrameBuffer::~FrameBuffer(){
    EVOLUTION_RELEASE(this->mp_graphic_command);
    EVOLUTION_RELEASE(this->mp_texture2d);
    EVOLUTION_RELEASE(this->mp_rendertarget_view);
    EVOLUTION_RELEASE(this->mp_shader_resource_view);
}

//グラフィックコマンドの設定
void FrameBuffer::SetGraphicCommand(IGraphicCommand* graphic_command){
    this->mp_graphic_command->Release();
    graphic_command->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&this->mp_graphic_command);
}

//クリアカラーの変更
void FrameBuffer::SetClearColor(f32 r, f32 g, f32 b, f32 a){
    this->m_clear_color.r = r;
    this->m_clear_color.g = g;
    this->m_clear_color.b = b;
    this->m_clear_color.a = a;
}

//クリアカラーの変更
void FrameBuffer::SetClearColor(const Color& color){
    this->m_clear_color = color;
}

//レンダーターゲットのクリア
void FrameBuffer::Clear(){
    IGraphicD3DExecute* cmd;
    this->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&cmd);
    this->mp_graphic_command->SetCommand(cmd, (ptr_t)&this->m_clear_color, sizeof(m_clear_color));
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT FrameBuffer::Execute(const CommandProperty& command_property, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    context->ClearRenderTargetView(this->mp_rendertarget_view, (f32*)command_property.val_ptr);
    return GraphicResult::RESULT_OK;
}

ID3D11RenderTargetView* FrameBuffer::GetID3D11RenderTargetView()const{
    return this->mp_rendertarget_view;
}

ID3D11ShaderResourceView* FrameBuffer::GetID3D11ShaderResourceView()const{
    return this->mp_shader_resource_view;
}