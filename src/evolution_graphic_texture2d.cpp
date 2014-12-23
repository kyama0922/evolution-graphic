#include "evolution_graphic_texture2d.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//IUnknown
u32 Texture2D::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT Texture2D::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IShaderResource))
    {
        *ppvObject = static_cast<IShaderResource *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_ITexture))
    {
        *ppvObject = static_cast<ITexture *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_ITexture2D))
    {
        *ppvObject = static_cast<ITexture2D *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_Texture2D))
    {
        *ppvObject = static_cast<Texture2D *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 Texture2D::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle Texture2D::GetGraphicHandle(){
    return this->m_handle;
}


Texture2D::Texture2D():
m_width(0),
m_heigth(0),
mp_shader_resource_view(nullptr) ,
mp_texture2d(nullptr)
{
}

GraphicResult::_RESULT Texture2D::Create(GraphicManager* graphic_manager, ID3D11Resource* resource_data){
    HRESULT hr = S_OK;
    try{
        hr = resource_data->QueryInterface(IID_ID3D11Texture2D, (void**)&this->mp_texture2d);
        if (FAILED(hr))
        {
            throw  EVOLUTION::GRAPHIC::GraphicException::CREATE_TEXTURE2D;
        }
        D3D11_TEXTURE2D_DESC desc;
        this->mp_texture2d->GetDesc(&desc);

        this->m_width = desc.Width;
        this->m_heigth = desc.Height;
        this->m_format_type = (FORMAT_TYPE::_FORMAT_TYPE)desc.Format;


        //ShaderResourceViewの作成
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;

        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        hr = graphic_manager->GetID3D11Device()->CreateShaderResourceView(this->mp_texture2d, &srvDesc, &this->mp_shader_resource_view);
        if (FAILED(hr))
        {
            throw  EVOLUTION::GRAPHIC::GraphicException::CREATE_SHADERRESOURCEVIEW;
        }
    }
    catch (EVOLUTION::GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_RELEASE(this->mp_texture2d);
        EVOLUTION_RELEASE(this->mp_shader_resource_view);

        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
        throw EVOLUTION::GRAPHIC::GraphicResult::CREATE_FAILED;
    }
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT Texture2D::Create(GraphicManager* graphic_manager, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type){
    return GraphicResult::RESULT_OK;
}

Texture2D::~Texture2D(){
    EVOLUTION_RELEASE(this->mp_texture2d);
    EVOLUTION_RELEASE(this->mp_shader_resource_view);
}

FORMAT_TYPE::_FORMAT_TYPE Texture2D::GetFormat(){
    return this->m_format_type;
}

//幅
u32 Texture2D::GetWidth(){
    return this->m_width;
}
//高さ
u32 Texture2D::GetHeigth(){
    return this->m_heigth;
}

ID3D11Resource* Texture2D::GetResource()const{
    return this->mp_texture2d;
}

ID3D11ShaderResourceView* Texture2D::GetShaderResourceView()const{
    return this->mp_shader_resource_view;
}