#include "SheathManipulator.h"
#include "ScriptHelper.h"
#include "ActorSheathStorage.h"

using namespace RE;

namespace SheathManipulator
{
	using VM = RE::BSScript::Internal::VirtualMachine;

	inline static int SOSFlaccidLevel = -11;
	inline static int SOSFastErectLevel = 11;
	inline static int SOSSlowErectLevel = 12;


	// Below is functions copied from the original papyrus converted to C++ with no storing

	void SetErectionLevel(RE::Actor* akActor, RE::BGSListForm* SchlongList, int level, bool fromAnimationHook)
	{

		std::string name = akActor->GetDisplayFullName();
		logger::info("Setting erection for {} to {}", name, std::to_string(level));
		int curMeshLevel = CurrentMeshLevel(akActor, SchlongList);
		if (curMeshLevel < 0) {
#ifdef _DEBUG
			DebugNotification("Schlong is covered!");
#endif  //  DEBUG
			return;
		}

		int newMeshLevel = ComputeMeshLevel(level);

		if (curMeshLevel != newMeshLevel) {
			//Have to have changed mesh level and have to be showing one of our schlongs
			RE::TESObjectARMO* newSheath = GetSheathAtIndex(SchlongList, newMeshLevel)->As<RE::TESObjectARMO>();
			logger::info("Setting new sheath to " + fmt::format("{:x}", newSheath->formID));  // Formats to HEX for easier reading
			auto ActorPtr = ScriptHelper::GetObjectPtr(akActor, "Actor", false);
			auto callback = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();

			auto args = RE::MakeFunctionArguments(std::move(newSheath->As<RE::TESForm>()), false, true);
			ScriptHelper::DispatchMethodCall(ActorPtr, "EquipItem"sv, callback, args);
			const auto task = SKSE::GetTaskInterface();
			// Try to force the engine to correctly equip new schlong
			task->AddTask([ActorPtr, callback, args]() {
				ScriptHelper::DispatchMethodCall(ActorPtr, "EquipItem"sv, callback, args);
			});
		}
		if (!fromAnimationHook) {
			auto callback2 = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
			logger::info("Duplicating animation event");
			ScriptHelper::DispatchStaticCall("Debug", "SendAnimationEvent", callback2, std::move(akActor->As<RE::TESObjectREFR>()), GetErectionAsEventName(level));
		}
	}

	BSFixedString GetErectionAsEventName(int erectionLevel)
	{
		if (erectionLevel == SOSFlaccidLevel) {
			return "SOSFlaccid";
		} else if (erectionLevel == SOSFastErectLevel) {
			return "SOSFastErect";
		} else if (erectionLevel == SOSSlowErectLevel) {
			return "SOSSlowErect";
		} else {
			return ("SOSBend" + std::to_string(erectionLevel));
		}
	}

	TESObjectARMO* GetCurrentSheath(Actor* akActor)
	{
		return akActor->GetWornArmor(BIPED_MODEL::BipedObjectSlot::kModPelvisSecondary);
	}

	TESForm* GetSheathAtIndex(BGSListForm* sheathList, int index)
	{
		return sheathList->forms[index];
	}

	int ComputeMeshLevel(int sosErectionLevel)
	{
		if (sosErectionLevel < -7)
			return 0;
		else if (sosErectionLevel < -3)
			return 1;
		else if (sosErectionLevel < 0)
			return 2;
		else
			return 3;
	}

	int CurrentMeshLevel(Actor* akActor, BGSListForm* sheathList)
	{
		if (GetCurrentSheath(akActor) == nullptr) {
			// No sheath on actor! Possibly covered? Or not using a sheath schlong anymore
			logger::error("No sheath mesh found!");
			return -1;
		}
		BSTArray<TESForm*> sheaths = sheathList->forms;
		for (std::uint32_t i = 0; i < sheaths.size(); i++) {
			if (sheaths[i] == nullptr) {
				continue;
			}
			if (GetCurrentSheath(akActor)->formID == sheaths[i]->formID) {
				return i;
			}
		}
		logger::info("No mesh level found");
		return -1;
	}
}
