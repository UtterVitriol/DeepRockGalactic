#include "Player.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DERR(s) log.AddLog("[-]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)
#define DMSG(s) log.AddLog("[+]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)

void MyPlayer::Start()
{
	moduleBase = (uintptr_t)GetModuleHandle(L"FSD-Win64-Shipping.exe");

	Shoot = (tShoot)(moduleBase + 0x1514AA0);
	fNameTable = (moduleBase + fNameTableOffset);

	// TODO: Resources Bad naming....
	pPlayer = (Player*)Hack::FindDMAAddy(moduleBase + firstOffset_Resources, { 0x8, 0x5F8, 0x0 });

	pGameData = (GameData*)Hack::FindDMAAddy(moduleBase + firstOffset_GameData, { 0x0, 0x20, 0x0 });
	pWeapon = pGameData->pWeaponData->pCurrentWeapon;


	pLast = pGameData;
}



void MyPlayer::Validate()
{
	pGameData = (GameData*)Hack::FindDMAAddy(moduleBase + firstOffset_GameData, { 0x0, 0x20, 0x0 });

	if (pGameData != pLast)
	{
		bIsOnMission = false;
		pGameData = NULL;
		Stop();


		while (!bIsOnMission)
		{
			Sleep(5);
		}


		Start();
		pLast = pGameData;


	}
	return;
}

void MyPlayer::Stop()
{
	bSave = true;
	bHasSaved = false;
	savedPoint = { 0 };

	bGoodWeapons = false;
	bGodWeapons = false;
	bRapidFire = false;
	bTeleport = false;
	bSteroids = false;


	pWeaponData = NULL;
	pWeapon = NULL;
	pBody = NULL;



	if (bHookMinerals)
	{
		Hack::Patch((BYTE*)(moduleBase + HookMineralsOffset), (BYTE*)"\xF3\x0F\x11\x49\x60\xF3\x0F\x11\x4C\x24\x28\xF3\x0F\x11\x41\x68", 16);
		bHookMinerals = false;
	}
	if (bHookedObjective)
	{
		Hack::Patch((BYTE*)(moduleBase + HookObjectiveOffset), (BYTE*)"\xF3\x0F\x5D\x91\x88\x01\x00\x00\xF3\x0F\x11\x91\x8C\x01\x00\x00", 16);
		bHookObjective = false;
	}
}

