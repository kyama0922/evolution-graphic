#include "evolution_graphic_rendering_pipeline.h"
#include "shader/evolution_graphic_shader_chain.h"
#include "evolution_graphic_framebuffer.h"
#include "evolution_graphic_command.h"
#include "evolution_graphic_buffer.h"
#include "evolution_graphic_depthbuffer.h"
#include "evolution_graphic_sampler_state.h"
#include "evolution_graphic_blend_state.h"
#include "evolution_graphic_depth_stencil_state.h"
#include "evolution_graphic_rasterrizer.h"
#include "evolution_graphic_constant_buffer.h"
using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//IUnknown
u32 RenderingPipeline::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT RenderingPipeline::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
    {
        *ppvObject = static_cast<IGraphicD3DExecute *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IRenderingPipeline))
    {
        *ppvObject = static_cast<IRenderingPipeline *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_RenderingPipeline))
    {
        *ppvObject = static_cast<RenderingPipeline *>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

u32 RenderingPipeline::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle RenderingPipeline::GetGraphicHandle(){
    return this->m_handle;
}

RenderingPipeline::RenderingPipeline(IGraphicCommand* graphic_command) :
mp_graphic_command(nullptr)
{
    memset(&this->m_command_data, 0, sizeof(RenderingPipeline::Command_DATA));
    this->SetGraphicCommand(graphic_command);
}
RenderingPipeline::~RenderingPipeline(){
    EVOLUTION_RELEASE(this->mp_graphic_command);
    this->Clear();
}

//グラフィックコマンドの設定
void RenderingPipeline::SetGraphicCommand(IGraphicCommand* command){
    EVOLUTION_RELEASE(this->mp_graphic_command);
    command->QueryInterface(EVOLUTION_GUID::IID_IGraphicCommand, (void**)&mp_graphic_command);
}

//インデクスバッファの設定
void RenderingPipeline::SetIndexBuffer(IBuffer* index_buffer){
    if (this->m_command_data.mp_index_buffer == index_buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_index_buffer);
    index_buffer->QueryInterface(EVOLUTION_GUID::IID_Buffer, (void**)& this->m_command_data.mp_index_buffer);
}

//頂点バッファの設定
void RenderingPipeline::SetVertexBuffer(IBuffer* vertex_buffer){
    if (this->m_command_data.mp_vertex_buffer[0] == vertex_buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_vertex_buffer[0]);
    vertex_buffer->QueryInterface(EVOLUTION_GUID::IID_Buffer, (void**)& this->m_command_data.mp_vertex_buffer[0]);
}

//頂点バッファの設定
void RenderingPipeline::SetVertexBuffer(IBuffer* vertex_buffer, u32 slot_number){
    if (this->m_command_data.mp_vertex_buffer[slot_number] == vertex_buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_vertex_buffer[slot_number]);
    vertex_buffer->QueryInterface(EVOLUTION_GUID::IID_Buffer, (void**)& this->m_command_data.mp_vertex_buffer[slot_number]);
}

//テクスチャの設定
void RenderingPipeline::SetTexture(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, ITexture* texture){
    IShaderResource* resource;
    texture->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IShaderResource, (void**)&resource);
    if (this->m_command_data.m_shader_input[shader_type].mp_shader_resource_view[slot_number] == resource)
    {
        EVOLUTION_RELEASE(resource);
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.m_shader_input[shader_type].mp_shader_resource_view[slot_number]);
    this->m_command_data.m_shader_input[shader_type].mp_shader_resource_view[slot_number] = resource;
}

//コンスタントバッファまたはユニフォーム用バッファの設定
void RenderingPipeline::SetBuffer(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, IBuffer* buffer){
    if (this->m_command_data.m_shader_input[shader_type].mp_constantbuffer[slot_number] == buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.m_shader_input[shader_type].mp_constantbuffer[slot_number]);
    buffer->QueryInterface(EVOLUTION_GUID::IID_ConstantBuffer, (void**)&this->m_command_data.m_shader_input[shader_type].mp_constantbuffer[slot_number]);
}

