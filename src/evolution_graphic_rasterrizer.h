#ifndef __EVOLUTION_GRAPHIC_RASTERRIZER_STATE_H__
#define __EVOLUTION_GRAPHIC_RASTERRIZER_STATE_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        class RasterrizerStateProperty :public IUnknown{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
   
            ID3D11RasterizerState* mp_rasterizer_state;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            RasterrizerStateProperty();
            ~RasterrizerStateProperty();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager , const D3D11_RASTERIZER_DESC& desc);

            ID3D11RasterizerState* GetRasterizerState()const;

        };

        //-----------------------
        //Rasterrizer
        //-----------------------
        class RasterrizerState : public IRasterrizerState{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;

            GraphicManager* mp_graphic_manager;
            RasterrizerStateProperty* mp_property;

            D3D11_RASTERIZER_DESC m_desc;
            D3D11_VIEWPORT m_view_port[D3D11CONST_VALUE::RENDER_TARGET_COUNT];
            D3D11_RECT  m_scissor_rect[D3D11CONST_VALUE::RENDER_TARGET_COUNT];
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            RasterrizerState(GraphicManager* graphic_manager);
            ~RasterrizerState();

            GraphicResult::_RESULT Create();

            FILL_MODE::_MODE GetFillMode()const;
            CULL_MODE::_MODE GetCullMode()const;
            bool IsFrontCounterClockwise()const;
            s32 GetDepthBias()const;
            f32 GetDepthBiasClamp()const;
            f32 GetSlopeScaledDepthBias()const;
            bool IsDepthClip()const;
            bool IsScissor()const;
            bool IsMultisample()const;
            bool IsAntialiasedLine()const;
            VIEW_PORT*  GetViewPort(u32 slot_number, VIEW_PORT* view_port)const;
            SCISSOR_RECT*  GetScissorRect(u32 slot_number, SCISSOR_RECT* scissor_rect)const;

            void SetProperty(FILL_MODE::_MODE type);
            void SetProperty(CULL_MODE::_MODE type);
            void SetProperty(s32 depth_bias);
            void SetProperty(RASTERRIZER_PROPERTY_TYPE::_TYPE1 type, f32 value);
            void SetProperty(RASTERRIZER_PROPERTY_TYPE::_TYPE2 type, bool flg);

            void SetViewPort(u32 slot_number,const VIEW_PORT& view_port);
            void SetScissorRect(u32 slot_number,const SCISSOR_RECT& scissor_rect);

            //設定したPropertyの反映
            GraphicResult::_RESULT ChangeProperty();

            D3D11_VIEWPORT* GetViewPort();
            D3D11_RECT*  GetScissorRect();
            RasterrizerStateProperty* GetRasterrizerStateProperty()const;
        };
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {9AF3AAF5-B3EB-4e91-80D3-D45B146A5A2B}
        static const EVOLUTION_IID IID_RasterrizerStateProperty =
        { 0x9af3aaf5, 0xb3eb, 0x4e91, { 0x80, 0xd3, 0xd4, 0x5b, 0x14, 0x6a, 0x5a, 0x2b } };

        // {51191F60-E0D6-4fb7-9E49-7ADFA1ED31FC}
        static const EVOLUTION_IID IID_RasterrizerState =
        { 0x51191f60, 0xe0d6, 0x4fb7, { 0x9e, 0x49, 0x7a, 0xdf, 0xa1, 0xed, 0x31, 0xfc } };

    }

}

#endif // !__EVOLUTION_GRAPHIC_FRAME_BUFFER_H__