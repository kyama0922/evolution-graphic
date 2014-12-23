#ifndef __EVOLUTION_GRAPHIC_MANAGER_H__
#define __EVOLUTION_GRAPHIC_MANAGER_H__

#include "evolution_graphic_interface_implement.h"
#include <d3d11.h>

namespace EVOLUTION{
    namespace GRAPHIC{

        //-----------------------
        //Graphic管理クラス
        //-----------------------
        class GraphicManager : public IUnknown {
        private:
            HWND m_hwnd;

            EVOLUTION::InstanceCounter m_instance_counter;

            IDXGIFactory* mp_dxgi_factory;
            IDXGIAdapter* mp_dxgi_adapter;
            IDXGISwapChain* mp_dxgi_swapchain;

            ID3D11Device* mp_d3d11_device;
            ID3D11DeviceContext* mp_d3d11_immediate_context;

            EVOLUTION::UTILITY::Collection m_correction;

            u32 m_flag;
            EVOLUTION::CORE::THREAD::IThread* mp_manager_thread;
            EVOLUTION::CORE::THREAD::IThreadPool* mp_thread_pool;

            struct STATE{
                enum{
                    MANAGEMENT_RUN = 0x00000001,
                };
            };
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            GraphicManager();
            ~GraphicManager();

            GraphicResult::_RESULT Create(HWND hwnd, s32 width, s32 heigth, s32 work_thread_count);

            IDXGIFactory* GetIDXGIFactory()const;
            IDXGISwapChain* GetIDXGISwapChain()const;

            ID3D11Device* GetID3D11Device()const;
            ID3D11DeviceContext* GetID3D11ImmediateContext()const;

            EVOLUTION::UTILITY::Collection& GetCorrection();
            EVOLUTION::CORE::THREAD::IThreadPool* GetThreadPool();
        private:
            static void Management(ptr_t address, const EVOLUTION::CORE::THREAD::IThread* thread);
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {525A6EDA-BA5A-451a-985E-9861F435B9ED}
        static const EVOLUTION_IID IID_GraphicManager =
        { 0x525a6eda, 0xba5a, 0x451a, { 0x98, 0x5e, 0x98, 0x61, 0xf4, 0x35, 0xb9, 0xed } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_MANAGER_H__