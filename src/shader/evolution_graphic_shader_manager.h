#ifndef __EVOLUTION_SHADER_MANAGER_H__
#define __EVOLUTION_SHADER_MANAGER_H__

#include "../evolution_graphic_shader.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //------------------------------------
        //		ShaderManager
        //------------------------------------
        class ShaderManager{
        private:
            GraphicManager* mp_graphic_manager;

            EVOLUTION::UTILITY::Collection m_shader_collection;
        public:


            ShaderManager(GraphicManager* manager);
            ~ShaderManager();

            GraphicResult::_RESULT CompileShader(const c8* szFileName, const c8* szEntryPoint, const c8* szShaderModel, ID3DBlob** ppBlobOut);

            //Shaderパイプラインの作成
            GraphicResult::_RESULT CreateShaderChain(IShaderChain** shader_chain, SHADER_INPUT_LAYOUT* input_layout, u32 input_layout_count, SHADER_PROPERTY* shader_property, u32 property_count);
        };
    }
}

#endif // !__EVOLUTION_VERTEX_SHADER_H__