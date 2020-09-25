#include "CRogueFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou
{
	//String labels for animations.
	const std::string CRogueFSM::idleClip = "idle";
	const std::string CRogueFSM::runClip = "walk";
	const std::string CRogueFSM::attackClip = "attack";

	CRogueFSM::CRogueFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CRogueFSM::SetState(CRogueFSM::AnimState state)
	{
		m_state = state;

		//Gets the animator from the entity.
		auto& animator = m_owner->Get<CSpriteAnimator>();

		ClearTriggers();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:
			animator.PlayLoop(idleClip);
			break;
		case AnimState::RUN:
			animator.PlayLoop(runClip);
			break;
		case AnimState::ATTACK:
			animator.PlayOnce(attackClip);
			break;
		default:
			break;
		}
	}


	void CRogueFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:

			if (GetVariable("movingRogue"))
				SetState(AnimState::RUN);
			if (GetTrigger("attackingRogue"))
				SetState(AnimState::ATTACK);

			break;
		case AnimState::RUN:

			if (!GetVariable("movingRogue"))
				SetState(AnimState::IDLE);
			if (GetTrigger("attackingRogue"))
				SetState(AnimState::ATTACK);

			break;
		default:

			if (animator.IsDone())
				SetState(AnimState::IDLE);

			break;
		}
	}
}