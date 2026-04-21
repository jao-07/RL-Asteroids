//
// Created by Lucas N. Ferreira on 08/09/23.
//

#include <SDL.h>
#include "RigidBodyComponent.h"
#include "../Actors/Actor.h"
#include "../Game.h"

RigidBodyComponent::RigidBodyComponent(class Actor* owner, float mass, int updateOrder)
        :Component(owner, updateOrder)
        ,mMass(mass)
        ,mAngularSpeed(0.0f)
        ,mVelocity(Vector2::Zero)
        ,mAcceleration(Vector2::Zero)
{

}

void RigidBodyComponent::ApplyForce(const Vector2 &force) {
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1. (~1 linha): Adicione à aceleração do objeto (mAcceleration) a forca (force) passada como parâmetro,
    //  multiplicando pelo inverso pela massa (mMass) do objeto.
    mAcceleration.x += force.x * 1.0/mMass;
    mAcceleration.y += force.y * 1.0/mMass;
}

void RigidBodyComponent::Update(float deltaTime)
{
    Vector2 position = mOwner->GetPosition();

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 2.1 (2 linhas): Atualize a velocidade (mVelocity) e a posição (position) do objeto utilizando
    //  o método de Euler semi-implícito.
    mVelocity += mAcceleration * deltaTime;
    mVelocity.x = mVelocity.x > mMaxVelocity ? mMaxVelocity : mVelocity.x;
    mVelocity.y = mVelocity.y > mMaxVelocity ? mMaxVelocity : mVelocity.y;

    mVelocity.x = mVelocity.x < mMinVelocity ? mMinVelocity : mVelocity.x;
    mVelocity.y = mVelocity.y < mMinVelocity ? mMinVelocity : mVelocity.y;

    position += mVelocity * deltaTime;

    ScreenWrap(position);

    mOwner->SetPosition(position);

    // TODO 2.2 (~3 linhas): Utilize a função Math::NearZero para verificar se o comprimento do vetor
    //  velocidade (mVelocity) está próximo de zero. Se estiver, use a função mVelocity.Set() para
    //  forçar velocidade zero. Isso evita movimentos muito pequenos.
    if (Math::NearZero(mVelocity.x) && Math::NearZero(mVelocity.y)) {
        mVelocity.Set(0, 0);
    }

    // TODO 2.3 (1 linhas): Utilize a função mAcceleration.Set() para reinicializar a aceleração para zero.
    mAcceleration.Set(0, 0);


    float rot = mOwner->GetRotation();

    // TODO 2.4 (1 linha): Some à rotação atual do objeto (rot) a velocidade angular (mAngularSpeed)
    //  multiplicada pelo deltaTime.
    rot += mAngularSpeed * deltaTime;

    mOwner->SetRotation(rot);
}

void RigidBodyComponent::ScreenWrap(Vector2 &position)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 3.1 (~6 linhas): Verifique se o objeto saiu pelo lado esquerdo da tela. Se tiver saído,
    //  altere sua posição horizontal para ser igual à largura da tela. Caso contrário, verifique
    //  se o objeto saiu pelo lado direito. Se tiver saído, altere sua posição horizontal para ser
    //  igual a zero.
    if (position.x < 0) {
        position.x = mOwner->GetGame()->GetWindowWidth();
    }
    else if (position.x > mOwner->GetGame()->GetWindowWidth()) {
        position.x = 0;
    }


    // TODO 3.2 (~6 linhas): Verifique se o objeto saiu por cima da tela. Se tiver saído,
    //  altere sua posição vertical para ser igual à altura da tela. Caso contrário, verifique
    //  se o objeto saiu por baixo. Se tiver saído, altere sua posição vertical para ser
    //  igual a zero.
    if (position.y < 0) {
        position.y = mOwner->GetGame()->GetWindowHeight();
    }
    else if (position.y > mOwner->GetGame()->GetWindowHeight()) {
        position.y = 0;
    }

}
