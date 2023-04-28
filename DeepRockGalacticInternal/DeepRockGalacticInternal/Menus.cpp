#include "Menus.h"
#include "Player.h"
#include "imgui.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DERR(s) g_player.log.AddLog("[-]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)
#define DMSG(s) g_player.log.AddLog("[+]: %s:%d:%s(): %s\n", __FILENAME__, __LINE__, __func__, s)

extern MyPlayer g_player;


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
	static bool bShowWeapon = false;

	static bool bShowMenu = false;

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

	ImGui::Text("Press END to Eject :)");             // Display some text (you can use a format strings too)

	if (ImGui::CollapsingHeader("Stats"))
	{
		if (ImGui::BeginTable("split", 4))
		{
			ImGui::TableNextColumn();  ImGui::Checkbox("Resources", &bShowResources);
			ImGui::TableNextColumn();  ImGui::Checkbox("Player", &bShowPlayer);

			if (g_player.bIsOnMission)
			{
				ImGui::TableNextColumn();  ImGui::Checkbox("Weapon", &bShowWeapon);
			}

			ImGui::EndTable();

		}

	}


	if (!g_player.bIsOnMission)
	{
		ImGui::Checkbox("On Mission", &g_player.bIsOnMission);
	}
	else
	{
		if (ImGui::CollapsingHeader("Hacks"))
		{
			if (ImGui::BeginTable("split", 3))
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("Good Weapons", &g_player.bGoodWeapons);
				ImGui::TableNextColumn(); ImGui::Checkbox("God Weapons", &g_player.bGodWeapons);
				ImGui::TableNextColumn(); ImGui::Checkbox("Rapid Fire", &g_player.bRapidFire);
				ImGui::TableNextColumn(); ImGui::Checkbox("Steroids", &g_player.bSteroids);
				ImGui::TableNextColumn(); ImGui::Checkbox("Hook Minerals", &g_player.bHookMinerals);
				ImGui::TableNextColumn(); ImGui::Checkbox("Hook Mineral Objective", &g_player.bHookObjective);
				ImGui::EndTable();
			}
		}
	}

	if (ImGui::CollapsingHeader("Settings")) {
		//if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp)) // Scale only this window
		//	ImGui::SetWindowFontScale(window_scale);
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

	if (bShowWeapon && g_player.bIsOnMission)
	{
		WeaponMenu();
	}


	// Update values once per frame
	//g_player.UpdateValues();
}

/// <summary>
/// Shows the debug log
/// </summary>
static void LogMenu()
{
	// For the demo: add a debug button _BEFORE_ the normal log window contents
	// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
	// Most of the contents of the window will be added by the log.Draw() call.
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	/*ImGui::Begin("Example: Log");
	ImGui::End();*/
	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
	g_player.log.Draw("Debug Log");
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

	ImGui::InputInt("Credits", &g_player.pPlayer->Credits);

	ImGui::SeparatorText("Cosmetic");

	ImGui::InputFloat("Phazyonite", &g_player.pPlayer->Resources->Phazyonite, 1, 100, "%.0f");

	ImGui::SeparatorText("Crafting");

	ImGui::InputFloat("Magnite", &g_player.pPlayer->Resources->Magnite, 1, 100, "%.0f");
	ImGui::InputFloat("Bismor", &g_player.pPlayer->Resources->Bismor, 1, 100, "%.0f");
	ImGui::InputFloat("Umanite", &g_player.pPlayer->Resources->Umanite, 1, 100, "%.0f");
	ImGui::InputFloat("Croppa", &g_player.pPlayer->Resources->Croppa, 1, 100, "%.0f");
	ImGui::InputFloat("Enor Pearl", &g_player.pPlayer->Resources->EnorPearl, 1, 100, "%.0f");
	ImGui::InputFloat("Jadiz", &g_player.pPlayer->Resources->Jadiz, 1, 100, "%.0f");

	ImGui::SeparatorText("Beer");

	ImGui::InputFloat("Barley Bulb", &g_player.pPlayer->Resources->BarleyBulb, 1, 100, "%.0f");
	ImGui::InputFloat("Yeast Cone", &g_player.pPlayer->Resources->YeastCone, 1, 100, "%.0f");
	ImGui::InputFloat("Malt Star", &g_player.pPlayer->Resources->MaltStar, 1, 100, "%.0f");
	ImGui::InputFloat("Starch Nut", &g_player.pPlayer->Resources->StarchNut, 1, 100, "%.0f");

	ImGui::End();

}

