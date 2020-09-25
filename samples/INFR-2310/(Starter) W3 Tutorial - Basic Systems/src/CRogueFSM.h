#pragma once

#include "FSM/FSM.h"
#include "NOU/Entity.h"

namespace nou
{
	class CRogueFSM : public FSM
	{
	public:

		static const std::string idleClip;
		static const std::string runClip;
		static const std::string attackClip;

		enum class AnimState
		{
			IDLE = 0,
			RUN,
			ATTACK
		};

		CRogueFSM(Entity& owner);
		~CRogueFSM() = default;

		CRogueFSM(CRogueFSM&&) = default;
		CRogueFSM& operator=(CRogueFSM&&) = default;

		void SetState(AnimState state);
		void Update();

	private:

		Entity* m_owner;
		AnimState m_state;
	};
}