/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2024 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "platform.h"
#include "stdint.h"
#include "utils/module.h"
#include "utlstring.h"
#include "variant.h"
#include "gameconfig.h"

#ifdef _WIN32
#define ROOTBIN "/bin/win64/"
#define GAMEBIN "/csgo/bin/win64/"
#else
#define ROOTBIN "/bin/linuxsteamrt64/"
#define GAMEBIN "/csgo/bin/linuxsteamrt64/"
#endif

namespace modules
{
	inline CModule *engine;
	inline CModule *tier0;
	inline CModule *server;
	inline CModule *schemasystem;
	inline CModule *vscript;
	inline CModule *client;
	inline CModule* networksystem;
#ifdef _WIN32
	inline CModule *hammer;
#endif
}

// Can't be forward-declared, can't include cgamerules.h.. just define it here
struct CGcBanInformation_t
{
	uint32_t m_uiReason;
	double m_dblExpiration;
	uint32_t m_uiAccountId;
};

namespace addresses
{
	bool Initialize(CGameConfig *g_GameConfig);
	bool InitializeBanMap(CGameConfig* g_GameConfig);

	inline CUtlMap<uint32, CGcBanInformation_t, uint32>* sm_mapGcBanInformation;
}