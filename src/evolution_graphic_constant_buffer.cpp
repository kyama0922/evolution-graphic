#include "evolution_graphic_manager.h"
#include "evolution_graphic_command.h"
#include "evolution_graphic_constant_buffer.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;


//IUnknown
u32 ConstantBuffer::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT ConstantBuffer::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IGraphicD3DExecute))
    {
        *ppvObject = static_cast<IGraphicD3DExecute*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IBuffer))
    {
        *ppvObject = static_cast<IBuffer*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IConstantBuffer))
    {
        *ppvObject = static_cast<IConstantBuffer*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_ConstantBuffer))
    {
        *ppvObject = static_cast<ConstantBuffer*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 ConstantBuffer::Release(){

    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

//グラフィックハンドルの取得
GraphicHandle ConstantBuffer::GetGraphicHandle(){
    return this->m_handle;
}


//グラフィックコマンドの設定
void ConstantBuffer::SetGraphicCommand(IGraphicCommand* command)
{
    this->mp_graphic_command->Release();
    command->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand, (void**)&this->mp_graphic_command);
}

ConstantBuffer::ConstantBuffer(IGraphicCommand* graphic_command){
    graphic_command->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_GraphicCommand , (void**)&mp_graphic_command);
}

ConstantBuffer::~ConstantBuffer(){
    EVOLUTION_DELETE_ARRAY(this->mp_values);
    EVOLUTION_DELETE_ARRAY(this->mp_buffer);
    EVOLUTION_RELEASE(this->mp_graphic_command);
    EVOLUTION_RELEASE(this->mp_d3d11_buffer);
}

GraphicResult::_RESULT ConstantBuffer::Create(GraphicManager* graphic_manager, const CONSTANT_BUFFER_LAYOUT* layout, s32 layout_count, bool dynamic){
    HRESULT hr = S_OK;


    this->mp_values = NEW ConstantBuffer::Alignment[layout_count];
    this->m_value_count = layout_count;
    u32 size = 0;
    u32 address = 0;
    for (s32 i = 0; i < layout_count; i++)
    {
        switch (layout[i].Type)
        {
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_BOOL:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_SINT:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_INT:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT:
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 4;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = 0;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT2:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_VECTOR2:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 8;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = 0;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT3:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_VECTOR3:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 12;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = 0;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT4:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_VECTOR4:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 16;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = 0;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_MATRIX:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 64;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = 0;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_SINT_ARRAY:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_INT_ARRAY:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT_ARRAY:
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 4;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = layout[i].ArrayCount;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT2_ARRAY:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_VECTOR2_ARRAY:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 8;
            this->mp_values[i].offset = 8;
            this->mp_values[i].array_count = layout[i].ArrayCount;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT3_ARRAY:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_VECTOR3_ARRAY:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 12;
            this->mp_values[i].offset = 4;
            this->mp_values[i].array_count = layout[i].ArrayCount;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_FLOAT4_ARRAY:
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_VECTOR4_ARRAY:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 16;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = layout[i].ArrayCount;
            break;
        case EVOLUTION::GRAPHIC::CONSTANT_BUFFER_INSTANCE_TYPE::_MATRIX_ARRAY:
            if ((address & 0x0000000F) != 0)
            {
                address += 16 - (address & 0x0000000F);
            }
            this->mp_values[i].pointer = address;
            this->mp_values[i].size = 64;
            this->mp_values[i].offset = 0;
            this->mp_values[i].array_count = layout[i].ArrayCount;
            break;
        default:
            break;
        }
        size = address;
        if (this->mp_values[i].array_count == 0){
            size += this->mp_values[i].size + this->mp_values[i].offset;
        }
        else
        {
            size += (this->mp_values[i].size + this->mp_values[i].offset) * this->mp_values[i].array_count;
        }
        address = size;
    }

    this->m_array_count = 1;
    this->m_single_size = size;

    this->mp_buffer = NEW u8[this->m_single_size * this->m_array_count];
    memset(this->mp_buffer, 0, this->m_single_size  * this->m_array_count);

    ptr_t pointer = (ptr_t)this->mp_buffer;
    //オフセット計算したメモリを割り当て
    for (s32 i = 0; i < layout_count; i++)
    {
        this->mp_values[i].pointer += pointer;
    }

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.ByteWidth = this->m_single_size * this->m_array_count;
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
        return GraphicResult::RESULT_OK;
    }
    
    return GraphicResult::RESULT_OK;
}

//BufferのSingleサイズ
u32 ConstantBuffer::GetSingleSize(){
    return this->m_single_size;
}

//Bufferの配列サイズ
u32 ConstantBuffer::GetArraySize(){
    return this->m_array_count;
}

//Bufferアドレスの取得
void* ConstantBuffer::GetBufferAddress(){
    return this->mp_buffer;
}