//サンプラーステートの設定
void RenderingPipeline::SetSamplerState(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, ISamplerState* sampler_state){
    if (this->m_command_data.m_shader_input[shader_type].mp_sampler_state[slot_number] == sampler_state)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.m_shader_input[shader_type].mp_sampler_state[slot_number]);
    sampler_state->QueryInterface(EVOLUTION_GUID::IID_SamplerState, (void**)&this->m_command_data.m_shader_input[shader_type].mp_sampler_state[slot_number]);
}
//ラスタライザを設定
void RenderingPipeline::SetRasterrizer(IRasterrizerState* rasterrizer_state){
    if (this->m_command_data.mp_rasterrizer_state == rasterrizer_state)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_rasterrizer_state);
    rasterrizer_state->QueryInterface(EVOLUTION_GUID::IID_RasterrizerState, (void**)&this->m_command_data.mp_rasterrizer_state);
}

//ブレンドステートを設定
void RenderingPipeline::SetBlendState(IBlendState* blend_state){
    if (this->m_command_data.mp_blend_state == blend_state)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_blend_state);
    blend_state->QueryInterface(EVOLUTION_GUID::IID_BlendState, (void**)&this->m_command_data.mp_blend_state);
}

//デプスステンシルを設定
void RenderingPipeline::SetDepthStencilState(IDepthStencilState* depth_stencil_state){
    if (this->m_command_data.mp_depth_stencil_state == depth_stencil_state)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_depth_stencil_state);
    depth_stencil_state->QueryInterface(EVOLUTION_GUID::IID_DepthStencilState, (void**)&this->m_command_data.mp_depth_stencil_state);
}

//フレームバッファを設定
void RenderingPipeline::SetFrameBuffer(IFrameBuffer* frame_buffer){
    if (this->m_command_data.mp_frame_buffer[0] == frame_buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_frame_buffer[0]);
    frame_buffer->QueryInterface(EVOLUTION_GUID::IID_FrameBuffer, (void**)&this->m_command_data.mp_frame_buffer[0]);
}

//フレームバッファを設定
void RenderingPipeline::SetFrameBuffer(IFrameBuffer* frame_buffer, u32 slot_number){
    if (this->m_command_data.mp_frame_buffer[slot_number] == frame_buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_frame_buffer[slot_number]);
    frame_buffer->QueryInterface(EVOLUTION_GUID::IID_FrameBuffer, (void**)&this->m_command_data.mp_frame_buffer[slot_number]);
}
//シェーダーを設定
void RenderingPipeline::SetShader(IShaderChain* shader_chain){
    if (this->m_command_data.mp_shader_chain == shader_chain)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_shader_chain);
    shader_chain->QueryInterface(EVOLUTION_GUID::IID_ShaderChain, (void**)&this->m_command_data.mp_shader_chain);
}

//レンダーターゲットの設定
void RenderingPipeline::SetRenderTarget(ITexture2D* render_target){
    if (this->m_command_data.mp_frame_buffer[0] == render_target)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_frame_buffer[0]);
    render_target->QueryInterface(EVOLUTION_GUID::IID_FrameBuffer, (void**)&this->m_command_data.mp_frame_buffer[0]);
}
//レンダーターゲットの設定
void RenderingPipeline::SetRenderTarget(ITexture2D* render_target, u32 slot_number){
    if (this->m_command_data.mp_frame_buffer[slot_number] == render_target)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_frame_buffer[slot_number]);
    render_target->QueryInterface(EVOLUTION_GUID::IID_FrameBuffer, (void**)&this->m_command_data.mp_frame_buffer[slot_number]);
}
//デプスバッファの設定
void RenderingPipeline::SetDepthBuffer(ITexture2D* depth_buffer){
    if (this->m_command_data.mp_deps_buffer == depth_buffer)
    {
        return;
    }
    EVOLUTION_RELEASE(this->m_command_data.mp_deps_buffer);
    depth_buffer->QueryInterface(EVOLUTION_GUID::IID_DepthBuffer, (void**)&this->m_command_data.mp_deps_buffer);
}

//ドローコール
void RenderingPipeline::Draw(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY primitive_topology, u32 vertex_count, u32 start_vertex_location){
    this->m_command_data.m_draw_mode = DRAW_MODE::DRAW;
    this->m_command_data.m_primitive_topology = primitive_topology;
    this->m_command_data.m_vertex_or_index_count = vertex_count;
    this->m_command_data.m_start_vertex_or_index_location = start_vertex_location;

    IGraphicD3DExecute* cmd;
    this->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&cmd);
    this->mp_graphic_command->SetCommand(cmd, (ptr_t)&this->m_command_data, sizeof(this->m_command_data));
}

