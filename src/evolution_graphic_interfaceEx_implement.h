//------------------------------------------------------
//          Graphicの拡張ヘッダー
//------------------------------------------------------
#ifndef __EVOLUTION_GRAPHIC_INTERFACE_EX_IMPLEMENT_H__
#define __EVOLUTION_GRAPHIC_INTERFACE_EX_IMPLEMENT_H__

#include "../evolution_graphic_interfaceEx.h"

namespace EVOLUTION{
    //namespace GRAPHIC{


    //    //-----------------------
    //    //一次元テクスチャ
    //    //-----------------------
    //    class ITexture1D : public ITexture1D{
    //        EVOLUTION_NOT_DESTRUCTOR(ITexture1D);
    //    public:
    //        //幅
    //        virtual u32 GetWidth() = 0;
    //    };

    //    //-----------------------
    //    //三次元テクスチャ
    //    //-----------------------
    //    class ITexture3D : public ITexture{
    //        EVOLUTION_NOT_DESTRUCTOR(ITexture3D);
    //    public:
    //        //幅
    //        virtual u32 GetWidth() = 0;
    //        //高さ
    //        virtual u32 GetHeigth() = 0;
    //        //深さ
    //        virtual u32 GetDepth() = 0;
    //    };

    //    //----------------------------------------------------
    //    //レンダリングパイプライン(拡張 Direct3D 11 又は GL4.0)
    //    //----------------------------------------------------
    //    class IRenderingPipelineEx : public IRenderingPipeline{
    //        EVOLUTION_NOT_DESTRUCTOR(IRenderingPipelineEx);
    //    public:
    //        //インデクスバッファの設定
    //        virtual void SetIndexBuffer(IBuffer* index_buffer) = 0;
    //        //頂点バッファの設定
    //        virtual void SetVertexBuffer(IBuffer* vertex_buffer) = 0;
    //        //テクスチャの設定
    //        virtual void SetTexture(u32 slot_number, ITexture* texture) = 0;
    //        //コンスタントバッファまたはユニフォーム用バッファの設定
    //        virtual void SetBuffer(u32 slot_number, IBuffer* buffer) = 0;
    //        //ラスタライザの設定
    //        virtual void SetRasterrizer() = 0;
    //        //ブレンドステートの設定
    //        virtual void SetBlendState(u32 slot_number) = 0;
    //        //デプスステンシルの設定
    //        virtual void SetDepthStencilState() = 0;
    //        //レンダーターゲットの設定
    //        virtual void SetRenderTarget(ITexture2D* render_target) = 0;
    //        //レンダーターゲットの設定
    //        virtual void SetRenderTarget(ITexture2D* render_target, u32 slot_number) = 0;
    //        //デプスバッファの設定
    //        virtual void SetDepthBuffer(ITexture2D* depth_buffer) = 0;
    //    };

    //    //-----------------------
    //    //拡張GraphicCommandの発行
    //    //-----------------------
    //    class IGraphicCommandEx : public IGraphicCommand{
    //        EVOLUTION_NOT_DESTRUCTOR(IGraphicCommandEx);
    //    public:
    //        //コマンドの実行
    //        virtual GraphicResult::_RESULT Execute(IRenderingPipeline* rendering_pipeline) = 0;
    //        //非同期型の実行
    //        virtual GraphicResult::_RESULT AsyncExecute(IRenderingPipeline* rendering_pipeline) = 0;
    //        //フレームバッファをフリップする
    //        virtual GraphicResult::_RESULT Flip() = 0;
    //    };

    //    //-----------------------
    //    //計算コマンドの発行
    //    //-----------------------
    //    class IComputeCommand :public IGraphicObject{
    //        EVOLUTION_NOT_DESTRUCTOR(IComputeCommand);
    //    public:
    //        //コマンドの実行
    //        virtual GraphicResult::_RESULT Execute() = 0;
    //        //非同期型の実行
    //        virtual GraphicResult::_RESULT AsyncExecute() = 0;
    //    };

    //    //--------------------------------------
    //    //拡張GraphicFactoryの作成
    //    //--------------------------------------
    //    class IGraphicFactoryEx : public IGraphicFactory{
    //        EVOLUTION_NOT_DESTRUCTOR(IGraphicFactoryEx);
    //    public:
    //        //テクスチャ1Dの作成
    //        virtual GraphicResult::_RESULT CreateTexture1D() = 0;
    //        //テクスチャ3Dの作成
    //        virtual GraphicResult::_RESULT CreateTexture3D() = 0;
    //        //レンダリングパイプラインの作成
    //        virtual GraphicResult::_RESULT CreateRenderingPipelineEx() = 0;
    //        //拡張グラフィックコマンドの作成
    //        virtual GraphicResult::_RESULT CreateGraphicCommandEx() = 0;
    //        //計算コマンドの作成
    //        virtual GraphicResult::_RESULT CreateComputeCommand() = 0;
    //    };

    //}

    ////-------------------------------------
    ////IGraphicFactor
    ////-------------------------------------
    //namespace FUNCTION{
    //    extern EVOLUTION::RESULT CreateGraphicFactoryEx(GRAPHIC::IGraphicFactoryEx** graphic_factory_ex);
    //}

    ////-------------------------------------------------------
    ////EVOLUTION Globally Unique Identifier
    ////-------------------------------------------------------
    //namespace EVOLUTION_GUID{
    //    // {73AD3514-0380-4356-A2B5-2ADFC7C65044}
    //    static const EVOLUTION_IID IID_ITexture1D = { 0x73ad3514, 0x380, 0x4356, { 0xa2, 0xb5, 0x2a, 0xdf, 0xc7, 0xc6, 0x50, 0x44 } };

    //    // {5E29642E-B625-4fb9-8510-BECE447EE946}
    //    static const EVOLUTION_IID IID_ITexture3D = { 0x5e29642e, 0xb625, 0x4fb9, { 0x85, 0x10, 0xbe, 0xce, 0x44, 0x7e, 0xe9, 0x46 } };

    //    // {D2061264-6F16-4796-8686-7352A2136687}
    //    static const EVOLUTION_IID IID_IRenderingPipelineEx = { 0xd2061264, 0x6f16, 0x4796, { 0x86, 0x86, 0x73, 0x52, 0xa2, 0x13, 0x66, 0x87 } };

    //    // {A60207B8-16CA-4199-BE4C-FA5BED437F9F}
    //    static const EVOLUTION_IID IID_IGraphicCommandEx = { 0xa60207b8, 0x16ca, 0x4199, { 0xbe, 0x4c, 0xfa, 0x5b, 0xed, 0x43, 0x7f, 0x9f } };

    //    // {BB83ACA8-2C40-47a9-A14B-6B2A20F7E639}
    //    static const EVOLUTION_IID IID_IComputeCommand = { 0xbb83aca8, 0x2c40, 0x47a9, { 0xa1, 0x4b, 0x6b, 0x2a, 0x20, 0xf7, 0xe6, 0x39 } };

    //    // {A020772B-F1AB-4081-AACD-E6C58AA89E3E}
    //    static const EVOLUTION_IID IID_IGraphicFactoryEx = { 0xa020772b, 0xf1ab, 0x4081, { 0xaa, 0xcd, 0xe6, 0xc5, 0x8a, 0xa8, 0x9e, 0x3e } };

    //}
}

#endif // !__EVOLUTION_GRAPHIC_INTERFACE_H__