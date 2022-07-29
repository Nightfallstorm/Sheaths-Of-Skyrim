
#include "ActorSheathStorage.h"
#include "ScriptHelper.h"

namespace ActorSheathStorage
{
	SKSE::RegistrationMap<RE::FormID, const RE::Actor*, std::uint32_t> animationEvent{ "OnSOSEvent"sv };
	std::map<RE::VMHandle, std::uint32_t> actorErections;

		void StoreActorErection(RE::Actor* akActor, int erectionlevel)
	{
		RE::VMHandle handle = ScriptHelper::GetHandle(akActor);
		ActorSheathStorage::actorErections.erase(handle);
		ActorSheathStorage::actorErections.insert(std::pair(handle, erectionlevel));
	}

	int GetActorErection(RE::Actor* akActor)
	{
		RE::VMHandle handle = ScriptHelper::GetHandle(akActor);
		try {
			return ActorSheathStorage::actorErections.at(handle);
		} catch (...) {
			return MININT;
		}
	}
}
