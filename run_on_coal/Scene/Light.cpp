#include "stdafx.h"
#include "Scene/Light.h"

ROC::Light::Light()
{
    m_direction = glm::vec3(0.f,-1.f,0.f);
    m_color = glm::vec3(1.f,1.f,1.f);
    m_params = glm::vec4(0.f,1.f,0.f,0.f);
}
ROC::Light::~Light()
{
}
