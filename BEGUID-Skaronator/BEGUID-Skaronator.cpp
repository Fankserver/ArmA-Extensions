// calcBattlEyeGUID.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdlib.h>

#include "md5\md5.h"

typedef unsigned char int8;

extern "C" {
	__declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void __stdcall RVExtension(char *output, int outputSize, const char *function) {
	std::vector<std::string> rawCmd = split(std::string(function), ':');

	long long int steamId = atoll(rawCmd[0].c_str());
	int8 i = 0, parts[8] = { 0 };
	do parts[i++] = steamId & 0xFF;
	while (steamId >>= 8);

	std::stringstream bestring;
	bestring << "BE";
	for (int i = 0; i < sizeof(parts); i++) {
		bestring << char(parts[i]);
	}

	std::stringstream playerName;
	for (std::vector<std::string>::iterator
		it = rawCmd.begin() + 1;
		it != rawCmd.end();
	) {
		playerName << ((rawCmd.begin() + 1) != it ? ":" : "") << *it;
		it++;
	}

	std::ofstream banFile("Skaronator_ServerCfg/BattlEye/bans.txt", std::ios::out | std::ios::app);
	if (banFile.is_open()) {
		std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct std::tm *time = std::localtime(&currentTime);

		banFile << md5(bestring.str()) << " -1 Skaronator.com Autoban [" << std::put_time(time, "%Y-%m-%d %H:%M:%S") << "] - " << playerName.str() << std::endl;
		banFile.close();
	}

	strncpy(output, md5(bestring.str()).c_str(), outputSize);
}