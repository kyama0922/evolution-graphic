#ifndef __EVOLUTION_GRAPHIC_SAMPLER_STATE_H__
#define __EVOLUTION_GRAPHIC_SAMPLER_STATE_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-----------------------
        //二次元テクスチャ
        //-----------------------
        class SamplerState : public ISamplerState{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;

            ID3D11SamplerState* mp_sampler_state;
            SAMPLER_DESC m_desc;
        public:
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            GraphicHandle GetGraphicHandle();

            SamplerState();
            ~SamplerState();

            GraphicException::_EXCEPTION Create(GraphicManager* graphic_manager , const SAMPLER_DESC& desc);

           ID3D11SamplerState* GetSamplerState()const;

           const SAMPLER_DESC& GetSampleDesc()const;
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{

        // {037ED921-1DDC-4951-9E01-93B28C78E695}
        static const EVOLUTION_IID IID_SamplerState =
        { 0x37ed921, 0x1ddc, 0x4951, { 0x9e, 0x1, 0x93, 0xb2, 0x8c, 0x78, 0xe6, 0x95 } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_SAMPLER_STATE_H__