//Bufferのコピー
GraphicResult::_RESULT ConstantBuffer::Copy(IBuffer** buffer){
    ConstantBuffer* work_buffer = NEW ConstantBuffer( this->mp_graphic_command);

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

//バリューを設定する
void ConstantBuffer::SetValueBool(s32 index, bool value){
    *((u32*)this->mp_values[index].pointer) = (value) ? 1 : 0;
}

//バリューを設定する
void ConstantBuffer::SetValueSInt(s32 index, s32 value){
    *((s32*)this->mp_values[index].pointer) = value;
}

//バリューを設定する
void ConstantBuffer::SetValueUInt(s32 index, u32 value){
    *((u32*)this->mp_values[index].pointer) = value;
}

//バリューを設定する
void ConstantBuffer::SetValueFloat(s32 index, f32 value){
    *((f32*)this->mp_values[index].pointer) = value;
}
 
//バリューを設定する
void ConstantBuffer::SetValueFloat2(s32 index, const f32(&value)[2]){
    ((f32*)this->mp_values[index].pointer)[0] = value[0];
    ((f32*)this->mp_values[index].pointer)[1] = value[1];
}

//バリューを設定する
void ConstantBuffer::SetValueFloat3(s32 index, const f32(&value)[3]){
    ((f32*)this->mp_values[index].pointer)[0] = value[0];
    ((f32*)this->mp_values[index].pointer)[1] = value[1];
    ((f32*)this->mp_values[index].pointer)[2] = value[2];
}

//バリューを設定する
void ConstantBuffer::SetValueFloat4(s32 index, const f32(&value)[4]){
    ((f32*)this->mp_values[index].pointer)[0] = value[0];
    ((f32*)this->mp_values[index].pointer)[1] = value[1];
    ((f32*)this->mp_values[index].pointer)[2] = value[2];
    ((f32*)this->mp_values[index].pointer)[3] = value[3];
}

//バリューを設定する
void ConstantBuffer::SetValueVector2(s32 index, f32 x, f32 y){
    ((f32*)this->mp_values[index].pointer)[0] = x;
    ((f32*)this->mp_values[index].pointer)[1] = y;
}

//バリューを設定する
void ConstantBuffer::SetValueVector2(s32 index, const MATH::Vector2& value){
    ((f32*)this->mp_values[index].pointer)[0] = value.x;
    ((f32*)this->mp_values[index].pointer)[1] = value.y;
}

//バリューを設定する
void ConstantBuffer::SetValueVector3(s32 index, f32 x, f32 y, f32 z){
    ((f32*)this->mp_values[index].pointer)[0] = x;
    ((f32*)this->mp_values[index].pointer)[1] = y;
    ((f32*)this->mp_values[index].pointer)[2] = z;
}

//バリューを設定する
void ConstantBuffer::SetValueVector3(s32 index, const MATH::Vector3& value){
    ((f32*)this->mp_values[index].pointer)[0] = value.x;
    ((f32*)this->mp_values[index].pointer)[1] = value.y;
    ((f32*)this->mp_values[index].pointer)[2] = value.z;
}

//バリューを設定する
void ConstantBuffer::SetValueVector4(s32 index, f32 x, f32 y, f32 z, f32 w){
    ((f32*)this->mp_values[index].pointer)[0] = x;
    ((f32*)this->mp_values[index].pointer)[1] = y;
    ((f32*)this->mp_values[index].pointer)[2] = z;
    ((f32*)this->mp_values[index].pointer)[3] = w;
}

//バリューを設定する
void ConstantBuffer::SetValueVector4(s32 index, const MATH::Vector4& value){
    ((f32*)this->mp_values[index].pointer)[0] = value.x;
    ((f32*)this->mp_values[index].pointer)[1] = value.y;
    ((f32*)this->mp_values[index].pointer)[2] = value.z;
    ((f32*)this->mp_values[index].pointer)[3] = value.w;
}

//バリューを設定する
void ConstantBuffer::SetValueMatrix(s32 index, const MATH::Matrix& value){
    ((f32*)this->mp_values[index].pointer)[0] = value.m16[0];
    ((f32*)this->mp_values[index].pointer)[1] = value.m16[1];
    ((f32*)this->mp_values[index].pointer)[2] = value.m16[2];
    ((f32*)this->mp_values[index].pointer)[3] = value.m16[3];
    ((f32*)this->mp_values[index].pointer)[4] = value.m16[4];
    ((f32*)this->mp_values[index].pointer)[5] = value.m16[5];
    ((f32*)this->mp_values[index].pointer)[6] = value.m16[6];
    ((f32*)this->mp_values[index].pointer)[7] = value.m16[7];
    ((f32*)this->mp_values[index].pointer)[8] = value.m16[8];
    ((f32*)this->mp_values[index].pointer)[9] = value.m16[9];
    ((f32*)this->mp_values[index].pointer)[10] = value.m16[10];
    ((f32*)this->mp_values[index].pointer)[11] = value.m16[11];
    ((f32*)this->mp_values[index].pointer)[12] = value.m16[12];
    ((f32*)this->mp_values[index].pointer)[13] = value.m16[13];
    ((f32*)this->mp_values[index].pointer)[14] = value.m16[14];
    ((f32*)this->mp_values[index].pointer)[15] = value.m16[15];
}

//バリューを設定する
void ConstantBuffer::SetValueSIntArray(s32 index, const s32* value, s32 count){
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((s32*)this->mp_values[index].pointer)[i] = value[i];
    }
}

