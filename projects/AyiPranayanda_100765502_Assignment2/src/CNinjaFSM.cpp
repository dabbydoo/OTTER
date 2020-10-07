/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020
*/

#include "CNinjaFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou
{
	const std::string CNinjaFSM::idleClip = "idle";
	const std::string CNinjaFSM::runClip = "run";
	const std::string CNinjaFSM::stealthClip = "stealth";
	const std::string CNinjaFSM::slashClip = "slash";
	const std::string CNinjaFSM::vSlashClip = "vSlash";
	const std::string CNinjaFSM::spinAttackClip = "spinAttack";
	const std::string CNinjaFSM::counterAttackClip = "counterAttack";
	const std::string CNinjaFSM::takeDamageClip = "takeDamage";
	const std::string CNinjaFSM::dieClip = "die";


	CNinjaFSM::CNinjaFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CNinjaFSM::SetState(AnimState state)
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
		case AnimState::STEALTH:
			animator.PlayOnce(stealthClip);
			break;
		case AnimState::SLASH:
			animator.PlayOnce(slashClip);
			break;
		case AnimState::VSLASH:
			animator.PlayOnce(vSlashClip);
			break;
		case AnimState::SPINATTACK:
			animator.PlayOnce(spinAttackClip);
			break;
		case AnimState::COUNTERATTACK:
			animator.PlayOnce(counterAttackClip);
			break;
		case AnimState::TAKEDAMAGE:
			animator.PlayOnce(takeDamageClip);
			break;
		case AnimState::DIE:
			animator.PlayOnce(dieClip);
			break;
		default:
			break;
		}
	}

	void CNinjaFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:

			if (GetVariable("running"))
				SetState(AnimState::RUN);
			if (GetTrigger("stealthing"))
				SetState(AnimState::STEALTH);

			if (GetTrigger("stealthing"))
				SetState(AnimState::STEALTH);

			break;
		case AnimState::RUN:

			if (!GetVariable("moving"))
				SetState(AnimState::IDLE);
			if (GetTrigger("stealthing"))
				SetState(AnimState::STEALTH);

			break;
		default:

			if (animator.IsDone())
				SetState(AnimState::IDLE);

			break;
		}
	}

}