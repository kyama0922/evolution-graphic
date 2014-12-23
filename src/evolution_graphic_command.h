#ifndef __EVOLUTION_GRAPHIC_COMMAND_H__
#define __EVOLUTION_GRAPHIC_COMMAND_H__

#include "evolution_graphic_interface_implement.h"
namespace EVOLUTION{
    namespace GRAPHIC{

        //-----------------------
        //GraphicCommand
        //-----------------------
        class GraphicCommand : public IGraphicCommand{
        private:
            struct GraphicCommandQueueProperty{
                ptr_t buffer_ptr;//バッファサイズ
                ptr_size_t buffer_ptr_size;

                Command* command_queue;//コマンドキュー
                ptr_size_t command_queue_size;//コマンドキューサイズ

                ptr_t property_buffer_ptr;//プロパティバッファのポインター
                ptr_size_t property_buffer_ptr_size;//プロパティバッファのサイズ

                ptr_size_t command_buffer_index;//コマンドバッファインデックス
                ptr_size_t property_buffer_index;//プロパティバッファインデックス


                ptr_size_t transaction_command_buffer_index;//トランザクションコマンドインデックス
                ptr_size_t transaction_property_buffer_index;//トランザクションプロパティバッファインデックス
            };

            EVOLUTION::InstanceCounter m_instance_counter;
            GraphicHandle m_handle;
            GraphicManager* mp_graphic_manager;
            GraphicCommandQueueProperty* m_property;
        public:
            //IUnknown
            u32 AddRef();
            RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
            u32 Release();

            //グラフィックハンドルの取得
            GraphicHandle GetGraphicHandle();

            GraphicCommand();
            ~GraphicCommand();

            GraphicResult::_RESULT Create(GraphicManager* graphic_manager, u32 queue_size, u32 work_buffer_size);

            //コマンドプールの数
            s32 CommandPoolSize();
            //登録コマンド数
            u32 ResistorCommandCount();
            //トランザクションの作成
            GraphicResult::_RESULT TransactionBegin();
            //トランザクション取り消す
            GraphicResult::_RESULT TransactionRollBack();
            //トランザクションを確定させる
            GraphicResult::_RESULT TransactionCommit();
            //コマンドのSwap
            GraphicResult::_RESULT Swap(GraphicCommand* command_queue);
            //コマンドのClear
            GraphicResult::_RESULT CommandClear();

            //コマンドの登録
            GraphicResult::_RESULT SetCommand(IGraphicD3DExecute* command, ptr_t command_property_ptr, ptr_size_t command_property_ptr_size);

            //コマンドの実行
            GraphicResult::_RESULT Execute(EXECUTE_COMMAND::_FLAG flag);

            //非同期型の実行
            GraphicResult::_RESULT AsyncExecute(EXECUTE_COMMAND::_FLAG flag);
        };

    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {9A6B4080-4D10-405c-9989-EC0C916BABEA}
        static const EVOLUTION_IID IID_GraphicCommand =
        { 0x9a6b4080, 0x4d10, 0x405c, { 0x99, 0x89, 0xec, 0xc, 0x91, 0x6b, 0xab, 0xea } };

    }
}

#endif // !__EVOLUTION_GRAPHIC_COMMAND_H__