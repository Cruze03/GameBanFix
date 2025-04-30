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
#include "cdetour.h"
#include "detours.h"
#include "gameconfig.h"

#include "tier0/memdbgon.h"

CUtlVector<CDetourBase *> g_vecDetours;

DECLARE_DETOUR(GameSystem_Think_CheckSteamBan, Detour_GameSystem_Think_CheckSteamBan);


bool InitDetours(CGameConfig *gameConfig)
{
	bool success = true;

	FOR_EACH_VEC(g_vecDetours, i)
	{
		if (!g_vecDetours[i]->CreateDetour(gameConfig))
			success = false;

		g_vecDetours[i]->EnableDetour();
	}

	return success;
}

void FlushAllDetours()
{
	g_vecDetours.Purge();
}

void FASTCALL Detour_GameSystem_Think_CheckSteamBan()
{
	// Implementation shared by @aiolos1045
	GameSystem_Think_CheckSteamBan();

	CUtlMap<uint32, CGcBanInformation_t, uint32>* pMap = addresses::sm_mapGcBanInformation;
	unsigned int count = pMap->Count();

	// After player has been kicked, remove any ban entries, to prevent spreading to all new joining players
	if (count > 0)
		pMap->RemoveAll();
}