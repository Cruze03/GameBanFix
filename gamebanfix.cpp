#include <stdio.h>
#include "gamebanfix.h"
#include "gameconfig.h"
#include "detours.h"
#include "utils/module.h"
#include "schemasystem/schemasystem.h"
#include <KeyValues.h>

CSchemaSystem* g_pSchemaSystem2 = nullptr;
IVEngineServer2 *g_pEngineServer2 = nullptr;
CGameConfig *g_GameConfig = nullptr;

GameBanFix g_Plugin;

void Message(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);


	char buf[1024] = {};
	V_vsnprintf(buf, sizeof(buf) - 1, msg, args);

	ConColorMsg(Color(255, 0, 255, 255), "\n[GameBanFix] %s\n", buf);

	va_end(args);
}

void Panic(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	char buf[1024] = {};
	V_vsnprintf(buf, sizeof(buf) - 1, msg, args);

	Warning("\n[GameBanFix] %s\n", buf);

	va_end(args);
}

PLUGIN_EXPOSE(GameBanFix, g_Plugin);
bool GameBanFix::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pSchemaSystem2, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pEngineServer2, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	CBufferStringGrowable<256> gamedirpath;
	g_pEngineServer2->GetGameDir(gamedirpath);

	std::string gamedirname = CGameConfig::GetDirectoryName(gamedirpath.Get());

	#ifdef _WIN32
		const char* gamedataPath = "addons/gamebanfix/bin/win64/gamedata/gamebanfix.games.txt";
	#else
		const char* gamedataPath = "addons/gamebanfix/bin/linuxsteamrt64/gamedata/gamebanfix.games.txt";
	#endif
	#ifdef DEBUG
	Message("Loading %s for game: %s\n", gamedataPath, gamedirname.c_str());
	#endif

	g_GameConfig = new CGameConfig(gamedirname, gamedataPath);
	char conf_error[255] = "";
	if (!g_GameConfig->Init(g_pFullFileSystem, conf_error, sizeof(conf_error)))
	{
		snprintf(error, maxlen, "Could not read %s: %s", g_GameConfig->GetPath().c_str(), conf_error);
		Error("%s\n", error);
		return false;
	}

	if (!addresses::Initialize(g_GameConfig))
	{
		snprintf(error, maxlen, "Could not initialize addresses. Signatures are likely outdated.");
		Error("%s\n", error);
		return false;
	}

	if (!InitDetours(g_GameConfig))
	{
		snprintf(error, maxlen, "Could not initialize detours.");
		Error("%s\n", error);
		return false;
	}

	return true;
}

bool GameBanFix::Unload(char *error, size_t maxlen)
{
	FlushAllDetours();

	return true;
}

bool GameBanFix::Pause(char *error, size_t maxlen)
{
	return true;
}

bool GameBanFix::Unpause(char *error, size_t maxlen)
{
	return true;
}

void GameBanFix::AllPluginsLoaded()
{
}

void GameBanFix::OnLevelInit(char const* pMapName,
	char const* pMapEntities,
	char const* pOldLevel,
	char const* pLandmarkName,
	bool loadGame,
	bool background)
{
}

void GameBanFix::OnLevelShutdown()
{
}

const char *GameBanFix::GetLicense()
{
	return "GPLv3";
}

const char *GameBanFix::GetVersion()
{
	return "1.0.1-minorfix";
}

const char *GameBanFix::GetDate()
{
	return __DATE__;
}

const char *GameBanFix::GetLogTag()
{
	return "GameBanFix";
}

const char *GameBanFix::GetAuthor()
{
	return "aiolos1045, Vauff - Standalone version by Cruze";
}

const char *GameBanFix::GetDescription()
{
	return "Fixes issue where if a player with game ban joins, other players even without a ban are then unable to join.";
}

const char *GameBanFix::GetName()
{
	return "Fix Game Bans";
}

const char *GameBanFix::GetURL()
{
	return "https://github.com/cruze03";
}