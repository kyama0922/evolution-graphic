#ifndef __EVOLUTION_GRAPHIC_RESULT_H__
#define __EVOLUTION_GRAPHIC_RESULT_H__


namespace EVOLUTION{
    namespace GRAPHIC{

        struct GraphicResult{
            enum _RESULT{
                //----------------------------成功----------------------------
                RESULT_OK = 0x00000000,//成功

                //----------------------------失敗----------------------------
                UNKNOWN_FAILED = 0x80000000, //失敗

                NONE_INSTANCE,               //インスタンスがありません

                CREATE_FAILED,    //作成に失敗

                CAST_SHADER_INPUT_LAYOUT,    //インプットレイアウトにキャスト失敗
                CAST_SHADER_VERTEX_SHADER,   //頂点シェーダーにキャスト失敗
                CAST_SHADER_DOMAIN_SHADER,   //ドメインシェーダーにキャスト失敗
                CAST_SHADER_HULL_SHADER,      //HULLシェーダーにキャスト失敗
                CAST_SHADER_GEOMETRY_SHADER, //ジオメトリシェーダーにキャスト失敗
                CAST_SHADER_PIXEL_SHADER,    //ピクセルまたはフラグメントシェーダーにキャスト失敗
                CAST_SHADER_COMPUTE_SHADER,  //計算シェーダーにキャスト失敗

                CAST_TEXTURE_LOADHANDLE,     //テクスチャローダーにキャスト失敗


                TEXTURE_LOAD_ERROR,  //テクスチャーを読み込めませんでした。
            };
        };

        struct GraphicException{
            enum _EXCEPTION{
                NONE_EXCEPTION,         //ERRORはありません。
                CREATE_DXGIFACTORY,     //DXGIファクトリーの作成失敗
                CREATE_DXGIADAPTER,     //DXGIアダプターの作成失敗
                CREATE_D3D11DEVICE,     //スワップチェインの作成失敗
                CREATE_SWAPCHAIN,       //グラフィックファクトリーの作成失敗
                CREATE_RENDERTARGETVIEW,//レンダーターゲットの作成失敗
                CREATE_SHADERRESOURCEVIEW,//シェーダーリソースビューの作成失敗
                CREATE_DEPTHSTENCILVIEW,//デプスステンシルビューの作成失敗
                
                CREATE_INPUT_LAYOUT,    //インプットレイアウト作成失敗
                CREATE_VERTEX_SHADER,   //頂点シェーダー作成失敗
                CREATE_DOMAIN_SHADER,   //ドメインシェーダー作成失敗
                CREATE_HULL_SHADER,     //HULLシェーダー作成失敗
                CREATE_GEOMETRY_SHADER, //ジオメトリシェーダー作成失敗
                CREATE_PIXEL_SHADER,    //ピクセルまたはフラグメントシェーダー作成失敗
                CREATE_COMPUTE_SHADER,  //計算シェーダー作成失敗

                CREATE_TEXTURE1D,       //テクスチャー(1D)の作成失敗
                CREATE_TEXTURE2D,       //テクスチャー(2D)の作成失敗
                CREATE_TEXTURE3D,       //テクスチャー(3D)の作成失敗

                CREATE_SAMPLER_STATE,   //SamplerStateの作成失敗
                
                GET_BUFFER,//バッファの取得に作成失敗

                CREATE_D3D11BUFFER,//D3D11BUFFERの作成失敗

                COMPILE_SHADER,//D3D11BUFFERの作成失敗
                
            };
        };
    }

    namespace FUNCTION{
        //成功したかの検証
        inline bool IsGraphicSucceeded(GRAPHIC::GraphicResult::_RESULT hr){
            return (((GRAPHIC::GraphicResult::_RESULT)(hr)) >= 0);
        }

        //失敗したかの検証
        inline bool IsGraphicFailed(GRAPHIC::GraphicResult::_RESULT hr){
            return (((GRAPHIC::GraphicResult::_RESULT)(hr)) < 0);
        }

        //失敗したらthrowする
        inline void IsGraphicFailedThrow(GRAPHIC::GraphicResult::_RESULT hr){
            if (((GRAPHIC::GraphicResult::_RESULT)(hr)) < 0)
            {
                throw;
            }
        }
    }
}

#endif // !__EVOLUTION_GRAPHIC_RESULT_H__