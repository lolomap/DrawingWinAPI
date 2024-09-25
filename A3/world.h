#pragma once
#include <vector>
#include <iostream>
#include <random>
#include "shape.h"

using namespace std;

class world
{
private:
	void ElasticCollision(shape* a, shape* b);

public:
	vector<shape*> shapes;
	vector<shape*> respawns;

	void Dispose();
	~world();

	shape* CreateSquare(vec position, vec direction, double speed);
	shape* CreateRandomSquare();

	void Process(double deltaTime);
};

