/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
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

#include "../schema.h"
#include "ehandle.h"

class CEntitySubclassVDataBase
{
public:
	DECLARE_SCHEMA_CLASS(CEntitySubclassVDataBase)
};

class CBaseEntity : public CEntityInstance
{
public:
	DECLARE_SCHEMA_CLASS(CBaseEntity);

	SCHEMA_FIELD(uint32, m_fFlags)
	SCHEMA_FIELD(LifeState_t, m_lifeState)

	int Index() { return m_pEntity->m_EHandle.GetEntryIndex(); }

	int Slot() { return m_pEntity->m_EHandle.GetEntryIndex()-1; }

	const char* Classname() { return m_pEntity->m_designerName.String(); }

	bool IsAlive() { return m_lifeState == LifeState_t::LIFE_ALIVE; }
};

class CBaseModelEntity : public CBaseEntity
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(CBaseModelEntity);
};

class SpawnPoint : public CBaseEntity
{
public:
	DECLARE_SCHEMA_CLASS(SpawnPoint);

	SCHEMA_FIELD(bool, m_bEnabled);
};