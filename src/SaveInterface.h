#include "ActorSheathStorage.h"

namespace Serialization
{
	enum : std::uint32_t
	{
		kSaveID = 'nSOS',
		kSaveVersion = '3',
	};

	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		ActorSheathStorage::animationEvent.Save(a_intfc);
	}
	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		ActorSheathStorage::animationEvent.Load(a_intfc);
	}

	void RevertCallback(SKSE::SerializationInterface* a_intfc)
	{
		ActorSheathStorage::animationEvent.Revert(a_intfc);
	}

	void FormDeleteCallback(RE::VMHandle a_handle)
	{
		ActorSheathStorage::animationEvent.UnregisterAll(a_handle);
	}

}
