#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

class TrainerDisplay
{
public:
	
	std::string sGameStatus = "ON MISSION";
	std::string sGoodWeapons = "OFF";
	std::string sGodWeapons = "OFF";
	std::string sRapidFire = "OFF";
	std::string sSteroids = "OFF";
	std::string sTeleport = "NOT SAVED";
	std::string sMineralHook = "NOT HOOKED";
	std::string sObjectiveHook = "NOT HOOKED";

	void SetWindow()
	{
		SetConsoleTitleA("UtterVitriol's Deep Rock Galactic Trainer");
		system("mode 53, 35");
	}
	void Print()
	{
		system("cls");
		std::cout << "----------------------------------------------------\n"
			<< "----------------------------------------------------\n\n"
			<< "                    REEEEEEEEE		                 \n"
			<< "     UtterVitriol's Deep Rock Galactic Trainer       \n\n"
			<< "----------------------------------------------------\n"
			<< "----------------------------------------------------\n\n"
			<< "[NUMPAD1] Good Weapons       -> " << sGoodWeapons << " <-\n\n"
			<< "[NUMPAD2] God Weapons        -> " << sGodWeapons << " <-\n\n"
			<< "[NUMPAD3] Rapid Fire         -> " << sRapidFire << " <-\n\n"
			<< "[NUMPAD4] Steroids           -> " << sSteroids << " <-\n\n"
			<< "[NUMPAD5] Mineral Hook       -> " << sMineralHook << " <-\n\n"
			<< "[NUMPAD6] Objective Hook       -> " << sObjectiveHook << " <-\n\n"
			<< "[NUMPAD7] Save Location\n"
			<< "[NUMPAD8] Teleport Location\n"
			<< "[END] Exit\n\n"
			<< "----------------------------------------------------\n"
			<< "----------------------------------------------------\n\n"
			<< "Game Status -> " << sGameStatus << " <- \n\n"
			<< "----------------------------------------------------\n"
			<< "----------------------------------------------------\n" <<std::flush;
	}

	void Print(std::string* StatusType, LPCSTR Status)
	{
		if (*StatusType != Status)
		{
			*StatusType = Status;
			Print();
		}
	}


};

