#include "stdafx.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Animation/BoneFrameData.h"
#include "Utils/Utils.h"

ROC::Animation::Animation()
{
    m_elementType = ElementType::AnimationElement;

    m_animFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_duration = 0U;
    m_durationTotal = 0U;
    m_fps = 0U;
    m_frameDelta = 0U;
    m_frameSize = 0U;
    m_bonesValue = 0U;
    m_lastLeftFrame = UINT_MAX;
    m_loaded = false;
}
ROC::Animation::~Animation()
{
    Clean();
}

void ROC::Animation::Clean()
{
    m_animFile.close();
    for(auto iter : m_rightFrame) delete iter;
    m_rightFrame.resize(0U);
    for(auto iter : m_leftFrame) delete iter;
    m_leftFrame.resize(0U);
    for(auto iter : m_interpolatedFrame) delete iter;
    m_interpolatedFrame.resize(0U);
    m_duration = 0U;
    m_bonesValue = 0U;
    m_fps = 0U;
    m_durationTotal = 0U;
    m_loaded = false;
}

bool ROC::Animation::Load(const std::string &f_path)
{
    if(!m_loaded)
    {
        try
        {
            m_animFile.open(f_path, std::ios::binary | std::ios::in);
            m_animFile.read(reinterpret_cast<char*>(&m_fps), sizeof(unsigned int));
            m_animFile.read(reinterpret_cast<char*>(&m_duration), sizeof(unsigned int));
            m_animFile.read(reinterpret_cast<char*>(&m_bonesValue), sizeof(unsigned int));

            m_durationTotal = static_cast<unsigned int>((static_cast<float>(m_duration) / static_cast<float>(m_fps))*1000.f);
            m_frameDelta = static_cast<unsigned int>((1.0 / static_cast<float>(m_fps))*1000.0);

            m_frameSize = m_bonesValue * 10U * sizeof(float);

            for(unsigned int i = 0; i < m_bonesValue; i++)
            {
                m_leftFrame.push_back(new BoneFrameData());
                m_rightFrame.push_back(new BoneFrameData());
                m_interpolatedFrame.push_back(new BoneFrameData());
            }
            CacheData(0U);

            m_loaded = true;
        }
        catch(const std::ifstream::failure&)
        {
            Clean();
        }
    }
    return m_loaded;
}

bool ROC::Animation::CacheData(unsigned int f_tick)
{
    unsigned int l_delta = f_tick%m_frameDelta;
    unsigned int l_leftFrame = ((f_tick - l_delta) / m_frameDelta) % m_duration;
    float l_lerp = static_cast<float>(l_delta) / static_cast<float>(m_frameDelta);
    bool l_interpolate = true;
    if(m_lastLeftFrame != l_leftFrame)
    {
        try
        {
            // Cache left frame
            m_animFile.seekg(12U + (l_leftFrame%m_duration)*m_frameSize, std::ios::beg);
            for(unsigned int i = 0; i < m_bonesValue; i++)
            {
                m_animFile.read(reinterpret_cast<char*>(&m_leftFrame[i]->m_position), sizeof(glm::vec3));
                m_animFile.read(reinterpret_cast<char*>(&m_leftFrame[i]->m_rotation), sizeof(glm::quat));
                m_animFile.read(reinterpret_cast<char*>(&m_leftFrame[i]->m_scale), sizeof(glm::vec3));
            }
            //Cache right frame
            m_animFile.seekg(12U + ((l_leftFrame + 1U) % m_duration)*m_frameSize, std::ios::beg);
            for(unsigned int i = 0; i < m_bonesValue; i++)
            {
                m_animFile.read(reinterpret_cast<char*>(&m_rightFrame[i]->m_position), sizeof(glm::vec3));
                m_animFile.read(reinterpret_cast<char*>(&m_rightFrame[i]->m_rotation), sizeof(glm::quat));
                m_animFile.read(reinterpret_cast<char*>(&m_rightFrame[i]->m_scale), sizeof(glm::vec3));
            }
            m_lastLeftFrame = l_leftFrame;
        }
        catch(const std::ifstream::failure&)
        {
            l_interpolate = false;
        }
    }
    //Interpolate
    if(l_interpolate)
    {
        for(unsigned int i = 0; i < m_bonesValue; i++)
        {
            m_interpolatedFrame[i]->m_position = glm::lerp(m_leftFrame[i]->m_position, m_rightFrame[i]->m_position, l_lerp);
            m_interpolatedFrame[i]->m_rotation = glm::slerp(m_leftFrame[i]->m_rotation, m_rightFrame[i]->m_rotation, l_lerp);
            m_interpolatedFrame[i]->m_scale = glm::lerp(m_leftFrame[i]->m_scale, m_rightFrame[i]->m_scale, l_lerp);
        }
    }
    return m_animFile.good();
}
