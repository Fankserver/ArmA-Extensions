// calcBattlEyeGUID.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "md5\md5.h"

typedef unsigned char int8;

extern "C" {
	__declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
}

void __stdcall RVExtension(char *output, int outputSize, const char *function) {
	long long int steamId = atoll(function);
	int8 i = 0, parts[8] = { 0 };
	do parts[i++] = steamId & 0xFF;
	while (steamId >>= 8);

	std::stringstream bestring;
	bestring << "BE";
	for (int i = 0; i < sizeof(parts); i++) {
		bestring << char(parts[i]);
	}

	//std::ofstream banFile("Skaronator_ServerCfg/BattlEye/bans.txt", std::ios::out | std::ios::app);
	//if (banFile.is_open()) {
	//	banFile << md5(bestring.str()) << " -1 Skaronator.com Autoban (Autoban Reason)" << std::endl;
	//	banFile.close();
	//}

	strncpy(output, md5(bestring.str()).c_str(), outputSize);
}