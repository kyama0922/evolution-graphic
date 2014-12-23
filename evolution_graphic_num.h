#ifndef __EVOLUTION_GRAPHIC_NUM_H__
#define __EVOLUTION_GRAPHIC_NUM_H__

namespace EVOLUTION{
    namespace GRAPHIC{

        //--------------------------------------------
        //フォーマット
        //--------------------------------------------
        struct FORMAT_TYPE{
            enum _FORMAT_TYPE{
                _UNKNOWN = 0,
                _R32G32B32A32_TYPELESS = 1,
                _R32G32B32A32_FLOAT = 2,
                _R32G32B32A32_UINT = 3,
                _R32G32B32A32_SINT = 4,
                _R32G32B32_TYPELESS = 5,
                _R32G32B32_FLOAT = 6,
                _R32G32B32_UINT = 7,
                _R32G32B32_SINT = 8,
                _R16G16B16A16_TYPELESS = 9,
                _R16G16B16A16_FLOAT = 10,
                _R16G16B16A16_UNORM = 11,
                _R16G16B16A16_UINT = 12,
                _R16G16B16A16_SNORM = 13,
                _R16G16B16A16_SINT = 14,
                _R32G32_TYPELESS = 15,
                _R32G32_FLOAT = 16,
                _R32G32_UINT = 17,
                _R32G32_SINT = 18,
                _R8G8B8A8_TYPELESS = 27,
                _R8G8B8A8_UNORM = 28,
                _R8G8B8A8_UNORM_SRGB = 29,
                _R8G8B8A8_UINT = 30,
                _R8G8B8A8_SNORM = 31,
                _R8G8B8A8_SINT = 32,
                _R16G16_TYPELESS = 33,
                _R16G16_FLOAT = 34,
                _R16G16_UNORM = 35,
                _R16G16_UINT = 36,
                _R16G16_SNORM = 37,
                _R16G16_SINT = 38,
                _R32_TYPELESS = 39,
                _D32_FLOAT = 40,
                _R32_FLOAT = 41,
                _R32_UINT = 42,
                _R32_SINT = 43,
                _R24G8_TYPELESS = 44,
                _D24_UNORM_S8_UINT = 45,
                _R24_UNORM_X8_TYPELESS = 46,
                _R8G8_TYPELESS = 48,
                _R8G8_UNORM = 49,
                _R8G8_UINT = 50,
                _R8G8_SNORM = 51,
                _R8G8_SINT = 52,
                _R16_TYPELESS = 53,
                _R16_FLOAT = 54,
                _D16_UNORM = 55,
                _R16_UNORM = 56,
                _R16_UINT = 57,
                _R16_SNORM = 58,
                _R16_SINT = 59,
                _R8_TYPELESS = 60,
                _R8_UNORM = 61,
                _R8_UINT = 62,
                _R8_SNORM = 63,
                _R8_SINT = 64,
                _A8_UNORM = 65,
                _R1_UNORM = 66,
                _B5G6R5_UNORM = 85,
                _B5G5R5A1_UNORM = 86,
                _B8G8R8A8_UNORM = 87,
                _B8G8R8X8_UNORM = 88,
                _B8G8R8A8_TYPELESS = 90,
                _B8G8R8A8_UNORM_SRGB = 91,
                _B8G8R8X8_TYPELESS = 92,
                _B8G8R8X8_UNORM_SRGB = 93,
            };
        };

        struct PRIMITIVE_TOPOLOGY{
            enum _PRIMITIVE_TOPOLOGY
            {
                _UNDEFINED = 0,
                _POINTLIST = 1,
                _LINELIST = 2,
                _LINESTRIP = 3,
                _TRIANGLELIST = 4,
                _TRIANGLESTRIP = 5,
            };
        };

        //--------------------------------------------------------------------------------------------------------------------------------------------
        //ブレンドステート

