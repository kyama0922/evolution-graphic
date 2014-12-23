#include "evolution_graphic_factory.h"

#include "evolution_graphic_buffer.h"
#include "evolution_graphic_command.h"
#include "evolution_graphic_depthbuffer.h"
#include "evolution_graphic_framebuffer.h"
#include "evolution_graphic_manager.h"
#include "evolution_graphic_rendering_pipeline.h"
#include "evolution_graphic_texture2d.h"
#include "shader\evolution_graphic_shader_manager.h"
#include "evolution_graphic_texture_loader.h"
#include "evolution_graphic_sampler_state.h"
#include "evolution_graphic_texture2d.h"
#include "evolution_graphic_blend_state.h"
#include "evolution_graphic_depth_stencil_state.h"
#include "evolution_graphic_rasterrizer.h"
#include "evolution_graphic_constant_buffer.h"

#include <Windows.h>
#pragma comment(lib ,"d3d11.lib" )
#pragma comment(lib ,"dxgi.lib" )

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


static EVOLUTION::RESULT CreateGraphicFactoryEx(GRAPHIC::IGraphicFactory** graphic_factory, void* hwnd, s32 width, s32 heigth, s32 work_thread_count){
    *graphic_factory = nullptr;
    try{
        GraphicFactory*  factory = NEW EVOLUTION::GRAPHIC::GraphicFactory();
        factory->Create(*((HWND*)hwnd), width, heigth, work_thread_count);
        *graphic_factory = factory;
    }
    catch (...)
    {
        return EVOLUTION::RESULT::S_false;
    }
    return EVOLUTION::RESULT::S_ok;
}

EVOLUTION::RESULT EVOLUTION::FUNCTION::CreateGraphicFactory(GRAPHIC::IGraphicFactory** graphic_factory, void* hwnd, s32 width, s32 heigth){
    return CreateGraphicFactoryEx(graphic_factory, hwnd, width, heigth, 2);
}

//IUnknown
u32 GraphicFactory::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT GraphicFactory::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IGraphicFactory))
    {
        *ppvObject = static_cast<IGraphicFactory*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_GraphicFactory))
    {
        *ppvObject = static_cast<GraphicFactory*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 GraphicFactory::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle GraphicFactory::GetGraphicHandle(){
    return this->m_handle;
}

GraphicFactory::GraphicFactory() :
m_handle(0)
{

}

GraphicFactory::~GraphicFactory(){
    EVOLUTION_RELEASE(this->mp_graphic_manager);
    EVOLUTION_DELETE(this->mp_shader_manager);

}

GraphicResult::_RESULT GraphicFactory::Create(HWND hwnd, s32 width, s32 heigth, s32 work_thread_count){
    mp_graphic_manager = NEW GraphicManager();
    mp_graphic_manager->Create(hwnd, width, heigth, work_thread_count);

    mp_shader_manager = NEW ShaderManager(mp_graphic_manager);

    return GraphicResult::RESULT_OK;
}

//テクスチャの作成
GraphicResult::_RESULT GraphicFactory::CreateTexture2D(ITexture2D** texture2d, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type){
    Texture2D* work_texture2d = NEW Texture2D();
    GraphicResult::_RESULT result = work_texture2d->Create(this->mp_graphic_manager, width, heigth, format_type);
    if (EVOLUTION::FUNCTION::IsGraphicFailed(result))
    {
        EVOLUTION_RELEASE(work_texture2d);
    }
    *texture2d = work_texture2d;
    return result;
}
//デプスバッファの作成
GraphicResult::_RESULT GraphicFactory::CreateDepthBuffer(IDepthBuffer** depth_buffer, IGraphicCommand* graphic_command, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type){
    DepthBuffer* work_depth = NEW DepthBuffer(graphic_command);
    GraphicResult::_RESULT result = work_depth->Create(this->mp_graphic_manager, width, heigth, format_type);
    if (EVOLUTION::FUNCTION::IsGraphicFailed(result))
    {
        EVOLUTION_RELEASE(work_depth);
    }
    *depth_buffer = work_depth;
    return result;
}
//フレームバッファの作成(アプリケーションのフレームバッファの作成)
GraphicResult::_RESULT GraphicFactory::CreateFrameBuffer(IFrameBuffer** frame_buffer, IGraphicCommand* graphic_command){
    HRESULT hr = S_OK;
    try{
        ID3D11Texture2D* p_back_buffer = nullptr;
        hr = this->mp_graphic_manager->GetIDXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_back_buffer);
        if (FAILED(hr)){
            throw EVOLUTION::GRAPHIC::GraphicException::GET_BUFFER;
        }
        *frame_buffer = NEW FrameBuffer(this->mp_graphic_manager, graphic_command, p_back_buffer);
        EVOLUTION_RELEASE(p_back_buffer);
    }
    catch (EVOLUTION::GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
    }
    return (*frame_buffer == nullptr) ? GraphicResult::_RESULT::CREATE_FAILED : GraphicResult::RESULT_OK;
}
//フレームバッファの作成
GraphicResult::_RESULT GraphicFactory::CreateFrameBuffer(IFrameBuffer** frame_buffer, IGraphicCommand* graphic_command, s32 width, s32 heigth, FORMAT_TYPE::_FORMAT_TYPE format){
    *frame_buffer = NEW FrameBuffer(this->mp_graphic_manager, graphic_command, width, heigth, format);
    return (*frame_buffer == nullptr) ? GraphicResult::_RESULT::CREATE_FAILED : GraphicResult::RESULT_OK;
}
//バッファの作成
GraphicResult::_RESULT GraphicFactory::CreateVertexBuffer(IBuffer** buffer, IGraphicCommand* graphic_command, void* buffer_address, u32 buffer_count, u32 single_size){
    Buffer* tmp_buffer = NEW Buffer(graphic_command);
    tmp_buffer->CreateVertexBuffer(this->mp_graphic_manager, buffer_address, buffer_count, single_size , true);
    *buffer = tmp_buffer;
    return GraphicResult::RESULT_OK;
}

