#ifndef __EVOLUTION_GRAPHIC_SHADER_H__
#define __EVOLUTION_GRAPHIC_SHADER_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //------------------------------------
        //		InputLayout
        //------------------------------------
        class InputLayout;

        //------------------------------------
        //		VertexShader
        //------------------------------------
        class VertexShader;

        //------------------------------------
        //		HullShader
        //------------------------------------
        class HullShader;

        //------------------------------------
        //		DomainShader
        //------------------------------------
        class DomainShader;

        //------------------------------------
        //		GeometryShader
        //------------------------------------
        class GeometryShader;

        //------------------------------------
        //		PixelShader
        //------------------------------------
        class PixelShader;

        //------------------------------------
        //		ComputeShader
        //------------------------------------
        class ComputeShader;

        //------------------------------------
        //		ShaderChain
        //------------------------------------
        class ShaderChain;
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {AC216A40-E2A2-4d77-A2B0-30FDD83A793A}
        static const EVOLUTION_IID IID_InputLayout =
        { 0xac216a40, 0xe2a2, 0x4d77, { 0xa2, 0xb0, 0x30, 0xfd, 0xd8, 0x3a, 0x79, 0x3a } };

        // {8A9219BB-A27E-4cd3-95FA-44E3DEAE8CD6}
        static const EVOLUTION_IID IID_VertexShader =
        { 0x8a9219bb, 0xa27e, 0x4cd3, { 0x95, 0xfa, 0x44, 0xe3, 0xde, 0xae, 0x8c, 0xd6 } };

        // {5C2A4721-83A1-46cb-8CAF-0DD84516EF8F}
        static const EVOLUTION_IID IID_HullShader =
        { 0x5c2a4721, 0x83a1, 0x46cb, { 0x8c, 0xaf, 0xd, 0xd8, 0x45, 0x16, 0xef, 0x8f } };

        // {82701A8E-733A-4a13-9C13-4F259A220070}
        static const EVOLUTION_IID IID_DomainShader =
        { 0x82701a8e, 0x733a, 0x4a13, { 0x9c, 0x13, 0x4f, 0x25, 0x9a, 0x22, 0x0, 0x70 } };

        // {82FECCAB-3ACB-4326-B1AA-71866B8F1B3D}
        static const EVOLUTION_IID IID_GeometryShader =
        { 0x82feccab, 0x3acb, 0x4326, { 0xb1, 0xaa, 0x71, 0x86, 0x6b, 0x8f, 0x1b, 0x3d } };

        // {54A2E0C4-EA24-4080-9344-C8D7EFE09638}
        static const EVOLUTION_IID IID_PixelShader =
        { 0x54a2e0c4, 0xea24, 0x4080, { 0x93, 0x44, 0xc8, 0xd7, 0xef, 0xe0, 0x96, 0x38 } };

        // {B13D043F-8768-4d2f-B26E-9B979ADA9143}
        static const EVOLUTION_IID IID_ComputeShader =
        { 0xb13d043f, 0x8768, 0x4d2f, { 0xb2, 0x6e, 0x9b, 0x97, 0x9a, 0xda, 0x91, 0x43 } };

        // {8921F8B3-1FFD-460c-9597-4ED655788CBE}
        static const EVOLUTION_IID IID_ShaderChain =
        { 0x8921f8b3, 0x1ffd, 0x460c, { 0x95, 0x97, 0x4e, 0xd6, 0x55, 0x78, 0x8c, 0xbe } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_COMMAND_H__