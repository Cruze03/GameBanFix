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

#include "addresses.h"
#include "utils/module.h"

#include "tier0/memdbgon.h"

extern CGameConfig *g_GameConfig;

#define RESOLVE_SIG(gameConfig, name, variable) \
	variable = (decltype(variable))gameConfig->ResolveSignature(name);	\
	if (!variable)														\
	{																	\
		Error("Failed to resolve %s\n", name);						\
		return false;													\
	}																	\
	Message("Found %s at 0x%p\n", name, variable);

bool addresses::Initialize(CGameConfig *g_GameConfig)
{
	modules::engine = new CModule(ROOTBIN, "engine2");
	modules::tier0 = new CModule(ROOTBIN, "tier0");
	modules::server = new CModule(GAMEBIN, "server");
	modules::schemasystem = new CModule(ROOTBIN, "schemasystem");
	modules::vscript = new CModule(ROOTBIN, "vscript");
	modules::networksystem = new CModule(ROOTBIN, "networksystem");
	modules::client = nullptr;

	if (!CommandLine()->HasParm("-dedicated"))
		modules::client = new CModule(GAMEBIN, "client");

#ifdef _WIN32
	modules::hammer = nullptr;
	if (CommandLine()->HasParm("-tools"))
		modules::hammer = new CModule(ROOTBIN, "tools/hammer");
#endif

	return InitializeBanMap(g_GameConfig);
}

bool addresses::InitializeBanMap(CGameConfig* g_GameConfig)
{
	// This signature directly points to the instruction referencing sm_mapGcBanInformation
	uintptr_t pAddr = (uintptr_t)g_GameConfig->ResolveSignature("CCSGameRules__sm_mapGcBanInformation");

	if (!pAddr)
		return false;

	// the opcode is 3 bytes so we skip those
	pAddr += 3;

	// Grab the offset as 4 bytes
	uint32 offset = *(uint32*)pAddr;

	// Go to the next instruction, which is what the relative address is based off
	pAddr += 4;

	// Get the real address
	addresses::sm_mapGcBanInformation = (decltype(addresses::sm_mapGcBanInformation))(pAddr + offset);

	if (!addresses::sm_mapGcBanInformation)
		return false;

	Message("Found %s at 0x%p\n", "CCSGameRules__sm_mapGcBanInformation", addresses::sm_mapGcBanInformation);
	return true;
}