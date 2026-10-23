#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#pragma once

#include <ISmmPlugin.h>
#include "engine2/ivengineserver2.h"
#include "igameevents.h"
#include "tier0/commonmacros.h"
#include "networksystem/inetworkserializer.h"
#include "edict.h"
#include "playerslot.h"
#include "khook.hpp"

#ifdef AMBUILD
#include "version_gen.h"
#else
#include "version_gen_placeholder.h"
#endif

class GameBanFix : public ISmmPlugin, public IMetamodListener
{
  public:
    bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late);
    bool Unload(char* error, size_t maxlen);
    bool Pause(char* error, size_t maxlen);
    bool Unpause(char* error, size_t maxlen);
    void AllPluginsLoaded();
    void OnLevelInit(char const* pMapName, char const* pMapEntities, char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background);
    void OnLevelShutdown();

  public:
    const char* GetAuthor() override { return PLUGIN_AUTHOR; }
    const char* GetName() override { return PLUGIN_DISPLAY_NAME; }
    const char* GetDescription() override { return PLUGIN_DESCRIPTION; }
    const char* GetURL() override { return PLUGIN_URL; }
    const char* GetLicense() override { return PLUGIN_LICENSE; }
    const char* GetVersion() override { return PLUGIN_FULL_VERSION; }
    const char* GetDate() override { return __DATE__; }
    const char* GetLogTag() override { return PLUGIN_LOGTAG; }
};

extern GameBanFix g_Plugin;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
