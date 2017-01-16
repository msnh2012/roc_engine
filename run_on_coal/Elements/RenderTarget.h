#pragma once
#define RENDERTARGET_TYPE_NONE -1
#define RENDERTARGET_TYPE_DEPTH 0
#define RENDERTARGET_TYPE_RGB 1
#define RENDERTARGET_TYPE_RGBA 2
#define RENDERTARGET_TYPE_RGBF 3

namespace ROC
{

class RenderTarget
{
    int m_type;

    GLuint m_frameBuffer;
    bool m_bFrameBuffer;
    GLuint m_renderBuffer;
    bool m_bRenderBuffer;
    bool m_bTexture;
    GLuint m_texture;

    glm::ivec2 m_size;

    std::string m_error;
    void Clear();
public:
    inline void GetSize(glm::ivec2 &f_size) { std::memcpy(&f_size, &m_size, sizeof(glm::ivec2)); }

    inline bool IsColored() { return (m_type >= RENDERTARGET_TYPE_RGB && m_type <= RENDERTARGET_TYPE_RGBF); }
    inline bool IsTransparent() { return (m_type == RENDERTARGET_TYPE_RGBA); }
    inline bool IsDepthable() { return (m_type == RENDERTARGET_TYPE_DEPTH); }
protected:
    RenderTarget();
    ~RenderTarget();
    bool Create(unsigned int f_num, glm::ivec2 &f_size, int f_type);
    inline GLuint GetTextureID() { return m_texture; }

    void BindTexture(unsigned int f_bind);
    void Enable();

    inline void GetError(std::string &f_str) { f_str.append(m_error); }
    friend class ElementManager;
    friend class RenderManager;
    friend class Shader;
};

}