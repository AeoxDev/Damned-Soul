#pragma once
#include <string>

////Hermano 3000
//#define GAME_TEST

//#define UI_TEST

////does not work, dont use
//#define MAIN_MENU_TEST

////Not done, Dont use
//#define PARTICLE_TEST

//Displays info in the application title
void UpdateDebugWindowTitle(std::string& title, std::string extra = "");

#ifdef GAME_TEST

//Simulate [sim] frames in every level for [total] frames
void SimulateGame(std::string& title, int sim, int total);

#endif // GAME_TEST

#ifdef UI_TEST

//Simulate Main Menu <-> Settings state switching [total] times
void SimulateUI(std::string& title, int total);

#endif // UI_TEST

#ifdef MAIN_MENU_TEST

//Simulate Main Menu enemies for [total] frames
void SimulateMainMenu(std::string& title, int total);

#endif // MAIN_MENU_TEST

#ifdef PARTICLE_TEST

//Not done, dont use
void SimulateParticleLevel(std::string& title, int total);

#endif // PARTICLE_TEST
