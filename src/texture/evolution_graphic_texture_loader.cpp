#include "DirectXTex\DirectXTex.h"
#include "../evolution_graphic_texture_loader.h"
#include "../evolution_graphic_manager.h"
#include "../evolution_graphic_texture2d.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//sample
// https://directxtex.codeplex.com/wikipage?title=CreateTexture&referringTitle=Direct3D%2011%20Helper%20Functions
//void Load()
//{
//    ID3D11Device* device;
//    DirectX::ScratchImage image;
//
//    //image.
//    ID3D11Resource* pResource = nullptr;
//    DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &pResource);
//}


//IUnknown
u32 TextureLoader::AddRef(){
    return this->m_instance_counter.AddRef();
}

RESULT TextureLoader::QueryInterface(EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_ITextureLoader))
    {
        *ppvObject = static_cast<ITextureLoader*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_TextureLoader))
    {
        *ppvObject = static_cast<TextureLoader*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 TextureLoader::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle TextureLoader::GetGraphicHandle(){
    return this->m_handle;
}

TextureLoader::TextureLoader(GraphicManager* graphic_manager) :mp_graphic_manager(nullptr){
    graphic_manager->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicManager, (void**)&mp_graphic_manager);
}

TextureLoader::~TextureLoader(){
    EVOLUTION_RELEASE(this->mp_graphic_manager);

}

//Textureの同期読み込み
GraphicResult::_RESULT TextureLoader::LoadFile(const tchar* filename, ITexture** pp_texture){
    CoInitialize(NULL);
    DirectX::ScratchImage image;
    HRESULT hr;
    hr = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, image);
    if (FAILED(hr)){
        hr = DirectX::LoadFromTGAFile(filename, nullptr, image);
        if (FAILED(hr)){
            hr = DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, image);
            if (FAILED(hr)){
                return GraphicResult::TEXTURE_LOAD_ERROR;
            }
        }
    }

    ID3D11Resource* pResource = nullptr;
    hr = DirectX::CreateTexture(this->mp_graphic_manager->GetID3D11Device(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &pResource);
    if (FAILED(hr)){
        return GraphicResult::CREATE_FAILED;
    }

    EVOLUTION::GRAPHIC::Texture2D* texture = NEW EVOLUTION::GRAPHIC::Texture2D();
    GraphicResult::_RESULT reslut = texture->Create(this->mp_graphic_manager, pResource);
    pResource->Release();

    *pp_texture = texture;
    CoUninitialize();
    return reslut;
}

//Textureの同期読み込み
GraphicResult::_RESULT TextureLoader::LoadMemory(const ptr_t buffer, ptr_size_t buffer_size){
    return GraphicResult::UNKNOWN_FAILED;
}

//非同期読み込み
GraphicResult::_RESULT TextureLoader::LoadAsyncFile(const tchar* filename, ITextureLoadHandle** pp_lord_handle){

    EVOLUTION::CORE::THREAD::IParameter* parameter;
    EVOLUTION::FUNCTION::CreateParameter(&parameter);

    TextureLoadHandle* texture_handler = NEW TextureLoadHandle();
    if (texture_handler->Create())
    {
        return GraphicResult::CREATE_FAILED;
    }

    parameter->CreateParameters(2);
    parameter->CreateParameterMemory(0, sizeof(tchar) * (EVOLUTION::FUNCTION::Strlen(filename) + 1));
    parameter->CreateParameterMemory(1, sizeof(ITextureLoadHandle**));
    parameter->SetParameter(0, filename, sizeof(tchar) * (EVOLUTION::FUNCTION::Strlen(filename) + 1));
    parameter->SetParameter(1, &texture_handler, sizeof(ITextureLoadHandle**));
    this->mp_graphic_manager->GetThreadPool()->TaskExecute(this, parameter);
    EVOLUTION_RELEASE(parameter);
    texture_handler->AddRef();
    *pp_lord_handle = texture_handler;

    return GraphicResult::RESULT_OK;
}

//実行
void TextureLoader::Execute(const EVOLUTION::CORE::THREAD::IParameter* parameter, EVOLUTION::CORE::THREAD::IResult* result){
    tchar* filename = (tchar*)parameter->GetParameter(0);
    TextureLoadHandle* texture_handler = *((TextureLoadHandle**)parameter->GetParameter(1));
    ITexture* texture = nullptr;
    this->LoadFile(filename, &texture);
    texture_handler->SetTexture(texture);
    EVOLUTION_RELEASE(texture);
    EVOLUTION_RELEASE(texture_handler);
}

//タスクが終了するとThreadPoolから呼び出せます。
void TextureLoader::OnEndExecute(const EVOLUTION::CORE::THREAD::IResult* result){

}