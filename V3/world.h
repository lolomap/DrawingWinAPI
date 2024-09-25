#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "shape.h"

using namespace std;

class world
{
public:
	vector<shape*> shapes;
	vector<shape*> respawns;

	void Dispose();
	~world();

	shape* CreateSquare(vec position);
	shape* CreateRandomSquare();
	void Process(double deltaTime);
};