        //ブレンディング オプションです。ブレンディング オプションは、データ ソース、および任意指定のブレンディング前の処理を識別します。
        struct BLEND{
            enum _TYPE{
                ZERO = 1,			//データ ソースの色は黒 (0, 0, 0, 0) です。ブレンディング前の処理はありません。
                ONE = 2,			//データ ソースの色は白 (1, 1, 1, 1) です。ブレンディング前の処理はありません。
                SRC_COLOR = 3,		//データ ソースは、ピクセル シェーダーからのカラー データ (RGB) です。ブレンディング前の処理はありません。
                INV_SRC_COLOR = 4,	//データ ソースは、ピクセル シェーダーからのカラー データ (RGB) です。ブレンディング前の処理によってデータが反転され、1 - RGB が生成されます。
                SRC_ALPHA = 5,		//データ ソースは、ピクセル シェーダーからのアルファ データ (A) です。ブレンディング前の処理はありません。
                INV_SRC_ALPHA = 6,	//データ ソースは、ピクセル シェーダーからのアルファ データ (A) です。ブレンディング前の処理によってデータが反転され、1 - A が生成されます。
                DEST_ALPHA = 7,		//データ ソースは、レンダー ターゲットからのアルファ データです。ブレンディング前の処理はありません。
                INV_DEST_ALPHA = 8,	//データ ソースは、レンダー ターゲットからのアルファ データです。ブレンディング前の処理によってデータが反転され、1 - A が生成されます。
                DEST_COLOR = 9,		//データ ソースは、レンダー ターゲットからのカラー データです。ブレンディング前の処理はありません。
                INV_DEST_COLOR = 10,//データ ソースは、レンダー ターゲットからのカラー データです。ブレンディング前の処理によってデータが反転され、1 - RGB が生成されます。
                SRC_ALPHA_SAT = 11,	//データ ソースは、ピクセル シェーダーからのアルファ データです。ブレンディング前の処理によってデータが 1 以下にクランプされます。 
                BLEND_FACTOR = 14,	//データ ソースは、ID3D11DeviceContext::OMSetBlendState で設定されたブレンディング係数です。ブレンディング前の処理はありません。
                INV_BLEND_FACTOR = 15,//データ ソースは、ID3D11DeviceContext::OMSetBlendState で設定されたブレンディング係数です。ブレンディング前の処理によってブレンディング係数が反転され、1 - blend_factor が生成されます。
                SRC1_COLOR = 16,	//データ ソースは、ピクセル シェーダーによって出力された両方のカラー データです。ブレンディング前の処理はありません。このオプションは、デュアル ソースのカラー ブレンディングをサポートします。
                INV_SRC1_COLOR = 17,//データ ソースは、ピクセル シェーダーによって出力された両方のカラー データです。ブレンディング前の処理によってデータが反転され、1 - RGB が生成されます。このオプションは、デュアル ソースのカラー ブレンディングをサポートします
                SRC1_ALPHA = 18,	//データ ソースは、ピクセル シェーダーによって出力されたアルファ データです。ブレンディング前の処理はありません。このオプションは、デュアル ソースのカラー ブレンディングをサポートします。
                INV_SRC1_ALPHA = 19	//データ ソースは、ピクセル シェーダーによって出力されたアルファ データです。ブレンディング前の処理によってデータが反転され、1 - A が生成されます。このオプションは、デュアル ソースのカラー ブレンディングをサポートします。
            };
        };

        //RGB またはアルファのブレンディング処理です。
        struct BLEND_OP{
            enum _OP{
                ADD = 1,			//ソース 1 とソース 2 を加算します。
                SUBTRACT = 2,		//ソース 1 からソース 2 を減算します。
                REV_SUBTRACT = 3,	//ソース 1 からソース 2 を減算します。
                MIN = 4,			//ソース 1 とソース 2 の最小値を選択します。
                MAX = 5				//ソース 1 とソース 2 の最大値を選択します。
            };
        };

