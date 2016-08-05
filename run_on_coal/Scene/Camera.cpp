#include "stdafx.h"
#include "Scene/Camera.h"

ROC::Camera::Camera()
{
    m_viewPosition = glm::vec3(0.f);
    m_viewDirection = glm::vec3(0.f,0.f,1.f);
    m_viewMatrix = glm::lookAt(m_viewPosition,m_viewPosition+m_viewDirection,glm::vec3(0.0f,1.0f,0.0f));

    m_fov = glm::pi<float>()/4.0f;
    m_perspectiveSize = glm::vec2(640.f,480.f);
    m_orthogonalSize = glm::vec4(-1.f,1.f,1.f,-1.f);
    m_depth = glm::vec2(0.2f,100.f);

    m_projectionType = CAMERA_PROJECTION_PERSPECTIVE;
    m_projectionMatrix = glm::perspectiveFov(m_fov,m_perspectiveSize.x,m_perspectiveSize.y,m_depth.x,m_depth.y);

    m_rebuildView = false;
    m_rebuildProjection = false;
}
ROC::Camera::~Camera()
{
}

void ROC::Camera::SetPosition(glm::vec3& f_pos)
{
    if(!std::memcmp(&m_viewPosition,&f_pos,sizeof(glm::vec3))) return;
    std::memcpy(&m_viewPosition,&f_pos,sizeof(glm::vec3));
    m_rebuildView = true;
}
void ROC::Camera::SetDirection(glm::vec3& f_pos)
{
    if(!std::memcmp(&m_viewDirection,&f_pos,sizeof(glm::vec3))) return;
    std::memcpy(&m_viewDirection,&f_pos,sizeof(glm::vec3));
    m_rebuildView = true;
}

void ROC::Camera::SetFOV(float f_fov)
{
    if(m_fov == f_fov) return;
    m_fov = f_fov;
    m_rebuildProjection = true;
}
void ROC::Camera::SetPerspectiveSize(glm::vec2 &f_size)
{
    if(!std::memcmp(&f_size,&m_perspectiveSize,sizeof(glm::vec2))) return;
    std::memcpy(&m_perspectiveSize,&f_size,sizeof(glm::vec2));
    m_rebuildProjection = true;
}
void ROC::Camera::SetOrthoSize(glm::vec4 &f_size)
{
    if(!std::memcmp(&f_size,&m_orthogonalSize,sizeof(glm::vec4))) return;
    std::memcpy(&m_orthogonalSize,&f_size,sizeof(glm::vec4));
    m_rebuildProjection = true;
}
void ROC::Camera::SetDepth(glm::vec2 &f_depth)
{
    if(!std::memcmp(&f_depth,&m_depth,sizeof(glm::vec2))) return;
    std::memcpy(&m_depth,&f_depth,sizeof(glm::vec2));
    m_rebuildProjection = true;
}

bool ROC::Camera::IsInFrustum(glm::vec3 &f_pos, float f_radius)
{
    for(auto &iter : m_planes)
    {
        if(iter.x*f_pos.x+iter.y*f_pos.y+iter.z*f_pos.z+iter.w < -f_radius) return false;
    }
    return true;
}

void ROC::Camera::GetViewMatrix(glm::mat4 &f_mat)
{
    UpdateMatrices();
    std::memcpy(&f_mat,&m_viewMatrix,sizeof(glm::mat4));
}
void ROC::Camera::GetProjectionMatrix(glm::mat4 &f_mat)
{
    UpdateMatrices();
    std::memcpy(&f_mat,&m_projectionMatrix,sizeof(glm::mat4));
}

void ROC::Camera::UpdateMatrices()
{
    if(m_rebuildView) m_viewMatrix = glm::lookAt(m_viewPosition,m_viewPosition+m_viewDirection,glm::vec3(0.f,1.f,0.f));
    if(m_rebuildProjection)
    {
        switch(m_projectionType)
        {
            case CAMERA_PROJECTION_PERSPECTIVE:
                m_projectionMatrix = glm::perspectiveFov(m_fov,m_perspectiveSize.x,m_perspectiveSize.y,m_depth.x,m_depth.y);
                break;
            case CAMERA_PROJECTION_ORTHOGONAL:
                m_projectionMatrix = glm::ortho(m_orthogonalSize.x,m_orthogonalSize.y,m_orthogonalSize.z,m_orthogonalSize.w,m_depth.x,m_depth.y);
                break;
        }
    }
    if(m_rebuildView || m_rebuildProjection)
    {
        glm::mat4 l_mat = m_projectionMatrix*m_viewMatrix;
        m_planes[0] = glm::row(l_mat,3)+glm::row(l_mat,0);
        m_planes[1] = glm::row(l_mat,3)-glm::row(l_mat,0);
        m_planes[2] = glm::row(l_mat,3)+glm::row(l_mat,1);
        m_planes[3] = glm::row(l_mat,3)-glm::row(l_mat,1);
        m_planes[4] = glm::row(l_mat,3)+glm::row(l_mat,2);
        m_planes[5] = glm::row(l_mat,3)-glm::row(l_mat,2);
        for(auto &iter : m_planes) iter /= sqrtf(iter.x*iter.x+iter.y*iter.y+iter.z*iter.z);
    }
    m_rebuildView = false;
    m_rebuildProjection = false;
}
