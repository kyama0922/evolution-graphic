#ifndef __EVOLUTION_GRAPHIC_RENDERING_PIPELINE_H__
#define __EVOLUTION_GRAPHIC_RENDERING_PIPELINE_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-------------------------------------------------
        //レンダリングパイプライン(Direct3D 9　GL2.0 ES)
        //-------------------------------------------------
        class RenderingPipeline : public IRenderingPipeline, public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;

            GraphicHandle m_handle;

            GraphicCommand* mp_graphic_command;

            struct DRAW_MODE{
                enum _MODE{
                    DRAW,
                    DRAW_AUTO,
                    DRAW_INDEXED,
                    DRAW_INDEXED_INSTANCED,
                    DRAW_INDEXED_INSTANCED_INDIRECT,
                    DRAW_INSTANCED,
                    DRAW_INSTANCED_INDIRECT,
                };
            };

            struct Shader_Input{
                EVOLUTION::GRAPHIC::IShaderResource* mp_shader_resource_view[D3D11CONST_VALUE::COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
                EVOLUTION::GRAPHIC::ConstantBuffer* mp_constantbuffer[D3D11CONST_VALUE::COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
                EVOLUTION::GRAPHIC::SamplerState* mp_sampler_state[D3D11CONST_VALUE::COMMONSHADER_SAMPLER_SLOT_COUNT];
            };

            struct Command_DATA{
                EVOLUTION::GRAPHIC::Buffer* mp_index_buffer;
                EVOLUTION::GRAPHIC::Buffer* mp_vertex_buffer[D3D11CONST_VALUE::NUM_VERTEX_INPUT_MAX_COUNT];
                
                EVOLUTION::GRAPHIC::ShaderChain* mp_shader_chain;
                EVOLUTION::GRAPHIC::FrameBuffer* mp_frame_buffer[D3D11CONST_VALUE::RENDER_TARGET_COUNT];
                EVOLUTION::GRAPHIC::DepthBuffer* mp_deps_buffer;

                EVOLUTION::GRAPHIC::DepthStencilState* mp_depth_stencil_state;
                EVOLUTION::GRAPHIC::BlendState* mp_blend_state;
                EVOLUTION::GRAPHIC::RasterrizerState* mp_rasterrizer_state;

                Shader_Input m_shader_input[D3D11CONST_VALUE::SHADER_TYPE_NUM];

                EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY m_primitive_topology;
                u32 m_vertex_or_index_count;
                u32 m_start_vertex_or_index_location;
                u32 m_instance_count;
                s32 m_bass_vertex_location;
                u32 m_start_instance_location;
                DRAW_MODE::_MODE m_draw_mode;
            }m_command_data;

        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            RenderingPipeline(IGraphicCommand* graphic_command);
            ~RenderingPipeline();

            //グラフィックコマンドの設定
            void SetGraphicCommand(IGraphicCommand* command);
            //インデクスバッファの設定
            void SetIndexBuffer(IBuffer* index_buffer);
            //頂点バッファの設定
            void SetVertexBuffer(IBuffer* vertex_buffer);
            //頂点バッファの設定
            void SetVertexBuffer(IBuffer* vertex_buffer, u32 slot_number);
            //テクスチャの設定
            void SetTexture(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, ITexture* texture);
            //コンスタントバッファまたはユニフォーム用バッファの設定
            void SetBuffer(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, IBuffer* buffer);
            //サンプラーステートの設定
            void SetSamplerState(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, ISamplerState* mp_sampler_state);
            //ラスタライザを設定
            void SetRasterrizer(IRasterrizerState* rasterrizer_state);
            //ブレンドステートを設定
            void SetBlendState(IBlendState* blend_state);
            //デプスステンシルを設定
            void SetDepthStencilState(IDepthStencilState* depth_stencil_state);
            //フレームバッファを設定
            void SetFrameBuffer(IFrameBuffer* frame_buffer);
            //フレームバッファを設定
            void SetFrameBuffer(IFrameBuffer* frame_buffer, u32 slot_number);
            //シェーダーを設定
            void SetShader(IShaderChain* shader_chain);
            //レンダーターゲットの設定
            void SetRenderTarget(ITexture2D* render_target);
            //レンダーターゲットの設定
            void SetRenderTarget(ITexture2D* render_target, u32 slot_number);
            //デプスバッファの設定
            void SetDepthBuffer(ITexture2D* depth_buffer);
            //ドローコール
            void Draw(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 vertex_count, u32 start_vertex_location);
            //ドローコール
            void DrawIndexed(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 index_cout, u32 start_index_location, s32 base_vertex_location);
            //ドローコール
            void DrawIndexedInstanced(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 index_cout, u32 instance_count, u32 start_index_location, s32 base_vertex_location, u32 start_instance_location);
            //Clear
            void Clear();

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_property, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

        };
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {FD5EBF65-F5C2-4b77-A8B9-FB9968609E71}
        static const EVOLUTION_IID IID_RenderingPipeline =
        { 0xfd5ebf65, 0xf5c2, 0x4b77, { 0xa8, 0xb9, 0xfb, 0x99, 0x68, 0x60, 0x9e, 0x71 } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_COMMAND_H__