//ドローコール
void RenderingPipeline::DrawIndexed(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 index_cout, u32 start_index_location, s32 base_vertex_location){
    this->m_command_data.m_draw_mode = DRAW_MODE::DRAW_INDEXED;
    this->m_command_data.m_primitive_topology = primitive_topology;
    this->m_command_data.m_vertex_or_index_count = index_cout;
    this->m_command_data.m_start_vertex_or_index_location = start_index_location;
    this->m_command_data.m_bass_vertex_location = base_vertex_location;

    IGraphicD3DExecute* cmd;
    this->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&cmd);
    this->mp_graphic_command->SetCommand(cmd, (ptr_t)&this->m_command_data, sizeof(this->m_command_data));
}

//ドローコール
void RenderingPipeline::DrawIndexedInstanced(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 index_cout, u32 instance_count, u32 start_index_location, s32 base_vertex_location, u32 start_instance_location){
    this->m_command_data.m_draw_mode = DRAW_MODE::DRAW_INDEXED_INSTANCED;
    this->m_command_data.m_primitive_topology = primitive_topology;
    this->m_command_data.m_vertex_or_index_count = index_cout;
    this->m_command_data.m_instance_count = instance_count;
    this->m_command_data.m_start_vertex_or_index_location = start_index_location;
    this->m_command_data.m_bass_vertex_location = base_vertex_location;
    this->m_command_data.m_start_instance_location = start_instance_location;

    IGraphicD3DExecute* cmd;
    this->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&cmd);
    this->mp_graphic_command->SetCommand(cmd, (ptr_t)&this->m_command_data, sizeof(this->m_command_data));
}

