#include "evolution_graphic_shader_manager.h"
#include "evolution_graphic_shader_chain.h"
#include "../evolution_graphic_manager.h"

#pragma comment(lib, "d3dcompiler.lib")

#include "evolution_graphic_input_layout.h"
#include "evolution_graphic_vertex_shader.h"
#include "evolution_graphic_domain_shader.h"
#include "evolution_graphic_hull_shader.h"
#include "evolution_graphic_geometry_shader.h"
#include "evolution_graphic_pixel_shader.h"
#include "evolution_graphic_compute_shader.h"
#include "evolution_graphic_d3dinclude.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

ShaderManager::ShaderManager(GraphicManager* manager) : mp_graphic_manager(manager){
}

ShaderManager::~ShaderManager(){

}

GraphicResult::_RESULT ShaderManager::CompileShader(const c8* szFileName, const c8* szEntryPoint, const c8* szShaderModel, ID3DBlob** ppBlobOut){
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_PARTIAL_PRECISION;

#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif
    EVOLUTION::CORE::FILE::IFileRead* file;
    FUNCTION::CreateFileRead(&file, szFileName);
    u32 file_size = (u32)file->GetFileSize();

    EVOLUTION::TEMPLATE::Array<u8> buffer(file_size);

    file->Read(&buffer[0], file_size);
    file->Close();
    EVOLUTION_RELEASE(file);
    D3DInclude include(szFileName);

    ID3DBlob* pErrorBlob;

    hr = D3DCompile(buffer.GetArray(), file_size, szFileName, NULL, &include, szEntryPoint, szShaderModel, dwShaderFlags, NULL, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != NULL){
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        }
        if (pErrorBlob){
            pErrorBlob->Release();
        }
        throw GraphicException::COMPILE_SHADER;
    }
    if (pErrorBlob)
    {
        pErrorBlob->Release();
    }


    return GraphicResult::RESULT_OK;
}

//Shaderパイプラインの作成
GraphicResult::_RESULT ShaderManager::CreateShaderChain(IShaderChain** pp_shader_chain, SHADER_INPUT_LAYOUT* input_layout, u32 input_layout_count, SHADER_PROPERTY* shader_property, u32 property_count){
   
    //SHADER_TYPEと同じ順にすること
    const c8* str_shader_type[] = {
            { "vs" },//SHADER_TYPE::VS
            { "ps" },//SHADER_TYPE::PS
            { "gs" },//SHADER_TYPE::GS
            { "ds" },//SHADER_TYPE::DS
            { "hs" },//SHADER_TYPE::GS
            { "cs" },//SHADER_TYPE::CS
    };

    const c8* str_shader_model[] = {
            { "_5_0" },
            { "_4_1" },
            { "_4_0" },
            { "_4_0_level_9_3" },
            { "_4_0_level_9_1" },
    };

    ShaderChain* shader_chain = NEW ShaderChain();

    for (size_t i = 0; i < property_count; i++)
    {
        ID3DBlob* blob;
        c8 ShaderModel[32] = { 0 };

        //不正タイプチェック
        EVOLUTION_ASSERT(shader_property[i].shader_type != SHADER_TYPE::INPUT);
        //シェーダータイプのセット
        memcpy(ShaderModel, str_shader_type[shader_property[i].shader_type], strlen(str_shader_type[shader_property[i].shader_type]));
        //シェーダーモデルのセット
        memcpy(ShaderModel + strlen(str_shader_type[shader_property[i].shader_type]), str_shader_model[shader_property[i].shader_model], strlen(str_shader_model[shader_property[i].shader_model]));

        //暗号化インスタンスの取得
        EVOLUTION::CORE::Hash* crypto = EVOLUTION::CORE::Hash::GetInstance();

        //ファイル名とエントリーポイントの結合
        std::string src;
        src.reserve(128);
        src += shader_property[i].filename;
        src += shader_property[i].entry_point;

        //ファイル名とエントリーポイントのHash化
        u32 hash = crypto->CRC32((u8*)src.c_str(), src.length());

        IUnknown* unknown_shader_object = nullptr;
        //ハッシュの検索
        if (this->m_shader_collection.Find(hash)){
            unknown_shader_object = this->m_shader_collection.QueryItem(hash);
        }
        else{
            //シェーダーのコンパイル
            this->CompileShader(shader_property[i].filename, shader_property[i].entry_point, ShaderModel, &blob);
        }


        switch (shader_property[i].shader_type)
        {
        case SHADER_TYPE::VS:
            //バーテックスバッファの作成
            if (unknown_shader_object == nullptr){

                unknown_shader_object = new VertexShader(this->mp_graphic_manager->GetID3D11Device(), blob, input_layout, input_layout_count);
                this->m_shader_collection.InsertItem(hash, unknown_shader_object);
            }
            shader_chain->Join((VertexShader*)unknown_shader_object);
            break;
        case SHADER_TYPE::HS:
            //ハルシェーダーの作成
            if (unknown_shader_object == nullptr){
                unknown_shader_object = new HullShader(this->mp_graphic_manager->GetID3D11Device(), blob);
                this->m_shader_collection.InsertItem(hash, unknown_shader_object);
            }
            shader_chain->Join((HullShader*)unknown_shader_object);
            break;
        case SHADER_TYPE::DS:
            //ドメインシェーダーの作成
            if (unknown_shader_object == nullptr){
                unknown_shader_object = new DomainShader(this->mp_graphic_manager->GetID3D11Device(), blob);
                this->m_shader_collection.InsertItem(hash, unknown_shader_object);
            }
            shader_chain->Join((DomainShader*)unknown_shader_object);
            break;
        case SHADER_TYPE::GS:
            //ジオメトリシェーダーの作成
            if (unknown_shader_object == nullptr){
                unknown_shader_object = new GeometryShader(this->mp_graphic_manager->GetID3D11Device(), blob);
                this->m_shader_collection.InsertItem(hash, unknown_shader_object);
            }
            shader_chain->Join((GeometryShader*)unknown_shader_object);
            break;
        case SHADER_TYPE::PS:
            //ピクセルシェーダーの作成
            if (unknown_shader_object == nullptr){
                unknown_shader_object = new PixelShader(this->mp_graphic_manager->GetID3D11Device(), blob);
                this->m_shader_collection.InsertItem(hash, unknown_shader_object);
            }
            shader_chain->Join((PixelShader*)unknown_shader_object);
            break;
        case SHADER_TYPE::CS:
            //コンピュートシェーダーの作成
            if (unknown_shader_object == nullptr){
                unknown_shader_object = new ComputeShader(this->mp_graphic_manager->GetID3D11Device(), blob);
                this->m_shader_collection.InsertItem(hash, unknown_shader_object);
            }
            shader_chain->Join((ComputeShader*)unknown_shader_object);
            break;
        default:
            //その他処理
            EVOLUTION_ASSERT(0);
            break;
        }

        //作成シェーダーの登録
        EVOLUTION_RELEASE(blob);
        EVOLUTION_RELEASE(unknown_shader_object);
    }

    *pp_shader_chain = shader_chain;
    return GraphicResult::RESULT_OK;
}

