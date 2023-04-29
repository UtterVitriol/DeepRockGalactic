#include "Game.h"


extern "C" void mineral_hook();
extern "C" void health_hook();
extern "C" void objective_hook();


void MyGame::Start()
{
	// Get module base address.
	m_ModuleBase = (uintptr_t)GetModuleHandle(L"FSD-Win64-Shipping.exe");

	// Get FNameTable.
	m_FNameTable = (m_ModuleBase + m_FNameTableOffset);

	// Actually SavedGame.
	pSavedGame = (FSDSavedGame*)Hack::FindDMAAddy(m_ModuleBase + m_ResourcesFirstOffset, { 0x8, 0x5F8, 0x0 });

	// Player character in game.
	pCharacter = (Character*)Hack::FindDMAAddy(m_ModuleBase + m_CharacterFirstOffset, { 0x0, 0x20, 0x0 });

	if (pCharacter->numEquipables >= 2)
	{
		pPrimary = pCharacter->pEquipables->pPrimaryGun;
		GetFName(pPrimary->FNameIndex, m_PrimaryName);
	}

	if (pCharacter->numEquipables >= 3)
	{
		pSecondary = pCharacter->pEquipables->pSecondayGun;
		GetFName(pSecondary->FNameIndex, m_SecondaryName);
	}

	if (pCharacter->numEquipables >= 2)
	{
		pTraversal = pCharacter->pEquipables->pTraversalTool;
		GetFName(pTraversal->FNameIndex, m_TraversalName);
	}

	if (&pCharacter->pInventoryComponent)
	{
		pFlairs = pCharacter->pInventoryComponent;
	}

}

void MyGame::ValidateCharacter()
{
	pCharacter = (Character*)Hack::FindDMAAddy(m_ModuleBase + m_CharacterFirstOffset, { 0x0, 0x20, 0x0 });

	if (pCharacter != pLast)
	{
		bIsOnMission = false;
		pCharacter = NULL;
		ExitMission();


		while (!bIsOnMission)
		{
			Sleep(5);
		}


		Start();
		pLast = pCharacter;
		
	}

	return;
}

void MyGame::UpdateValues()
{

	ValidateCharacter();

	if (!pCharacter)
	{
		pLast = NULL;
	}

	return;


}

void MyGame::ExitMission()
{
	m_TelePortLocation = { 0 };

	bGoodWeapons = false;
	bGodWeapons = false;
	bRapidFire = false;
	bTeleport = false;
	bSteroids = false;

	Weapon* pPrimary = NULL;
	Weapon* pSecondary = NULL;
	Weapon* pTraversal = NULL;
	Weapon* pSupport = NULL;
	InventoryComponent* pFlairs = NULL;

}


void SetName(char* toSet, char* str)
{
	memset(toSet, 0, MAX_FNAME_LEN);

	for (int i = 0; i < MAX_FNAME_LEN; i++)
	{
		if (str[i] > 0 && str[i] < 255)
		{
			toSet[i] = str[i];
		}
		else
		{
			return;
		}
	}
	return;
}

char* MyGame::GetFName(uint32_t index, char* destination)
{
	char* Name = (char*)"Name Not Found";

	uint32_t FNameIndex = index;

	if (FNameIndex < 1)
	{
		return Name;
	}

	uint32_t NumElements = (uint32_t)m_FNameTable + 0xC;
	uint32_t NumChunks = (uint32_t)m_FNameTable + 0x8;
	uint32_t ChunkIndex = FNameIndex >> 16;
	uint32_t NameIndex = (FNameIndex << 16) >> 16;

	if (NameIndex < NumElements && ChunkIndex < NumChunks)
	{
		uintptr_t ChunkPtr = *(uintptr_t*)(m_FNameTable + ((ChunkIndex + 0x2) * 0x8));

		if (ChunkPtr)
		{
			uintptr_t NamePtr = (ChunkPtr + (NameIndex * 2));
			uintptr_t NameEntryIndex = *(uintptr_t*)(NamePtr);
			Name = (char*)(NamePtr + 0x2);
			if (destination)
			{
				SetName(destination, Name);
			}
		}
	}
	return Name;
}


void MyGame::SaveLocation()
{
	m_TelePortLocation.x = pCharacter->pCharacterMovement->pPosition->position.x;
	m_TelePortLocation.y = pCharacter->pCharacterMovement->pPosition->position.y;
	m_TelePortLocation.z = pCharacter->pCharacterMovement->pPosition->position.z;
}

void MyGame::TeleportLocation()
{
	pCharacter->pCharacterMovement->pPosition->position.x = m_TelePortLocation.x;
	pCharacter->pCharacterMovement->pPosition->position.y = m_TelePortLocation.y;
	pCharacter->pCharacterMovement->pPosition->position.z = m_TelePortLocation.z;
}


void MyGame::HookMinerals()
{
	if (bHookMinerals)
	{
		Hack::Detour((BYTE*)m_ModuleBase + m_HookMineralsOffset, mineral_hook, 16);
	}
	else {
		Hack::Patch((BYTE*)(m_ModuleBase + m_HookMineralsOffset),
			(BYTE*)"\xF3\x0F\x11\x49\x60\xF3\x0F\x11\x4C\x24\x28\xF3\x0F\x11\x41\x68", 16);
	}
}

void MyGame::HookObjective()
{
	if (bHookObjective)
	{
		Hack::Detour((BYTE*)m_ModuleBase + m_HookObjectiveOffset, objective_hook, 16);
	}
	else
	{
		Hack::Patch((BYTE*)(m_ModuleBase + m_HookObjectiveOffset),
			(BYTE*)"\xF3\x0F\x5D\x91\x88\x01\x00\x00\xF3\x0F\x11\x91\x8C\x01\x00\x00", 16);
	}
}


void MyGame::HookHealth()
{
	if (bHookHealth)
	{
		Hack::Detour((BYTE*)m_ModuleBase + m_HookHealthOffset, health_hook, 17);
	}
	else
	{
		Hack::Patch((BYTE*)(m_ModuleBase + m_HookHealthOffset),
			(BYTE*)"\xF3\x0F\x5D\xC8\x44\x0F\x28\xC9\xF3\x44\x0F\x11\x8B\xB0\x01\x00\x00", 17);
	}
}

void MyGame::HookArmor()
{
	if (bHookArmor) {
		Hack::Nop((BYTE*)(m_ModuleBase + m_ShieldDamageOffset), 8);
	}
	else {
		Hack::Patch((BYTE*)(m_ModuleBase + m_ShieldDamageOffset),
			(BYTE*)"\xF3\x0F\x11\x91\xC8\x02\x00\x00", 8);
	}
}