        //レンダー ターゲットの各ピクセルのどの成分がブレンド時に書き込み可能になるかを識別します。
        struct COLOR_WRITE_ENABLE{
            enum _TYPE{
                COLOR_WRITE_ENABLE_RED = 1,		//赤成分へのデータの格納を許可します。
                COLOR_WRITE_ENABLE_GREEN = 2,	//緑成分へのデータの格納を許可します。
                COLOR_WRITE_ENABLE_BLUE = 4,	//青成分へのデータの格納を許可します。
                COLOR_WRITE_ENABLE_ALPHA = 8,	//アルファ成分へのデータの格納を許可します。
                COLOR_WRITE_ENABLE_ALL = (COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN | COLOR_WRITE_ENABLE_BLUE | COLOR_WRITE_ENABLE_ALPHA) //すべての成分へのデータの格納を許可します。
            };
        };

        //ブレンドステートプロパティ
        struct BLEND_STATE_PROPERTY{
            enum _TYPE{
                ALPHATOCOVERAGE, //ピクセルをレンダー ターゲットに設定するときに、アルファトゥカバレッジをマルチサンプリング テクニックとして使用するかどうかを決定します。
                INDEPENDENTBLEND,//同時処理のレンダー ターゲットで独立したブレンディングを有効にするには、TRUE に設定します。FALSE に設定すると、RenderTarget[0] のメンバーのみが使用されます。RenderTarget[1..7] は無視されます。
            };
            enum _OP{
                BLEND_OP,
                BLEND_OP_ALPHA,
            };
            enum _OBJECT{
                SRC,
                DEST,
                SRCALPHA,
                DESTALPHA,
            };
        };

        //ブレンドステート
        struct BLEND_STATE{
            enum _TYPE{
                DEFAULT,
                ADD,
                SUB,
                MULTIPLE,
                ALPHA_BLEND,
            };
        };


        //--------------------------------------------------------------------------------------------------------------------------------------------
        //ラスタライザー

        //三角形のレンダリング時に使用する描画モードを決定します。
        struct FILL_MODE{
            enum _MODE{
                WIREFRAME = 2,	//頂点を結ぶ線を描画します。隣接する頂点は描画されません。
                SOLID = 3		//頂点によって形成された三角形を塗りつぶします。隣接する頂点は描画されません。
            };
        };

        //特定の方向を向いている三角形の描画の有無を示します。
        struct CULL_MODE{
            enum _MODE{
                NONE = 1,	//常にすべての三角形を描画します。
                FRONT = 2,	//前向きの三角形を描画しません。
                BACK = 3	//後ろ向きの三角形を描画しません。
            };
        };

        //IRasterrizer設定プロパティ
        struct RASTERRIZER_PROPERTY_TYPE{
            enum _TYPE1{
                DEPTHBIASCLAMP,//ピクセルの最大深度バイアスです。
                SLOPESCALEDDEPTHBIAS//指定のピクセルのスロープに対するスカラです。
            };

            enum _TYPE2{
                FRONTCOUNTERCLOCKWISE, // 三角形が前向きか後ろ向きかを決定します。このパラメーターが true の場合、三角形の頂点がレンダー ターゲット上で左回りならば三角形は前向きと見なされ、右回りならば後ろ向きと見なされます。このパラメーターが false の場合は逆になります。
                DEPTHCLIP,//距離に基づいてクリッピングを有効にします。
                SCISSOR,//シザー矩形カリングを有効にします。アクティブなシザー矩形の外側のピクセルはすべてカリングされます。
                MULTISAMPLE,//マルチサンプリングのアンチエイリアシングを有効にします。
                ANTIALIASEDLINE,//線のアンチエイリアシングを有効にします。線を描画中で Multisample が false の場合にのみ適用されます。
            };
        };

        //--------------------------------------------------------------------------------------------------------------------------------------------
        //サンプラーステート

