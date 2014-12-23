#ifndef __EVOLUTION_GRAPHIC_CONSTANT_BUFFER_H__
#define __EVOLUTION_GRAPHIC_CONSTANT_BUFFER_H__

#include "evolution_graphic_interface_implement.h"

namespace EVOLUTION{
    namespace GRAPHIC{

        //-------------------------------
        //Buffer 
        //-------------------------------
        class ConstantBuffer : public IConstantBuffer, public IGraphicD3DExecute{
        private:
            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            GraphicCommand* mp_graphic_command;

            D3D11_USAGE m_type;
            ID3D11Buffer* mp_d3d11_buffer;

            void* mp_buffer;
            u32 m_array_count;
            u32 m_single_size;

            struct Alignment{
                ptr_t pointer;
                ptr_size_t size;
                ptr_size_t offset;
                ptr_size_t array_count;
            };

            Alignment* mp_values;
            u32 m_value_count;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            //グラフィックコマンドの設定
            void SetGraphicCommand(IGraphicCommand* command);

            ConstantBuffer( IGraphicCommand* graphic_command);
            ~ConstantBuffer();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, const CONSTANT_BUFFER_LAYOUT* layout, s32 count, bool dynamic);

            //BufferのSingleサイズ
            u32 GetSingleSize();
            //Bufferの配列サイズ
            u32 GetArraySize();

            //Bufferアドレスの取得
            void* GetBufferAddress();
            //Bufferのコピー
            GraphicResult::_RESULT Copy(IBuffer** buffer);

            //バリューを設定する
            void SetValueBool(s32 index, bool value);
            //バリューを設定する
            void SetValueSInt(s32 index, s32 value);
            //バリューを設定する
            void SetValueUInt(s32 index, u32 value);
            //バリューを設定する
            void SetValueFloat(s32 index, f32 value);
            //バリューを設定する
            void SetValueFloat2(s32 index, const f32(&value)[2]);
            //バリューを設定する
            void SetValueFloat3(s32 index, const f32(&value)[3]);
            //バリューを設定する
            void SetValueFloat4(s32 index, const f32(&value)[4]);
            //バリューを設定する
            void SetValueVector2(s32 index, f32 x, f32 y);
            //バリューを設定する
            void SetValueVector2(s32 index, const MATH::Vector2& value);
            //バリューを設定する
            void SetValueVector3(s32 index, f32 x, f32 y, f32 z);
            //バリューを設定する
            void SetValueVector3(s32 index, const MATH::Vector3& value);
            //バリューを設定する
            void SetValueVector4(s32 index, f32 x, f32 y, f32 z, f32 w);
            //バリューを設定する
            void SetValueVector4(s32 index, const MATH::Vector4& value);
            //バリューを設定する
            void SetValueMatrix(s32 index, const MATH::Matrix& value);

            //バリューを設定する
            void SetValueSIntArray(s32 index, const s32* value, s32 count);
            //バリューを設定する
            void SetValueUIntArray(s32 index, const u32* value, s32 count);
            //バリューを設定する
            void SetValueFloatArray(s32 index, const f32* value, s32 count);
            //バリューを設定する
            void SetValueVector2Array(s32 index, const MATH::Vector2* value, s32 count);
            //バリューを設定する
            void SetValueVector3Array(s32 index, const MATH::Vector3* value, s32 count);
            //バリューを設定する
            void SetValueVector4Array(s32 index, const MATH::Vector4* value, s32 count);
            //バリューを設定する
            void SetValueMatrixArray(s32 index, const MATH::Matrix* value, s32 count);
            //設定した値を変更する
            GraphicResult::_RESULT ChangeValue();

            //Direct3Dコマンドの実行
            GraphicResult::_RESULT Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain);

            ID3D11Buffer* GetID3D11Buffer()const;
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {B3AFF834-D2A7-4926-B5E3-5DF968A80298}
        static const EVOLUTION_IID IID_ConstantBuffer =
        { 0xb3aff834, 0xd2a7, 0x4926, { 0xb5, 0xe3, 0x5d, 0xf9, 0x68, 0xa8, 0x2, 0x98 } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_TEXTURE2D_H__