#ifndef __EVOLUTION_SHADER_CHAIN_H__
#define __EVOLUTION_SHADER_CHAIN_H__

#include "../evolution_graphic_shader.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //------------------------------------
        //		InputLayout
        //------------------------------------
        class ShaderChain : public IShaderChain, public IGraphicD3DExecute {
        public:

            struct SHADER_NUMBER{
                enum{
                    VERTEX_NUMBER = 0,
                    PIXEL_NUMBER = 1,
                    GEOMETRY_NUMBER = 2,
                    DOMAIN_NUMBER = 3,
                    HULL_NUMBER = 4,
                    COMPUTE_NUMBER = 5,
                };
            };

            struct SHADER_BIND_FLAG{
                enum _FLAG{
                    VERTEXSHADER = 1 << SHADER_NUMBER::VERTEX_NUMBER,
                    PIXELSHADER = 1 << SHADER_NUMBER::PIXEL_NUMBER,
                    GEOMETRYSHADER = 1 << SHADER_NUMBER::GEOMETRY_NUMBER,
                    DOMAINSHADER = 1 << SHADER_NUMBER::DOMAIN_NUMBER,
                    HULLSHADER = 1 << SHADER_NUMBER::HULL_NUMBER,
                    COMPUTESHADER = 1 << SHADER_NUMBER::COMPUTE_NUMBER,
                };
            };
        private:
  
            EVOLUTION::InstanceCounter m_instance_counter;

            //V:VertexShaderFLAG  | D:DomainShaderFLAG | H:HullShaderFLAG | G:GeometryShaderFLAG | P:BindFLAG | C:BindFLAG
            //bit : | 0000 0000 | 0000 0000 | 0000 0000 | 00VD HGPC | 
            u32 m_shader_bind_flag;

            //[0] VertexShader
            //[1] PixelShader
            //[2] GeometryShader
            //[3] DomainShader
            //[4] HullShader
            //[5] ComputeShader
            IGraphicD3DExecute* m_shaders[6];

            GraphicHandle m_handle;


        public:
            //IUnknown 
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            GraphicHandle GetGraphicHandle();

            ShaderChain();
            ~ShaderChain();

            GraphicResult::_RESULT Join(VertexShader* vertex_shader);
            GraphicResult::_RESULT Join(DomainShader* domain_shader);
            GraphicResult::_RESULT Join(HullShader* hull_shader);
            GraphicResult::_RESULT Join(GeometryShader* geometry_shader);
            GraphicResult::_RESULT Join(PixelShader* pixel_shader);
            GraphicResult::_RESULT Join(ComputeShader* compute_shader);

            u32 GetBindFlag()const;
            GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);
        };
    }
}

#endif // !__EVOLUTION_GRAPHIC_COMMAND_H__