        struct FILTER{
            enum _FILTER_TYPE{
                MIN_MAG_MIP_POINT = 0,//縮小、拡大、およびミップレベルのサンプリングでポイント サンプリングを使用します。
                MIN_MAG_POINT_MIP_LINEAR = 0x1,//縮小、拡大にポイント サンプリングを使用し、ミップレベルのサンプリングに線形補間を使用します。
                MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,//縮小にポイント サンプリングを使用し、拡大に線形補間を使用し、ミップレベルのサンプリングにポイント サンプリングを使用します。
                MIN_POINT_MAG_MIP_LINEAR = 0x5,//縮小にポイント サンプリングを使用し、拡大およびミップレベルのサンプリングに線形補間を使用します。
                MIN_LINEAR_MAG_MIP_POINT = 0x10,//縮小に線形補間を使用し、拡大およびミップレベルのサンプリングにポイント サンプリングを使用します。
                MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,//縮小に線形補間を使用し、拡大にポイント サンプリングを使用し、ミップレベルのサンプリングに線形補間を使用します。
                MIN_MAG_LINEAR_MIP_POINT = 0x14,//縮小および拡大に線形補間を使用し、ミップレベルのサンプリングにポイント サンプリングを使用します。
                MIN_MAG_MIP_LINEAR = 0x15,//縮小、拡大、およびミップレベルのサンプリングに線形補間を使用します。
                ANISOTROPIC = 0x55,//縮小、拡大、およびミップレベルのサンプリングに異方性補間を使用します
                COMPARISON_MIN_MAG_MIP_POINT = 0x80,//縮小、拡大、およびミップレベルのサンプリングでポイント サンプリングを使用します。結果を比較値と比較します。
                COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,//縮小、拡大にポイント サンプリングを使用し、ミップレベルのサンプリングに線形補間を使用します。結果を比較値と比較します。
                COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,//縮小にポイント サンプリングを使用し、拡大に線形補間を使用し、ミップレベルのサンプリングにポイント サンプリングを使用します。結果を比較値と比較します。
                COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,//縮小にポイント サンプリングを使用し、拡大およびミップレベルのサンプリングに線形補間を使用します。結果を比較値と比較します。
                COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,//縮小に線形補間を使用し、拡大およびミップレベルのサンプリングにポイント サンプリングを使用します。結果を比較値と比較します。
                COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,//縮小に線形補間を使用し、拡大にポイント サンプリングを使用し、ミップレベルのサンプリングに線形補間を使用します。結果を比較値と比較します。
                COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,//縮小および拡大に線形補間を使用し、ミップレベルのサンプリングにポイント サンプリングを使用します。結果を比較値と比較します。
                COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,//縮小、拡大、およびミップレベルのサンプリングに線形補間を使用します。結果を比較値と比較します。
                COMPARISON_ANISOTROPIC = 0xd5,//縮小、拡大、およびミップレベルのサンプリングに異方性補間を使用します。結果を比較値と比較します。
                TEXT_1BIT = 0x80000000,//R1_UNORM フォーマットのテクスチャーを持つピクセル シェーダーで使用します。
            };
        };

        struct TEXTURE_ADDRESS{
            enum _MODE{
                WRAP = 1,       //(u,v) 整数区切りごとにテクスチャーを繰り返します。たとえば、u の値が 0 から 3 までの場合、テクスチャーが 3 回繰り返されます。
                MIRROR = 2,     //(u,v) 整数区切りごとにテクスチャーを反転させます。たとえば、u の値が 0 から 1 までの場合、テクスチャーの処理が通常どおりに行われますが、1 から 2 までの場合は反転表示 (ミラー化) され、2 から 3 までの場合は再び通常の処理が行われます。これ以降も、このパターンで処理されます。 
                CLAMP = 3,      //範囲 [0.0, 1.0] の外にあるテクスチャー座標には、それぞれ 0.0 または 1.0 のテクスチャー カラーが設定されます。
                BORDER = 4,     //範囲 [0.0, 1.0] の外にあるテクスチャー座標には、D3D11_SAMPLER_DESC または HLSL コードで指定した境界色が設定されます。
                MIRROR_ONCE = 5,//D3D11_TEXTURE_ADDRESS_MIRROR および D3D11_TEXTURE_ADDRESS_CLAMP と似ています。テクスチャー座標の絶対値を取得し (0 を中心としたミラーリング)、最大値に固定します。
            };
            enum _INSTANCE{
                U,
                V,
                W
            };
        };

