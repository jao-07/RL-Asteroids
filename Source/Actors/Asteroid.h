//
// Created by Lucas N. Ferreira on 10/09/23.
//

#pragma once
#include "Actor.h"

enum class AsteroidSize
{
    Large,
    Small
};

class Asteroid : public Actor
{
public:
    Asteroid(Game* game, AsteroidSize size = AsteroidSize::Large, Vector2 position = Vector2::Zero, const int numVertices = 10, const float forwardForce = 1000.0f);
    ~Asteroid();

    AsteroidSize GetSize() const { return mSize; }

private:
    static std::vector<Vector2> GenerateVertices(int numVertices, float radius);
    static Vector2 GenerateRandomStartingForce(float min, float max);

    static float CalculateAverageVerticesLength(std::vector<Vector2>& vertices);

    class DrawComponent* mDrawComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class CircleColliderComponent* mCircleColliderComponent;

    AsteroidSize mSize;
};
