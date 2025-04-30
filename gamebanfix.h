#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#pragma once

#include "igameevents.h"
#include "tier0/commonmacros.h"
#include "networksystem/inetworkserializer.h"
#include <ISmmPlugin.h>
#include <iplayerinfo.h>
#include <iserver.h>
#include <sh_vector.h>

class GameBanFix : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	bool Pause(char *error, size_t maxlen);
	bool Unpause(char *error, size_t maxlen);
	void AllPluginsLoaded();
	void OnLevelInit( char const *pMapName,
				 char const *pMapEntities,
				 char const *pOldLevel,
				 char const *pLandmarkName,
				 bool loadGame,
				 bool background );
	void OnLevelShutdown();

public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
};

extern GameBanFix g_Plugin;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