        struct SAMPLER_PROPERTY{
            enum _PROPERTY{
                MIPLODBIAS,
                MINLOD,
                MAXLOD,
            };
        };

        struct COMPARISON_FUNC{
            enum _FUNC{
                NEVER = 1, //比較は常に不合格です。
                LESS = 2, //ソース データが対象データよりも小さい場合、比較に合格します。
                EQUAL = 3,//ソース データが対象データと等しい場合、比較に合格します。
                LESS_EQUAL = 4,//ソース データが対象データよりも小さいか等しい場合、比較に合格します。
                GREATER = 5,//ソース データが対象データよりも大きい場合、比較に合格します。
                NOT_EQUAL = 6,//ソース データが対象データと等しくない場合、比較に合格します。
                GREATER_EQUAL = 7,//ソース データが対象データよりも大きいか等しい場合、比較に合格します。
                ALWAYS = 8,//比較は常に合格です。
            };
        };

        //--------------------------------------------------------------------------------------------------------------------------------------------
        //デプスステンシル

        //深度ステンシル バッファーの中で、深度データを書き込む部分を識別します。
        struct DEPTH_WRITE_MASK{
            enum _MASK{
                ZERO = 0,//深度ステンシル バッファーへの書き込みをオフにします。
                ALL = 1, //深度ステンシル バッファーへの書き込みをオンにします。
            };
        };

        //深度ステンシル テスト時に実行可能なステンシル処理です。
        struct STENCIL_OP{
            enum _OP{
                KEEP = 1,	//既存のステンシル データを保持します。
                ZERO = 2,	//ステンシル データを 0 に設定します。
                REPLACE = 3,//ステンシル データを、ID3D11DeviceContext::OMSetDepthStencilState を呼び出して設定された参照値に設定します。
                INCR_SAT = 4,//ステンシルの値を 1 増やし、その結果をクランプします。
                DECR_SAT = 5,//ステンシルの値を 1 減らし、その結果をクランプします。
                INVERT = 6,//ステンシル データを反転します。
                INCR = 7,//ステンシルの値を 1 増やし、必要に応じて結果をラップします。
                DECR = 8,//ステンシルの値を 1 増やし、必要に応じて結果をラップします。
            };
        };

        //ステンシル テストの結果に基づいて実行可能なステンシル処理です。
        struct DEPTH_STENCILOP_DESC{
            STENCIL_OP::_OP StencilFailOp;
            STENCIL_OP::_OP StencilDepthFailOp;
            STENCIL_OP::_OP StencilPassOp;
            COMPARISON_FUNC::_FUNC StencilFunc;
        };

        //DepthStencilStateプロパティ
        struct DEPTHSTENCILSTATE_PROPERTY{
            enum _TYPE{
                DEPTH,
                STENCIL,
            };

            enum _FORCE{
                Front,//FrontFace
                Back,//BackFace
            };

            enum _MASK{
                READ,
                WRITE
            };
        };

        struct DEPTH_CLEAR_FLAG{
            enum _FLAG{
               DEPTH = 0x1L,
               STENCIL = 0x2L,
               ALL = (DEPTH | STENCIL),
            };
        };

        //--------------------------------------------------------------------------------------------------------------------------------------------
        //シェーダー

        struct INPUT_CLASSIFICATION{
            enum _INPUT_CLASSIFICATION{
                VERTEX_DATA = 0,
                INSTANCE_DATA = 1,
            };
        };

        //シェーダーモデルの設定
        struct SHADER_MODEL{
            enum _SHADER_MODEL{
                _AUTO = 0,
                _5_0 = _AUTO,
                _4_1 = _5_0 + 1,
                _4_0 = _4_1 + 1,
                _4_0_LEVEL_9_3 = _4_0 + 1,
                _4_0_LEVEL_9_1 = _4_0_LEVEL_9_3 + 1,
            };
        };

