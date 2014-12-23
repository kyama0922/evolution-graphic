#ifndef __EVOLUTION_GRAPHIC_TEXTURE_LOADER_H__
#define __EVOLUTION_GRAPHIC_TEXTURE_LOADER_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //--------------------------------------
        //TextureのロードHandle
        //--------------------------------------
        class TextureLoadHandle : public ITextureLoadHandle{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            TEXTURELOADHANDLESTATE::_STATE m_state;
            ITexture* mp_texture;
            CORE::TIMER::IStopWatch* mp_stop_watch;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            TextureLoadHandle();
            ~TextureLoadHandle();

            GraphicResult::_RESULT Create();

            //Textureの取得
            GraphicResult::_RESULT GetTexture(ITexture** pp_texture);
            //ロード終了チェック
            TEXTURELOADHANDLESTATE::_STATE GetStatus()const;
            //ロードを待つ(ms秒)
            bool Join(s32 time_out_ms)const;
            //Textureのセット
            void SetTexture(ITexture* p_texture);
        };

        //--------------------------------------
        //グラフィックファクトリークラス
        //--------------------------------------
        class TextureLoader : public ITextureLoader , public EVOLUTION::CORE::THREAD::ITask{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            GraphicManager* mp_graphic_manager;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            TextureLoader(GraphicManager* graphic_manager);
            ~TextureLoader();

            //Textureの同期読み込み
            GraphicResult::_RESULT LoadFile(const tchar* filename, ITexture** pp_texture);
            //Textureの同期読み込み
            GraphicResult::_RESULT LoadMemory(const ptr_t buffer, ptr_size_t buffer_size);
            //非同期読み込み
            GraphicResult::_RESULT LoadAsyncFile(const tchar* filename, ITextureLoadHandle** pp_lord_handle);

            //実行
            void Execute(const EVOLUTION::CORE::THREAD::IParameter* parameter, EVOLUTION::CORE::THREAD::IResult* result);
            //タスクが終了するとThreadPoolから呼び出せます。
            void OnEndExecute(const EVOLUTION::CORE::THREAD::IResult* result);
        };
    }
    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{

        // {2F9D8418-8B95-441b-BBDE-C2C9E8A3B6F0}
        static const EVOLUTION_IID IID_TextureLoadHandle =
        { 0x2f9d8418, 0x8b95, 0x441b, { 0xbb, 0xde, 0xc2, 0xc9, 0xe8, 0xa3, 0xb6, 0xf0 } };

        // {2C61015F-A88C-4e15-A5FE-A9435D38D30A}
        static const EVOLUTION_IID IID_TextureLoader =
        { 0x2c61015f, 0xa88c, 0x4e15, { 0xa5, 0xfe, 0xa9, 0x43, 0x5d, 0x38, 0xd3, 0xa } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_TEXTURE_LOADER_H__