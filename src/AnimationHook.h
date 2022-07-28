#pragma once
#include "ActorSheathStorage.h"

using namespace RE;
using namespace RE::BSScript;

namespace AnimationHook
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;

	struct AnimationEventHook
	{
		static void thunk(VM* vm, RE::BSFixedString* eventName, std::int64_t unk2, RE::Actor* akActor,
			std::int64_t unk4)
		{
#ifdef _DEBUG
			logger::info("Hook called!");
#endif
			ActorSheathStorage::animationEvent.QueueEvent(akActor->formID, akActor, 2);
			return func(vm, eventName, unk2, akActor, unk4);
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};

	template <class T>
	static void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(14);

		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	// Install our hook at the specified address
	inline void Install()
	{
		// Debug::SendAnimationEvent(int64 a1, int64 a2, int64 a3, int64 a4, int64 a5) function call
		// Hook first call (BSFixedString::SET) as (VM* vm, RE::BSFixedString* eventName, unk0, RE::TESObjectREFR*
		// akActor, unk1)

		// 1.6.353: 1409971F0 (Closest ID is 1409971D0, ID 55380)
		// 1.5.97: 14096DE80 (Closest ID that is sync with VR is 14094d850, ID 53948)
		// 1.4.15: 1409A7F60 (Closest ID is 140987ab0, ID 53948)
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(53948, 55380), REL::VariantOffset(0x20630, 0x20, 0x204B0) };

		write_thunk_call<AnimationEventHook>(target.address());

		logger::info("Debug.SendAnimationEvent() hook at address {}", fmt::format("{:x}", target.address()));
		logger::info("Debug.SendAnimationEvent() hook at offset {}", fmt::format("{:x}", target.offset()));
	}

}  // namespace AnimationHook
