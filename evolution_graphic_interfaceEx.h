//------------------------------------------------------
//          Graphicの拡張ヘッダー
//------------------------------------------------------
#ifndef __EVOLUTION_GRAPHIC_INTERFACE_EX_H__
#define __EVOLUTION_GRAPHIC_INTERFACE_EX_H__

#include "evolution_graphic_interface.h"

namespace EVOLUTION{
    namespace GRAPHIC{
        //--------------------------------------------
        //拡張フォーマット
        //--------------------------------------------
        //struct FORMAT_TYPE{
        //    enum _FORMAT_TYPE{
        //        _UNKNOWN = 0,
        //        _R32G32B32A32_TYPELESS = 1,
        //        _R32G32B32A32_FLOAT = 2,
        //        _R32G32B32A32_UINT = 3,
        //        _R32G32B32A32_SINT = 4,
        //        _R32G32B32_TYPELESS = 5,
        //        _R32G32B32_FLOAT = 6,
        //        _R32G32B32_UINT = 7,
        //        _R32G32B32_SINT = 8,
        //        _R16G16B16A16_TYPELESS = 9,
        //        _R16G16B16A16_FLOAT = 10,
        //        _R16G16B16A16_UNORM = 11,
        //        _R16G16B16A16_UINT = 12,
        //        _R16G16B16A16_SNORM = 13,
        //        _R16G16B16A16_SINT = 14,
        //        _R32G32_TYPELESS = 15,
        //        _R32G32_FLOAT = 16,
        //        _R32G32_UINT = 17,
        //        _R32G32_SINT = 18,
        //        _R32G8X24_TYPELESS = 19,
        //        _D32_FLOAT_S8X24_UINT = 20,
        //        _R32_FLOAT_X8X24_TYPELESS = 21,
        //        _X32_TYPELESS_G8X24_UINT = 22,
        //        _R10G10B10A2_TYPELESS = 23,
        //        _R10G10B10A2_UNORM = 24,
        //        _R10G10B10A2_UINT = 25,
        //        _R11G11B10_FLOAT = 26,
        //        _R8G8B8A8_TYPELESS = 27,
        //        _R8G8B8A8_UNORM = 28,
        //        _R8G8B8A8_UNORM_SRGB = 29,
        //        _R8G8B8A8_UINT = 30,
        //        _R8G8B8A8_SNORM = 31,
        //        _R8G8B8A8_SINT = 32,
        //        _R16G16_TYPELESS = 33,
        //        _R16G16_FLOAT = 34,
        //        _R16G16_UNORM = 35,
        //        _R16G16_UINT = 36,
        //        _R16G16_SNORM = 37,
        //        _R16G16_SINT = 38,
        //        _R32_TYPELESS = 39,
        //        _D32_FLOAT = 40,
        //        _R32_FLOAT = 41,
        //        _R32_UINT = 42,
        //        _R32_SINT = 43,
        //        _R24G8_TYPELESS = 44,
        //        _D24_UNORM_S8_UINT = 45,
        //        _R24_UNORM_X8_TYPELESS = 46,
        //        _X24_TYPELESS_G8_UINT = 47,
        //        _R8G8_TYPELESS = 48,
        //        _R8G8_UNORM = 49,
        //        _R8G8_UINT = 50,
        //        _R8G8_SNORM = 51,
        //        _R8G8_SINT = 52,
        //        _R16_TYPELESS = 53,
        //        _R16_FLOAT = 54,
        //        _D16_UNORM = 55,
        //        _R16_UNORM = 56,
        //        _R16_UINT = 57,
        //        _R16_SNORM = 58,
        //        _R16_SINT = 59,
        //        _R8_TYPELESS = 60,
        //        _R8_UNORM = 61,
        //        _R8_UINT = 62,
        //        _R8_SNORM = 63,
        //        _R8_SINT = 64,
        //        _A8_UNORM = 65,
        //        _R1_UNORM = 66,
        //        _R9G9B9E5_SHAREDEXP = 67,
        //        _R8G8_B8G8_UNORM = 68,
        //        _G8R8_G8B8_UNORM = 69,
        //        _BC1_TYPELESS = 70,
        //        _BC1_UNORM = 71,
        //        _BC1_UNORM_SRGB = 72,
        //        _BC2_TYPELESS = 73,
        //        _BC2_UNORM = 74,
        //        _BC2_UNORM_SRGB = 75,
        //        _BC3_TYPELESS = 76,
        //        _BC3_UNORM = 77,
        //        _BC3_UNORM_SRGB = 78,
        //        _BC4_TYPELESS = 79,
        //        _BC4_UNORM = 80,
        //        _BC4_SNORM = 81,
        //        _BC5_TYPELESS = 82,
        //        _BC5_UNORM = 83,
        //        _BC5_SNORM = 84,
        //        _B5G6R5_UNORM = 85,
        //        _B5G5R5A1_UNORM = 86,
        //        _B8G8R8A8_UNORM = 87,
        //        _B8G8R8X8_UNORM = 88,
        //        _R10G10B10_XR_BIAS_A2_UNORM = 89,
        //        _B8G8R8A8_TYPELESS = 90,
        //        _B8G8R8A8_UNORM_SRGB = 91,
        //        _B8G8R8X8_TYPELESS = 92,
        //        _B8G8R8X8_UNORM_SRGB = 93,
        //        _BC6H_TYPELESS = 94,
        //        _BC6H_UF16 = 95,
        //        _BC6H_SF16 = 96,
        //        _BC7_TYPELESS = 97,
        //        _BC7_UNORM = 98,
        //        _BC7_UNORM_SRGB = 99,
        //        _AYUV = 100,
        //        _Y410 = 101,
        //        _Y416 = 102,
        //        _NV12 = 103,
        //        _P010 = 104,
        //        _P016 = 105,
        //        _420_OPAQUE = 106,
        //        _YUY2 = 107,
        //        _Y210 = 108,
        //        _Y216 = 109,
        //        _NV11 = 110,
        //        _AI44 = 111,
        //        _IA44 = 112,
        //        _P8 = 113,
        //        _A8P8 = 114,
        //        _B4G4R4A4_UNORM = 115,
        //        _FORCE_UINT = 0xffffffff
        //    };
        //};

