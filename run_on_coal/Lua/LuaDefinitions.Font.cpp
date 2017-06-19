#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Font.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Font.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_fontFilteringTypesTable
{
    "nearest", "linear"
};

int fontCreate(lua_State *f_vm)
{
    std::string l_path;
    int l_size;
    glm::ivec2 l_atlasSize(ROC_FONT_ATLAS_SIZE);
    std::string l_filter;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadInteger(l_size);
    argStream.ReadNextInteger(l_atlasSize.x);
    argStream.ReadNextInteger(l_atlasSize.y);
    argStream.ReadNextText(l_filter);
    if(!argStream.HasErrors() && !l_path.empty() && l_size > 0)
    {
        int l_filteringType = Utils::Enum::ReadEnumVector(g_fontFilteringTypesTable, l_filter);
        if(l_filteringType == -1) l_filteringType = 0;
        Font *l_font = LuaManager::GetCore()->GetElementManager()->CreateFont_(l_path, l_size, l_atlasSize, l_filteringType);
        l_font ? argStream.PushPointer(l_font) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fontDraw(lua_State *f_vm)
{
    Font *l_font;
    glm::vec2 l_pos;
    std::string l_text;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_font);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    argStream.ReadText(l_text);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        sf::String l_utf32Text = sf::String::fromUtf8(l_text.begin(), l_text.end());
        LuaManager::GetCore()->GetRenderManager()->Render(l_font, l_pos, l_utf32Text, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