/// <summary>
/// Shows Player Stats
/// </summary>
void PlayerMenu()
{

	ImGui::Begin("Player Data");

	ImGui::InputFloat("Health Damage Taken", &g_player.pGameData->pPlayerData->Health, 1, 100);
	ImGui::InputFloat("Shield Damage Taken", &g_player.pGameData->pPlayerData->ShieldInversePercentage, 1, 100);
	ImGui::InputInt("Jump Number", &g_player.pGameData->pPlayerData->pBody->jumpNumber);
	ImGui::InputFloat("Jump Heigt", &g_player.pGameData->pPlayerData->pBody->pMovement->jumpHeight, 1, 100, "%.0f");
	ImGui::InputFloat("Walk Speed", &g_player.pGameData->pPlayerData->pBody->pMovement->walkSpeed, 1, 100, "%.0f");
	ImGui::InputFloat("Run Speed", &g_player.pGameData->pPlayerData->pBody->pMovement->pRunSpeed->runSpeed, 1, 100, "%.0f");

	ImGui::Separator();

	if (ImGui::InputFloat3("Position", (float*)&g_player.pGameData->pPlayerData->pBody->pMovement->pPosition->position, "%.0f"))
	{
		g_player.log.AddLog("Changed");
	}
	ImGui::InputFloat3("Velocity", (float*)&g_player.pGameData->pPlayerData->pBody->pMovement->velocity, "%.0f");

	ImGui::End();
}

/// <summary>
/// Shows Weapon Stats Crashes if you pick anything up while open
/// </summary>
void WeaponMenu()
{
	ImGui::Begin(g_player.GetEquiptName());

	ImGui::InputInt("Shot Cost", &g_player.pGameData->pWeaponData->pCurrentWeapon->shotCost);
	ImGui::InputInt("Max Ammo", &g_player.pGameData->pWeaponData->pCurrentWeapon->maxAmmo);
	ImGui::InputInt("Mag Size", &g_player.pGameData->pWeaponData->pCurrentWeapon->clipSize);
	ImGui::InputInt("Reserve Ammo", &g_player.pGameData->pWeaponData->pCurrentWeapon->reserveAmmo);
	ImGui::InputInt("Ammo", &g_player.pGameData->pWeaponData->pCurrentWeapon->ammo);

	ImGui::Separator();

	ImGui::InputFloat("Fire Rate", &g_player.pGameData->pWeaponData->pCurrentWeapon->fireRate, 1, 100, "%.0f");
	ImGui::Checkbox("Automattic Fire", (bool*)&g_player.pGameData->pWeaponData->pCurrentWeapon->hasAutomaticFire);

	ImGui::Separator();

	ImGui::InputFloat("Reload Duration", &g_player.pGameData->pWeaponData->pCurrentWeapon->reloadDuration, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Vertical Recoil Min", &g_player.pGameData->pWeaponData->pCurrentWeapon->recoilPitchMin, 1, 100, "%.0f");
	ImGui::InputFloat("Vertical Recoil Max", &g_player.pGameData->pWeaponData->pCurrentWeapon->recoilPitchMax, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Recoil Min", &g_player.pGameData->pWeaponData->pCurrentWeapon->recoilYawMin, 1, 100, "%.0f");
	ImGui::InputFloat("Horizontal Recoil Max", &g_player.pGameData->pWeaponData->pCurrentWeapon->recoilYawMax, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputInt("Max Penetrations", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->maxPenetrations);
	ImGui::InputFloat("Ricochet Chance", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->ricochetChance, 1, 100, "%.0f");
	ImGui::InputFloat("Ricochet Max Range", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->ricochetMaxRange, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::Checkbox("Bullets Can Carve", (bool*)&g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->bulletsCanCarve);
	ImGui::InputFloat("Carve Diameter", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->carveDiameter, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Armor Damage Multiplier", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->armorDamageMultiplier, 1, 100, "%.0f");
	ImGui::InputFloat("Armor Penetration", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->armorPenetration, 1, 100, "%.0f");
	ImGui::InputFloat("Shatters Armor", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->shattersArmor, 1, 100, "%.0f");
	ImGui::InputFloat("Armor Damage Type", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->armorDamageType, 1, 100, "%.0f");
	ImGui::InputFloat("Weak Point Multiplier", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->weakpointMultiplier, 1, 100, "%.0f");
	ImGui::InputFloat("Stagger Chance", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->staggerChance, 1, 100, "%.0f");
	ImGui::InputFloat("Stagger Duration", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->staggerDuration, 1, 100, "%.0f");
	ImGui::InputFloat("Fear Factor", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->fearFactor, 1, 100, "%.0f");

	ImGui::Separator();

	ImGui::InputFloat("Damage", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->damage, 1, 100, "%.0f");
	ImGui::InputFloat("Minimum Damage Percentage", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->minimumDamagePercentage, 1, 100, "%.0f");
	ImGui::InputFloat("Radial Damage", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->radialDamage, 1, 100, "%.0f");
	ImGui::InputFloat("Damage Radius", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->damageRadius, 1, 100, "%.0f");
	ImGui::InputFloat("Max Damage Radius", &g_player.pGameData->pWeaponData->pCurrentWeapon->pWeaponFire->pDamageComponent->maxDamageRadius, 1, 100, "%.0f");


	ImGui::End();
}
