#ifndef __EVOLUTION_GRAPHIC_FRAME_BUFFER_H__
#define __EVOLUTION_GRAPHIC_FRAME_BUFFER_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-----------------------
        //フレームバッファ 
        //-----------------------
        class FrameBuffer : public IFrameBuffer, public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;

            GraphicHandle m_handle;

            FORMAT_TYPE::_FORMAT_TYPE m_format_type;
            u32 m_width;
            u32 m_heigth;

            GraphicCommand* mp_graphic_command;

            Color m_clear_color;

            ID3D11Texture2D* mp_texture2d;
            ID3D11RenderTargetView* mp_rendertarget_view;
            ID3D11ShaderResourceView* mp_shader_resource_view;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            //テクスチャフォーマット
            FORMAT_TYPE::_FORMAT_TYPE GetFormat();

            //幅
            u32 GetWidth();
            //高さ
            u32 GetHeigth();

            FrameBuffer(GraphicManager* graphic_manager, IGraphicCommand* graphic_command, ID3D11Texture2D* p_texture);
            FrameBuffer(GraphicManager* graphic_manager, IGraphicCommand* graphic_command, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format);
            ~FrameBuffer();

            //グラフィックコマンドの設定
            void SetGraphicCommand(IGraphicCommand* graphic_command);

            //クリアカラーの変更
            void SetClearColor(f32 r, f32 g, f32 b, f32 a);
            //クリアカラーの変更
            void SetClearColor(const Color& color);
            //レンダーターゲットのクリア
            void Clear();

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_property, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

            ID3D11RenderTargetView* GetID3D11RenderTargetView()const;
            ID3D11ShaderResourceView* GetID3D11ShaderResourceView()const;
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {40B3FA65-D016-4c68-92CF-EFBF96269274}
        static const EVOLUTION_IID IID_FrameBuffer =
        { 0x40b3fa65, 0xd016, 0x4c68, { 0x92, 0xcf, 0xef, 0xbf, 0x96, 0x26, 0x92, 0x74 } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_FRAME_BUFFER_H__