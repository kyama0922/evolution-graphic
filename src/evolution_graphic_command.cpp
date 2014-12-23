#include "evolution_graphic_command.h"
#include "evolution_graphic_manager.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 GraphicCommand::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT GraphicCommand::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IGraphicCommand))
    {
        *ppvObject = static_cast<IGraphicCommand*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_GraphicCommand))
    {
        *ppvObject = static_cast<GraphicCommand*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 GraphicCommand::Release(){

    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle GraphicCommand::GetGraphicHandle(){
    return this->m_handle;
}

GraphicCommand::GraphicCommand() :
mp_graphic_manager(),
m_property(nullptr)
{

}

GraphicCommand::~GraphicCommand(){
    delete[](void*)this->m_property->buffer_ptr;
    EVOLUTION_RELEASE(this->mp_graphic_manager);
    EVOLUTION_DELETE(this->m_property);
}

GraphicResult::_RESULT GraphicCommand::Create(GraphicManager* graphic_manager, u32 queue_size, u32 work_buffer_size){
    mp_graphic_manager = graphic_manager;
    graphic_manager->AddRef();
    this->m_property = NEW GraphicCommand::GraphicCommandQueueProperty;
    if (this->m_property == nullptr)
    {
        return GraphicResult::CREATE_FAILED;
    }
    memset(this->m_property, 0, sizeof(GraphicCommand::GraphicCommandQueueProperty));
    this->m_property->command_queue_size = queue_size;
    this->m_property->property_buffer_ptr_size = work_buffer_size;
    this->m_property->buffer_ptr_size = (sizeof(Command) *  this->m_property->command_queue_size) + this->m_property->property_buffer_ptr_size;
    this->m_property->buffer_ptr = (ptr_t)NEW u8[this->m_property->buffer_ptr_size];
    this->m_property->command_queue = (Command*)this->m_property->buffer_ptr;
    this->m_property->property_buffer_ptr = this->m_property->buffer_ptr + (sizeof(Command) *  this->m_property->command_queue_size);
    this->CommandClear();
    return GraphicResult::RESULT_OK;
}

//コマンドプールの数
s32 GraphicCommand::CommandPoolSize(){
    return (u32)this->m_property->command_queue_size;
}

//登録コマンド数
u32 GraphicCommand::ResistorCommandCount(){
    return (u32)this->m_property->command_buffer_index;
}

//トランザクションの作成
GraphicResult::_RESULT GraphicCommand::TransactionBegin(){
    this->m_property->transaction_command_buffer_index = this->m_property->command_buffer_index;
    this->m_property->transaction_property_buffer_index = this->m_property->property_buffer_index;
    return GraphicResult::RESULT_OK;
}

//トランザクション取り消す
GraphicResult::_RESULT GraphicCommand::TransactionRollBack(){
    this->m_property->command_buffer_index = this->m_property->transaction_command_buffer_index;
    this->m_property->property_buffer_index = this->m_property->transaction_property_buffer_index;
    return GraphicResult::RESULT_OK;
}

//トランザクションを確定させる
GraphicResult::_RESULT GraphicCommand::TransactionCommit(){
    return GraphicResult::RESULT_OK;
}

//コマンドのSwap
GraphicResult::_RESULT GraphicCommand::Swap(GraphicCommand* command_queue){
    return GraphicResult::RESULT_OK;
}
//コマンドのClear
GraphicResult::_RESULT GraphicCommand::CommandClear(){
    Command* cmd = this->m_property->command_queue;
    for (u32 i = 0; i < this->m_property->command_buffer_index; i++)
    {
        EVOLUTION_RELEASE(cmd[i].mp_graphic_d3d_execute);
    }
    this->m_property->property_buffer_index = 0;
    this->m_property->command_buffer_index = 0;
    return GraphicResult::RESULT_OK;
}

//コマンドの登録
GraphicResult::_RESULT GraphicCommand::SetCommand(IGraphicD3DExecute* command, ptr_t command_property_ptr, ptr_size_t command_property_ptr_size){

    ID3D11DeviceContext* context = this->mp_graphic_manager->GetID3D11ImmediateContext();
    IDXGISwapChain* swap_chain = this->mp_graphic_manager->GetIDXGISwapChain();
    CommandProperty cp;
    cp.val_ptr = command_property_ptr;
    cp.val_size = command_property_ptr_size;

    Command* cmd = &this->m_property->command_queue[this->m_property->command_buffer_index];
    cmd->mp_graphic_d3d_execute = command;
    void* property_buffer_ptr = (void*)(this->m_property->property_buffer_ptr + this->m_property->property_buffer_index);
    memcpy(property_buffer_ptr, (void*)command_property_ptr, command_property_ptr_size);
    cmd->m_property.val_ptr = (ptr_t)property_buffer_ptr;
    cmd->m_property.val_size = command_property_ptr_size;

    this->m_property->property_buffer_index += command_property_ptr_size;
    this->m_property->command_buffer_index++;

    return GraphicResult::RESULT_OK;
}

//コマンドの登録
GraphicResult::_RESULT GraphicCommand::Execute(EXECUTE_COMMAND::_FLAG flag){
    Command* cmd = this->m_property->command_queue;

    ID3D11DeviceContext* context = this->mp_graphic_manager->GetID3D11ImmediateContext();
    IDXGISwapChain* swap_chain = this->mp_graphic_manager->GetIDXGISwapChain();
    for (u32 i = 0; i < this->m_property->command_buffer_index; i++)
    {
        cmd[i].mp_graphic_d3d_execute->Execute(cmd[i].m_property, context, swap_chain);
    }

    if (EVOLUTION_IS_STATE(flag, EXECUTE_COMMAND::END_FLIP))
    {
        swap_chain->Present(0, 0);
    }
    if (EVOLUTION_IS_STATE(flag, EXECUTE_COMMAND::END_COMMAND_CLEAR))
    {
        this->CommandClear();
    }
    return GraphicResult::RESULT_OK;
}

//コマンドの登録
GraphicResult::_RESULT GraphicCommand::AsyncExecute(EXECUTE_COMMAND::_FLAG flag){
    ID3D11DeviceContext* context = this->mp_graphic_manager->GetID3D11ImmediateContext();
    IDXGISwapChain* swap_chain = this->mp_graphic_manager->GetIDXGISwapChain();

    if (EVOLUTION_IS_STATE(flag, EXECUTE_COMMAND::END_FLIP))
    {
        swap_chain->Present(0, 0);
    }
    if (EVOLUTION_IS_STATE(flag, EXECUTE_COMMAND::END_COMMAND_CLEAR))
    {
        this->CommandClear();
    }
    return GraphicResult::RESULT_OK;
}