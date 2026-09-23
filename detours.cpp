/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2026 Source2ZE
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
#include "detours.h"
#include "convar.h"
#include "gameconfig.h"
#include "khook_helpers.h"

KHOOK_FUNCTION(GameSystem_Think_CheckSteamBan, Detour_GameSystem_Think_CheckSteamBan, Detour_GameSystem_Think_CheckSteamBan_Post);

std::vector<CKHookBase*>& GetKHookList()
{
    static std::vector<CKHookBase*> s_vecSigHooks;
    return s_vecSigHooks;
}

void InitKHooks()
{
    for (auto hook : GetKHookList())
        hook->Configure();
}

KHook::Return<void> Detour_GameSystem_Think_CheckSteamBan()
{
    auto pMap = addresses::sm_mapGcBanInformation;
    static ConVarRefAbstract sv_kick_players_with_cooldown("sv_kick_players_with_cooldown");

    // Fix competitive cooldowns still being applied without sv_kick_players_with_cooldown 2
    if (sv_kick_players_with_cooldown.GetInt() < 2)
    {
        for (int i = pMap->FirstInorder(); i != pMap->InvalidIndex();)
        {
            int next = pMap->NextInorder(i);
            uint32_t reason = pMap->Element(i).m_uiReason;

            if (reason == 20 || reason == 22 || reason == 23) pMap->RemoveAt(i);

            i = next;
        }
    }

    return { KHook::Action::Ignore };
}

KHook::Return<void> Detour_GameSystem_Think_CheckSteamBan_Post()
{
    auto pMap = addresses::sm_mapGcBanInformation;

    // After player has been kicked, remove any ban entries, to prevent spreading to all new joining players
    // Implementation shared by @aiolos1045
    if (pMap->Count() > 0) pMap->RemoveAll();

    return { KHook::Action::Ignore };
}
