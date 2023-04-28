#pragma once

#include "GameData.h"
#include "Hack.h"
#include "Display.h"
#include "Menus.h"

#include <vector>

extern "C" void mineral_hook();
extern "C" void health_hook();
extern "C" void objective_hook();

class MyPlayer
{
public:
	struct AppLog log;

	void Start();
	void Validate();
	void Stop();
	char* GetEquiptName();
	void UpdateValues();

	uintptr_t moduleBase = 0;

	uint64_t fNameTableOffset = 0x606BC40;
	uintptr_t fNameTable = NULL;
	char equiptName[1024];

	uint32_t firstOffset_Resources = 0x061E9F80;
	std::vector<unsigned int> offsets_Resources = { 0x8, 0x5F8, 0x0 };

	uint32_t firstOffset_GameData = 0x61E2200;
	std::vector<unsigned int> offsets_GameData = { 0x0, 0x20, 0x0 };

	// resources 
	Player* pPlayer = NULL;

	GameData* pGameData = NULL;
	GameData* pLast = NULL;

	WeaponData* pWeaponData = NULL;
	Weapon* pWeapon = NULL;
	Body* pBody = NULL;


	void GoodWeapons();
	void GodWeapons();
	void RapidFire();
	void Steroids();
	void Teleport();

	TrainerDisplay Display;



	typedef void(*tShootCharged)(Weapon*, float);
	tShootCharged ShootCharged = nullptr;

	typedef void (*tShoot)(Weapon*);
	tShoot Shoot = nullptr;

	bool bHookMinerals = false;
	bool bHookedMinerals = false;
	uintptr_t HookMineralsOffset = 0x142B590;
	void HookMinerals();

	bool bHookObjective = false;
	bool bHookedObjective = false;
	uintptr_t HookObjectiveOffset = 0x145227B;
	void HookObjective();
		

	uintptr_t ShieldDamageOffset = 0x1231E55;

	uintptr_t HookHealthOffset = 0x141A562;

	bool bIsOnMission = false;

	bool bSave = true;
	bool bHasSaved = false;
	Vec3 savedPoint = { 0 };

	bool bGoodWeapons = false;
	bool bGodWeapons = false;
	bool bRapidFire = false;
	bool bSteroids = false;
	bool bTeleport = false;


};

