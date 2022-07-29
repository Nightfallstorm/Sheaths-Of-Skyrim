#include "ActorSheathStorage.h"

namespace Serialization
{
	enum : std::uint32_t
	{
		kSaveID = 'nSoS',
		kVersion = '1',
		kSOSEvent = 'SoSE'
	};

	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		logger::info("Saving data...");
		ActorSheathStorage::animationEvent.Save(a_intfc, kSOSEvent, kVersion);
		logger::info("Data saved!");
	}
	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		logger::info("Loading save data...");
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version != kVersion) {
				logger::critical("Loaded data is out of date!");
				continue;
			}
			switch (type) {
			case kSOSEvent:
				ActorSheathStorage::animationEvent.Load(a_intfc);
				break;
			}
		}		
		logger::info("Save data loaded!");
	}

	void RevertCallback(SKSE::SerializationInterface* a_intfc)
	{
		logger::info("Reverting save data...");
		ActorSheathStorage::animationEvent.Revert(a_intfc);
		logger::info("Save data reverted!");
	}

	void FormDeleteCallback(RE::VMHandle a_handle)
	{
		ActorSheathStorage::animationEvent.UnregisterAll(a_handle);
		ActorSheathStorage::actorErections.erase(a_handle);
	}

}
