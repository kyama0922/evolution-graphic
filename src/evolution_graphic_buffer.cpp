#include "evolution_graphic_buffer.h"
#include "evolution_graphic_manager.h"
#include "evolution_graphic_command.h"

using namespace EVOLUTION;
using namespace EVOLUTION::CORE;
using namespace EVOLUTION::GRAPHIC;



//IUnknown
u32 Buffer::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT Buffer::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
    {
        *ppvObject = static_cast<IGraphicD3DExecute*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IBuffer))
    {
        *ppvObject = static_cast<IBuffer*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_Buffer))
    {
        *ppvObject = static_cast<Buffer*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 Buffer::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}


Buffer::Buffer(IGraphicCommand* graphic_command) :
mp_buffer(nullptr),
mp_d3d11_buffer(nullptr)
{
    graphic_command->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&this->mp_graphic_command);
}

Buffer::~Buffer(){
    EVOLUTION_DELETE_ARRAY(this->mp_buffer);
    EVOLUTION_RELEASE(this->mp_d3d11_buffer);
    EVOLUTION_RELEASE(this->mp_graphic_command);
}

//グラフィックコマンドの設定
void Buffer::SetGraphicCommand(IGraphicCommand* graphic_command){
    this->mp_graphic_command->Release();
    graphic_command->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&this->mp_graphic_command);
}

//グラフィックハンドルの取得
GraphicHandle Buffer::GetGraphicHandle(){
    return this->m_handle;
}

GraphicResult::_RESULT Buffer::CreateVertexBuffer(GraphicManager* graphic_manager, const void* buffer, u32 buffer_count, u32 single_size, bool dynamic){
    HRESULT hr = S_OK;
    try{
        this->m_array_count = buffer_count;
        this->m_single_size = single_size;

        this->mp_buffer = NEW u8[buffer_count * single_size];
        memcpy(this->mp_buffer, buffer, this->m_single_size  * this->m_array_count);

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.ByteWidth = buffer_count * single_size;
        bd.CPUAccessFlags = 0;

        if (dynamic){
            bd.Usage = D3D11_USAGE_DYNAMIC;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        this->m_type = bd.Usage;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = this->mp_buffer;

        hr = graphic_manager->GetID3D11Device()->CreateBuffer(&bd, &InitData, &this->mp_d3d11_buffer);

        if (FAILED(hr))
        {
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_D3D11BUFFER;
        }

    }
    catch (GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_DELETE_ARRAY(this->mp_buffer);
        EVOLUTION_RELEASE(this->mp_d3d11_buffer);

        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
        return GraphicResult::RESULT_OK;
    }
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT Buffer::CreateIndexBuffer(GraphicManager* graphic_manager, const void* buffer, u32 buffer_count, u32 single_size){
    HRESULT hr = S_OK;
    try{
        this->m_array_count = buffer_count;
        this->m_single_size = single_size;

        //インデックスバッファは更新頻度が低いため
        this->mp_buffer = NEW u8[buffer_count * single_size];
        memcpy(this->mp_buffer, buffer, this->m_single_size  * this->m_array_count);

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.ByteWidth = buffer_count * single_size;
        bd.CPUAccessFlags = 0;

        this->m_type = bd.Usage;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = buffer;

        hr = graphic_manager->GetID3D11Device()->CreateBuffer(&bd, &InitData, &this->mp_d3d11_buffer);

        if (FAILED(hr))
        {
            throw EVOLUTION::GRAPHIC::GraphicException::CREATE_D3D11BUFFER;
        }

    }
    catch (GRAPHIC::GraphicException::_EXCEPTION exception)
    {
        EVOLUTION_DELETE_ARRAY(this->mp_buffer);
        EVOLUTION_RELEASE(this->mp_d3d11_buffer);

        EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(exception);
        return GraphicResult::RESULT_OK;
    }
    return GraphicResult::RESULT_OK;
}

//BufferのSingleサイズ
u32 Buffer::GetSingleSize(){
    return this->m_single_size;
}

//Bufferの配列サイズ
u32 Buffer::GetArraySize(){
    return this->m_array_count;
}

void* Buffer::GetBufferAddress(){
    return this->mp_buffer;
}

//設定した値を変更する
GraphicResult::_RESULT Buffer::ChangeValue(){
    IGraphicD3DExecute* myself;
    this->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&myself);
    return this->mp_graphic_command->SetCommand(myself, (ptr_t)this->mp_buffer, this->m_single_size * this->m_array_count);
}

//Bufferのコピー
GraphicResult::_RESULT  Buffer::Copy(IBuffer** buffer){
    Buffer* work_buffer = NEW Buffer(this->mp_graphic_command);
    
    ID3D11Device* d3ddevice = nullptr;
    this->mp_d3d11_buffer->GetDevice(&d3ddevice);

    work_buffer->m_array_count = this->m_array_count;
    work_buffer->m_single_size = this->m_single_size;
    work_buffer->m_type = this->m_type;

    work_buffer->mp_buffer = NEW u8[work_buffer->m_array_count * work_buffer->m_single_size];
    memcpy(work_buffer->mp_buffer, this->mp_buffer, this->m_single_size  * this->m_array_count);

    D3D11_BUFFER_DESC bd;
    this->mp_d3d11_buffer->GetDesc(&bd);

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = this->mp_buffer;

    HRESULT hr = d3ddevice->CreateBuffer(&bd, &InitData, &work_buffer->mp_d3d11_buffer);
    if (FAILED(hr))
    {
        return GraphicResult::CREATE_FAILED;
    }
    EVOLUTION_RELEASE(d3ddevice);

    *buffer = work_buffer;

    return GraphicResult::RESULT_OK;
}

ID3D11Buffer* Buffer::GetID3D11Buffer()const{
    return this->mp_d3d11_buffer;
}

GraphicResult::_RESULT Buffer::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    D3D11_MAPPED_SUBRESOURCE map_resource;
    if (this->m_type == D3D11_USAGE_DYNAMIC){
        context->Map(this->mp_d3d11_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map_resource);
        memcpy(map_resource.pData, (void*)command_data.val_ptr, command_data.val_size);
        context->Unmap(this->mp_d3d11_buffer, 0);

    }
    else{
        context->UpdateSubresource(this->mp_d3d11_buffer, 0, NULL, (void*)command_data.val_ptr, 0, 0);
    }
    return GraphicResult::RESULT_OK;
}

void Buffer::__DeleteTempBuffer(){
    EVOLUTION_DELETE_ARRAY(this->mp_buffer);
}