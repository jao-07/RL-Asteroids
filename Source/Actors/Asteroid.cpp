//
// Created by Lucas N. Ferreira on 10/09/23.
//

#define DB_PERLIN_IMPL

#include "Asteroid.h"
#include "../Game.h"
#include "../Random.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponent.h"

Asteroid::Asteroid(Game* game, AsteroidSize size, Vector2 position, const int numVertices, const float forwardForce)
        :Actor(game)
        ,mRigidBodyComponent(nullptr)
        ,mDrawComponent(nullptr)
        ,mCircleColliderComponent(nullptr)
        ,mSize(size)
{
    Vector2 randStartingForce = Vector2::Zero;
    float averageLength = 0.0f;
    std::vector<Vector2> vertices;

    if (size == AsteroidSize::Large) {
        vertices = GenerateVertices(numVertices, 80);
        averageLength = CalculateAverageVerticesLength(vertices);
        Vector2 pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
        while (!(pos.y > (mGame->GetWindowHeight() - 200) || pos.y < 200))
            pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
        SetPosition(pos);

        randStartingForce = GenerateRandomStartingForce(1200.0f, 1500.0f);
    }
    else {
        vertices = GenerateVertices(numVertices, 40);
        averageLength = CalculateAverageVerticesLength(vertices);
        SetPosition(position);
        randStartingForce = GenerateRandomStartingForce(2100.0f, 2400.0f);
    }

    mDrawComponent = new DrawComponent(this, vertices);
    mRigidBodyComponent = new RigidBodyComponent(this);
    mCircleColliderComponent = new CircleColliderComponent(this, averageLength);

    mRigidBodyComponent->ApplyForce(randStartingForce);

    mGame->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
    mGame->RemoveAsteroid(this);
}


std::vector<Vector2> Asteroid::GenerateVertices(const int numVertices, const float radius)
{
    // Gerar um conjunto de vértices em uma circunferência, adicionando um pequeno ruído a cada um deles.

    std::vector<Vector2> vertices;

    float angle = 0.0f;
    for (int i = 0; i < numVertices; i++) {
        float randLength = Random::GetFloatRange(0.7, 1) * radius;
        vertices.push_back(Vector2(randLength * cos(angle), randLength * sin(angle)));
        angle += 2 * M_PI / numVertices;
    }
    return vertices;
}

float Asteroid::CalculateAverageVerticesLength(std::vector<Vector2>& vertices)
{
    float total = 0;
    for (int i = 0; i < vertices.size(); i++) {
        total += vertices[i].Length();
    }
    return total / vertices.size();
}

Vector2 Asteroid::GenerateRandomStartingForce(const float min, const float max)
{
    float randDirX = Random::GetFloat() < 0.5 ? 1.0f : -1.0f;
    float randDirY = Random::GetFloat() < 0.5 ? 1.0f : -1.0f;
    Vector2 randForce = Random::GetVector(Vector2(min, min), Vector2(max, max));

    randForce.x *= randDirX;
    randForce.y *= randDirY;
    return randForce;
}