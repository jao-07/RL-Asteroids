//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Ship.h"
#include "Asteroid.h"
#include "../Game.h"
#include "../Actors/Laser.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponent.h"

Ship::Ship(Game* game,
           const float height,
           const float forwardForce,
           const float rotationForce,
           const float frictionCoefficient)
        : Actor(game)
        , mLaserCooldown(0.f)
        , mHeight(height)
        , mRigidBodyComponent(nullptr)
        , mDrawComponent(nullptr)
        , mForwardSpeed(forwardForce)
        , mRotationForce(rotationForce)
        , mFrictionCoefficient(frictionCoefficient)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (~3 linhas): Crie 3 vértices (Vector2) considerando o centro da nave como origem e o
    //  atributo mHeight como altura do triângulo. Por exemplo: v1 = (-h,h/2), v2 = (h, 0) e v3 = (-h/2)

    // TODO 1.2 (~4 linhas): Adicione esses 3 vértices end um container std::vector.

    // TODO 1.3 (3 linhas): Instancie os componentes DrawComponent, RigidBodyComponent e CircleColliderComponent.
    //  Armazene esses componentes nos ponteiros mDrawComponent, mRigidBodyComponent e mCircleColliderComponent,
    //  respectivamente. O container de vértices criado na etapa anterior será passado como parâmetro para o
    //  DrawComponente. E, para o CircleColliderComponent, passe a metade da altura da nave como raio de colisão.
}

void Ship::OnProcessInput(const uint8_t* state)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 2.1 (~4 linhas): Verifique se o jogador está pressionando a tecla W e, se estiver,
    //  aplique uma força para frente com magnitude dada pelo atributo mForwardSpeed. Utilize o método
    //  GetForward() para obter o vetor da frente e a função ApplyForce do componente mRigidBodyComponent
    //  para aplicar a força.

    // TODO 2.2 (~4 linhas): Inicialize uma variável local angularSpeed com zero e verifique se o jogador
    //  está pressionando a tecla A. Se estiver, some à velocidade angular (angularSpeed) a velocidade de
    //  rotação mRotationForce.

    // TODO 2.3 (~4 linhas): Verifique se o jogador está pressionando a tecla D. Se estiver,
    //  subtraia da velocidade angular (angularSpeed) a velocidade de rotação (mRotationForce).

    // TODO 2.4 (1 linha): Verifique se o jogador está pressionando a barra de Espaço e se o tempo de
    //  resfriamento do laser já passou (mLaserCooldown <= 0.0f). Se ambas as condições forem verdadeiras:

        // TODO 2.4.1 (1 linha): Instancie uma nova partícula de laser com tamanho 5.0;

        // TODO 2.4.2 (1 linha): Posicione esse partícula na ponta da frente da nave (posição da nave + vetor forward *
        //  altura do triângulo da nave);

        // TODO 2.4.3 (1 linha): Inicialize a rotação dessa partícula com o ângulo da nave. Basta utilizar
        //  os métodos SetRotation do laser e GetRotation da nave;

        // TODO 2.4.4 (1 linha): Aplique uma força para frente nessa partícula com magnitude 3000.0;

        // TODO 2.4.5 (1 linha): Reinicialize o tempo de resfriamento do laser para um quarto de segundo (0.25).

    // TODO 2.5 (1 linha): Altere a velocidade angular (SetAngularSpeed) com o novo valor calculado (angularSpeed).
}

void Ship::OnUpdate(float deltaTime)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 3.1 (1 linha): Subtraia deltaTime do tempo de resfriamento do laser (mLaserCooldown)
    mLaserCooldown -= deltaTime;

    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if(velocity.Length() > 0) {
        // TODO 3.2 (~6 linhas): Calcule a força de resistência do ar para parar lentamente a nave
        //  lembre-se que essa força é um vetor f_r = -v.norm() * ||v||^2 * c_r. Onde v é o vetor
        //  velocidade (velocity) e c_r é o coeficiente de resistência (mFrictionCoefficient).
        //  Armazena a força calculada em um vetor chamado drag.

        // TODO 3.3 (1 linha): Aplique a força drag na nave com a função ApplyForce do mRigidBodyComponent.
    }

    // TODO 3.4 (~6 linhas): Percorra a lista de asteroides do jogo e verifique, para cada asteroide,
    //  se ele está colidindo com a nave. Lembre-se que o método GetGame()->GetAsteroids() retorna a lista
    //  de asteroides. Além disso, você já implementou o método Intersect do CircleColliderComponent. Tanto a
    //  nave quanto os asteroides possuem esse componente, então basta utilizar essa função para verificar
    //  a colisão. Se houver colisão da nave com algum asteroide, termine o jogo (GetGame()->Quit()).
}
