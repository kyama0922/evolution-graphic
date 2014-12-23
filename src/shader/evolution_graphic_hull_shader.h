#ifndef __EVOLUTION_HULL_SHADER_H__
#define __EVOLUTION_HULL_SHADER_H__

#include "../evolution_graphic_shader.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //------------------------------------
        //		HullShader
        //------------------------------------
        class HullShader : public IShaderChain, public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            ID3D11HullShader* mp_hull_shader;
            GraphicHandle m_graphic_handle;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();
            
            HullShader(ID3D11Device* device, ID3DBlob* blob);
            ~HullShader();

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

        };

    }

}

#endif // !__EVOLUTION_HULL_SHADER_H__