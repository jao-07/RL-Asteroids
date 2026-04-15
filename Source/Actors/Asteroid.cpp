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
    // --------------
    // TODO - PARTE 3
    // --------------

    Vector2 randStartingForce = Vector2::Zero;
    float averageLength = 0.0f;
    std::vector<Vector2> vertices;

    if (size == AsteroidSize::Large) {
        // Create a circle with numVertices
        vertices = GenerateVertices(numVertices, 80);

        // Calculate average vertices length to be the collider radius
        averageLength = CalculateAverageVerticesLength(vertices);

        //Set the position of the asteroid so that there is no collision with the ship in the instance
         Vector2 pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
         while (!(pos.y > (mGame->GetWindowHeight() - 200) || pos.y < 200))
             pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
        SetPosition(pos);

        // Generate random starting force
        randStartingForce = GenerateRandomStartingForce(1000.0f, 1500.0f);
    }
    else {
        // Create a circle with numVertices
        vertices = GenerateVertices(numVertices, 40);

        // Calculate average vertices length to be the collider radius
        averageLength = CalculateAverageVerticesLength(vertices);

        SetPosition(position);

        // Generate random starting force
        randStartingForce = GenerateRandomStartingForce(2000.0f, 2500.0f);
    }

    // TODO 1.3 (3 linhas): Instancie os componentes DrawComponent, RigidBodyComponent e CircleColliderComponent.
    //  Armazene esses componentes nos ponteiros mDrawComponent, mRigidBodyComponent e mCircleColliderComponent,
    //  respectivamente. O container de vértices criado na etapa anterior será passado como parâmetro para o
    //  DrawComponente. E, para o CircleColliderComponent, passe a média dos comprimentos dos vértices gerados
    //  (averageLength) como raio de colisão.
    mDrawComponent = new DrawComponent(this, vertices);
    mRigidBodyComponent = new RigidBodyComponent(this);
    mCircleColliderComponent = new CircleColliderComponent(this, averageLength);

    // TODO 1.4 (1 linha): Aplique a força aleatória gerada anteriormente (randStartingForce) para mover o
    //  asteroide. Utilize a função ApplyForce do componente mRigidBodyComponent
    mRigidBodyComponent->ApplyForce(randStartingForce);

    // TODO 1.5 (1 linha): Adicione (game->AddAsteroid()) esse asteroide (this) à lista de asteroides do jogo.
    mGame->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 4.1 (1 linha): Remova (game->RemoveAsteroid()) esse asteroide (this) da lista de asteroides do jogo.
    mGame->RemoveAsteroid(this);
}


std::vector<Vector2> Asteroid::GenerateVertices(const int numVertices, const float radius)
{
    // Gerar um conjunto de vértices em uma circunferência, adicionando um pequeno ruído a cada um deles.

    std::vector<Vector2> vertices;

    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 5.1 (1 linha): Inicialize variável angle (float) com zero. Ela será
    //  utilizada para percorrer o arco de uma circunferência em intervalos angulares de tamanho fixo.
    float angle = 0.0f;

    // TODO 5.2 (1 linha): Repita o seguinte procedimento para um dado número de vértices (numVertices):
    for (int i = 0; i < numVertices; i++) {

    //  (a) Gere um número real entre 0.5 e 1.0 e multiplique pelo raio da circunferência (radius). Armazene o
    //  resultado em uma variável randLength;
        float randLength = Random::GetFloatRange(0.7, 1) * radius;
    //  (b) Calcule a coordenada x do novo vértice multiplicando randLength pelo cosseno do ângulo corrente (angle);
    //  (b) Calcule a coordenada y da mesma forma, porém multiplicando pelo seno do ângulo corrente;
    //  (c) Adicione o vetor (x,y) ao conjunto de vértices (vertices);
        vertices.push_back(Vector2(randLength * cos(angle), randLength * sin(angle)));
    //  (d) Incremente o ângulo corrente por 2*PI dividido pelo número de vértices (numVertices).
        angle += 2 * M_PI / numVertices;
    }
    return vertices;
}

float Asteroid::CalculateAverageVerticesLength(std::vector<Vector2>& vertices)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 6.1 (~4 linhas): Calcule e retorne a média de comprimento dos vértices em (vertices).
    float total = 0;
    for (int i = 0; i < vertices.size(); i++) {
        total += vertices[i].Length();
    }
    return total / vertices.size();
}

Vector2 Asteroid::GenerateRandomStartingForce(const float min, const float max)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 7.1 (~3 linhas): Escolha aleatóriamente (com probabilidade uniforme) uma direção horizontal (-1.0 ou 1.0).
    //  Dica: utilize a função Random::GetFloat() e verifique se o resultado é menor do que 0.5. Se for, a direção
    //  é 1.0, se não é -1.0. Armazene o resultado em uma variável randDirX.
    float randDirX = Random::GetFloat() < 0.5 ? 1.0f : -1.0f;

    // TODO 7.2 (~3 linhas): Escolha aleatóriamente (com probabilidade uniforme) uma direção vertical (-1.0 ou 1.0).
    //  Dica: utilize a função Random::GetFloat() e verifique se o resultado é menor do que 0.5. Se for, a direção
    //  é 1.0, se não é -1.0. Armazene o resultado em uma variável randDirY.
    float randDirY = Random::GetFloat() < 0.5 ? 1.0f : -1.0f;

    // TODO 7.3 (~3 linhas): Gere um vetor aleatório chamado randForce com a função Random::GetVector. Utilize os
    //  parâmetros min e max desse método para restringir os limites inferior e superior do vetor.
    Vector2 randForce = Random::GetVector(Vector2(min, min), Vector2(max, max));

    // TODO 7.4 (~3 linhas): Multiplique a componente horizontal de randForce por randDirX e a vertical por randDirY.
    //  Depois, retorn randForce.
    randForce.x *= randDirX;
    randForce.y *= randDirY;
    return randForce;
}