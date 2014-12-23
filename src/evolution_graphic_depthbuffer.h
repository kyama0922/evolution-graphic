#ifndef __EVOLUTION_GRAPHIC_DEPTH_BUFFER_H__
#define __EVOLUTION_GRAPHIC_DEPTH_BUFFER_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-----------------------
        //デプスバッファ
        //-----------------------
        class DepthBuffer : public IDepthBuffer, public IGraphicD3DExecute{
        public:
            struct ClearProperty{
                u32 clear_flag;
                f32 depth;
                u8 stencil;
            };
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;

            FORMAT_TYPE::_FORMAT_TYPE m_format_type;
            u32 m_width;
            u32 m_heigth;

            GraphicCommand* mp_graphic_command;

            ClearProperty m_clear_property;

            ID3D11Texture2D* mp_texture2d;
            ID3D11DepthStencilView* mp_depth_stencil_view;
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

            DepthBuffer( IGraphicCommand* command_queue);
            ~DepthBuffer();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format);

            //デプスバッファのクリア
            void Clear(DEPTH_CLEAR_FLAG::_FLAG flag, f32 depth, u8 stencil);

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_property, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

            ID3D11DepthStencilView* GetID3D11DepthStencilView()const;
        };
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {CA0BCBF5-CDCD-4d8a-872E-C6C3305C62AF}
        static const EVOLUTION_IID IID_DepthBuffer =
        { 0xca0bcbf5, 0xcdcd, 0x4d8a, { 0x87, 0x2e, 0xc6, 0xc3, 0x30, 0x5c, 0x62, 0xaf } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_DEPTH_BUFFER_H__