void SetName(char* toSet, char* str)
{
	memset(toSet, 0, 1024);

	for (int i = 0; i < 1024; i++)
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

char* MyPlayer::GetEquiptName()
{
	char* Name = (char*)"Name Not Found";
	uint32_t FNameIndex = pGameData->pWeaponData->pCurrentWeapon->FNameIndex;
	if (FNameIndex < 1)
	{
		return Name;
	}

	uint32_t NumElements = (uint32_t)fNameTable + 0xC;
	uint32_t NumChunks = (uint32_t)fNameTable + 0x8;
	uint32_t ChunkIndex = FNameIndex >> 16;
	uint32_t NameIndex = (FNameIndex << 16) >> 16;

	if (NameIndex < NumElements && ChunkIndex < NumChunks)
	{
		uintptr_t ChunkPtr = *(uintptr_t*)(fNameTable + ((ChunkIndex + 0x2) * 0x8));

		if (ChunkPtr)
		{
			uintptr_t NamePtr = (ChunkPtr + (NameIndex * 2));
			uintptr_t NameEntryIndex = *(uintptr_t*)(NamePtr);
			Name = (char*)(NamePtr + 0x2);
			SetName(equiptName, Name);
			return equiptName;
		}
	}
	return Name;
}

void MyPlayer::UpdateValues()
{

	Validate();

	if (!pGameData || !pGameData->pPlayerData)
	{
		pLast = NULL;
		return;
	}


	pWeaponData = pGameData->pWeaponData;
	pWeapon = pWeaponData->pCurrentWeapon;
	pBody = pGameData->pPlayerData->pBody;

	Steroids();
	HookMinerals();
	HookObjective();


	if (bGoodWeapons)
	{
		GoodWeapons();
	}

	if (bGodWeapons)
	{
		GodWeapons();
	}

	if (bRapidFire)
	{
		RapidFire();
	}


	if (bTeleport)
	{
		Teleport();
	}
}

void MyPlayer::HookMinerals()
{
	if (bHookMinerals)
	{
		if (bHookedMinerals)
		{
			return;
		}

		Hack::Detour((BYTE*)moduleBase + HookMineralsOffset, mineral_hook, 16);
		bHookedMinerals = true;

	}
	else {

		//FSD-Win64-Shipping.exe+142B590 - F3 0F11 49 60    - movss [rcx55+60],xmm1
		//FSD-Win64-Shipping.exe+142B595 - F3 0F11 4C 24 28 - movss[rsp + 28], xmm1
		//FSD-Win64-Shipping.exe+142B59B - F3 0F11 41 68    - movss[rcx + 68], xmm0


		Hack::Patch((BYTE*)(moduleBase + HookMineralsOffset), (BYTE*)"\xF3\x0F\x11\x49\x60\xF3\x0F\x11\x4C\x24\x28\xF3\x0F\x11\x41\x68", 16);
		bHookedMinerals = false;
	}
}

void MyPlayer::HookObjective()
{
	if (bHookObjective)
	{
		Hack::Detour((BYTE*)moduleBase + HookObjectiveOffset, objective_hook, 16);
		bHookedObjective = true;
	}
	else
	{
		Hack::Patch((BYTE*)(moduleBase + HookObjectiveOffset), (BYTE*)"\xF3\x0F\x5D\x91\x88\x01\x00\x00\xF3\x0F\x11\x91\x8C\x01\x00\x00", 16);
		bHookedObjective = false;
	}
}

void MyPlayer::GoodWeapons()
{

	if (!pWeapon)
	{
		return;
	}

	switch (pWeapon->pWeaponType->weaponType)
	{
	case minigun:
	case pistol:
	case zipline:
	case piack_axe:
	case autocannon:
		pWeapon->shotCost = -1;
	}

	/*switch (pWeapon->pWeaponType->weaponType)
	{
	case minigun:
	case pistol:
	case zipline:
		pWeapon->ammo = 1336;
	}*/

	switch (pWeapon->pWeaponType->weaponType)
	{
	case minigun:
	case pistol:
	case shotgun:
	case platformgun:

		pWeapon->ammo = 1336;
		pWeapon->fireRate = 100;

		pWeapon->recoilYawMin = 0;
		pWeapon->recoilYawMax = 0;
		pWeapon->recoilPitchMin = 0;
		pWeapon->recoilPitchMax = 0;
		/*pWeapon->pStats->spreadMultX = 0;
		pWeapon->pStats->spreadX = 0;
		pWeapon->pStats->spreadMultY = 0;
		pWeapon->pStats->spreadY = 0;*/

		pWeapon->miniGunSpinUpTime = 0;

		[[fallthrough]];

	case grenadelauncher:
		pWeapon->ammo = 1336;
		pWeapon->fireRate = 100;
		break;

	case zipline:
		pWeapon->zipLineMaxAngle = 92;
		pWeapon->zipLineMaxDistance = 1000000;
		pWeapon->zipLineMinDistance = 0;
		break;

	case coilgun:
	/*	pWeapon->reserveAmmo = 1337;
		pWeapon->reloadTime = 0;
		pWeapon->coilGunCharge = 1;
		pWeapon->coilGunChargeRate = 10000;
		pWeapon->CoilGunSubtractionRate = 0;
		pWeapon->fireRate = 1000;
		pWeapon->recoilX = 0;
		pWeapon->recoilxMag = 0;
		pWeapon->recoilY = 0;
		pWeapon->recoilYMag = 0;
		pWeapon->pStats->spreadMultX = 0;
		pWeapon->pStats->spreadX = 0;
		pWeapon->pStats->spreadMultY = 0;
		pWeapon->pStats->spreadY = 0;*/
		break;

	case autocannon:
		pWeapon->zipLineMaxDistance = 1000;
		pWeapon->zipLineMinDistance = 1000;
		pWeapon->ammo = 1336;
		/*pWeapon->recoilX = 0;
		pWeapon->recoilxMag = 0;
		pWeapon->recoilY = 0;
		pWeapon->recoilYMag = 0;
		pWeapon->pStats->spreadMultX = 0;
		pWeapon->pStats->spreadX = 0;
		pWeapon->pStats->spreadMultY = 0;
		pWeapon->pStats->spreadY = 0;*/
	}


	pWeaponData->flairs = 4;
	pWeaponData->flairThrowCooldown = 0;

	pWeaponData->pGrenade->grenades = 1338;
}

void MyPlayer::GodWeapons()
{
	if (!pWeapon)
	{
		return;
	}

	switch (pWeapon->pWeaponType->weaponType)
	{
	case minigun:
	case pistol:
	case shotgun:
	case autocannon:
		/*pWeapon->pStats->pWeaponDamage->damage = 1000;
		pWeapon->pStats->pWeaponDamage->fearFactor = 100;
		pWeapon->pStats->pWeaponDamage->stunChance = 100;
		pWeapon->pStats->pWeaponDamage->stunDuraton = 100;*/
		pWeapon->fireRate = 1000;
		break;

	case grenadelauncher:
		pWeapon->fireRate = 1000;

	}

	if (pWeapon->pWeaponType->weaponType == autocannon)
	{
		/*pWeapon->pStats->pWeaponDamage->areaDamage = 1000;
		pWeapon->pStats->pWeaponDamage->areaRange = 1000000;*/
	}
}

void MyPlayer::RapidFire()
{
	if (!pWeapon)
	{
		return;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {

		switch (pWeapon->pWeaponType->weaponType)
		{
		case pistol:
		case shotgun:
			Shoot(pWeapon);
			break;

		}
	}
}

void MyPlayer::Steroids()
{
	if (!pBody || !pBody->pMovement)
	{
		return;
	}

	if (bSteroids)
	{

		// FSD-Win64-Shipping.exe + 141A562 - F3 0F 5D C8 - minss xmm1, xmm0
		// FSD-Win64-Shipping.exe + 141A566 - 44 0F 28 C9 - movaps xmm9, xmm1
		// FSD-Win64-Shipping.exe + 141A56A - F3 44 0F 11 8B B0 01 00 00 - movss[rbx + 000001B0], xmm9
		Hack::Detour((BYTE*)moduleBase + HookHealthOffset, health_hook, 17);

		// FSD-Win64-Shipping.exe+1231E55 - F3 0F11 91 C8020000   - movss [rcx+000002C8],xmm2
		Hack::Nop((BYTE*)(moduleBase + ShieldDamageOffset), 8);

		pBody->jumpNumber = 100;
		pBody->pMovement->jumpHeight = 1000;
		pBody->pMovement->pRunSpeed->runSpeed = 1000;

		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			pBody->pMovement->walkSpeed = 1000;
		}
		else {
			pBody->pMovement->walkSpeed = 300;
		}
	}
	else
	{
		Hack::Patch((BYTE*)(moduleBase + HookHealthOffset), (BYTE*)"\xF3\x0F\x5D\xC8\x44\x0F\x28\xC9\xF3\x44\x0F\x11\x8B\xB0\x01\x00\x00", 17);
		Hack::Patch((BYTE*)(moduleBase + ShieldDamageOffset), (BYTE*)"\xF3\x0F\x11\x91\xC8\x02\x00\x00", 8);
		pBody->jumpNumber = 1;
		pBody->pMovement->jumpHeight = 500;
		pBody->pMovement->pRunSpeed->runSpeed = 450;
	}

}


void MyPlayer::Teleport()
{
	if (!pBody || !pBody->pMovement)
	{
		return;
	}

	if (!bSave) {
		if (!bHasSaved)
		{
			return;
		}

		pBody->pMovement->pPosition->position.x = savedPoint.x;
		pBody->pMovement->pPosition->position.y = savedPoint.y;
		pBody->pMovement->pPosition->position.z = savedPoint.z;

		return;
	}


	savedPoint.x = pBody->pMovement->pPosition->position.x;
	savedPoint.y = pBody->pMovement->pPosition->position.y;
	savedPoint.z = pBody->pMovement->pPosition->position.z;

	bHasSaved = true;
}

