#pragma once

namespace ROC
{
namespace Lua
{

int sceneCreate(lua_State *f_vm);
int sceneSetCamera(lua_State *f_vm);
int sceneGetCamera(lua_State *f_vm);
int sceneRemoveCamera(lua_State *f_vm);
int sceneSetLight(lua_State *f_vm);
int sceneGetLight(lua_State *f_vm);
int sceneRemoveLight(lua_State *f_vm);

}
}
