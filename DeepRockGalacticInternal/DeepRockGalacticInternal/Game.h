#pragma once

#include "GameData.h"
#include "Hack.h"

#include <vector>

constexpr auto MAX_FNAME_LEN = 1024;;

class MyGame
{
public:
	struct AppLog log;



	uintptr_t m_ModuleBase = 0;

	// FnameTable sutff.
	uintptr_t m_FNameTable = NULL;
	uint64_t m_FNameTableOffset = 0x606BC40;

	// Resources stuff.
	uint64_t m_ResourcesFirstOffset = 0x061E9F80;
	std::vector<unsigned int> m_ResourcesOffsets = { 0x8, 0x5F8, 0x0 };

	// Character stuff.
	Character* pCharacter = NULL;
	Character* pLast = NULL;
	uint64_t m_CharacterFirstOffset = 0x61E2200;
	std::vector<unsigned int> m_CharacterOffsets = { 0x0, 0x20, 0x0 };

	// Hook stuff.
	uintptr_t m_HookMineralsOffset = 0x142B590;
	uintptr_t m_HookObjectiveOffset = 0x145227B;
	uintptr_t m_ShieldDamageOffset = 0x1231E55;
	uintptr_t m_HookHealthOffset = 0x141A562;

	// Pointers.
	FSDSavedGame* pSavedGame = NULL;
	Weapon* pPrimary = NULL;
	char m_PrimaryName[MAX_FNAME_LEN];

	Weapon* pSecondary = NULL;
	char m_SecondaryName[MAX_FNAME_LEN];

	TraversalTool* pTraversal = NULL;
	char m_TraversalName[MAX_FNAME_LEN];

	Weapon* pSupport = NULL;
	InventoryComponent* pFlairs = NULL;

	// State stuff.
	bool bIsOnMission = false;

	bool bGoodWeapons = false;
	bool bGodWeapons = false;
	bool bRapidFire = false;
	bool bTeleport = false;
	bool bSteroids = false;
	bool bHookMinerals = false;
	bool bHookObjective = false;
	bool bHookHealth = false;
	bool bHookArmor = false;


	bool bHasSavedTeleportLocation = false;
	Vec3 m_TelePortLocation = { 0 };


	// Setup pointers.
	void Start();

	// Check if character pointer changed.
	void ValidateCharacter();

	// Main working loop
	void UpdateValues();

	// Reset state stuff;
	void ExitMission();

	/// <summary>
	/// Get FName of object and optionally store in destination
	/// </summary>
	/// <param name="index"></param>
	/// <param name="destination"></param>
	/// <returns></returns>
	char* GetFName(uint32_t index, char* destination = NULL);

	// Teleport
	void SaveLocation();
	void TeleportLocation();

	// Hook stuff.
	void HookMinerals();
	void HookObjective();
	void HookHealth();
	void HookArmor();

};

