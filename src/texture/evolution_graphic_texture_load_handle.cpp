#include "../evolution_graphic_texture_loader.h"


using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 TextureLoadHandle::AddRef(){
    return this->m_instance_counter.AddRef();
}

RESULT TextureLoadHandle::QueryInterface(EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_ITextureLoadHandle))
    {
        *ppvObject = static_cast<ITextureLoadHandle*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_TextureLoadHandle))
    {
        *ppvObject = static_cast<TextureLoadHandle*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 TextureLoadHandle::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

TextureLoadHandle::TextureLoadHandle() : mp_texture(nullptr) , mp_stop_watch(nullptr){

}

TextureLoadHandle::~TextureLoadHandle(){
    EVOLUTION_RELEASE(this->mp_stop_watch);
    EVOLUTION_RELEASE(this->mp_texture);
}
GraphicResult::_RESULT TextureLoadHandle::Create(){
    if (EVOLUTION::FUNCTION::IsFailed(EVOLUTION::FUNCTION::CreateStopWatch(&this->mp_stop_watch)))
    {
        return GraphicResult::CREATE_FAILED;
    }
    return GraphicResult::RESULT_OK;
}
//Textureの取得
GraphicResult::_RESULT TextureLoadHandle::GetTexture(ITexture** pp_texture){
    if (this->mp_texture == nullptr)
    {
        return GraphicResult::NONE_INSTANCE;
    }
    RESULT result = this->mp_texture->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_ITexture, (void**)pp_texture);
    if (EVOLUTION_FAILED(result))
    {
        return GraphicResult::CAST_TEXTURE_LOADHANDLE;
    }
    return GraphicResult::RESULT_OK;
}

//ロード終了チェック
TEXTURELOADHANDLESTATE::_STATE TextureLoadHandle::GetStatus()const{
    return m_state;
}

//ロードを待つ(ms秒)
bool TextureLoadHandle::Join(s32 time_out_ms)const{
    this->mp_stop_watch->Start();
    while (!(this->GetStatus() == TEXTURELOADHANDLESTATE::END))
    {
        if (time_out_ms < (s32)this->mp_stop_watch->GetMilliSecondU32())
        {
            return false;
        }
    }
    return true;
}
//Textureのセット
void TextureLoadHandle::SetTexture(ITexture* p_texture){
    if (p_texture)
    {
        p_texture->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_ITexture, (void**)&this->mp_texture);
        this->m_state = TEXTURELOADHANDLESTATE::END;
    }
    this->m_state = TEXTURELOADHANDLESTATE::FAILURE;
}