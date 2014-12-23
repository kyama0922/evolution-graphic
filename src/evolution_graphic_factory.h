#ifndef __EVOLUTION_GRAPHIC_FACTORY_H__
#define __EVOLUTION_GRAPHIC_FACTORY_H__

#include "evolution_graphic_interface_implement.h"


namespace EVOLUTION{
    namespace GRAPHIC{
        class ShaderManager;
        //--------------------------------------
        //グラフィックファクトリークラス
        //--------------------------------------
        class GraphicFactory : public IGraphicFactory{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;

            GraphicHandle m_handle;

            GraphicManager* mp_graphic_manager;

            ShaderManager* mp_shader_manager;

            EVOLUTION::UTILITY::Collection m_unknown_correction;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            GraphicFactory();
            ~GraphicFactory();

            GraphicResult::_RESULT Create(HWND hwnd, s32 width, s32 heigth, s32 work_thread_count);

            //テクスチャの作成
            GraphicResult::_RESULT CreateTexture2D(ITexture2D** texture2d, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type);
            //デプスバッファの作成
            GraphicResult::_RESULT CreateDepthBuffer(IDepthBuffer** depth_buffer, IGraphicCommand* graphic_command, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type);
            //フレームバッファの作成(アプリケーションのフレームバッファの作成)
            GraphicResult::_RESULT CreateFrameBuffer(IFrameBuffer** frame_buffer, IGraphicCommand* graphic_command);
            //フレームバッファの作成
            GraphicResult::_RESULT CreateFrameBuffer(IFrameBuffer** frame_buffer, IGraphicCommand* graphic_command, s32 width, s32 heigth, FORMAT_TYPE::_FORMAT_TYPE format);
            //バッファの作成
            GraphicResult::_RESULT CreateVertexBuffer(IBuffer** buffer, IGraphicCommand* graphic_command, const void* index_buffer_address, u32 buffer_count, u32 single_size);
            //インデックスバッファの作成
            GraphicResult::_RESULT CreateIndexBuffer(IBuffer** buffer, IGraphicCommand* graphic_command, const u16* index_buffer_address, u32 buffer_count);
            //インデックスバッファの作成
            GraphicResult::_RESULT CreateIndexBuffer(IBuffer** buffer, IGraphicCommand* graphic_command, const u32* index_buffer_address, u32 buffer_count);
            //レンダリングパイプライン作成
            GraphicResult::_RESULT CreateRenderingPipeline(IRenderingPipeline** rendering_pipeline, IGraphicCommand* graphic_command);
            //グラフィックコマンド作成
            GraphicResult::_RESULT CreateGraphicCommand(IGraphicCommand** graphic_command, u32 queue_size, u32 buffer_byte);
            //シェーダーパイプラインの作成
            GraphicResult::_RESULT CreateShaderChain(IShaderChain** shader_chain, SHADER_INPUT_LAYOUT* input_layout, u32 input_layout_count, SHADER_PROPERTY* shader_property, u32 property_count);
            //TextureLoaderの作成
            GraphicResult::_RESULT CreateTextureLoader(ITextureLoader** pp_texture_loader);
            //SamplerStateの作成
            GraphicResult::_RESULT CreateSamplerState(ISamplerState** pp_sampler_state, const SAMPLER_DESC& desc);
            //BlendStateの作成
            GraphicResult::_RESULT CreateBlendState(IBlendState** pp_blend_state);
            //ラスタライザーの作成
            GraphicResult::_RESULT CreateRasterrizerState(IRasterrizerState** pp_rasterrizer_state);
            //ステンシルステートの作成
            GraphicResult::_RESULT CreateDepthStencilState(IDepthStencilState** pp_stencil_state);
            //コンスタントバッファの作成
            GraphicResult::_RESULT CreateConstantBuffer(IConstantBuffer** pp_constant_buffer, IGraphicCommand* graphic_command, const CONSTANT_BUFFER_LAYOUT* layout, s32 count, bool dynamic);

            //グラフィックコレクションを取得
            EVOLUTION::UTILITY::Collection& GetGraphicUnknownCollection();

        };
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {F02748F5-C35B-425a-A18D-D6A0AE739453}
        static const EVOLUTION_IID IID_GraphicFactory =
        { 0xf02748f5, 0xc35b, 0x425a, { 0xa1, 0x8d, 0xd6, 0xa0, 0xae, 0x73, 0x94, 0x53 } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_COMMAND_H__