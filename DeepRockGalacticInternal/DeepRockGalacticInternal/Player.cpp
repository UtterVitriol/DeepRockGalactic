#include "Player.h"

void MyPlayer::Start()
{
	Display.SetWindow();
	Display.Print();

	moduleBase = (uintptr_t)GetModuleHandle(L"FSD-Win64-Shipping.exe");
	
	Shoot = (tShoot)(moduleBase + 0x1514AA0);

	// 014c67f0
	// 014d6ec0
	// 014b3e50
	// 0164bf50
	// 01723ce0
	// 01723be0
	// 014bc289
	// 01647e60
	// 0185f6c0
	// 014cae80
	ShootCharged = (tShootCharged)(moduleBase + 0x01647ea0);
	// 01647ea0

	pGameData = (GameData*)Hack::FindDMAAddy(moduleBase + firstOffset, { 0x0, 0x20, 0x0 });
	pLast = pGameData;
}



void MyPlayer::Validate()
{
	pGameData = (GameData*)Hack::FindDMAAddy(moduleBase + firstOffset, { 0x0, 0x20, 0x0 });

	if (pGameData != pLast)
	{
		pGameData = NULL;
		this->Stop();
			
		Display.Print(&Display.sGameStatus, "PRES HOME WHEN NEXT MISSION LOADED");

		while (!GetAsyncKeyState(VK_HOME) & 1)
		{
			if(GetAsyncKeyState(VK_END) & 1)
			{
				return;
			}

			Sleep(5);
		}


		pGameData = (GameData*)Hack::FindDMAAddy(moduleBase + firstOffset, { 0x0, 0x20, 0x0 });
		pLast = pGameData;

		Display.Print(&Display.sGameStatus, "ON MISSION");

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

	Display.Print(&Display.sGoodWeapons, "OFF");
	Display.Print(&Display.sGodWeapons, "OFF");
	Display.Print(&Display.sRapidFire, "OFF");
	Display.Print(&Display.sSteroids, "OFF");
	Display.Print(&Display.sTeleport, "NOT SAVED");

	if (bHookMinerals) {
		Hack::Patch((BYTE*)(moduleBase + HookMineralsOffset), (BYTE*)"\xF3\x0F\x11\x49\x60\xF3\x0F\x11\x4C\x24\x28\xF3\x0F\x11\x41\x68", 16);
		bHookMinerals = false;
	}
	Display.Print(&Display.sMineralHook, "NOT HOOKED");
}

void MyPlayer::SetBools()
{
	if (!pGameData)
	{
		return;
	}

	if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
		bGoodWeapons = !bGoodWeapons;
		if (bGoodWeapons)
		{
			Display.Print(&Display.sGoodWeapons, "ON");
		}
		else
		{
			Display.Print(&Display.sGoodWeapons, "OFF");

		}
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
		bGodWeapons = !bGodWeapons;
		if (bGodWeapons)
		{
			Display.Print(&Display.sGodWeapons, "ON");
		}
		else
		{
			Display.Print(&Display.sGodWeapons, "OFF");

		}
	}

	if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
		bRapidFire = !bRapidFire;
		if (bRapidFire)
		{
			Display.Print(&Display.sRapidFire, "ON");
		}
		else
		{
			Display.Print(&Display.sRapidFire, "OFF");

		}
	}

	if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
		bSteroids = !bSteroids;
		if (bSteroids)
		{
			Display.Print(&Display.sSteroids, "ON");
		}

		else
		{
			Display.Print(&Display.sSteroids, "OFF");

		}
	}

	if (GetAsyncKeyState(VK_NUMPAD5) & 1)
	{
		if (!bHookMinerals)
		{

			Display.Print(&Display. sMineralHook, "HOOKED");
			Hack::Detour((BYTE*)moduleBase + HookMineralsOffset, mineral_hook, 16);
			bHookMinerals = true;
		}
		else {

			//FSD-Win64-Shipping.exe+142B590 - F3 0F11 49 60    - movss [rcx55+60],xmm1
			//FSD-Win64-Shipping.exe+142B595 - F3 0F11 4C 24 28 - movss[rsp + 28], xmm1
			//FSD-Win64-Shipping.exe+142B59B - F3 0F11 41 68    - movss[rcx + 68], xmm0
			

			Display.Print(&Display. sMineralHook, "NOT HOOKED");
			Hack::Patch((BYTE*)(moduleBase + HookMineralsOffset), (BYTE*)"\xF3\x0F\x11\x49\x60\xF3\x0F\x11\x4C\x24\x28\xF3\x0F\x11\x41\x68", 16);
			bHookMinerals = false;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD6) & 1)
	{
		// FSD-Win64-Shipping.exe + 145227B - F3 0F 5D 91 88 01 00 00 - minss xmm2, [rcx + 00000188]
		// FSD-Win64-Shipping.exe + 1452283 - F3 0F 11 91 8C 01 00 00 - movss[rcx + 0000018C], xmm2

		if (!bHookObjective)
		{
			Display.Print(&Display.sObjectiveHook, "HOOKED");
			Hack::Detour((BYTE*)moduleBase + HookObjectiveOffset, objective_hook, 16);


			bHookObjective = true;
		}
		else
		{
			Display.Print(&Display.sObjectiveHook, "NOT HOOKED");
			Hack::Patch((BYTE*)(moduleBase + HookObjectiveOffset), (BYTE*)"\xF3\x0F\x5D\x91\x88\x01\x00\x00\xF3\x0F\x11\x91\x8C\x01\x00\x00", 16);
			bHookObjective = false;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD7) & 1) {
		bSave = true;
		Teleport();
		Display.Print(&Display.sTeleport, "SAVED");
	}

	if (GetAsyncKeyState(VK_NUMPAD8) & 1) {
		bSave = false;
		Teleport();

	}

}

