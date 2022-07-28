#include "Papyrus.h"
#include "ActorSheathStorage.h"

using namespace RE;

namespace Papyrus {
    const std::string PapyrusClass = "SchlongRegister";

    bool RegisterSpecialSchlong(VM* vm, StackID id, StaticFunctionTag*, ActiveEffect* SchlongEnchantment, Actor* SOSActor) {
		if (SchlongEnchantment == nullptr || SOSActor == nullptr || SOSActor->formID == 0 || !SOSActor->Is3DLoaded()) {
			logger::error("Invalid actor or enchantment passed!");
			return false;
		}		
		auto& regs = ActorSheathStorage::animationEvent;
		regs.Register(SchlongEnchantment, SOSActor->GetFormID());
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

    bool Bind(VM* a_vm) {
        if (!a_vm) {
            logger::critical("couldn't get VM State"sv);
            return false;
        }

        logger::info("{:*^30}", "FUNCTIONS"sv);

        a_vm->RegisterFunction("RegisterSpecialSchlong"sv, PapyrusClass, RegisterSpecialSchlong, true);
        a_vm->RegisterFunction("UnRegisterSpecialSchlong"sv, PapyrusClass, UnRegisterSpecialSchlong, true);

        logger::info("Registered RegisterSpecialSchlong"sv);
		logger::info("Registered UnRegisterSpecialSchlong"sv);
        return true;
    }
}  // namespace Papyrus
