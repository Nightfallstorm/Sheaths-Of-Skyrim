#pragma once
#include "ActorSheathStorage.h"

using namespace SKSE::log;
using namespace RE;
using namespace RE::BSScript;
using namespace SKSE;

namespace AnimationHook
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;

	struct AnimationEventHook
	{
		static void thunk(VM* vm, RE::BSFixedString* eventName, std::int64_t unk2, RE::TESObjectREFR* akActor,
			std::int64_t unk4)
		{
#ifdef _DEBUG
			logger::info("Hook called!");
#endif
			if (akActor == nullptr || akActor->As<RE::Actor>() == nullptr || eventName == nullptr) {
				logger::info("Invalid animation event for SOS handling");
				return func(vm, eventName, unk2, akActor, unk4);
			}
			handleAnimationEvent(akActor->As<RE::Actor>(), eventName);

			return func(vm, eventName, unk2, akActor, unk4);
		}

		static void handleAnimationEvent(RE::Actor* akActor, RE::BSFixedString* eventName)
		{
			if (!akActor->Is3DLoaded()) {
				// Actor not loaded, do nothing
			} else if (IsSOSErection(eventName)) {
#ifdef _DEBUG
				std::string DisplayName = akActor->GetDisplayFullName();
				std::string SOSEventName = eventName->c_str();
				std::string message = SOSEventName + " on " + DisplayName;
				RE::DebugNotification(message.c_str());
#endif
				ActorSheathStorage::animationEvent.QueueEvent(akActor->formID, akActor, GetSOSErectionlevel(eventName));
			}
		}

		inline static std::string SOSFlaccid = "SOSFlaccid";
		inline static std::string SOSBend = "SOSBend";
		inline static std::string SOSSlowErect = "SOSSlowErect";
		inline static std::string SOSFastErect = "SOSFastErect";
		inline static int SOSFlaccidLevel = -11;
		inline static int SOSFastErectLevel = 11;
		inline static int SOSSlowErectLevel = 12;

		static bool IsSOSErection(RE::BSFixedString* eventName)
		{
			std::string name = eventName->c_str();
			return iequals(SOSBend, name.substr(0, SOSBend.size())) || iequals(SOSFastErect, name) ||
			       iequals(SOSSlowErect, name) || iequals(SOSFlaccid, name);
		}

		static int GetSOSErectionlevel(RE::BSFixedString* eventName)
		{
			std::string name = eventName->c_str();
			if (iequals(SOSBend, name.substr(0, SOSBend.size()))) {
				return std::stoi(name.substr(SOSBend.size()));
			} else if (iequals(SOSFastErect, name)) {
				return SOSFastErectLevel;
			} else if (iequals(SOSSlowErect, name)) {
				return SOSSlowErectLevel;
			} else {
				return SOSFlaccidLevel;
			}
		}

#pragma warning(push)
		static bool iequals(const std::string& a, const std::string& b)
		{
			std::size_t sz = a.size();
			if (b.size() != sz)
				return false;
			for (unsigned int i = 0; i < sz; ++i)
				if (tolower(a[i]) != tolower(b[i]))
					return false;
			return true;
		}
#pragma warning(pop)

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
