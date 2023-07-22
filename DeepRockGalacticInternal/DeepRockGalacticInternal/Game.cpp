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
	pSavedGame = (FSDSavedGame*)Hack::FindDMAAddy(m_ModuleBase + m_SavedGameFirstOffset, { 0x0 });

	// Player character in game.
	pLocalPlayer = (LocalPlayer*)Hack::FindDMAAddy(m_ModuleBase + m_LocalPlayerFirstOffset, { 0x0 });
	pCharacter = pLocalPlayer->pPlayerController->pCharacter;

	// Equipables is actuall child objects for our Character class.
	// But, these child objects are our equipable items.
	// e.g. Equipables[0] = Pickaxe , [1] = Primary weapon, etc.
	// When we are not in a mission, only our pickaxe is loaded,
	// and our number of child objects is only 1.
	if (pCharacter->numEquipables >= 2)
	{
		pPrimary = pCharacter->pEquipables->pPrimaryGun;
		memcpy(&m_Primary, pCharacter->pEquipables->pPrimaryGun, sizeof(m_Primary));
		memcpy(&m_WPrimary, pCharacter->pEquipables->pPrimaryGun->pWeaponFire, sizeof(m_Primary));
		GetFName(pPrimary->FNameIndex, m_PrimaryName);
	}

	if (pCharacter->numEquipables >= 3)
	{
		pSecondary = pCharacter->pEquipables->pSecondaryGun;
		memcpy(&m_Secondary, pCharacter->pEquipables->pSecondaryGun, sizeof(m_Secondary));
		memcpy(&m_WSecondary, pCharacter->pEquipables->pSecondaryGun->pWeaponFire, sizeof(m_Primary));
		GetFName(pSecondary->FNameIndex, m_SecondaryName);
	}

	if (pCharacter->numEquipables >= 4)
	{
		pTraversal = pCharacter->pEquipables->pTraversalTool;
		memcpy(&m_Traversal, pCharacter->pEquipables->pTraversalTool, sizeof(m_Traversal));
		GetFName(pTraversal->FNameIndex, m_TraversalName);
	}

	if (&pCharacter->pInventoryComponent)
	{
		pFlairs = pCharacter->pInventoryComponent;
	}

}

void MyGame::ValidateCharacter()
{
	// When we move between the base and a mission,
	// our character gets reloaded.
	//pCharacter = (Character*)Hack::FindDMAAddy(m_ModuleBase + m_CharacterFirstOffset, { 0x0, 0x20, 0x0 });
	pCharacter = pLocalPlayer->pPlayerController->pCharacter;

	if (pCharacter != pLast)
	{
		bIsOnMission = false;
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
	// This is our working function for our DLL.

	ValidateCharacter();

	if (!pCharacter)
	{
		pLast = NULL;
	}

	return;
}

void MyGame::ExitMission()
{
	m_TeleportLocation = { 0 };

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
	m_TeleportLocation.x = pCharacter->pCharacterMovement->pPosition->position.x;
	m_TeleportLocation.y = pCharacter->pCharacterMovement->pPosition->position.y;
	m_TeleportLocation.z = pCharacter->pCharacterMovement->pPosition->position.z;
}

void MyGame::TeleportLocation()
{
	pCharacter->pCharacterMovement->pPosition->position.x = m_TeleportLocation.x;
	pCharacter->pCharacterMovement->pPosition->position.y = m_TeleportLocation.y;
	pCharacter->pCharacterMovement->pPosition->position.z = m_TeleportLocation.z;
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

void MyGame::NoRecoil()
{
	if (bNoRecoil)
	{
		pPrimary->recoilPitchMax = 0;
		pPrimary->recoilPitchMin = 0;
		pPrimary->recoilYawMax = 0;
		pPrimary->recoilYawMin = 0;

		pSecondary->recoilPitchMax = 0;
		pSecondary->recoilPitchMin = 0;
		pSecondary->recoilYawMax = 0;
		pSecondary->recoilYawMin = 0;

		pTraversal->recoilPitchMax = 0;
		pTraversal->recoilPitchMin = 0;
		pTraversal->recoilYawMax = 0;
		pTraversal->recoilYawMin = 0;
	}
	else
	{
		pPrimary->recoilPitchMax = m_Primary.recoilPitchMax;
		pPrimary->recoilPitchMin = m_Primary.recoilPitchMin;
		pPrimary->recoilYawMax = m_Primary.recoilYawMax;
		pPrimary->recoilYawMin = m_Primary.recoilYawMin;

		pSecondary->recoilPitchMax = m_Secondary.recoilPitchMax;
		pSecondary->recoilPitchMin = m_Secondary.recoilPitchMin;
		pSecondary->recoilYawMax = m_Secondary.recoilYawMax;
		pSecondary->recoilYawMin = m_Secondary.recoilYawMin;

		pTraversal->recoilPitchMax = m_Traversal.recoilPitchMax;
		pTraversal->recoilPitchMin = m_Traversal.recoilPitchMin;
		pTraversal->recoilYawMax = m_Traversal.recoilYawMax;
		pTraversal->recoilYawMin = m_Traversal.recoilYawMin;
	}
}

void MyGame::NoBulletSpread()
{
	if (bNoBulletSpread)
	{
		pPrimary->pWeaponFire->maxSpread = 0;
		pPrimary->pWeaponFire->minSpead = 0;
		pPrimary->pWeaponFire->maxVerticalSpread = 0;
		pPrimary->pWeaponFire->maxHorizontalSpread = 0;

		pSecondary->pWeaponFire->maxSpread = 0;
		pSecondary->pWeaponFire->minSpead = 0;
		pSecondary->pWeaponFire->maxVerticalSpread = 0;
		pSecondary->pWeaponFire->maxHorizontalSpread = 0;
	}
	else
	{
		pPrimary->pWeaponFire->maxSpread = m_WPrimary.maxSpread;
		pPrimary->pWeaponFire->minSpead = m_WPrimary.minSpead;
		pPrimary->pWeaponFire->maxVerticalSpread = m_WPrimary.maxVerticalSpread;
		pPrimary->pWeaponFire->maxHorizontalSpread = m_WPrimary.maxHorizontalSpread;

		pSecondary->pWeaponFire->maxSpread = m_WSecondary.maxSpread;
		pSecondary->pWeaponFire->minSpead = m_WSecondary.minSpead;
		pSecondary->pWeaponFire->maxVerticalSpread = m_WSecondary.maxVerticalSpread;
		pSecondary->pWeaponFire->maxHorizontalSpread = m_WSecondary.maxHorizontalSpread;
	}
}