/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.cpp
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphAnimator.h"
#include "CMorphMeshRenderer.h"
#include "NOU/Mesh.h"

namespace nou
{
	CMorphAnimator::AnimData::AnimData()
	{
		frameTime = 1.0f;
	}

	CMorphAnimator::CMorphAnimator(Entity& owner)
	{
		m_owner = &owner;
		m_data = std::make_unique<AnimData>();
		m_timer = 0.0f;
		m_forwards = true;
	}

	void CMorphAnimator::Update(float deltaTime)
	{
		//TODO: Complete this function.
		if (m_data->frames.empty())
			return;

		float t;

		if (m_data->frameTime > 0.0f)
		{
			m_timer += deltaTime;
			
			if (m_timer > m_data->frameTime)
			{
				m_data->frameIndex += 1;

				if (m_data->frameIndex >= m_data->frames.size())
					m_data->frameIndex = 0;
			}

			m_timer = fmod(m_timer, m_data->frameTime);

			t = m_timer / m_data->frameTime;
		}
		else
			t = 0.0f;

		//segment index between p0 and p1
		int p0Index, p1Index;
		p1Index = m_data->frameIndex;
		p0Index = (p1Index == 0) ? m_data->frames.size() - 1 : p1Index - 1;

		m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[p0Index], *m_data->frames[p1Index], t);
	}

	void CMorphAnimator::SetFrames(const std::vector<std::unique_ptr<Mesh>> &frames)
	{
		for (int i = 0; i < frames.size(); i++)
		{
			m_data->frames.push_back(frames[i].get());
		}
		
	}

	void CMorphAnimator::SetFrameTime(float frameTime)
	{
		m_data->frameTime = frameTime;
	}

}