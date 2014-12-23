#ifndef __EVOLUTION_GRAPHIC_BLEND_STATE_H__
#define __EVOLUTION_GRAPHIC_BLEND_STATE_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        class BlendStateProperty :public IUnknown{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            ID3D11BlendState* mp_blend_state;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            BlendStateProperty();
            ~BlendStateProperty();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, const D3D11_BLEND_DESC& desc);

            ID3D11BlendState* GetBlendState()const;
        };

        //-----------------------
        //Rasterrizer
        //-----------------------
        class BlendState : public IBlendState{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            GraphicManager* mp_graphic_manager;

            BlendStateProperty* mp_property;

            D3D11_BLEND_DESC m_desc;

            BLEND_OP::_OP GetType(D3D11_BLEND_OP op)const;
            BLEND::_TYPE GetType(D3D11_BLEND type)const;

            f32 m_blend_factor[4];
            u32 m_mask;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();


            BlendState(GraphicManager* graphic_manager);
            ~BlendState();

            GraphicResult::_RESULT Create();

            BLEND::_TYPE GetSrcBlend(u8 number) const;
            BLEND::_TYPE GetDestBlend(u8 number)const;
            BLEND_OP::_OP GetBlendOp(u8 number) const;
            BLEND::_TYPE GetSrcBlendAlpha(u8 number) const;
            BLEND::_TYPE GetDestBlendAlpha(u8 number)const;
            BLEND_OP::_OP GetBlendOpAlpha(u8 number) const;
            COLOR_WRITE_ENABLE::_TYPE GetRenderTargetWriteMask(u8 number)const;
            u32 GetMask()const;
            const f32* GetBlendFactor(f32(&blend_factor)[4])const;

            void SetProperty(BLEND_STATE_PROPERTY::_TYPE type, bool blend);
            void SetProperty(BLEND_STATE_PROPERTY::_OP operation, u8 number, BLEND_OP::_OP op);
            void SetProperty(BLEND_STATE_PROPERTY::_OBJECT object, u8 number, BLEND::_TYPE type);
            void SetProperty(u8 number, COLOR_WRITE_ENABLE::_TYPE type);
            void SetProperty(u8 number, bool blend_enable);

            void SetMask(u32 mask);
            void SetBlendFactor(const f32(&blend_factor)[4]);

            GraphicResult::_RESULT ChangeProperty();

            BlendStateProperty* GetBlendStateProperty()const;
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {EB33211B-6C38-4462-939B-4E5410EE3963}
        static const EVOLUTION_IID IID_BlendStateProperty =
        { 0xeb33211b, 0x6c38, 0x4462, { 0x93, 0x9b, 0x4e, 0x54, 0x10, 0xee, 0x39, 0x63 } };

        // {F0ED9106-9145-487f-843B-FA09F68DFB08}
        static const EVOLUTION_IID IID_BlendState =
        { 0xf0ed9106, 0x9145, 0x487f, { 0x84, 0x3b, 0xfa, 0x9, 0xf6, 0x8d, 0xfb, 0x8 } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_FRAME_BUFFER_H__