#pragma once

#include "GameData.h"
#include "Hack.h"
#include "Display.h"

#include <vector>

extern "C" void mineral_hook();
extern "C" void health_hook();
extern "C" void objective_hook();

class MyPlayer
{
public:
	void Start();
	void Validate();
	void Stop();
	void SetBools();
	void UpdateValues();

	uintptr_t moduleBase = 0;
	uint32_t firstOffset = 0x61E2200;
	std::vector<unsigned int> offsets = { 0x0, 0x20, 0x0 };

	GameData* pGameData = NULL;

	GameData* pLast = NULL;
	std::vector<unsigned int> validateOffsets = { 0x0 };

private:
	void GoodWeapons();
	void GodWeapons();
	void RapidFire();

	void Steroids();

	void Teleport();

	TrainerDisplay Display;

	WeaponData* pWeaponData = NULL;
	Weapon* pWeapon = NULL;
	Body* pBody = NULL;

	typedef void(*tShootCharged)(Weapon*, float);
	tShootCharged ShootCharged = nullptr;

	typedef void (*tShoot)(Weapon*);
	tShoot Shoot = nullptr;

	bool bHookMinerals = false;
	uintptr_t HookMineralsOffset = 0x142B590;

	bool bHookObjective = false;
	uintptr_t HookObjectiveOffset = 0x145227B;
		

	uintptr_t ShieldDamageOffset = 0x1231E55;

	uintptr_t HookHealthOffset = 0x141A562;

	bool bSave = true;
	bool bHasSaved = false;
	Vec3 savedPoint = { 0 };

	bool bGoodWeapons = false;
	bool bGodWeapons = false;
	bool bRapidFire = false;
	bool bSteroids = false;
	bool bTeleport = false;


};

