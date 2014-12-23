#ifndef __EVOLUTION_GRAPHIC_BUFFER_H__
#define __EVOLUTION_GRAPHIC_BUFFER_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-------------------------------
        //Buffer 
        //-------------------------------
        class Buffer : public IBuffer , public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            GraphicCommand* mp_graphic_command;

            D3D11_USAGE m_type;
            ID3D11Buffer* mp_d3d11_buffer;

            void* mp_buffer;
            u32 m_array_count;
            u32 m_single_size;

        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            //グラフィックコマンドの設定
            void SetGraphicCommand(IGraphicCommand* command);

            Buffer( IGraphicCommand* graphic_command);
            ~Buffer();

            GraphicResult::_RESULT CreateVertexBuffer(GraphicManager* graphic_manager, void* buffer, u32 buffer_count, u32 single_size, bool dynamic);
            GraphicResult::_RESULT CreateIndexBuffer(GraphicManager* graphic_manager, void* buffer, u32 buffer_count, u32 single_size);

            //BufferのSingleサイズ
            u32 GetSingleSize();
            //Bufferの配列サイズ
            u32 GetArraySize();

            //Bufferアドレスの取得
            void* GetBufferAddress();
            //設定した値を変更する
            GraphicResult::_RESULT ChangeValue();
            //Bufferのコピー
            GraphicResult::_RESULT Copy(IBuffer** buffer);

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

            ID3D11Buffer* GetID3D11Buffer()const;
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {76F2D0F8-A4B4-4af8-ABF1-0BF0CD98EB2B}
        static const EVOLUTION_IID IID_Buffer =
        { 0x76f2d0f8, 0xa4b4, 0x4af8, { 0xab, 0xf1, 0xb, 0xf0, 0xcd, 0x98, 0xeb, 0x2b } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_TEXTURE2D_H__