#ifndef __EVOLUTION_GRAPHIC_INTERFACE_H__
#define __EVOLUTION_GRAPHIC_INTERFACE_H__

#include <evolution.h>
#include <evolution-utility/etc/evolution_color.h>

#include "evolution_graphic_num.h"

//result結果一覧
#include "evolution_graphic_result.h"
namespace EVOLUTION{
    namespace GRAPHIC{
        typedef UTILITY::Color Color;
        typedef u64 GraphicHandle;//グラフィックハンドル

        //----------------------------------
        //グラフィックオブジェクト
        //----------------------------------

        class IGraphicObject : public IUnknown{
            EVOLUTION_NOT_DESTRUCTOR(IGraphicObject);
        public:
            //グラフィックハンドルの取得
            virtual GraphicHandle GetGraphicHandle() = 0;
        };


        //-----------------------
        //GraphicCommandQueue
        //-----------------------
        class IGraphicCommand : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(IGraphicCommand);
        public:
            //コマンドプールの数
            virtual s32 CommandPoolSize() = 0;
            //登録コマンド数
            virtual u32 ResistorCommandCount() = 0;
            //トランザクションの作成
            virtual GraphicResult::_RESULT TransactionBegin() = 0;
            //トランザクション取り消す
            virtual GraphicResult::_RESULT TransactionRollBack() = 0;
            //トランザクションを確定させる
            virtual GraphicResult::_RESULT TransactionCommit() = 0;
            //コマンドの実行
            virtual GraphicResult::_RESULT Execute(EXECUTE_COMMAND::_FLAG flag) = 0;
            //非同期型の実行
            virtual GraphicResult::_RESULT AsyncExecute(EXECUTE_COMMAND::_FLAG flag) = 0;
        };

        //-----------------------------------------------------------------------------
        //             Resource  
        //-----------------------------------------------------------------------------

