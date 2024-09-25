#include "world.h"
#include "defines.h"

void world::Dispose()
{
    for (shape* instance : shapes)
    {
        delete instance;
    }
    shapes.clear();
}

world::~world()
{
    Dispose();
}

shape* world::CreateSquare(vec position)
{
    shape* instance = new shape(position, SQUARE_SIZE / 2.f, SQUARE_SIZE / 2.f);
    instance->direction = vec(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 - 1,
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 - 1);
    shapes.push_back(instance);

    return instance;
}

shape* world::CreateRandomSquare()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(SQUARE_SIZE + SPAWN_BORDER, WINDOW_WIDTH - SQUARE_SIZE - SPAWN_BORDER);
    return CreateSquare(vec(distrib(gen), distrib(gen)));
    //return CreateSquare(point(300, 300));
}

void world::Process(double deltaTime)
{
    for (shape* instance : shapes)
    {
        instance->center.Move(instance->direction, SQUARE_SPEED, deltaTime);
        
        if (instance->center.x + instance->halfWidth >= WINDOW_WIDTH ||
            instance->center.x - instance->halfWidth <= 0 ||
            instance->center.y + instance->halfHeight >= WINDOW_HEIGHT ||
            instance->center.y - instance->halfHeight <= 0)
        {
            respawns.push_back(instance);
        }
    }


    for (shape* instance : respawns)
    {
        vector<shape*>::iterator pos = find(shapes.begin(), shapes.end(), instance);
        if (pos != shapes.end())
        {
            delete *pos;
            shapes.erase(pos);
        }
        CreateRandomSquare();
    }
    respawns.clear();
}
