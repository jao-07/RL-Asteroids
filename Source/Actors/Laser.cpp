//
// Created by Lucas N. Ferreira on 12/09/23.
//

#include "Laser.h"
#include "Asteroid.h"
#include "../Game.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponent.h"

Laser::Laser(Game* game, const float length, const float deathTimer)
        :Actor(game)
        ,mLength(length)
        ,mDeathTimer(deathTimer)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (~2 linhas): Crie 2 vértices (Vector2) considerando o centro da nave como origem e o
    //  atributo mLength como o comprimento do raio laser. Por exemplo: v1 = (-l/2, 0) e v2 = (l/2, 0)
    Vector2 v1 = Vector2(-mLength/2, 0);
    Vector2 v2 = Vector2(mLength/2, 0);


    // TODO 1.2 (~3 linhas): Adicione esses 3 vértices end um container std::vector.
    std::vector<Vector2> verts;
    verts.push_back(v1);
    verts.push_back(v2);

    // TODO 1.3 (3 linhas): Instancie os componentes DrawComponent, RigidBodyComponent e CircleColliderComponent.
    //  Armazene esses componentes nos ponteiros mDrawComponent, mRigidBodyComponent e mCircleColliderComponent,
    //  respectivamente. O container de vértices criado na etapa anterior será passado como parâmetro para o
    //  DrawComponente. Para o RigidBodyComponent, passe uma massa pequena (e.g., 0.1) como parâmetro.
    //  Para o CircleColliderComponent, passe o comprimento do raio lases (mLenght) como raio de colisão.
    mDrawComponent = new DrawComponent(this, verts);
    mRigidBodyComponent = new RigidBodyComponent(this, 0.1);
    mCircleColliderComponent = new CircleColliderComponent(this, mLength);
}

void Laser::OnUpdate(float deltaTime)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 2.1 (~9 linhas): O raio laser deve ser destruído depois de um tempo desde sua emissão ou quando houver
    //  colisão com um asteroide. Para contar quanto tempo percorreu desde a emissão do raio laser, subtraia o
    //  deltaTime do cronômetro criado para essa contagem (mDeathTimer). Verifique se esse cronômetro é menor ou
    //  igual a zero. Se for, destrua o laser alterando seu estado (SetState) para ActorState::Destroy. Caso
    //  contrário, percorra a lista de asteroids (GetGame()->GetAsteroids()), verificando se o laser colide com
    //  algum deles. Se houver colisão, destrua o laser e o asteroide alterando seus estados para ActorState::Destroy.
    mDeathTimer -= deltaTime;
    if(mDeathTimer <= 0)
        SetState(ActorState::Destroy);

    std::vector<Asteroid*> asteroids = mGame->GetAsteroids();
    for (int i=0; i<asteroids.size(); i++) {
        auto *ast = asteroids[i]->GetComponent<CircleColliderComponent>();
        if (this->mCircleColliderComponent->Intersect(*ast)) {
            SetState(ActorState::Destroy);
            asteroids[i]->SetState(ActorState::Destroy);
            break;
        }
    }
}