//バリューを設定する
void ConstantBuffer::SetValueUIntArray(s32 index, const u32* value, s32 count){
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((u32*)this->mp_values[index].pointer)[i] = value[i];
    }
}

//バリューを設定する
void ConstantBuffer::SetValueFloatArray(s32 index, const f32* value, s32 count){
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((f32*)this->mp_values[index].pointer)[i] = value[i];
    }
}

//バリューを設定する
void ConstantBuffer::SetValueVector2Array(s32 index, const MATH::Vector2* value, s32 count){
    ptr_t pointer = this->mp_values[index].pointer;
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((f32*)pointer)[0] = value[i].x;
        ((f32*)pointer)[1] = value[i].y;
        pointer += this->mp_values[index].size + this->mp_values[index].offset;
    }
}

//バリューを設定する
void ConstantBuffer::SetValueVector3Array(s32 index, const MATH::Vector3* value, s32 count){
    ptr_t pointer = this->mp_values[index].pointer;
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((f32*)pointer)[0] = value[i].x;
        ((f32*)pointer)[1] = value[i].y;
        ((f32*)pointer)[2] = value[i].z;
        pointer += this->mp_values[index].size + this->mp_values[index].offset;
    }
}

//バリューを設定する
void ConstantBuffer::SetValueVector4Array(s32 index, const MATH::Vector4* value, s32 count){
    ptr_t pointer = this->mp_values[index].pointer;
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((f32*)pointer)[0] = value[i].x;
        ((f32*)pointer)[1] = value[i].y;
        ((f32*)pointer)[2] = value[i].z;
        ((f32*)pointer)[3] = value[i].w;
        pointer += this->mp_values[index].size + this->mp_values[index].offset;
    }
}
//バリューを設定する
void ConstantBuffer::SetValueMatrixArray(s32 index, const MATH::Matrix* value, s32 count){
    ptr_t pointer = this->mp_values[index].pointer;
    for (ptr_size_t i = 0; i < this->mp_values[index].array_count; i++)
    {
        ((f32*)pointer)[0] = value[i].m16[0];
        ((f32*)pointer)[1] = value[i].m16[1];
        ((f32*)pointer)[2] = value[i].m16[2];
        ((f32*)pointer)[3] = value[i].m16[3];
        ((f32*)pointer)[4] = value[i].m16[4];
        ((f32*)pointer)[5] = value[i].m16[5];
        ((f32*)pointer)[6] = value[i].m16[6];
        ((f32*)pointer)[7] = value[i].m16[7];
        ((f32*)pointer)[8] = value[i].m16[8];
        ((f32*)pointer)[9] = value[i].m16[9];
        ((f32*)pointer)[10] = value[i].m16[10];
        ((f32*)pointer)[11] = value[i].m16[11];
        ((f32*)pointer)[12] = value[i].m16[12];
        ((f32*)pointer)[13] = value[i].m16[13];
        ((f32*)pointer)[14] = value[i].m16[14];
        ((f32*)pointer)[15] = value[i].m16[15];
        pointer += this->mp_values[index].size + this->mp_values[index].offset;
    }
}
//設定した値を変更する
GraphicResult::_RESULT ConstantBuffer::ChangeValue(){
    IGraphicD3DExecute* myself;
    this->QueryInterface( EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&myself);
    return this->mp_graphic_command->SetCommand(myself, (ptr_t)this->mp_buffer, this->m_array_count * this->m_single_size);
}

//Direct3Dコマンドの実行
GraphicResult::_RESULT ConstantBuffer::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    D3D11_MAPPED_SUBRESOURCE map_resource;
    if (this->m_type == D3D11_USAGE_DYNAMIC){
        context->Map(this->mp_d3d11_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map_resource);
        memcpy(map_resource.pData, (void*)command_data.val_ptr, command_data.val_size );
        context->Unmap(this->mp_d3d11_buffer, 0);

    }
    else{
        context->UpdateSubresource(this->mp_d3d11_buffer, 0, NULL, (void*)command_data.val_ptr, 0, 0);
    }
    return GraphicResult::RESULT_OK;
}

ID3D11Buffer* ConstantBuffer::GetID3D11Buffer()const{
    return this->mp_d3d11_buffer;
}