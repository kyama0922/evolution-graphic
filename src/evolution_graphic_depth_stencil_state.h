#ifndef __EVOLUTION_GRAPHIC_DEPTH_STENCIL_STATE_H__
#define __EVOLUTION_GRAPHIC_DEPTH_STENCIL_STATE_H__

#include "evolution_graphic_interface_implement.h"
namespace EVOLUTION{
    namespace GRAPHIC{

        class DepthStencilProperty :public IUnknown{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            ID3D11DepthStencilState* mp_rendertarget_view;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            DepthStencilProperty();
            ~DepthStencilProperty();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, const D3D11_DEPTH_STENCIL_DESC& desc);

            ID3D11DepthStencilState* GetRasterizerState()const;
        };

        //-----------------------
        //Rasterrizer
        //-----------------------
        class DepthStencilState : public IDepthStencilState{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            GraphicManager* mp_graphic_manager;

            DepthStencilProperty* mp_property;

            D3D11_DEPTH_STENCIL_DESC m_desc;
            u32 m_stencil_ref;

        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();


            DepthStencilState(GraphicManager* graphic_manager);
            ~DepthStencilState();

            GraphicResult::_RESULT Create();

            bool IsDepth()const;
            DEPTH_WRITE_MASK::_MASK GetDepthWriteMask()const;
            COMPARISON_FUNC::_FUNC GetDepthFunc()const;
            bool IsStencil()const;
            u8 GetStencilReadMask()const;
            u8 GetStencilWriteMask()const;
            DEPTH_STENCILOP_DESC GetFrontFace()const;
            DEPTH_STENCILOP_DESC GetBackFace()const;
            u32 GetStencilRef()const;

            void SetProperty(DEPTHSTENCILSTATE_PROPERTY::_TYPE type, bool _property);
            void SetProperty(COMPARISON_FUNC::_FUNC func);
            void SetProperty(DEPTH_WRITE_MASK::_MASK mask);
            void SetProperty(DEPTHSTENCILSTATE_PROPERTY::_MASK mask, u8 value);
            void SetProperty(DEPTHSTENCILSTATE_PROPERTY::_FORCE force, DEPTH_STENCILOP_DESC desc);
            void SetStencilRef(u32 stencil_ref);

            GraphicResult::_RESULT ChangeProperty();
            DepthStencilProperty* GetDepthStencilProperty()const;
        private:
            STENCIL_OP::_OP GetType(D3D11_STENCIL_OP in_type)const;
            COMPARISON_FUNC::_FUNC GetType(D3D11_COMPARISON_FUNC in_func)const;
            D3D11_STENCIL_OP GetType(STENCIL_OP::_OP in_op)const;
        };
    }
    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {CFEE62FF-EDB6-4bce-A81F-6FADACD1C1ED}
        static const EVOLUTION_IID IID_DepthStencilProperty =
        { 0xcfee62ff, 0xedb6, 0x4bce, { 0xa8, 0x1f, 0x6f, 0xad, 0xac, 0xd1, 0xc1, 0xed } };

        // {1AEDDB47-F8C6-4cfd-8A03-A4E2E0718E4B}
        static const EVOLUTION_IID IID_DepthStencilState =
        { 0x1aeddb47, 0xf8c6, 0x4cfd, { 0x8a, 0x3, 0xa4, 0xe2, 0xe0, 0x71, 0x8e, 0x4b } };

    }
}

#endif // !__EVOLUTION_TEMPLATE_FILE_H__