        //-----------------------
        //テクスチャ規定クラス
        //-----------------------
        class ITexture : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(ITexture);
        public:
            //テクスチャフォーマット
            virtual FORMAT_TYPE::_FORMAT_TYPE GetFormat() = 0;
        };

        //-----------------------
        //二次元テクスチャ
        //-----------------------
        class ITexture2D : public ITexture{
            EVOLUTION_NOT_DESTRUCTOR(ITexture2D);
        public:
            //幅
            virtual u32 GetWidth() = 0;
            //高さ
            virtual u32 GetHeigth() = 0;
        };

        //-----------------------
        //デプスバッファ
        //-----------------------
        class IDepthBuffer : public ITexture2D{
            EVOLUTION_NOT_DESTRUCTOR(IDepthBuffer);
        public:
            //デプスバッファのクリア
            virtual void Clear(DEPTH_CLEAR_FLAG::_FLAG flag, f32 depth, u8 stencil) = 0;
        };

        //-----------------------
        //フレームバッファ 
        //-----------------------
        class IFrameBuffer : public ITexture2D{
            EVOLUTION_NOT_DESTRUCTOR(IFrameBuffer);
        public:
            //グラフィックコマンドの設定
            virtual void SetGraphicCommand(IGraphicCommand* command) = 0;
            //クリアカラーの変更
            virtual void SetClearColor(f32 r, f32 g, f32 b, f32 a) = 0;
            //クリアカラーの変更
            virtual void SetClearColor(const Color& color) = 0;
            //レンダーターゲットのクリア
            virtual void Clear() = 0;
        };

        //-------------------------------
        //Buffer 
        //-------------------------------
        class IBuffer : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(IBuffer);
        public:
            //グラフィックコマンドの設定
            virtual void SetGraphicCommand(IGraphicCommand* command) = 0;
            //BufferのSingleサイズ
            virtual u32 GetSingleSize() = 0;
            //Bufferの配列サイズ
            virtual u32 GetArraySize() = 0;
            //Bufferアドレスの取得
            virtual void* GetBufferAddress() = 0;
            //設定した値を変更する
            virtual GraphicResult::_RESULT ChangeValue() = 0;
            //Bufferのコピー
            virtual GraphicResult::_RESULT Copy(IBuffer** buffer) = 0;
        };

        class IConstantBuffer : public IBuffer{
            EVOLUTION_NOT_DESTRUCTOR(IConstantBuffer);
        public:
            //バリューを設定する
            virtual void SetValueBool(s32 index, bool value) = 0;
            //バリューを設定する
            virtual void SetValueSInt(s32 index, s32 value) = 0;
            //バリューを設定する
            virtual void SetValueUInt(s32 index, u32 value) = 0;
            //バリューを設定する
            virtual void SetValueFloat(s32 index, f32 value) = 0;
            //バリューを設定する
            virtual void SetValueFloat2(s32 index, const f32(&value)[2]) = 0;
            //バリューを設定する
            virtual void SetValueFloat3(s32 index, const f32(&value)[3]) = 0;
            //バリューを設定する
            virtual void SetValueFloat4(s32 index, const f32(&value)[4]) = 0;
            //バリューを設定する
            virtual void SetValueVector2(s32 index, f32 x, f32 y) = 0;
            //バリューを設定する
            virtual void SetValueVector2(s32 index, const MATH::Vector2& value) = 0;
            //バリューを設定する
            virtual void SetValueVector3(s32 index, f32 x, f32 y, f32 z) = 0;
            //バリューを設定する
            virtual void SetValueVector3(s32 index, const MATH::Vector3& value) = 0;
            //バリューを設定する
            virtual void SetValueVector4(s32 index, f32 x, f32 y, f32 z, f32 w) = 0;
            //バリューを設定する
            virtual void SetValueVector4(s32 index, const MATH::Vector4& value) = 0;
            //バリューを設定する
            virtual void SetValueMatrix(s32 index, const MATH::Matrix& value) = 0;

            //バリューを設定する
            virtual void SetValueSIntArray(s32 index, const s32* value, s32 count) = 0;
            //バリューを設定する
            virtual void SetValueUIntArray(s32 index, const u32* value, s32 count) = 0;
            //バリューを設定する
            virtual void SetValueFloatArray(s32 index, const f32* value, s32 count) = 0;
            //バリューを設定する
            virtual void SetValueVector2Array(s32 index, const MATH::Vector2* value, s32 count) = 0;
            //バリューを設定する
            virtual void SetValueVector3Array(s32 index, const MATH::Vector3* value, s32 count) = 0;
            //バリューを設定する
            virtual void SetValueVector4Array(s32 index, const MATH::Vector4* value, s32 count) = 0;
            //バリューを設定する
            virtual void SetValueMatrixArray(s32 index, const MATH::Matrix* value, s32 count) = 0;
            //設定した値を変更する
            virtual  GraphicResult::_RESULT ChangeValue() = 0;
        };

        //-------------------------------
        //IShader 
        //-------------------------------
        class IShaderChain : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(IShaderChain);
        public:
        };

        //-------------------------------
        //SamplerState
        //-------------------------------
        class ISamplerState : public IGraphicObject {
            EVOLUTION_NOT_DESTRUCTOR(ISamplerState);
        public:
            virtual const SAMPLER_DESC& GetSampleDesc()const = 0;
        };


        //-------------------------------
        //SamplerState
        //-------------------------------
        class IBlendState : public IGraphicObject {
            EVOLUTION_NOT_DESTRUCTOR(IBlendState);
        public:
            virtual BLEND::_TYPE GetSrcBlend(u8 number) const = 0;
            virtual BLEND::_TYPE GetDestBlend(u8 number)const = 0;
            virtual BLEND_OP::_OP GetBlendOp(u8 number) const = 0;
            virtual BLEND::_TYPE GetSrcBlendAlpha(u8 number) const = 0;
            virtual BLEND::_TYPE GetDestBlendAlpha(u8 number)const = 0;
            virtual BLEND_OP::_OP GetBlendOpAlpha(u8 number) const = 0;
            virtual COLOR_WRITE_ENABLE::_TYPE GetRenderTargetWriteMask(u8 number)const = 0;
            virtual u32 GetMask()const = 0;
            virtual const f32* GetBlendFactor(f32(&blend_factor)[4])const = 0;

            virtual void SetProperty(BLEND_STATE_PROPERTY::_TYPE type, bool blend) = 0;
            virtual void SetProperty(BLEND_STATE_PROPERTY::_OP operation, u8 number, BLEND_OP::_OP op) = 0;
            virtual void SetProperty(BLEND_STATE_PROPERTY::_OBJECT object, u8 number, BLEND::_TYPE type) = 0;
            virtual void SetProperty(u8 number, COLOR_WRITE_ENABLE::_TYPE type) = 0;
            virtual void SetProperty(u8 number, bool blend_enable) = 0;

            virtual void SetMask(u32 mask) = 0;
            virtual void SetBlendFactor(const f32(&blend_factor)[4]) = 0;
            virtual GraphicResult::_RESULT ChangeProperty() = 0;
        };

        //-------------------------------
        //DepthStencilState
        //-------------------------------
        class IDepthStencilState : public IGraphicObject {
            EVOLUTION_NOT_DESTRUCTOR(IDepthStencilState);
        public:
            virtual bool IsDepth()const = 0;
            virtual DEPTH_WRITE_MASK::_MASK GetDepthWriteMask()const = 0;
            virtual COMPARISON_FUNC::_FUNC GetDepthFunc()const = 0;
            virtual bool IsStencil()const = 0;
            virtual u8 GetStencilReadMask()const = 0;
            virtual u8 GetStencilWriteMask()const = 0;
            virtual DEPTH_STENCILOP_DESC GetFrontFace()const = 0;
            virtual DEPTH_STENCILOP_DESC GetBackFace()const = 0;
            virtual u32 GetStencilRef()const = 0;

            virtual void SetProperty(DEPTHSTENCILSTATE_PROPERTY::_TYPE type, bool _property) = 0;
            virtual void SetProperty(COMPARISON_FUNC::_FUNC func) = 0;
            virtual void SetProperty(DEPTH_WRITE_MASK::_MASK mask) = 0;
            virtual void SetProperty(DEPTHSTENCILSTATE_PROPERTY::_MASK mask, u8 value) = 0;
            virtual void SetProperty(DEPTHSTENCILSTATE_PROPERTY::_FORCE force, DEPTH_STENCILOP_DESC desc) = 0;
            virtual void SetStencilRef(u32 stencil_ref) = 0;

            virtual GraphicResult::_RESULT ChangeProperty() = 0;
        };

        //-------------------------------
        //Rasterrizer
        //-------------------------------
        class IRasterrizerState : public IGraphicObject {
            EVOLUTION_NOT_DESTRUCTOR(IRasterrizerState);
        public:

            virtual FILL_MODE::_MODE GetFillMode()const = 0;
            virtual CULL_MODE::_MODE GetCullMode()const = 0;
            virtual bool IsFrontCounterClockwise()const = 0;
            virtual s32 GetDepthBias()const = 0;
            virtual f32 GetDepthBiasClamp()const = 0;
            virtual f32 GetSlopeScaledDepthBias()const = 0;
            virtual bool IsDepthClip()const = 0;
            virtual bool IsScissor()const = 0;
            virtual bool IsMultisample()const = 0;
            virtual bool IsAntialiasedLine()const = 0;
            virtual VIEW_PORT*  GetViewPort(u32 slot_number, VIEW_PORT* view_port)const = 0;
            virtual SCISSOR_RECT*  GetScissorRect(u32 slot_number, SCISSOR_RECT* scissor_rect)const = 0;

            virtual void SetProperty(FILL_MODE::_MODE type) = 0;
            virtual void SetProperty(CULL_MODE::_MODE type) = 0;
            virtual void SetProperty(s32 depth_bias) = 0;
            virtual void SetProperty(RASTERRIZER_PROPERTY_TYPE::_TYPE1 type, f32 value) = 0;
            virtual void SetProperty(RASTERRIZER_PROPERTY_TYPE::_TYPE2 type, bool flg) = 0;

            virtual void SetViewPort(u32 slot_number,const VIEW_PORT& view_port) = 0;
            virtual void SetScissorRect(u32 slot_number,const SCISSOR_RECT& scissor_rect) = 0;

            //設定したPropertyの反映
            virtual GraphicResult::_RESULT ChangeProperty() = 0;
        };

        //-------------------------------------------------
        //レンダリングパイプライン(Direct3D 9　GL2.0 ES)
        //-------------------------------------------------
        class IRenderingPipeline : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(IRenderingPipeline);
        public:
            //グラフィックコマンドの設定
            virtual void SetGraphicCommand(IGraphicCommand* command) = 0;
            //インデクスバッファの設定
            virtual void SetIndexBuffer(IBuffer* index_buffer) = 0;
            //頂点バッファの設定
            virtual void SetVertexBuffer(IBuffer* vertex_buffer) = 0;
            //頂点バッファの設定
            virtual void SetVertexBuffer(IBuffer* vertex_buffer, u32 slot_number) = 0;
            //テクスチャの設定
            virtual void SetTexture(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, ITexture* texture) = 0;
            //コンスタントバッファまたはユニフォーム用バッファの設定
            virtual void SetBuffer(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, IBuffer* buffer) = 0;
            //サンプラーステートを設定
            virtual void SetSamplerState(GRAPHIC::SHADER_TYPE::_SHADER_TYPE shader_type, u32 slot_number, ISamplerState* mp_sampler_state) = 0;
            //ラスタライザを設定
            virtual void SetRasterrizer(IRasterrizerState* rasterrizer_state) = 0;
            //ブレンドステートを設定
            virtual void SetBlendState(IBlendState* blend_state) = 0;
            //デプスステンシルを設定
            virtual void SetDepthStencilState(IDepthStencilState* depth_stencil_state) = 0;
            //フレームバッファを設定
            virtual void SetFrameBuffer(IFrameBuffer* frame_buffer) = 0;
            //フレームバッファを設定
            virtual void SetFrameBuffer(IFrameBuffer* frame_buffer, u32 slot_number) = 0;
            //シェーダーを設定
            virtual void SetShader(IShaderChain* shader_chain) = 0;
            //レンダーターゲットの設定
            virtual void SetRenderTarget(ITexture2D* render_target) = 0;
            //レンダーターゲットの設定
            virtual void SetRenderTarget(ITexture2D* render_target, u32 slot_number) = 0;
            //デプスバッファの設定
            virtual void SetDepthBuffer(ITexture2D* depth_buffer) = 0;
            //ドローコール
            virtual void Draw(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 vertex_count, u32 start_vertex_location) = 0;
            //ドローコール
            virtual void DrawIndexed(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 index_cout, u32 start_index_location, s32 base_vertex_location) = 0;
            //ドローコール
            virtual void DrawIndexedInstanced(EVOLUTION::GRAPHIC::PRIMITIVE_TOPOLOGY::_PRIMITIVE_TOPOLOGY  primitive_topology, u32 index_cout, u32 instance_count, u32 start_index_location, s32 base_vertex_location, u32 start_instance_location) = 0;
        };

        struct TEXTURELOADHANDLESTATE{
            enum _STATE{
                END = 0x00000001,
                FAILURE = 0x00000008,
            };
        };
        //--------------------------------------
        //TextureのロードHandle
        //--------------------------------------
        class ITextureLoadHandle : public IUnknown{
        public:
            //Textureの取得
            virtual GraphicResult::_RESULT GetTexture(ITexture** pp_texture) = 0;
            //ロード終了チェック
            virtual TEXTURELOADHANDLESTATE::_STATE GetStatus()const = 0;
            //ロードを待つ(ms秒)
            virtual bool Join(s32 time_out_ms)const = 0;
        };

        //--------------------------------------
        //グラフィックファクトリークラス
        //--------------------------------------
        class ITextureLoader : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(ITextureLoader);
        public:
            //Textureの同期読み込み
            virtual GraphicResult::_RESULT LoadFile(const tchar* filename, ITexture** pp_texture) = 0;
            //Textureの同期読み込み
            virtual GraphicResult::_RESULT LoadMemory(const ptr_t buffer, ptr_size_t buffer_size) = 0;
            //非同期読み込み
            virtual GraphicResult::_RESULT LoadAsyncFile(const tchar* filename, ITextureLoadHandle** pp_lord_handle) = 0;
        };

        //--------------------------------------
        //グラフィックファクトリークラス
        //--------------------------------------
        class IGraphicFactory : public IGraphicObject{
            EVOLUTION_NOT_DESTRUCTOR(IGraphicFactory);
        public:
            //テクスチャの作成
            virtual GraphicResult::_RESULT CreateTexture2D(ITexture2D** texture2d, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type) = 0;
            //デプスバッファの作成
            virtual GraphicResult::_RESULT CreateDepthBuffer(IDepthBuffer** depth_buffer, IGraphicCommand* command, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type) = 0;
            //フレームバッファの作成(アプリケーションのフレームバッファの作成)
            virtual GraphicResult::_RESULT CreateFrameBuffer(IFrameBuffer** frame_buffer, IGraphicCommand* command) = 0;
            //フレームバッファの作成
            virtual GraphicResult::_RESULT CreateFrameBuffer(IFrameBuffer** frame_buffer, IGraphicCommand* command, s32 width, s32 heigth, FORMAT_TYPE::_FORMAT_TYPE format) = 0;
            //バッファの作成
            virtual GraphicResult::_RESULT CreateVertexBuffer(IBuffer** buffer, IGraphicCommand* command, void* buffer_address, u32 buffer_count, u32 single_size) = 0;
            //レンダリングパイプラインの作成
            virtual GraphicResult::_RESULT CreateRenderingPipeline(IRenderingPipeline** rendering_pipeline, IGraphicCommand* command) = 0;
            //グラフィックコマンドの作成
            virtual GraphicResult::_RESULT CreateGraphicCommand(IGraphicCommand** command, u32 command_size, u32 buffer_byte) = 0;
            //シェーダーパイプラインの作成
            virtual GraphicResult::_RESULT CreateShaderChain(IShaderChain** shader_chain, SHADER_INPUT_LAYOUT* input_layout, u32 input_layout_count, SHADER_PROPERTY* shader_property, u32 property_count) = 0;
            //TextureLoaderの作成
            virtual GraphicResult::_RESULT CreateTextureLoader(ITextureLoader** pp_texture_loader) = 0;
            //SamplerStateの作成
            virtual GraphicResult::_RESULT CreateSamplerState(ISamplerState** pp_sampler_state, const SAMPLER_DESC& desc) = 0;
            //BlendStateの作成
            virtual GraphicResult::_RESULT CreateBlendState(IBlendState** pp_blend_state) = 0;
            //ラスタライザーの作成
            virtual GraphicResult::_RESULT CreateRasterrizerState(IRasterrizerState** pp_rasterrizer_state) = 0;
            //ステンシルステートの作成
            virtual GraphicResult::_RESULT CreateDepthStencilState(IDepthStencilState** pp_stencil_state) = 0;
            //コンスタントバッファの作成
            virtual GraphicResult::_RESULT CreateConstantBuffer(IConstantBuffer** pp_constant_buffer, IGraphicCommand* graphic_command, const CONSTANT_BUFFER_LAYOUT* layout, s32 count,bool dynamic = true) = 0;
        };

    }

    namespace FUNCTION{
#if defined(_WIN32) | defined(_WIN64)
        extern EVOLUTION::RESULT CreateGraphicFactory(GRAPHIC::IGraphicFactory** graphic_factory, void* hwnd, s32 width, s32 heigth);


        //サンプラーステートのDefault設定関数
        //desc.Filter = FILTER::MIN_MAG_MIP_LINEAR;
        //desc.AddressU = TEXTURE_ADDRESS::CLAMP;
        //desc.AddressV = TEXTURE_ADDRESS::CLAMP;
        //desc.AddressW = TEXTURE_ADDRESS::CLAMP;
        //desc.MinLOD = -3.402823466e+38F;
        //desc.MaxLOD = 3.402823466e+38F;
        //desc.MipLODBias= 0.0f;
        //desc.MaxAnisotropy= 1;
        //desc.ComparisonFunc = COMPARISON_FUNC::NEVER;
        //desc.BorderColor[0] = 1.0f;
        //desc.BorderColor[1] = 1.0f;
        //desc.BorderColor[2] = 1.0f;
        //desc.BorderColor[3] = 1.0f;
        extern void SetDefaultSamplerState(GRAPHIC::SAMPLER_DESC& desc);
#endif
    }
    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {83BA19CF-C557-4797-B451-850AD053B212}
        static const EVOLUTION_IID IID_IGraphicObject =
        { 0x83ba19cf, 0xc557, 0x4797, { 0xb4, 0x51, 0x85, 0xa, 0xd0, 0x53, 0xb2, 0x12 } };

        // {BAA19D97-21DF-4ff9-BD51-5612041165D3}
        static const EVOLUTION_IID IID_ITexture =
        { 0xbaa19d97, 0x21df, 0x4ff9, { 0xbd, 0x51, 0x56, 0x12, 0x4, 0x11, 0x65, 0xd3 } };

        // {A2888AA0-904B-42fa-A6CA-A74296DC759E}
        static const EVOLUTION_IID IID_ITexture2D =
        { 0xa2888aa0, 0x904b, 0x42fa, { 0xa6, 0xca, 0xa7, 0x42, 0x96, 0xdc, 0x75, 0x9e } };

        // {DC767B79-4D0D-467c-A301-D326ABD3504E}
        static const EVOLUTION_IID IID_IDepthBuffer =
        { 0xdc767b79, 0x4d0d, 0x467c, { 0xa3, 0x1, 0xd3, 0x26, 0xab, 0xd3, 0x50, 0x4e } };

        // {86B4A041-C4CD-42f6-A5F9-3325389BAD0E}
        static const EVOLUTION_IID IID_IFrameBuffer =
        { 0x86b4a041, 0xc4cd, 0x42f6, { 0xa5, 0xf9, 0x33, 0x25, 0x38, 0x9b, 0xad, 0xe } };

        // {B66FDC5C-CB4D-4846-B689-C373F5D33CA3}
        static const EVOLUTION_IID IID_IBuffer =
        { 0xb66fdc5c, 0xcb4d, 0x4846, { 0xb6, 0x89, 0xc3, 0x73, 0xf5, 0xd3, 0x3c, 0xa3 } };

        // {06F6B845-94D3-4f95-90BF-4525ECE1F706}
        static const EVOLUTION_IID IID_IConstantBuffer =
        { 0x6f6b845, 0x94d3, 0x4f95, { 0x90, 0xbf, 0x45, 0x25, 0xec, 0xe1, 0xf7, 0x6 } };

        // {72B00817-AA6F-4e81-AFB8-88327EE689EA}
        static const EVOLUTION_IID IID_IRenderingPipeline =
        { 0x72b00817, 0xaa6f, 0x4e81, { 0xaf, 0xb8, 0x88, 0x32, 0x7e, 0xe6, 0x89, 0xea } };

        // {6E9A3B4E-07DB-4640-B321-0272181AF98B}
        static const EVOLUTION_IID IID_IShaderChain =
        { 0x6e9a3b4e, 0x7db, 0x4640, { 0xb3, 0x21, 0x2, 0x72, 0x18, 0x1a, 0xf9, 0x8b } };

        // {7B766052-C9D2-493e-8934-4FA3E7236137}
        static const EVOLUTION_IID IID_ISamplerState =
        { 0x7b766052, 0xc9d2, 0x493e, { 0x89, 0x34, 0x4f, 0xa3, 0xe7, 0x23, 0x61, 0x37 } };

        // {A7AE6E3D-B661-48f5-AA30-176BB05D9D35}
        static const EVOLUTION_IID IID_ITextureLoadHandle =
        { 0xa7ae6e3d, 0xb661, 0x48f5, { 0xaa, 0x30, 0x17, 0x6b, 0xb0, 0x5d, 0x9d, 0x35 } };

        // {2FA1D634-CDF2-4cc7-B6FE-EDCC18A30685}
        static const EVOLUTION_IID IID_ITextureLoader =
        { 0x2fa1d634, 0xcdf2, 0x4cc7, { 0xb6, 0xfe, 0xed, 0xcc, 0x18, 0xa3, 0x6, 0x85 } };

        // {CBB47EDA-4AAD-451c-B9E3-84B8AEE2A817}
        static const EVOLUTION_IID IID_IBlendState =
        { 0xcbb47eda, 0x4aad, 0x451c, { 0xb9, 0xe3, 0x84, 0xb8, 0xae, 0xe2, 0xa8, 0x17 } };

        // {2FBCEB5B-3AD7-48e8-93DA-82B4B47323C6}
        static const EVOLUTION_IID IID_IRasterrizerState =
        { 0x2fbceb5b, 0x3ad7, 0x48e8, { 0x93, 0xda, 0x82, 0xb4, 0xb4, 0x73, 0x23, 0xc6 } };

        // {96A6A4D3-DAB9-4c0b-885F-453AA5261429}
        static const EVOLUTION_IID IID_IDepthStencilState =
        { 0x96a6a4d3, 0xdab9, 0x4c0b, { 0x88, 0x5f, 0x45, 0x3a, 0xa5, 0x26, 0x14, 0x29 } };


        // {C59BEACC-4C8B-4fa0-85F3-54E1C76E8AD5}
        static const EVOLUTION_IID IID_IGraphicCommand =
        { 0xc59beacc, 0x4c8b, 0x4fa0, { 0x85, 0xf3, 0x54, 0xe1, 0xc7, 0x6e, 0x8a, 0xd5 } };

        // {B452CD60-7355-4d72-AE3F-98DB95385BED}
        static const EVOLUTION_IID IID_IGraphicFactory =
        { 0xb452cd60, 0x7355, 0x4d72, { 0xae, 0x3f, 0x98, 0xdb, 0x95, 0x38, 0x5b, 0xed } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_INTERFACE_H__