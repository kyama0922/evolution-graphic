#include "evolution_graphic_manager.h"

#pragma comment(lib ,"d3d11.lib" )
#pragma comment(lib ,"dxgi.lib" )

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//IUnknown
u32 GraphicManager::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT GraphicManager::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_GraphicManager))
    {
        *ppvObject = static_cast<GraphicManager *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 GraphicManager::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


GraphicManager::GraphicManager() :
m_hwnd(NULL),
mp_dxgi_factory(nullptr),
mp_dxgi_adapter(nullptr),
mp_dxgi_swapchain(nullptr),
mp_d3d11_device(nullptr),
mp_d3d11_immediate_context(nullptr),
m_flag(0),
mp_manager_thread(nullptr),
mp_thread_pool(nullptr)
{
 
}

GraphicManager::~GraphicManager(){
    EVOLUTION_RELEASE(this->mp_dxgi_swapchain);
    EVOLUTION_RELEASE(this->mp_dxgi_factory);

    EVOLUTION_RELEASE(this->mp_d3d11_immediate_context);
    EVOLUTION_RELEASE(this->mp_d3d11_device);

    this->m_correction.ReleaseAll();

    EVOLUTION_DISABLED_STATE(this->m_flag, GraphicManager::STATE::MANAGEMENT_RUN);
    this->mp_manager_thread->Join();
    EVOLUTION_RELEASE(this->mp_manager_thread);
    EVOLUTION_RELEASE(this->mp_thread_pool);
}

GraphicResult::_RESULT GraphicManager::Create(HWND hwnd, s32 width, s32 heigth, s32 work_thread_count){
    HRESULT hr = S_OK;
    try{
        m_hwnd = hwnd;
        hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&this->mp_dxgi_factory));
        if (FAILED(hr))
        {
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_DXGIFACTORY;
        }

        hr = this->mp_dxgi_factory->EnumAdapters(0, &mp_dxgi_adapter);
        if (FAILED(hr))
        {
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_DXGIADAPTER;
        }



        // フューチャーレベルの初期化順番
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1,
        };
        u32 flag = 0;

#if defined(_DEBUG)
        flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevel;

        // デバイスの初期化
        hr = D3D11CreateDevice(
            mp_dxgi_adapter,
            D3D_DRIVER_TYPE_UNKNOWN,
            NULL,
            flag,
            featureLevels,
            sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
            D3D11_SDK_VERSION,
            &this->mp_d3d11_device,
            &featureLevel,
            &this->mp_d3d11_immediate_context);

        //デバイスの初期化が出来なかった場合
        if (FAILED(hr)) {
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_D3D11DEVICE;
        }


        //SwapChainの作成
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));

        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = heigth;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = m_hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = true;

        hr = this->mp_dxgi_factory->CreateSwapChain(this->mp_d3d11_device, &sd, &this->mp_dxgi_swapchain);
        if (FAILED(hr)) {
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_SWAPCHAIN;
        }
    }
    catch (EVOLUTION::GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_RELEASE(this->mp_dxgi_swapchain);
        EVOLUTION_RELEASE(this->mp_dxgi_factory);
        EVOLUTION_RELEASE(this->mp_dxgi_adapter);
        EVOLUTION_RELEASE(this->mp_d3d11_immediate_context);
        EVOLUTION_RELEASE(this->mp_d3d11_device);

        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
        throw EVOLUTION::GRAPHIC::GraphicResult::CREATE_FAILED;
    }

    EVOLUTION::FUNCTION::CreateThread(&this->mp_manager_thread, EVOLUTION::KByte * 64);
    EVOLUTION::FUNCTION::CreateThreadPool(&this->mp_thread_pool, EVOLUTION::KByte * 64, work_thread_count, 8192);

    EVOLUTION_ENABLE_STATE(this->m_flag, GraphicManager::STATE::MANAGEMENT_RUN);
    this->mp_manager_thread->Run(GraphicManager::Management, (ptr_t)this);

    return GraphicResult::RESULT_OK;
}

IDXGIFactory* GraphicManager::GetIDXGIFactory()const{
    return this->mp_dxgi_factory;
}
IDXGISwapChain* GraphicManager::GetIDXGISwapChain()const{
    return this->mp_dxgi_swapchain;
}

ID3D11Device* GraphicManager::GetID3D11Device()const{
    return this->mp_d3d11_device;
}

ID3D11DeviceContext* GraphicManager::GetID3D11ImmediateContext()const{
    return this->mp_d3d11_immediate_context;
}

EVOLUTION::UTILITY::Collection& GraphicManager::GetCorrection(){
    return this->m_correction;
}

EVOLUTION::CORE::THREAD::IThreadPool* GraphicManager::GetThreadPool(){
    return this->mp_thread_pool;
}

void GraphicManager::Management(ptr_t address, const EVOLUTION::CORE::THREAD::IThread* thread)
{
    GraphicManager* manager = (GraphicManager*)address;
    while (EVOLUTION_IS_STATE(manager->m_flag, GraphicManager::STATE::MANAGEMENT_RUN))
    {
        manager->mp_thread_pool->Execute(10);
    }
}