namespace SheathManipulator
{
	// Below is functions copied from the original papyrus converted to C++

	void SetErectionLevel(RE::Actor* akActor, RE::BGSListForm* SchlongList, int level, bool fromAnimationHook);

	RE::BSFixedString GetErectionAsEventName(int erectionLevel);

	RE::TESObjectARMO* GetCurrentSheath(RE::Actor* akActor);

	RE::TESForm* GetSheathAtIndex(RE::BGSListForm* sheathList, int index);

	int ComputeMeshLevel(int sosErectionLevel);

	int CurrentMeshLevel(RE::Actor* akActor, RE::BGSListForm* sheathList);

	void StoreActorErection(RE::Actor* akActor, int erectionlevel);

	int GetActorErection(RE::Actor* akActor);
}
