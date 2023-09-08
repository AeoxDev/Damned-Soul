#pragma once
#include <string>
#include <iostream>

enum class Scene
{
	Level,
	Shop
};

struct Level
{
	std::string id;

	Level(std::string id) :id(id) {};

	void Update() 
	{ 
		std::cout << "I am in " << id << std::endl; 
	};
};

struct Shop
{
	std::string id = "Shop";

	void Update() 
	{ 
		std::cout << "I am in " << id << std::endl; 
	};
};