void MyPlayer::UpdateValues()
{

	Validate();

	if (!pGameData || !pGameData->pPlayerData)
	{
		pLast = NULL;
		return;
	}

	SetBools();

	pWeaponData = pGameData->pWeaponData;
	pWeapon = pWeaponData->pCurrentWeapon;
	pBody = pGameData->pPlayerData->pBody;

	Steroids();


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
		pWeapon->ammoSubtraction = -1;
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

		pWeapon->recoilX = 0;
		pWeapon->recoilxMag = 0;
		pWeapon->recoilY = 0;
		pWeapon->recoilYMag = 0;
		pWeapon->pStats->spreadMultX = 0;
		pWeapon->pStats->spreadX = 0;
		pWeapon->pStats->spreadMultY = 0;
		pWeapon->pStats->spreadY = 0;

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
		pWeapon->reserveAmmo = 1337;
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
		pWeapon->pStats->spreadY = 0;
		break;

	case autocannon:
		pWeapon->zipLineMaxDistance = 1000;
		pWeapon->zipLineMinDistance = 1000;
		pWeapon->ammo = 1336;
		pWeapon->recoilX = 0;
		pWeapon->recoilxMag = 0;
		pWeapon->recoilY = 0;
		pWeapon->recoilYMag = 0;
		pWeapon->pStats->spreadMultX = 0;
		pWeapon->pStats->spreadX = 0;
		pWeapon->pStats->spreadMultY = 0;
		pWeapon->pStats->spreadY = 0;
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
		pWeapon->pStats->pWeaponDamage->damage = 1000;
		pWeapon->pStats->pWeaponDamage->fearFactor = 100;
		pWeapon->pStats->pWeaponDamage->stunChance = 100;
		pWeapon->pStats->pWeaponDamage->stunDuraton = 100;
		pWeapon->fireRate = 1000;
		break;

	case grenadelauncher:
		pWeapon->fireRate = 1000;

	}

	if (pWeapon->pWeaponType->weaponType == autocannon)
	{
		pWeapon->pStats->pWeaponDamage->areaDamage = 1000;
		pWeapon->pStats->pWeaponDamage->areaRange = 1000000;
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

