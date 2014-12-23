#ifndef __EVOLUTION_GRAPHIC_INTERFACE_IMPLEMENT_H__
#define __EVOLUTION_GRAPHIC_INTERFACE_IMPLEMENT_H__

#include <evolution.h>

#include <evolution-utility/evolution_utility.h>

#include <d3d11.h>
#include <d3dcompiler.h>

//グラフィックインターフェイス
#include "../evolution_graphic_interface.h"

//グラフィックException
#include "evolution_graphic_exception.h"



namespace EVOLUTION{
    namespace GRAPHIC{

        struct D3D11CONST_VALUE{
            enum _VALUE{
                NUM_VERTEX_INPUT_MAX_COUNT = 32,   //頂点バッファの最大入力カウント
                COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT = 128,//インプットシェーダーリソースの最大スロットカウント
                COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT = 14,//コンスタントバッファの最大スロットカウント
                COMMONSHADER_SAMPLER_SLOT_COUNT = 16,//サンプラーの最大スロットカウント
                SHADER_TYPE_NUM = 5,//シェーダーの総数
                PS_CS_UAV_REGISTER_COUNT = 8,//アンオーダーアクセスビュー最大スロットカウント
                RENDER_TARGET_COUNT = 8,//レンダーターゲット数
                //DEFAULT_STENCIL_READ_MASK = 0xff,
                //DEFAULT_STENCIL_WRITE_MASK = 0xff,
            };
        };

        typedef struct CommandPropertyTag{
            ptr_size_t val_size;
            ptr_t val_ptr;
        }CommandProperty;


        typedef struct CommandTag{
            class IGraphicD3DExecute* mp_graphic_d3d_execute;
            CommandProperty m_property;
        }Command;

        //コマンド実行用インターフェイス
        class IGraphicD3DExecute{
        public:
            //インターフェイスの参照カウントを 1 ずつ増やす。
            virtual u32 AddRef() = 0;
            //オブジェクトが特定の COM インターフェイスをサポートしているかどうかを判別する。インターフェイスをサポートしている場合、システムはオブジェクトの参照カウントを増やす。アプリケーションは、そのインターフェイスをすぐに使える。
            virtual RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject) = 0;
            //インターフェイスの参照カウントを 1 ずつ減らす。
            virtual u32 Release() = 0;
            //Direct3Dコマンドの実行
            virtual GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain) = 0;
        };


        class IShaderResource{
        public:
            //インターフェイスの参照カウントを 1 ずつ増やす。
            virtual u32 AddRef() = 0;
            //オブジェクトが特定の COM インターフェイスをサポートしているかどうかを判別する。インターフェイスをサポートしている場合、システムはオブジェクトの参照カウントを増やす。アプリケーションは、そのインターフェイスをすぐに使える。
            virtual RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject) = 0;
            //インターフェイスの参照カウントを 1 ずつ減らす。
            virtual u32 Release() = 0;

            virtual ID3D11Resource* GetResource()const = 0;
            virtual ID3D11ShaderResourceView* GetShaderResourceView()const = 0;
        };
        //-------------------------------------------------
        //コマンド実行用インターフェイス
        //-------------------------------------------------
        class IGraphicD3DExecute;

        //-------------------------------------------------
        //Buffer 
        //-------------------------------------------------
        class Buffer;
        //-------------------------------------------------
        //ConstantBuffer 
        //-------------------------------------------------
        class ConstantBuffer;
        //-------------------------------------------------
        //デプスバッファ
        //-------------------------------------------------
        class DepthBuffer;

        //-------------------------------------------------
        //グラフィックファクトリークラス
        //-------------------------------------------------
        class GraphicFactory;

        //-------------------------------------------------
        //GraphicCommand
        //-------------------------------------------------
        class GraphicCommand;

        //-------------------------------------------------
        //フレームバッファ 
        //-------------------------------------------------
        class FrameBuffer;

        //-------------------------------------------------
        //Graphic管理クラス
        //-------------------------------------------------
        class GraphicManager;

        //-------------------------------------------------
        //レンダリングパイプライン(Direct3D 9　GL2.0 ES)
        //-------------------------------------------------
        class RenderingPipeline;

        //-------------------------------------------------
        //二次元テクスチャ
        //-------------------------------------------------
        class Texture2D;

        //-------------------------------------------------
        //シェーダー
        //-------------------------------------------------
        class ShaderChain;

        //--------------------------------------
        //TextureのロードHandle
        //--------------------------------------
        class ITextureLoadHandle;

        //--------------------------------------
        //SamplerState
        //--------------------------------------
        class SamplerState;

        //--------------------------------------
        //グラフィックファクトリークラス
        //--------------------------------------
        class TextureLoader;

        //--------------------------------------
        //ラスタライザー
        //--------------------------------------
        class RasterrizerState;

        //--------------------------------------
        //ラスタライザー
        //--------------------------------------
        class BlendState;

        //--------------------------------------
        //DepthStencil
        //--------------------------------------
        class DepthStencilState;
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {1E2078CA-7F59-4940-A22B-8BF1A5E98B65}
        static const EVOLUTION_IID IID_IGraphicD3DExecute =
        { 0x1e2078ca, 0x7f59, 0x4940, { 0xa2, 0x2b, 0x8b, 0xf1, 0xa5, 0xe9, 0x8b, 0x65 } };

        // {EE26CBD4-13B3-41f6-AE99-641618317F62}
        static const EVOLUTION_IID IID_IShaderResource =
        { 0xee26cbd4, 0x13b3, 0x41f6, { 0xae, 0x99, 0x64, 0x16, 0x18, 0x31, 0x7f, 0x62 } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_INTERFACE_H__