        //struct PRIMITIVE_TOPOLOGY{
        //    enum _PRIMITIVE_TOPOLOGY
        //    {
        //        _UNDEFINED = 0,
        //        _POINTLIST = 1,
        //        _LINELIST = 2,
        //        _LINESTRIP = 3,
        //        _TRIANGLELIST = 4,
        //        _TRIANGLESTRIP = 5,
        //        _LINELIST_ADJ = 10,
        //        _LINESTRIP_ADJ = 11,
        //        _TRIANGLELIST_ADJ = 12,
        //        _TRIANGLESTRIP_ADJ = 13,
        //        _1_CONTROL_POINT_PATCHLIST = 33,
        //        _2_CONTROL_POINT_PATCHLIST = 34,
        //        _3_CONTROL_POINT_PATCHLIST = 35,
        //        _4_CONTROL_POINT_PATCHLIST = 36,
        //        _5_CONTROL_POINT_PATCHLIST = 37,
        //        _6_CONTROL_POINT_PATCHLIST = 38,
        //        _7_CONTROL_POINT_PATCHLIST = 39,
        //        _8_CONTROL_POINT_PATCHLIST = 40,
        //        _9_CONTROL_POINT_PATCHLIST = 41,
        //        _10_CONTROL_POINT_PATCHLIST = 42,
        //        _11_CONTROL_POINT_PATCHLIST = 43,
        //        _12_CONTROL_POINT_PATCHLIST = 44,
        //        _13_CONTROL_POINT_PATCHLIST = 45,
        //        _14_CONTROL_POINT_PATCHLIST = 46,
        //        _15_CONTROL_POINT_PATCHLIST = 47,
        //        _16_CONTROL_POINT_PATCHLIST = 48,
        //        _17_CONTROL_POINT_PATCHLIST = 49,
        //        _18_CONTROL_POINT_PATCHLIST = 50,
        //        _19_CONTROL_POINT_PATCHLIST = 51,
        //        _20_CONTROL_POINT_PATCHLIST = 52,
        //        _21_CONTROL_POINT_PATCHLIST = 53,
        //        _22_CONTROL_POINT_PATCHLIST = 54,
        //        _23_CONTROL_POINT_PATCHLIST = 55,
        //        _24_CONTROL_POINT_PATCHLIST = 56,
        //        _25_CONTROL_POINT_PATCHLIST = 57,
        //        _26_CONTROL_POINT_PATCHLIST = 58,
        //        _27_CONTROL_POINT_PATCHLIST = 59,
        //        _28_CONTROL_POINT_PATCHLIST = 60,
        //        _29_CONTROL_POINT_PATCHLIST = 61,
        //        _30_CONTROL_POINT_PATCHLIST = 62,
        //        _31_CONTROL_POINT_PATCHLIST = 63,
        //        _32_CONTROL_POINT_PATCHLIST = 64,
        //    };
        //};

    //    //-----------------------
    //    //一次元テクスチャ
    //    //-----------------------
    //    class ITexture1D : public ITexture{
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

    }
}

#endif // !__EVOLUTION_GRAPHIC_INTERFACE_H__