        //シェーダーの種類
        struct SHADER_TYPE{
            enum _SHADER_TYPE{
                INPUT = -1,//インプットレイアウト
                VS = 0,//バーテックスシェーダー
                PS = 1,//ピクセルシェーダー
                GS = 2,//ジオメトリシェーダー
                DS = 3,//ドメインシェーダー
                HS = 4,//HULLシェーダー
                CS = 5,//コンピュートシェーダー
            };
        };

        /*シェーダーのプロパティ設定
        SHADER_PROPERTY shader_property[] = {
        { "ファイル名", "エントリーポイント", コンパイルモデル, シェーダータイプ },
        };
        例1)
        SHADER_PROPERTY shader_property[] = {
        { "VertexShader.hlsl", "main", EVOLUTION::GRAPHIC::SHADER_MODEL::_5_0, EVOLUTION::GRAPHIC::SHADER_TYPE::VS },
        { "PixelShader.hlsl", "main", EVOLUTION::GRAPHIC::SHADER_MODEL::_5_0, EVOLUTION::GRAPHIC::SHADER_TYPE::PS },
        };

        例2)
        SHADER_PROPERTY shader_property[] = {
        { "cs.hlsl", "main", EVOLUTION::GRAPHIC::SHADER_MODEL::_AUTO, EVOLUTION::GRAPHIC::SHADER_TYPE::CS },
        };
        */
        struct SHADER_PROPERTY{
            //シェーダーファイル名
            c8* filename;
            //エントリーポイント
            c8* entry_point;
            //コンパイルモデル
            SHADER_MODEL::_SHADER_MODEL shader_model;
            //シェーダータイプ
            SHADER_TYPE::_SHADER_TYPE shader_type;
        };

        /*シェーダーインプットレイアウト
        SHADER_INPUT_LAYOUT input[] = {
        { "セマンティクス名", セマンティクスインデックス, フォーマット, インプットソート, バイトオフセット, インプットソートクラス, インスタンス番号 },
        };
        例1)
        SHADER_INPUT_LAYOUT input[] = {
        { "POSITION", 0, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32B32_FLOAT, 0, 0, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::VERTEX_DATA, 0 },
        };
        例2)
        SHADER_INPUT_LAYOUT input[] = {
        { "POSITION", 0, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32B32_FLOAT, 0, 0, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::VERTEX_DATA, 0 },
        { "TEXTURE_UV", 0, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32_FLOAT, 0, 12, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::VERTEX_DATA, 0 },


        { "INSTANCE_MATRIX", 0, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32B32A32_FLOAT, 1,  0, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::INSTANCE_DATA, 1 },
        { "INSTANCE_MATRIX", 1, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32B32A32_FLOAT, 1, 16, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::INSTANCE_DATA, 1 },
        { "INSTANCE_MATRIX", 2, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32B32A32_FLOAT, 1, 32, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::INSTANCE_DATA, 1 },
        { "INSTANCE_MATRIX", 3, EVOLUTION::GRAPHIC::FROMAT_TYPE::_R32G32B32A32_FLOAT, 1, 48, EVOLUTION::GRAPHIC::INPUT_CLASSIFICATION::INSTANCE_DATA, 1 },
        };
        */
        struct SHADER_INPUT_LAYOUT{
            //セマンティクス名
            c8* semantic_name;
            //セマンティクスインデックス
            u32 semantic_index;
            //フォーマット
            FORMAT_TYPE::_FORMAT_TYPE format;
            //インプットソート
            u32 input_slot;
            //バイトオフセット
            u32 aligned_byte_offset;
            //インプットソートクラス
            INPUT_CLASSIFICATION::_INPUT_CLASSIFICATION input_slot_class;
            //インスタンス番号
            u32 instance_data_step_rate;
        };

