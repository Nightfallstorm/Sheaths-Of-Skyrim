

namespace ActorSheathStorage
{
	extern SKSE::RegistrationMap<RE::FormID, const RE::Actor*, std::uint32_t> animationEvent;
	extern std::map<RE::VMHandle, std::uint32_t> actorErections;

	void StoreActorErection(RE::Actor* akActor, int erectionlevel);

	int GetActorErection(RE::Actor* akActor);
}
