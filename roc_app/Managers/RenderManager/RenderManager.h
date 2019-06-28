#pragma once
#include "Interfaces/IRenderManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;
class VRManager;
class Drawable;
class Font;
class Model;
class Scene;
class Shader;
class RenderTarget;
class Texture;
class Quad2D;
class Quad3D;
class PhysicsDrawer;

class RenderManager final : public IRenderManager
{
    Core *m_core;
    VRManager *m_vrManager;

    bool m_active;
    bool m_vrActive;

    glm::vec3 m_modelPosition;
    glm::mat4 m_textureMatrix;

    glm::vec4 m_clearColor;
    glm::ivec2 m_viewportSize;

    Scene *m_activeScene;
    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    Texture *m_dummyTexture;
    PhysicsDrawer *m_physicsDrawer;
    Texture *m_lastTexture;

    CustomArguments m_arguments;

    float m_time;

    bool m_depthEnabled;
    bool m_blendEnabled;
    bool m_cullEnabled;
    bool m_skipNoDepthMaterials;
    void DisableDepth();
    void EnableDepth();
    void DisableBlending();
    void EnableBlending();
    void DisableCulling();
    void EnableCulling();

    RenderManager(const RenderManager &that) = delete;
    RenderManager& operator=(const RenderManager &that) = delete;

    // Interfaces reroute
    bool SetActiveScene(IScene *f_scene);
    bool Render(IFont *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color);
    bool Render(IDrawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    bool Render(IDrawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);
    bool DrawScene(IScene *f_scene);
public:
    bool SetActiveScene(Scene *f_scene);

    bool Render(Font *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color);
    bool Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    bool Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);
    bool DrawScene(Scene *f_scene);
    bool DrawPhysics(float f_width);

    bool ClearRenderArea(bool f_depth = true, bool f_color = true);
    bool SetClearColour(const glm::vec4 &f_color);
    bool SetViewport(const glm::ivec4 &f_area);
    bool SetPolygonMode(int f_mode);
protected:
    explicit RenderManager(Core *f_core);
    ~RenderManager();

    void RemoveAsActiveScene(Scene *f_scene);
    void RemoveAsActiveRenderTarget(RenderTarget *f_rt);

    void UpdateViewportSize(const glm::ivec2 &f_size);

    void DoPulse();

    friend class Core;
    friend class ElementManager;
    friend class InheritanceManager;
    friend class SfmlManager;
};

}
