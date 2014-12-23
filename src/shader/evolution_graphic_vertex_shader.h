#ifndef __EVOLUTION_VERTEX_SHADER_H__
#define __EVOLUTION_VERTEX_SHADER_H__

#include "../evolution_graphic_shader.h"

namespace EVOLUTION{
    namespace GRAPHIC{


        //------------------------------------
        //		VertexShader
        //------------------------------------
        class VertexShader : public IShaderChain, public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            ID3D11VertexShader* mp_vertex_shader;
            InputLayout* mp_input_layout;

            GraphicHandle m_graphic_handle;
        public:

            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

            VertexShader(ID3D11Device* device, ID3DBlob* blob, const SHADER_INPUT_LAYOUT* input, u32 input_size);
            ~VertexShader();
        };
    }
}

#endif // !__EVOLUTION_VERTEX_SHADER_H__