void RenderingPipeline::Clear(){

    //シェーダーインプットバッファのClear
    for (s32 i = 0; i < D3D11CONST_VALUE::SHADER_TYPE_NUM; i++)
    {
        //コンスタントバッファのClear
        for (s32 j = 0; j < D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; j++)
        {
            EVOLUTION_RELEASE(this->m_command_data.m_shader_input[i].mp_constantbuffer[j]);
        }
        //リソースのClear
        for (s32 j = 0; j < D3D11CONST_VALUE::COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; j++)
        {
            EVOLUTION_RELEASE(this->m_command_data.m_shader_input[i].mp_shader_resource_view[j]);
        }

        //サンプラーのClear
        for (s32 j = 0; j < D3D11CONST_VALUE::COMMONSHADER_SAMPLER_SLOT_COUNT; j++)
        {
            EVOLUTION_RELEASE(this->m_command_data.m_shader_input[i].mp_sampler_state[j]);
        }
    }

    //レンダリングターゲット(Framebuffer)のClear
    for (s32 i = 0; i < D3D11CONST_VALUE::RENDER_TARGET_COUNT; i++)
    {
        EVOLUTION_RELEASE(this->m_command_data.mp_frame_buffer[i]);
    }

    //頂点バッファのClear
    for (s32 i = 0; i < D3D11CONST_VALUE::NUM_VERTEX_INPUT_MAX_COUNT; i++)
    {
        EVOLUTION_RELEASE(this->m_command_data.mp_vertex_buffer[i]);
    }


    //デプスバッファのClear
    EVOLUTION_RELEASE(this->m_command_data.mp_deps_buffer);
    //インデックスバッファのClear
    EVOLUTION_RELEASE(this->m_command_data.mp_index_buffer);
    //シェーダーの開放
    EVOLUTION_RELEASE(this->m_command_data.mp_shader_chain);

    EVOLUTION_RELEASE(this->m_command_data.mp_depth_stencil_state);
    EVOLUTION_RELEASE(this->m_command_data.mp_blend_state);
    EVOLUTION_RELEASE(this->m_command_data.mp_rasterrizer_state);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT RenderingPipeline::Execute(const CommandProperty& command_property, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){

    const RenderingPipeline::Command_DATA* command = (RenderingPipeline::Command_DATA*)command_property.val_ptr;

    command->mp_shader_chain->Execute(command_property, context, swapchain);

    u32 shader_bind_flag = command->mp_shader_chain->GetBindFlag();

    //シェーダーインプットバッファのClear
    for (s32 i = 0; i < D3D11CONST_VALUE::SHADER_TYPE_NUM; i++)
    {
        //バインドされていなければ省く
        if (EVOLUTION_IS_NOT_STATE(shader_bind_flag , (1 << i)))
        {
            continue;
        }

        //コンスタントバッファの設定
        ID3D11Buffer* send_constant_buffer[D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { 0 };
        for (s32 j = 0; j < D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; j++)
        {
            if (this->m_command_data.m_shader_input[i].mp_constantbuffer[j])
            {
                send_constant_buffer[j] = this->m_command_data.m_shader_input[i].mp_constantbuffer[j]->GetID3D11Buffer();
            }
        }
        switch (shader_bind_flag & (1 << i))
        {
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::VERTEXSHADER:
            context->VSSetConstantBuffers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_constant_buffer);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::PIXELSHADER:
            context->PSSetConstantBuffers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_constant_buffer);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::GEOMETRYSHADER:
            context->GSSetConstantBuffers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_constant_buffer);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::DOMAINSHADER:
            context->DSSetConstantBuffers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_constant_buffer);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::HULLSHADER:
            context->HSSetConstantBuffers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_constant_buffer);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::COMPUTESHADER:
            context->CSSetConstantBuffers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_constant_buffer);
            break;
        }

        //リソースの設定
        ID3D11ShaderResourceView* send_shader_resource_view[D3D11CONST_VALUE::COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
        for (s32 j = 0; j < D3D11CONST_VALUE::COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; j++)
        {
            if (this->m_command_data.m_shader_input[i].mp_shader_resource_view[j])
            {
                send_shader_resource_view[j] = this->m_command_data.m_shader_input[i].mp_shader_resource_view[j]->GetShaderResourceView();
            }
        }
        switch (shader_bind_flag & (1 << i))
        {
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::VERTEXSHADER:
            context->VSSetShaderResources(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_shader_resource_view);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::PIXELSHADER:
            context->PSSetShaderResources(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_shader_resource_view);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::GEOMETRYSHADER:
            context->GSSetShaderResources(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_shader_resource_view);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::DOMAINSHADER:
            context->DSSetShaderResources(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_shader_resource_view);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::HULLSHADER:
            context->HSSetShaderResources(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_shader_resource_view);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::COMPUTESHADER:
            context->CSSetShaderResources(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_shader_resource_view);
            break;
        }

        //サンプラーの設定
        ID3D11SamplerState* send_sampler_state[D3D11CONST_VALUE::COMMONSHADER_SAMPLER_SLOT_COUNT] = { 0 };
        for (s32 j = 0; j < D3D11CONST_VALUE::COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; j++)
        {
            if (this->m_command_data.m_shader_input[i].mp_sampler_state[j])
            {
                send_sampler_state[j] = this->m_command_data.m_shader_input[i].mp_sampler_state[j]->GetSamplerState();
            }
        }

        switch (shader_bind_flag & (1 << i))
        {
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::VERTEXSHADER:
            context->VSSetSamplers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_sampler_state);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::PIXELSHADER:
            context->PSSetSamplers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_sampler_state);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::GEOMETRYSHADER:
            context->GSSetSamplers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_sampler_state);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::DOMAINSHADER:
            context->DSSetSamplers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_sampler_state);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::HULLSHADER:
            context->HSSetSamplers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_sampler_state);
            break;
        case EVOLUTION::GRAPHIC::ShaderChain::SHADER_BIND_FLAG::COMPUTESHADER:
            context->CSSetSamplers(0, D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, send_sampler_state);
            break;
        }
    }

    //レンダリングターゲット(Framebuffer)のClear
    ID3D11RenderTargetView* send_render_target[D3D11CONST_VALUE::RENDER_TARGET_COUNT] = {0};
    for (s32 i = 0; i < D3D11CONST_VALUE::RENDER_TARGET_COUNT; i++)
    {
        if (command->mp_frame_buffer[i])
        {
            send_render_target[i] = command->mp_frame_buffer[i]->GetID3D11RenderTargetView();
        }
    }

    ID3D11DepthStencilView* send_depth_stencil_view = nullptr;
    if (this->m_command_data.mp_deps_buffer)
    {
        send_depth_stencil_view = command->mp_deps_buffer->GetID3D11DepthStencilView();
    }
    context->OMSetRenderTargets(8, send_render_target, send_depth_stencil_view);

    //頂点バッファのSet
    ID3D11Buffer* send_vertex_buffer[D3D11CONST_VALUE::NUM_VERTEX_INPUT_MAX_COUNT] = { 0 };
    u32 stride[D3D11CONST_VALUE::NUM_VERTEX_INPUT_MAX_COUNT] = { 0 };
    u32 offset[D3D11CONST_VALUE::NUM_VERTEX_INPUT_MAX_COUNT] = { 0 };
    for (s32 i = 0; i < D3D11CONST_VALUE::NUM_VERTEX_INPUT_MAX_COUNT; i++)
    {
        if (command->mp_vertex_buffer[i])
        {
            send_vertex_buffer[i] = command->mp_vertex_buffer[i]->GetID3D11Buffer();
            stride[i] = command->mp_vertex_buffer[i]->GetSingleSize();
        }
    }
    context->IASetVertexBuffers(0, 32, send_vertex_buffer, stride, offset);

    //インデックスバッファのClear
    if (this->m_command_data.mp_index_buffer)
    {
        if (this->m_command_data.mp_index_buffer->GetSingleSize() == 2)
        {
            context->IASetIndexBuffer(this->m_command_data.mp_index_buffer->GetID3D11Buffer(), (DXGI_FORMAT)FORMAT_TYPE::_R16_UINT, 0);
        }
        else{
            context->IASetIndexBuffer(this->m_command_data.mp_index_buffer->GetID3D11Buffer(), (DXGI_FORMAT)FORMAT_TYPE::_R32_UINT, 0);
        }
    }

    context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)command->m_primitive_topology);
   
  

    //ブレンドステートの設定
    if (this->m_command_data.mp_blend_state)
    {
        BlendStateProperty* blend_property = this->m_command_data.mp_blend_state->GetBlendStateProperty();
        f32 BlendFactor[4];
        this->m_command_data.mp_blend_state->GetBlendFactor(BlendFactor);
        context->OMSetBlendState(blend_property->GetBlendState(), BlendFactor, this->m_command_data.mp_blend_state->GetMask());
    }

    //デプスステンシルの設定
    if (this->m_command_data.mp_depth_stencil_state)
    {
        DepthStencilProperty* depth_stencil_property = this->m_command_data.mp_depth_stencil_state->GetDepthStencilProperty();
        context->OMSetDepthStencilState(depth_stencil_property->GetRasterizerState(), this->m_command_data.mp_depth_stencil_state->GetStencilRef());
    }

    //ラスタライザーの設定の設定
    if (this->m_command_data.mp_rasterrizer_state)
    {
        RasterrizerStateProperty* rasterrizer_state_property = this->m_command_data.mp_rasterrizer_state->GetRasterrizerStateProperty();
        context->RSSetState(rasterrizer_state_property->GetRasterizerState());
        context->RSSetViewports(D3D11CONST_VALUE::RENDER_TARGET_COUNT, this->m_command_data.mp_rasterrizer_state->GetViewPort());
        if(this->m_command_data.mp_rasterrizer_state->IsScissor())
        {
            context->RSSetScissorRects(D3D11CONST_VALUE::RENDER_TARGET_COUNT, this->m_command_data.mp_rasterrizer_state->GetScissorRect());
        }
    }

    switch (command->m_draw_mode)
    {
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW:
        context->Draw(command->m_vertex_or_index_count, command->m_start_vertex_or_index_location);
        break;
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW_AUTO:
        context->DrawAuto();
        break;
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW_INDEXED:
        context->DrawIndexed(command->m_vertex_or_index_count, command->m_start_vertex_or_index_location, command->m_bass_vertex_location);
        break;
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW_INDEXED_INSTANCED:
        context->DrawIndexedInstanced(command->m_vertex_or_index_count, command->m_instance_count, command->m_start_vertex_or_index_location, command->m_bass_vertex_location , command->m_start_instance_location);
        break;
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW_INDEXED_INSTANCED_INDIRECT:
        //context->DrawIndexedInstancedIndirect();
        break;
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW_INSTANCED:
        //context->DrawInstanced();
        break;
    case EVOLUTION::GRAPHIC::RenderingPipeline::DRAW_MODE::DRAW_INSTANCED_INDIRECT:
        //context->DrawInstancedIndirect();
        break;
    }

    return GraphicResult::RESULT_OK;
}