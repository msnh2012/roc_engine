#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Drawable.h"
#include "Elements/Shader.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Shader.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_uniformTypesTable
{
    "uint", "uvec2", "uvec3", "uvec4",
    "int", "ivec2", "ivec3", "ivec4",
    "float", "vec2", "vec3", "vec4",
    "double", "dvec2", "dvec3", "dvec4",
    "mat2", "mat3", "mat4"
};

int shaderCreate(lua_State *f_vm)
{
    std::string l_vsp, l_fsp, l_gsp;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_vsp);
    argStream.ReadText(l_fsp);
    argStream.ReadNextText(l_gsp);
    if(!argStream.HasErrors() && !l_vsp.empty() && !l_fsp.empty())
    {
        Shader *l_shader = LuaManager::GetCore()->GetElementManager()->CreateShader(l_vsp, l_fsp, l_gsp);
        l_shader ? argStream.PushPointer(l_shader) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int shaderSetUniformValue(lua_State *f_vm)
{
    Shader *l_shader;
    std::string l_uniform;
    std::string l_type;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadText(l_uniform);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && !l_uniform.empty() && !l_type.empty())
    {
        switch(Utils::Enum::ReadEnumVector(g_uniformTypesTable, l_type))
        {
            case 0: // Unsigned int
            {
                unsigned int l_val;
                argStream.ReadInteger(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 1: // Unsigned int 2
            {
                glm::uvec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 2: // Unsigned int 3
            {
                glm::uvec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 3: // Unsigned int 4
            {
                glm::uvec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 4: // Integer
            {
                int l_val;
                argStream.ReadInteger(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 5: // Integer 2
            {
                glm::ivec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 6: // Integer 3
            {
                glm::ivec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 7: // Integer 4
            {
                glm::ivec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 8: // Float
            {
                float l_val;
                argStream.ReadNumber(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 9: // Float 2
            {
                glm::vec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 10: // Float 3
            {
                glm::vec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 11: // Float 4
            {
                glm::vec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            }
            case 12: // Double
            {
                double l_val;
                argStream.ReadNumber(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 13: // Double 2
            {
                glm::dvec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 14: // Double 3
            {
                glm::dvec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 15: // Double 4
            {
                glm::dvec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            }
            case 16: // Matrix 2x2
            {
                glm::mat2 l_mat;
                argStream.ReadMatrix(glm::value_ptr(l_mat), 4);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_mat);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 17: // Matrix 3x3
            {
                glm::mat3 l_mat;
                argStream.ReadMatrix(glm::value_ptr(l_mat), 9);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_mat);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case 18: // Matrix 4x4
            {
                glm::mat4 l_mat;
                argStream.ReadMatrix(glm::value_ptr(l_mat), 16);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_mat);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            default:
                argStream.PushBoolean(false);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int shaderAttachDrawable(lua_State *f_vm)
{
    Shader *l_shader;
    Drawable *l_drawable;
    std::string l_uniform;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    argStream.ReadText(l_uniform);
    if(!argStream.HasErrors() && !l_uniform.empty())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->AttachDrawableToShader(l_shader, l_drawable, l_uniform);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int shaderDetachDrawable(lua_State *f_vm)
{
    Shader *l_shader;
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->DetachDrawableFromShader(l_shader, l_drawable);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
