#include "Menus.h"
#include "Game.h"
#include "imgui.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DERR(s) g_player.log.AddLog("[-]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)
#define DMSG(s) g_player.log.AddLog("[+]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)

extern MyGame g_Game;

void MyMenu()
{
	const float MAX_SCALE = 3.0F;
	const float MIN_SCALE = 0.3F;

	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = 0;

	static float window_scale = 1.5f;

	static bool bShowAppLog = false;
	static bool bShowResources = false;
	static bool bShowPlayer = false;
	static bool bShowPrimaryWeapon = false;
	static bool bShowSecondaryWeapon = false;
	static bool bShowTraversalTool = false;

	if (!ImGui::Begin("Tingle's Internal Trainer", (bool*)0, ImGuiWindowFlags_MenuBar))
	{
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("Log", NULL, &bShowAppLog);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("Press END to Eject :)");
	ImGui::SeparatorText("Disclaimer");
	ImGui::TextWrapped("Making more than 1,000 voxel changes might crash the game. E.G. You set your autocannon fire rate to 100 and shoot for a while.");
	ImGui::Separator();

	if (!g_Game.bIsOnMission)
	{
		ImGui::Checkbox("On Mission", &g_Game.bIsOnMission);
	}
	else
	{
		if (ImGui::CollapsingHeader("Hacks"))
		{
			if (ImGui::BeginTable("split", 3))
			{
				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Infinite Health", &g_Game.bHookHealth))
				{
					g_Game.HookHealth();
				}
				
				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Infinite Armor", &g_Game.bHookArmor))
				{
					g_Game.HookArmor();
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("No Recoil", &g_Game.bNoRecoil))
				{
					g_Game.NoRecoil();
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("No Bullet Spread", &g_Game.bNoBulletSpread))
				{
					g_Game.NoBulletSpread();
				}

				ImGui::TableNextColumn(); 
				if (ImGui::Checkbox("Hook Minerals", &g_Game.bHookMinerals))
				{
					g_Game.HookMinerals();
				}

				ImGui::TableNextColumn(); 
				if (ImGui::Checkbox("Hook Mineral Objective", &g_Game.bHookObjective))
				{
					g_Game.HookObjective();
				}
				ImGui::EndTable();
			}
		}
	}

	if (ImGui::CollapsingHeader("Set Values"))
	{
		if (ImGui::BeginTable("split", 4))
		{
			ImGui::TableNextColumn();  ImGui::Checkbox("Resources", &bShowResources);
			ImGui::TableNextColumn();  ImGui::Checkbox("Player", &bShowPlayer);

			if (g_Game.bIsOnMission)
			{
				ImGui::TableNextColumn();  ImGui::Checkbox("Primary Weapon", &bShowPrimaryWeapon);
				ImGui::TableNextColumn();  ImGui::Checkbox("Secondary Weapon", &bShowSecondaryWeapon);
				ImGui::TableNextColumn();  ImGui::Checkbox("Traversal Tool", &bShowTraversalTool);
			}

			ImGui::EndTable();

		}

	}
	
	if (ImGui::CollapsingHeader("Settings")) {
		ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();

	if (bShowAppLog)
	{
		LogMenu();
	}
	if (bShowResources)
	{
		ResourceMenu(&bShowResources);
	}

	if (bShowPlayer)
	{
		PlayerMenu();
	}

	if (bShowPrimaryWeapon && g_Game.bIsOnMission)
	{
		WeaponMenu(g_Game.m_PrimaryName, &g_Game.pCharacter->pEquipables->pPrimaryGun);
	}

	if (bShowSecondaryWeapon && g_Game.bIsOnMission)
	{
		WeaponMenu(g_Game.m_SecondaryName, & g_Game.pCharacter->pEquipables->pSecondaryGun);
	}

	if (bShowTraversalTool && g_Game.bIsOnMission)
	{
		TraversalMenu(g_Game.m_TraversalName, &g_Game.pCharacter->pEquipables->pTraversalTool);
	}
}

/// <summary>
/// Shows the debug log
/// </summary>
static void LogMenu()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	g_Game.log.Draw("Debug Log");
}

/// <summary>
/// Shows Player Resources
/// </summary>
/// <param name="bShowMenu"></param>
void ResourceMenu(bool* bShowMenu)
{
	if (!*bShowMenu)
	{
		return;
	}

	ImGui::Begin("Player Resources", bShowMenu);

	ImGui::SeparatorText("Misc");

	ImGui::InputInt("Credits", &g_Game.pSavedGame->Credits);

	ImGui::SeparatorText("Cosmetic");

	ImGui::InputFloat("Phazyonite", &g_Game.pSavedGame->Resources->Phazyonite, 1, 100, "%.0f");

	ImGui::SeparatorText("Crafting");

	ImGui::InputFloat("Magnite", &g_Game.pSavedGame->Resources->Magnite, 1, 100, "%.0f");
	ImGui::InputFloat("Bismor", &g_Game.pSavedGame->Resources->Bismor, 1, 100, "%.0f");
	ImGui::InputFloat("Umanite", &g_Game.pSavedGame->Resources->Umanite, 1, 100, "%.0f");
	ImGui::InputFloat("Croppa", &g_Game.pSavedGame->Resources->Croppa, 1, 100, "%.0f");
	ImGui::InputFloat("Enor Pearl", &g_Game.pSavedGame->Resources->EnorPearl, 1, 100, "%.0f");
	ImGui::InputFloat("Jadiz", &g_Game.pSavedGame->Resources->Jadiz, 1, 100, "%.0f");

	ImGui::SeparatorText("Beer");

	ImGui::InputFloat("Barley Bulb", &g_Game.pSavedGame->Resources->BarleyBulb, 1, 100, "%.0f");
	ImGui::InputFloat("Yeast Cone", &g_Game.pSavedGame->Resources->YeastCone, 1, 100, "%.0f");
	ImGui::InputFloat("Malt Star", &g_Game.pSavedGame->Resources->MaltStar, 1, 100, "%.0f");
	ImGui::InputFloat("Starch Nut", &g_Game.pSavedGame->Resources->StarchNut, 1, 100, "%.0f");

	ImGui::End();
}

/// <summary>
/// Shows Player Stats
/// </summary>
void PlayerMenu()
{
	const char* DanceMoves[] = { "Dance Move 1", "Dance Move 2", "Dance Move 3", "Dance Move 4", "Dance Move 5",
		"Dance Move 6", "Dance Move 7", "Dance Move 8", "Dance Move 9", "Dance Move 10", "Dance Move 11",
		"Dance Move 12", "Dance Move 13", "Dance Move 14", "Dance Move 15", "Dance Move 16", "Dance Move 17",
		"Dance Move 18" };

	ImGui::Begin("Player Data");

	ImGui::SeparatorText("General");

	ImGui::InputFloat("Health Damage Taken", &g_Game.pCharacter->pHealthComponent->characterDamage, 1, 100);
	ImGui::InputFloat("Shield Damage Taken", &g_Game.pCharacter->pHealthComponent->shieldDamage, 1, 100);
	ImGui::InputInt("Jump Number", &g_Game.pCharacter->jumpMaxCount);
	ImGui::InputFloat("Jump Heigt", &g_Game.pCharacter->pCharacterMovement->jumpHeight, 1, 100, "%.0f");
	ImGui::InputFloat("Walk Speed", &g_Game.pCharacter->pCharacterMovement->walkSpeed, 1, 100, "%.0f");
	ImGui::InputFloat("Run Speed", &g_Game.pCharacter->runningSpeed, 1, 100, "%.0f");
	ImGui::InputFloat("Run Boost", &g_Game.pCharacter->runBoost, 1, 100, "%.0f");
	ImGui::InputFloat("Carrying Movement Penalty", &g_Game.pCharacter->carryingMovementSpeedPenalty, 1, 100, "%.0f");
	ImGui::InputFloat("Carrying Throw Min Force", &g_Game.pCharacter->carryingThrowMinForce, 1, 100, "%.0f");
	ImGui::InputFloat("Carrying Throw Max Force", &g_Game.pCharacter->carryingThrowMaxForce, 1, 100, "%.0f");
	ImGui::InputFloat("Gravity Scale", &g_Game.pCharacter->pCharacterMovement->gravityScale, .01, .1, "%.3f");

	ImGui::SeparatorText("Flairs");
	ImGui::InputInt("Flares", &g_Game.pFlairs->flares);
	ImGui::InputFloat("Flare Production Time", &g_Game.pFlairs->flareProductionTime, 1, 100, "%.3f");
	ImGui::InputFloat("Flare Cooldown", &g_Game.pFlairs->flareCooldown, 1, 100, "%.3f");
	
	ImGui::SeparatorText("Dancing");

	ImGui::Checkbox("Dance", (bool*)&g_Game.pCharacter->isDanceRange);

	if (g_Game.pCharacter->isDanceRange) {
		ImGui::Combo("Dance Moves", &g_Game.pCharacter->danceMove, DanceMoves, IM_ARRAYSIZE(DanceMoves));
	}

	ImGui::SeparatorText("Position");

	if (ImGui::InputFloat3("Position", (float*)&g_Game.pCharacter->pCharacterMovement->pPosition->position, "%.0f"))
	{
		g_Game.log.AddLog("Changed");
	}

	ImGui::Separator();
	
	if (ImGui::Button("Save"))
	{
		g_Game.SaveLocation();
	}

	ImGui::SameLine();
	if (ImGui::Button("Teleport"))
	{
		g_Game.TeleportLocation();
	}

	if (ImGui::InputFloat3("Saved Position", (float*)&g_Game.m_TeleportLocation, "%.0f", ImGuiInputTextFlags_ReadOnly))
	{
		g_Game.log.AddLog("Changed");
	}

	ImGui::End();
}

/// <summary>
/// Shows Weapon Stats Crashes if you pick anything up while open
/// </summary>
void WeaponMenu(char* MenuName, Weapon** pWeapon)
{
	ImGui::Begin(MenuName);

	ImGui::InputInt("Shot Cost", &(*pWeapon)->shotCost);
	ImGui::InputInt("Max Ammo", &(*pWeapon)->maxAmmo);
	ImGui::InputInt("Mag Size", &(*pWeapon)->clipSize);
	ImGui::InputInt("Reserve Ammo", &(*pWeapon)->reserveAmmo);
	ImGui::InputInt("Ammo", &(*pWeapon)->ammo);

	ImGui::Separator();

	ImGui::InputFloat("Fire Rate", &(*pWeapon)->fireRate, 1, 100, "%.0f");
	ImGui::InputFloat("Starting Fire Rate", &(*pWeapon)->startingFireRate, 1, 100, "%.0f");
	ImGui::InputFloat("Max Fire Rate", &(*pWeapon)->maxFireRate, 1, 100, "%.0f");
	ImGui::Checkbox("Automattic Fire", (bool*)&(*pWeapon)->hasAutomaticFire);

	ImGui::Separator();

	ImGui::InputFloat("Reload Duration", &(*pWeapon)->reloadDuration, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Vertical Recoil Min", &(*pWeapon)->recoilPitchMin, 1, 100, "%.0f");
	ImGui::InputFloat("Vertical Recoil Max", &(*pWeapon)->recoilPitchMax, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Recoil Min", &(*pWeapon)->recoilYawMin, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Recoil Max", &(*pWeapon)->recoilYawMax, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Min Bullet Spread", &(*pWeapon)->pWeaponFire->minSpead, 1, 100, "%.0f");
	ImGui::InputFloat("Max Bullet Spread", &(*pWeapon)->pWeaponFire->maxSpread, 1, 100, "%.0f");
	ImGui::InputFloat("Min Bullet Spread While Moving", &(*pWeapon)->pWeaponFire->minSpreadWhileMoving, 1, 100, "%.0f");
	ImGui::InputFloat("Min Bullet Spread While Sprinting", &(*pWeapon)->pWeaponFire->minSpreadWhileSprinting, 1, 100, "%.0f");
	ImGui::InputFloat("Vertical Spread Multiplier", &(*pWeapon)->pWeaponFire->verticalSpreadMultiplier, 1, 100, "%.0f");
	ImGui::InputFloat("Max Vertical Spread", &(*pWeapon)->pWeaponFire->maxVerticalSpread, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Spread Multiplier", &(*pWeapon)->pWeaponFire->horizontalSpreadMultiplier, 1, 100, "%.0f");
	ImGui::InputFloat("Max Horizontal Spread", &(*pWeapon)->pWeaponFire->maxHorizontalSpread, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputInt("Max Penetrations", &(*pWeapon)->pWeaponFire->maxPenetrations);
	ImGui::InputFloat("Ricochet Chance", &(*pWeapon)->pWeaponFire->ricochetChance, 1, 100, "%.0f");
	ImGui::InputFloat("Ricochet Max Range", &(*pWeapon)->pWeaponFire->ricochetMaxRange, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::Checkbox("Bullets Can Carve", (bool*)&(*pWeapon)->pWeaponFire->bulletsCanCarve);
	ImGui::InputFloat("Carve Diameter", &(*pWeapon)->pWeaponFire->carveDiameter, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Armor Damage Multiplier", &(*pWeapon)->pWeaponFire->pDamageComponent->armorDamageMultiplier, 1, 100, "%.0f");
	ImGui::InputFloat("Armor Penetration", &(*pWeapon)->pWeaponFire->pDamageComponent->armorPenetration, 1, 100, "%.0f");
	ImGui::InputFloat("Shatters Armor", &(*pWeapon)->pWeaponFire->pDamageComponent->shattersArmor, 1, 100, "%.0f");
	ImGui::InputFloat("Armor Damage Type", &(*pWeapon)->pWeaponFire->pDamageComponent->armorDamageType, 1, 100, "%.0f");
	ImGui::InputFloat("Weak Point Multiplier", &(*pWeapon)->pWeaponFire->pDamageComponent->weakpointMultiplier, 1, 100, "%.0f");
	ImGui::InputFloat("Stagger Chance", &(*pWeapon)->pWeaponFire->pDamageComponent->staggerChance, 1, 100, "%.0f");
	ImGui::InputFloat("Stagger Duration", &(*pWeapon)->pWeaponFire->pDamageComponent->staggerDuration, 1, 100, "%.0f");
	ImGui::InputFloat("Fear Factor", &(*pWeapon)->pWeaponFire->pDamageComponent->fearFactor, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Damage", &(*pWeapon)->pWeaponFire->pDamageComponent->damage, 1, 100, "%.0f");
	ImGui::InputFloat("Minimum Damage Percentage", &(*pWeapon)->pWeaponFire->pDamageComponent->minimumDamagePercentage, 1, 100, "%.0f");
	ImGui::InputFloat("Radial Damage", &(*pWeapon)->pWeaponFire->pDamageComponent->radialDamage, 1, 100, "%.0f");
	ImGui::InputFloat("Damage Radius", &(*pWeapon)->pWeaponFire->pDamageComponent->damageRadius, 1, 100, "%.0f");
	ImGui::InputFloat("Max Damage Radius", &(*pWeapon)->pWeaponFire->pDamageComponent->maxDamageRadius, 1, 100, "%.0f");

	ImGui::End();
}

/// <summary>
/// Shows Weapon Stats Crashes if you pick anything up while open
/// </summary>
void TraversalMenu(char* MenuName, TraversalTool** pWeapon)
{
	ImGui::Begin(MenuName);

	ImGui::InputInt("Shot Cost", &(*pWeapon)->shotCost);
	ImGui::InputInt("Max Ammo", &(*pWeapon)->maxAmmo);
	ImGui::InputInt("Mag Size", &(*pWeapon)->clipSize);
	ImGui::InputInt("Reserve Ammo", &(*pWeapon)->reserveAmmo);
	ImGui::InputInt("Ammo", &(*pWeapon)->ammo);

	ImGui::Separator();

	ImGui::InputFloat("Fire Rate", &(*pWeapon)->fireRate, 1, 100, "%.0f");
	ImGui::Checkbox("Automattic Fire", (bool*)&(*pWeapon)->hasAutomaticFire);

	ImGui::Separator();

	ImGui::InputFloat("Reload Duration", &(*pWeapon)->reloadDuration, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Vertical Recoil Min", &(*pWeapon)->recoilPitchMin, 1, 100, "%.0f");
	ImGui::InputFloat("Vertical Recoil Max", &(*pWeapon)->recoilPitchMax, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Recoil Min", &(*pWeapon)->recoilYawMin, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Recoil Max", &(*pWeapon)->recoilYawMax, 1, 100, "%.0f");

	ImGui::SeparatorText("Zipline Gun");

	ImGui::InputFloat("Zipline Min Distance", &(*pWeapon)->zipLineMinDistance, 1, 100, "%.0f");
	ImGui::InputFloat("Zipline Max Distance", &(*pWeapon)->zipLineMaxDistance, 1, 100, "%.0f");
	ImGui::InputFloat("Zipline Min Angle", &(*pWeapon)->zipLineBaseAngle, 1, 100, "%.0f");
	ImGui::InputFloat("Zipline Max Angle", &(*pWeapon)->zipLineMaxAngle, 1, 100, "%.0f");

	ImGui::End();
}
