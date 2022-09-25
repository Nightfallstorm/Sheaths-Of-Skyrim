
#include "AnimationHook.h"
#include "Papyrus.h"
#include "SaveInterface.h"

void InitializeSerialization()
{
	logger::trace("Initializing cosave serialization...");
	auto* serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kSaveID);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);
	serialization->SetRevertCallback(Serialization::RevertCallback);
	serialization->SetFormDeleteCallback(Serialization::FormDeleteCallback);
	logger::trace("Cosave serialization initialized.");
}

void InitializePapyrus() {
	logger::trace("Initializing Papyrus binding...");
	auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(Papyrus::Bind);
}

void InitializeLog()
{
	auto path = logger::log_directory();
	if (!path) {
		//stl::report_and_fail("Failed to find standard logging directory"sv); // Doesn't work in VR
	}

	*path /= Version::PROJECT;
	*path += ".log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%H:%M:%S:%e] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

void InitializeHooking() {
	logger::trace("Initializing Animation hook...");
	AnimationHook::Install();
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion(Version::MAJOR);
	v.PluginName("Sheaths Of Skyrim");
	v.AuthorName("nightfallstorm");
	v.UsesAddressLibrary(true);
	v.HasNoStructUse(true);
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST_AE });

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	SKSE::Init(a_skse);
	InitializeHooking();
	InitializePapyrus();
	InitializeSerialization();
	logger::info("Loaded Plugin");
	return true;
}

extern "C" DLLEXPORT const char* APIENTRY GetPluginVersion()
{
	return Version::NAME.data();
}
