#include "Papyrus.h"

using namespace RE;

namespace Papyrus {
    const std::string PapyrusClass = "SchlongRegister";

    bool RegisterSpecialSchlong(VM* vm, StackID id, StaticFunctionTag*, ActiveEffect* SchlongEnchantment, Actor* SOSActor) {
        return true;
    }

    bool UnRegisterSpecialSchlong(VM* vm, StackID id, StaticFunctionTag*, ActiveEffect* SchlongEnchantment, Actor* SOSActor) {
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
