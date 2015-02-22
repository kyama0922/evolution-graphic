#include "evolution_graphic_shader_chain.h"
#include "evolution_graphic_input_layout.h"
#include "evolution_graphic_vertex_shader.h"
#include "evolution_graphic_domain_shader.h"
#include "evolution_graphic_hull_shader.h"
#include "evolution_graphic_geometry_shader.h"
#include "evolution_graphic_pixel_shader.h"
#include "evolution_graphic_compute_shader.h"

using namespace EVOLUTION;
using namespace EVOLUTION::GRAPHIC;

//IUnknown
u32 ShaderChain::AddRef(){
    return m_instance_counter.AddRef();
}

RESULT ShaderChain::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){
    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IShaderChain))
    {
        *ppvObject = static_cast<IShaderChain*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_ShaderChain))
    {
        *ppvObject = static_cast<ShaderChain*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }

    return RESULT::S_ok;
}

u32 ShaderChain::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        delete this;
    }
    return counter;
}

GraphicHandle ShaderChain::GetGraphicHandle(){
    return this->m_handle;
}


ShaderChain::ShaderChain() : m_shader_bind_flag(0){
    for (s32 i = 0; i < EVOLUTION_ARRAYSIZE(this->m_shaders); i++)
    {
        this->m_shaders[i] = nullptr;
    }
}

ShaderChain::~ShaderChain(){
    for (s32 i = 0; i < EVOLUTION_ARRAYSIZE(this->m_shaders); i++)
    {
        EVOLUTION_RELEASE(this->m_shaders[i]);
    }
}

GraphicResult::_RESULT ShaderChain::Join(VertexShader* vertex_shader){
    EVOLUTION_RELEASE(this->m_shaders[SHADER_NUMBER::VERTEX_NUMBER]);

    if (EVOLUTION_FAILED(vertex_shader->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&this->m_shaders[SHADER_NUMBER::VERTEX_NUMBER])))
    {
        return GraphicResult::CAST_SHADER_VERTEX_SHADER;
    }
    EVOLUTION_ENABLE_STATE(this->m_shader_bind_flag, ShaderChain::SHADER_BIND_FLAG::VERTEXSHADER);
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT ShaderChain::Join(DomainShader* domain_shader){
    EVOLUTION_RELEASE(this->m_shaders[SHADER_NUMBER::DOMAIN_NUMBER]);

    if (EVOLUTION_FAILED(domain_shader->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&this->m_shaders[SHADER_NUMBER::DOMAIN_NUMBER])))
    {
        return GraphicResult::CAST_SHADER_DOMAIN_SHADER;
    }
    EVOLUTION_ENABLE_STATE(this->m_shader_bind_flag, ShaderChain::SHADER_BIND_FLAG::DOMAINSHADER);
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT ShaderChain::Join(HullShader* hull_shader){
    EVOLUTION_RELEASE(this->m_shaders[SHADER_NUMBER::HULL_NUMBER]);

    if (EVOLUTION_FAILED(hull_shader->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&this->m_shaders[SHADER_NUMBER::HULL_NUMBER])))
    {
        return GraphicResult::CAST_SHADER_HULL_SHADER;
    }
    EVOLUTION_ENABLE_STATE(this->m_shader_bind_flag, ShaderChain::SHADER_BIND_FLAG::HULLSHADER);
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT ShaderChain::Join(GeometryShader* geometry_shader){
    EVOLUTION_RELEASE(this->m_shaders[SHADER_NUMBER::GEOMETRY_NUMBER]);

    if (EVOLUTION_FAILED(geometry_shader->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&this->m_shaders[SHADER_NUMBER::GEOMETRY_NUMBER])))
    {
        return GraphicResult::CAST_SHADER_GEOMETRY_SHADER;
    }
    EVOLUTION_ENABLE_STATE(this->m_shader_bind_flag, ShaderChain::SHADER_BIND_FLAG::GEOMETRYSHADER);
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT ShaderChain::Join(PixelShader* pixel_shader){
    EVOLUTION_RELEASE(this->m_shaders[SHADER_NUMBER::PIXEL_NUMBER]);

    if (EVOLUTION_FAILED(pixel_shader->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&this->m_shaders[SHADER_NUMBER::PIXEL_NUMBER])))
    {
        return GraphicResult::CAST_SHADER_PIXEL_SHADER;
    }
    EVOLUTION_ENABLE_STATE(this->m_shader_bind_flag, ShaderChain::SHADER_BIND_FLAG::PIXELSHADER);
    return GraphicResult::RESULT_OK;
}

GraphicResult::_RESULT ShaderChain::Join(ComputeShader* compute_shader){
    EVOLUTION_RELEASE(this->m_shaders[SHADER_NUMBER::COMPUTE_NUMBER]);

    if (EVOLUTION_FAILED(compute_shader->QueryInterface(EVOLUTION::EVOLUTION_GUID::IID_IGraphicD3DExecute, (void**)&this->m_shaders[SHADER_NUMBER::COMPUTE_NUMBER])))
    {
        return GraphicResult::CAST_SHADER_COMPUTE_SHADER;
    }
    EVOLUTION_ENABLE_STATE(this->m_shader_bind_flag, ShaderChain::SHADER_BIND_FLAG::COMPUTESHADER);
    return GraphicResult::RESULT_OK;
}

u32 ShaderChain::GetBindFlag()const{
    return this->m_shader_bind_flag;
}

GraphicResult::_RESULT ShaderChain::Execute(const CommandProperty& command_data, ID3D11DeviceContext* context, IDXGISwapChain* swapchain){
    static ShaderChain* bind = nullptr;
    if (bind == this)
    {
        return GraphicResult::RESULT_OK;
    }

    if (EVOLUTION_IS_STATE(this->m_shader_bind_flag, SHADER_BIND_FLAG::VERTEXSHADER))
    {
        this->m_shaders[SHADER_NUMBER::VERTEX_NUMBER]->Execute(command_data, context, swapchain);
    }
    if (EVOLUTION_IS_STATE(this->m_shader_bind_flag, SHADER_BIND_FLAG::PIXELSHADER))
    {
        this->m_shaders[SHADER_NUMBER::PIXEL_NUMBER]->Execute(command_data, context, swapchain);
    }
    if (EVOLUTION_IS_STATE(this->m_shader_bind_flag, SHADER_BIND_FLAG::GEOMETRYSHADER))
    {
        this->m_shaders[SHADER_NUMBER::GEOMETRY_NUMBER]->Execute(command_data, context, swapchain);
    }
    if (EVOLUTION_IS_STATE(this->m_shader_bind_flag, SHADER_BIND_FLAG::DOMAINSHADER))
    {
        this->m_shaders[SHADER_NUMBER::DOMAIN_NUMBER]->Execute(command_data, context, swapchain);
    }
    if (EVOLUTION_IS_STATE(this->m_shader_bind_flag, SHADER_BIND_FLAG::HULLSHADER))
    {
        this->m_shaders[SHADER_NUMBER::HULL_NUMBER]->Execute(command_data, context, swapchain);
    }
    if (EVOLUTION_IS_STATE(this->m_shader_bind_flag, SHADER_BIND_FLAG::COMPUTESHADER))
    {
        this->m_shaders[SHADER_NUMBER::COMPUTE_NUMBER]->Execute(command_data, context, swapchain);
    }

    return GraphicResult::RESULT_OK;
}
