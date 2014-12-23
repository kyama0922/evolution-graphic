#ifndef __EVOLUTION_GRAPHIC_TEXTURE2D_H__
#define __EVOLUTION_GRAPHIC_TEXTURE2D_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-----------------------
        //二次元テクスチャ
        //-----------------------
        class Texture2D : public ITexture2D, public IShaderResource{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;

            FORMAT_TYPE::_FORMAT_TYPE m_format_type;
            u32 m_width;
            u32 m_heigth;

            ID3D11Texture2D* mp_texture2d;
            ID3D11ShaderResourceView* mp_shader_resource_view;
        public:
            //インターフェイスの参照カウントを 1 ずつ増やす。
            u32 AddRef();
            //オブジェクトが特定の COM インターフェイスをサポートしているかどうかを判別する。インターフェイスをサポートしている場合、システムはオブジェクトの参照カウントを増やす。アプリケーションは、そのインターフェイスをすぐに使える。
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            //インターフェイスの参照カウントを 1 ずつ減らす。
            u32 Release();

            GraphicHandle GetGraphicHandle();

            Texture2D();
            ~Texture2D();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, ID3D11Resource* resource_data);
            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, u32 width, u32 heigth, FORMAT_TYPE::_FORMAT_TYPE format_type);

            FORMAT_TYPE::_FORMAT_TYPE GetFormat();

            //幅
            u32 GetWidth();
            //高さ
            u32 GetHeigth();

           ID3D11Resource* GetResource()const;
           ID3D11ShaderResourceView* GetShaderResourceView()const;
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{


        // {E1C61753-035E-47b3-8A73-CD35895AD946}
        static const EVOLUTION_IID IID_Texture2D =
        { 0xe1c61753, 0x35e, 0x47b3, { 0x8a, 0x73, 0xcd, 0x35, 0x89, 0x5a, 0xd9, 0x46 } };
    }
}

#endif // !__EVOLUTION_GRAPHIC_TEXTURE2D_H__