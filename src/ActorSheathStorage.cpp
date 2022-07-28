
#include "ActorSheathStorage.h"

namespace ActorSheathStorage
{
	SKSE::RegistrationMap<RE::FormID, const RE::Actor*, std::uint32_t> animationEvent{ "OnSOSEvent"sv };
}
