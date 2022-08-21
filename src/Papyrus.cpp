#include "Papyrus.h"
#include "ActorSheathStorage.h"
#include "SheathManipulator.h"

using namespace RE;

namespace Papyrus {
    const std::string PapyrusClass = "SchlongRegister";

    bool RegisterSpecialSchlong(VM* vm, StackID id, StaticFunctionTag*, ActiveEffect* SchlongEnchantment, Actor* SOSActor, BGSListForm* SchlongList)
	{
		if (SchlongEnchantment == nullptr || SOSActor == nullptr || SOSActor->formID == 0 || !SOSActor->Is3DLoaded()) {
			logger::error("Invalid actor or enchantment passed!");
			return false;
		}		
		if (SchlongList == nullptr || SchlongList->forms.size() != 4) {
			logger::error("Invalid schlong list");
			return false;
		}
				
		auto& regs = ActorSheathStorage::animationEvent;
		regs.Register(SchlongEnchantment, SOSActor->GetFormID());
		auto erection = ActorSheathStorage::GetActorErection(SOSActor);
		if (erection != MININT) {
			SheathManipulator::SetErectionLevel(SOSActor, SchlongList, erection, false);
		}
        return true;
    }

    bool UnRegisterSpecialSchlong(VM* vm, StackID id, StaticFunctionTag*, ActiveEffect* SchlongEnchantment) {
		if (SchlongEnchantment == nullptr) {
			logger::error("Invalid enchantment passed!");
			return false;
		}	
		ActorSheathStorage::animationEvent.UnregisterAll(SchlongEnchantment);
		return true;
    }

	bool ChangeActorErection(VM* vm, StackID id, StaticFunctionTag*, Actor* akActor, BGSListForm* SchlongList, std::uint32_t erectionLevel){
		if (akActor == nullptr || !akActor->Is3DLoaded() || SchlongList == nullptr || SchlongList->forms.size() != 4) {
			logger::error("Invalid params for changing actor erection!");
			return false;
		}
		ActorSheathStorage::StoreActorErection(akActor, erectionLevel);
		SheathManipulator::SetErectionLevel(akActor, SchlongList, erectionLevel, true);
		return true;
	}

    bool Bind(VM* a_vm) {
        if (!a_vm) {
            logger::critical("couldn't get VM State"sv);
            return false;
        }

        logger::info("{:*^30}", "FUNCTIONS"sv);

        a_vm->RegisterFunction("RegisterSpecialSchlong"sv, PapyrusClass, RegisterSpecialSchlong, true);
        a_vm->RegisterFunction("UnRegisterSpecialSchlong"sv, PapyrusClass, UnRegisterSpecialSchlong, true);
		a_vm->RegisterFunction("ChangeActorErection"sv, PapyrusClass, ChangeActorErection, true);

        logger::info("Registered RegisterSpecialSchlong"sv);
		logger::info("Registered UnRegisterSpecialSchlong"sv);
		logger::info("Registered ChangeActorErection");
        return true;
    }
}  // namespace Papyrus
