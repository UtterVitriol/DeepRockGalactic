#include "Menus.h"
#include "Player.h"
#include "imgui.h"

extern MyPlayer g_player;
const float MAX_SCALE = 3.0F;
const float MIN_SCALE = 0.3F;

void MyMenu()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = 0;

	static float window_scale = 1.0f;

	static bool bShowResources = false;
	static bool bShowAppLog = false;
	static bool bShowPlayer = false;

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

	ImGui::Checkbox("Resources", &bShowResources);
	ImGui::Checkbox("Player", &bShowPlayer);

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

	if (bShowResources)
	{
		ResourceMenu(&bShowResources);
	}

	if (bShowAppLog)
	{
		LogMenu();
	}

	if (bShowPlayer)
	{
		PlayerMenu();
	}

	// Update values once per frame
	//g_player.UpdateValues();
}

void MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("Log", NULL, &LogMenu);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}


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

void PlayerMenu()
{

	ImGui::Begin("Player Data");
	ImGui::InputFloat("Health", &g_player.pGameData->pPlayerData->Health, 1, 100);
	ImGui::InputFloat("Shield", &g_player.pGameData->pPlayerData->ShieldInversePercentage, 1, 100);
	ImGui::InputInt("Jump Number", &g_player.pGameData->pPlayerData->pBody->jumpNumber);
	ImGui::InputFloat("Jump Heigt", &g_player.pGameData->pPlayerData->pBody->pMovement->jumpHeight, 1, 100, "%.0f");
	ImGui::InputFloat("Walk Speed", &g_player.pGameData->pPlayerData->pBody->pMovement->walkSpeed, 1, 100, "%.0f");
	ImGui::InputFloat("Speed Multiplier", &g_player.pGameData->pPlayerData->pBody->pMovement->speedMultiplier, 1, 100, "%.0f");


	ImGui::End();
}