//レンダリングパイプライン作成
GraphicResult::_RESULT GraphicFactory::CreateRenderingPipeline(IRenderingPipeline** rendering_pipeline, IGraphicCommand* graphic_command){
    RenderingPipeline* tmp_rendering_pipeline = NEW RenderingPipeline(graphic_command);

    *rendering_pipeline = tmp_rendering_pipeline;
    return GraphicResult::RESULT_OK;
}
//グラフィックコマンド作成
GraphicResult::_RESULT GraphicFactory::CreateGraphicCommand(IGraphicCommand** graphic_graphic_command, u32 queue_size, u32 buffer_byte){
    *graphic_graphic_command = nullptr;
    GraphicCommand* graphic_command = NEW GraphicCommand();
    if (EVOLUTION::FUNCTION::IsGraphicFailed(graphic_command->Create(this->mp_graphic_manager, queue_size, buffer_byte)))
    {
        EVOLUTION_RELEASE(graphic_command);
        return GraphicResult::CREATE_FAILED;
    }
    *graphic_graphic_command = graphic_command;
    return GraphicResult::RESULT_OK;
}


GraphicResult::_RESULT GraphicFactory::CreateShaderChain(IShaderChain** shader_chain, SHADER_INPUT_LAYOUT* input_layout, u32 input_layout_count, SHADER_PROPERTY* shader_property, u32 property_count){
    return this->mp_shader_manager->CreateShaderChain(shader_chain, input_layout, input_layout_count, shader_property, property_count);
}

GraphicResult::_RESULT GraphicFactory::CreateTextureLoader(ITextureLoader** pp_texture_loader){
    *pp_texture_loader = NEW TextureLoader(this->mp_graphic_manager);
    return GraphicResult::RESULT_OK;
}

//SamplerStateの作成
GraphicResult::_RESULT GraphicFactory::CreateSamplerState(ISamplerState** pp_sampler_state, const SAMPLER_DESC& desc){
    *pp_sampler_state = nullptr;
    SamplerState* sample_state = NEW SamplerState();
    if (sample_state->Create(this->mp_graphic_manager , desc) != GraphicException::NONE_EXCEPTION)
    {
        EVOLUTION_RELEASE(sample_state);
        return GraphicResult::CREATE_FAILED;
    }
    *pp_sampler_state = sample_state;
    return GraphicResult::RESULT_OK;
}

//BlendStateの作成
GraphicResult::_RESULT GraphicFactory::CreateBlendState(IBlendState** pp_blend_state){
    BlendState* blend_state = NEW BlendState(this->mp_graphic_manager);
    GraphicResult::_RESULT result = blend_state->Create();
    if (FUNCTION::IsGraphicFailed(result))
    {
        EVOLUTION_RELEASE(blend_state);
    }
    *pp_blend_state = blend_state;
    return result;
}

//ラスタライザーの作成
GraphicResult::_RESULT GraphicFactory::CreateRasterrizerState(IRasterrizerState** pp_rasterrizer_state){
    RasterrizerState* rasterrizer_state = NEW RasterrizerState(this->mp_graphic_manager);
    GraphicResult::_RESULT result = rasterrizer_state->Create();
    if (FUNCTION::IsGraphicFailed(result))
    {
        EVOLUTION_RELEASE(rasterrizer_state);
    }
    *pp_rasterrizer_state = rasterrizer_state;
    return result;
}

//ステンシルステートの作成
GraphicResult::_RESULT GraphicFactory::CreateDepthStencilState(IDepthStencilState** pp_stencil_state){
    DepthStencilState* depth_stencil_state = NEW DepthStencilState(this->mp_graphic_manager);
    GraphicResult::_RESULT result = depth_stencil_state->Create();
    if (FUNCTION::IsGraphicFailed(result))
    {
        EVOLUTION_RELEASE(depth_stencil_state);
    }
    *pp_stencil_state = depth_stencil_state;
    return result;
}

//コンスタントバッファの作成
GraphicResult::_RESULT GraphicFactory::CreateConstantBuffer(IConstantBuffer** pp_constant_buffer, IGraphicCommand* graphic_command, const CONSTANT_BUFFER_LAYOUT* layout, s32 count,bool dynamic){
    ConstantBuffer* constant_buffer = NEW ConstantBuffer( graphic_command);
    GraphicResult::_RESULT result = constant_buffer->Create(this->mp_graphic_manager, layout, count, dynamic);
    if (FUNCTION::IsGraphicFailed(result))
    {
        EVOLUTION_RELEASE(constant_buffer);
    }
    *pp_constant_buffer = constant_buffer;
    return result;
}

//グラフィックコレクションを取得
EVOLUTION::UTILITY::Collection& GraphicFactory::GetGraphicUnknownCollection(){
    return this->m_unknown_correction;
}
