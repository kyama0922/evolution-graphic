#ifndef __EVOLUTION_INPUT_LAYOUT_H__
#define __EVOLUTION_INPUT_LAYOUT_H__

#include "../evolution_graphic_shader.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //------------------------------------
        //		InputLayout
        //------------------------------------
        class InputLayout : public IShaderChain, public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            ID3D11InputLayout* mp_inputlayout;
            GraphicHandle m_graphic_handle;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            InputLayout(ID3D11Device* device, ID3DBlob* vertex_blob, const SHADER_INPUT_LAYOUT* input, u32 input_size);
            ~InputLayout();


            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);
        };

    }
}
#endif // !__EVOLUTION_INPUT_LAYOUT_H__