/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020
*/

#pragma once

#include "FSM/FSM.h"
#include "NOU/Entity.h"

namespace nou
{
	class CNinjaFSM : public FSM
	{
		public:

			static const std::string idleClip;
			static const std::string runClip;
			static const std::string stealthClip;
			static const std::string slashClip;
			static const std::string vSlashClip;
			static const std::string spinAttackClip;
			static const std::string counterAttackClip;
			static const std::string takeDamageClip;
			static const std::string dieClip;

			enum class AnimState
			{
				IDLE = 0,
				RUN,
				STEALTH,
				SLASH,
				VSLASH,
				SPINATTACK,
				COUNTERATTACK,
				TAKEDAMAGE,
				DIE
			};

			CNinjaFSM(Entity& owner);
			~CNinjaFSM() = default;

			CNinjaFSM(CNinjaFSM&&) = default;
			CNinjaFSM& operator=(CNinjaFSM&&) = default;

			void SetState(AnimState state);
			void Update();

		private:

			Entity* m_owner;
			AnimState m_state;
	};
}