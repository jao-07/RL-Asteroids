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
        vertices = GenerateVertices(numVertices, 16);
        averageLength = CalculateAverageVerticesLength(vertices);
        Vector2 pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
        float yLimit = mGame->GetWindowHeight() / 3.0f;
        float xLimit = mGame->GetWindowWidth() / 3.0f;
        while (!((pos.y > yLimit * 2 || pos.y < yLimit) && (pos.x > xLimit * 2 || pos.x < xLimit)))
            pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
        SetPosition(pos);

        // randStartingForce = GenerateRandomStartingForce(1200.0f, 1500.0f);
        randStartingForce = GenerateRandomStartingForce(500.0f, 750.0f);
    }
    else {
        vertices = GenerateVertices(numVertices, 8);
        averageLength = CalculateAverageVerticesLength(vertices);
        SetPosition(position);
        randStartingForce = GenerateRandomStartingForce(1000.0f, 1250.0f);
        // randStartingForce = GenerateRandomStartingForce(2200.0f, 2500.0f);
    }

    mDrawComponent = new DrawComponent(this, vertices);
    mRigidBodyComponent = new RigidBodyComponent(this);
    mCircleColliderComponent = new CircleColliderComponent(this, averageLength);

    mRigidBodyComponent->ApplyForce(randStartingForce);

    mGame->AddAsteroid(this);
    mGame->IncreaseAsteroidsNumber();
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
    float speed = Random::GetFloatRange(min, max);
    constexpr float TWO_PI = 2.0f * 3.1415926535f;
    float randomAngle = Random::GetFloat() * TWO_PI;

    Vector2 randForce;
    randForce.x = speed * cos(randomAngle);
    randForce.y = speed * sin(randomAngle);

    if (randForce.x == 0.0f && randForce.y == 0.0f) {
        randForce.x = max;
    }

    return randForce;
}