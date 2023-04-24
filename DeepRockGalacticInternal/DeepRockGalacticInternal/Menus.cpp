#include "Menus.h"
#include "Player.h"
#include "imgui.h"

extern MyPlayer g_player;
const float MAX_SCALE = 3.0F;
const float MIN_SCALE = 0.3F;

void MyMenu()
{
	ImGuiIO& io = ImGui::GetIO();

	
	static float window_scale = 1.0f;

	static bool bShowResources = false;

	ImGui::Begin("Tingle's Internal Trainer");   // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Press END to Eject");             // Display some text (you can use a format strings too)

	ImGui::Checkbox("Resources", &bShowResources);

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

	ImGui::InputFloat("Phazyonite", &g_player.pPlayer->Resources->Phazyonite);

	ImGui::SeparatorText("Crafting");

	ImGui::InputFloat("Magnite", &g_player.pPlayer->Resources->Magnite);
	ImGui::InputFloat("Bismor", &g_player.pPlayer->Resources->Bismor);
	ImGui::InputFloat("Umanite", &g_player.pPlayer->Resources->Umanite);
	ImGui::InputFloat("Croppa", &g_player.pPlayer->Resources->Croppa);
	ImGui::InputFloat("Enor Pearl", &g_player.pPlayer->Resources->EnorPearl);
	ImGui::InputFloat("Jadiz", &g_player.pPlayer->Resources->Jadiz);

	ImGui::SeparatorText("Beer");

	ImGui::InputFloat("Barley Bulb", &g_player.pPlayer->Resources->BarleyBulb);
	ImGui::InputFloat("Yeast Cone", &g_player.pPlayer->Resources->YeastCone);
	ImGui::InputFloat("Malt Star", &g_player.pPlayer->Resources->MaltStar);
	ImGui::InputFloat("Starch Nut", &g_player.pPlayer->Resources->StarchNut);

	ImGui::End();
}