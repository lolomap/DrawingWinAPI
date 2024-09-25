#include "world.h"
#include "defines.h"

void world::ElasticCollision(shape* a, shape* b)
{
    vec velocityA = a->direction * a->speed;
    vec velocityB = b->direction * b->speed;

    vec collisionDirection = (b->center - a->center).Normalized();

    // Projections on collision vector
    double collisionA = velocityA.Dot(collisionDirection);
    double collisionB = velocityB.Dot(collisionDirection);

    vec normalA = velocityA - collisionDirection * collisionA;
    vec normalB = velocityB - collisionDirection * collisionB;

    swap(collisionA, collisionB);
    velocityA = normalA + collisionDirection * collisionA;
    velocityB = normalB + collisionDirection * collisionB;


    a->direction = velocityA.Normalized();
    b->direction = velocityB.Normalized();
    a->speed = velocityA.GetLength();
    b->speed = velocityB.GetLength();
}

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

shape* world::CreateSquare(vec position, vec direction, double speed)
{
    shape* instance = new shape(position, SQUARE_SIZE / 2.f, SQUARE_SIZE / 2.f);
    instance->direction = direction;
    instance->speed = speed;
    shapes.push_back(instance);

    return instance;
}

shape* world::CreateRandomSquare()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(SQUARE_SIZE + SPAWN_BORDER, WINDOW_WIDTH - SQUARE_SIZE - SPAWN_BORDER);
    return CreateSquare(vec(distrib(gen), distrib(gen)),
        vec(static_cast <double> (rand()) / static_cast <double> (RAND_MAX) * 2 - 1,
            static_cast <double> (rand()) / static_cast <double> (RAND_MAX) * 2 - 1),
        rand() % 300 + 50);
}

void world::Process(double deltaTime)
{
    for (shape* instance : shapes)
    {
        instance->center.Move(instance->direction, instance->speed  , deltaTime);
        
        if (instance->center.x + instance->halfWidth >= WINDOW_WIDTH ||
            instance->center.x - instance->halfWidth <= 0 ||
            instance->center.y + instance->halfHeight >= WINDOW_HEIGHT ||
            instance->center.y - instance->halfHeight <= 0)
        {
            respawns.push_back(instance);
        }

        for (shape* i2 : shapes)
        {
            if (instance == i2) continue;

            double distance = instance->center.GetDistance(i2->center);
            double minDistance = instance->GetRadius() + i2->GetRadius();
            double difference = (minDistance - distance);
            
            if (distance <= minDistance)
            {  
                // Push out
                if (difference > EQ_TOLERANCE)
                {
                    vec iD = instance->direction.Normalized() * -1.;
                    vec i2D = i2->direction.Normalized() * -1.;
                    instance->center.x += iD.x * difference;
                    instance->center.y += iD.y * difference;
                    i2->center.x += i2D.x * difference;
                    i2->center.y += i2D.y * difference;
                }

                ElasticCollision(instance, i2);
            }
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