        struct EXECUTE_COMMAND{
            enum _FLAG{
                NONE,//オプション指定なし
                END_FLIP,//コマンド終了時にフレームバッファをフリップします。
                END_COMMAND_CLEAR,//コマンド終了時にコマンドを消します。
                END_COMMAND_CLEAR_AND_FLIP,//マンド終了時にフレームバッファをフリップしてコマンドを消します。
            };
        };

        struct SAMPLER_DESC {
            //テクスチャーのサンプリング時に使用するフィルタリング メソッドです
            FILTER::_FILTER_TYPE Filter;
            //0 ～ 1 の範囲外にある u テクスチャー座標を解決するために使用されるメソッドです
            TEXTURE_ADDRESS::_MODE AddressU;
            //0 ～ 1 の範囲外にある v テクスチャー座標を解決するために使用されるメソッドです
            TEXTURE_ADDRESS::_MODE AddressV;
            //0 ～ 1 の範囲外にある w テクスチャー座標を解決するために使用されるメソッドです
            TEXTURE_ADDRESS::_MODE AddressW;
            //計算されたミップマップ レベルからのオフセットです。たとえば、Direct3D によってテクスチャーをミップマップ レベル 3 でサンプリングする必要があると計算された場合、MipLODBias を 2 にするとテクスチャーはミップマップ レベル 5 でサンプリングされます。
            f32 MipLODBias;
            //Filter に D3D11_FILTER_ANISOTROPIC または D3D11_FILTER_COMPARISON_ANISOTROPIC が指定されている場合に使用されるクランプ値です。有効な値は 1 ～ 16 です。
            u32 MaxAnisotropy;
            //既存のサンプリング データに対してデータを比較する関数です。
            COMPARISON_FUNC::_FUNC ComparisonFunc;
            //AddressU、AddressV、または AddressW に TEXTURE_ADDRESS が指定されている場合に使用される境界の色です。0.0 ～ 1.0 の範囲で指定する必要があります。
            f32 BorderColor[4];
            //アクセスをクランプするミップマップ範囲の下限です。0 は最大かつ最も詳細なミップマップ レベルを表し、レベルの値が大きくなるほど詳細でなくなります。
            f32 MinLOD;
            //アクセスをクランプするミップマップ範囲の上限です。0 は最大かつ最も詳細なミップマップ レベルを表し、レベルの値が大きくなるほど詳細でなくなります。この値は MinLOD 以上にする必要があります。LOD で上限を設定しない場合は、これを大きい値 (3.402823466e+38F (FLT_MAX)) に設定してください。
            f32 MaxLOD;
        };

        //ViewPort
        struct VIEW_PORT{
            f32 TopLeftX;
            f32 TopLeftY;
            f32 Width;
            f32 Height;
            f32 MinDepth;
            f32 MaxDepth;
        };

        typedef struct{
            s32 Left;
            s32 Top;
            s32 Right;
            s32 Bottom;
        }SCISSOR_RECT;

        struct CONSTANT_BUFFER_INSTANCE_TYPE{
            enum _TYPE{
                _BOOL,
                _SINT,
                _INT,
                _FLOAT,
                _FLOAT2,
                _FLOAT3,
                _FLOAT4,
                _VECTOR2,
                _VECTOR3,
                _VECTOR4,
                _MATRIX,
                _SINT_ARRAY,
                _INT_ARRAY,
                _FLOAT_ARRAY,
                _FLOAT2_ARRAY,
                _FLOAT3_ARRAY,
                _FLOAT4_ARRAY,
                _VECTOR2_ARRAY,
                _VECTOR3_ARRAY,
                _VECTOR4_ARRAY,
                _MATRIX_ARRAY,
            };
        };

        /*コンスタントバッファレイアウト
        例
        */
        struct CONSTANT_BUFFER_LAYOUT{
            //コンスタントバッファ型指定
            CONSTANT_BUFFER_INSTANCE_TYPE::_TYPE Type;
            //配列指定し場合のみ有効
            s32 ArrayCount;
        };
    }
}

#endif // !__EVOLUTION_GRAPHIC_INTERFACE_H__