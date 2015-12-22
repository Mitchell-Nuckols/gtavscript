/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#include "script.h"

#include <string>
#include <ctime>
#include <string>

using namespace std;

bool get_key_pressed(int nVirtKey)
{
	return (GetAsyncKeyState(nVirtKey) & 0x8000) != 0;
}

DWORD trainerResetTime = 0;

bool mod_switch_pressed()
{
	return (GetTickCount() > trainerResetTime + 1000) && get_key_pressed(VK_F4);
}

void reset_mod_switch()
{
	trainerResetTime = GetTickCount();
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		} else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

bool enabled = false;

inline string getModState() {
	return enabled ? "~g~Enabled" : "~r~Disabled";
}

void sendNotification(char* msg) {
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(msg);
	UI::_0x2ED7843F8F801023(false, true);
}

void main()
{
	while (true)
	{
		if (mod_switch_pressed())
		{
			reset_mod_switch();
			DWORD time = GetTickCount() + 1000;

			enabled = !enabled;

			string msg = "Mitch's Mod: " + getModState();

			sendNotification((char*)msg.c_str());

			PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), enabled);

			while (GetTickCount() < time)
			{
				update_status_text();
				WAIT(0);
			}
			reset_mod_switch();
		}

		if(enabled && PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false);
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
