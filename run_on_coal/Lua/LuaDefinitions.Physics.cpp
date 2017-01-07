#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Physics.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int physicsSetEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetPhysicsManager()->SetPhysicsEnabled(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int physicsGetEnabled(lua_State *f_vm)
{
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.PushBoolean(LuaManager::m_corePointer->GetPhysicsManager()->GetPhysicsEnabled());
    return argStream.GetReturnValue();
}

int physicsSetFloorEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetPhysicsManager()->SetFloorEnabled(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int physicsGetFloorEnabled(lua_State *f_vm)
{
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.PushBoolean(LuaManager::m_corePointer->GetPhysicsManager()->GetFloorEnabled());
    return argStream.GetReturnValue();
}

int physicsSetGravity(lua_State *f_vm)
{
    glm::vec3 l_gravity;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_gravity[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetPhysicsManager()->SetGravity(l_gravity);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int physicsGetGravity(lua_State *f_vm)
{
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    glm::vec3 l_grav;
    LuaManager::m_corePointer->GetPhysicsManager()->GetGravity(l_grav);
    argStream.PushNumber(l_grav.x);
    argStream.PushNumber(l_grav.y);
    argStream.PushNumber(l_grav.z);
    return argStream.GetReturnValue();
}

int physicsRayCast(lua_State *f_vm)
{
    glm::vec3 l_start, l_end;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_start[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_end[i]);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_hitNormal;
        void *l_hitElement = NULL;
        if(LuaManager::m_corePointer->GetPhysicsManager()->RayCast(l_start, l_end, l_hitNormal, &l_hitElement))
        {
            argStream.PushNumber(l_end.x);
            argStream.PushNumber(l_end.y);
            argStream.PushNumber(l_end.z);
            argStream.PushNumber(l_hitNormal.x);
            argStream.PushNumber(l_hitNormal.y);
            argStream.PushNumber(l_hitNormal.z);
            l_hitElement ? argStream.PushPointer(l_hitElement) : argStream.PushText("floor");
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
