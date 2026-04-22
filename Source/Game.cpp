// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <vector>
#include "Game.h"
#include "Actors/Ship.h"
#include "Actors/Asteroid.h"
#include "Components/DrawComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Random.h"
#include "Actors/Particle.h"

Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mShip(nullptr)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mGameState(GameState::Playing)
{

}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("TP2: Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    return true;
}

void Game::CreateAsteroids() {
    for (int i=0; i < 10; i++) {
        Asteroid* ast = new Asteroid(this, AsteroidSize::Large);
    }
}

void Game::InitializeActors()
{
    mShip = new Ship(this, 20);
    mShip->SetPosition(Vector2(mWindowWidth / 2.0f, mWindowHeight / 2.0f));
    
    CreateAsteroids();

}

void Game::RunLoop()
{
    GenerateOutput();
    while (mIsRunning)
    {
        ProcessInput();
        if (!mWaitingForAction) {
            UpdateGame();
            GenerateOutput();
        }
    }
}

void Game::ApplyAction(Action action) {
    mSelectedAction = action;
    mWaitingForAction = false;
    mFramesToProcess = 4;
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) Quit();

        // if (mWaitingForAction && event.type == SDL_KEYDOWN) {
        //     switch (event.key.keysym.sym) {
        //         case SDLK_1: mSelectedAction = Action::Left; break;
        //         case SDLK_2: mSelectedAction = Action::Right; break;
        //         case SDLK_3: mSelectedAction = Action::Forward; break;
        //         case SDLK_4: mSelectedAction = Action::Shoot; break;
        //         case SDLK_r: Reset();
        //         default: mSelectedAction = Action::Nothing; break;
        //     }
        //
        //     mWaitingForAction = false;
        //     mFramesToProcess = 4;
        // }
    }
}

// void Game::ProcessInput()
// {
//
//     SDL_Event event;
//     while (SDL_PollEvent(&event))
//     {
//         switch (event.type)
//         {
//             case SDL_QUIT:
//                 Quit();
//                 break;
//         }
//     }
//
//     const Uint8* state = SDL_GetKeyboardState(nullptr);
//     mUpdatingActors = true;
//     for (auto actor : mActors)
//     {
//         actor->ProcessInput(state);
//     }
//     mUpdatingActors = false;
// }

void Game::UpdateGame() {
    //if (!mWaitingForAction && mFramesToProcess > 0) {
    for (int i=mFramesToProcess; i>0; i--){

        for (auto actor : mActors){
             actor->ProcessInput(mSelectedAction);
         }

        constexpr float fixedDT = 1.0f / 60.0f;
        UpdateActors(fixedDT);

        // mFramesToProcess--;

        // if (mFramesToProcess <= 0) {
        //     mWaitingForAction = true;
        //     mSelectedAction = Action::NoAction;
        // }
    }
}

// void Game::UpdateGame()
// {
//     while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
//
//     float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
//     if (deltaTime > 0.05f)
//     {
//         deltaTime = 0.05f;
//     }
//
//     mTicksCount = SDL_GetTicks();
//
//     // Update all actors and pending actors
//     UpdateActors(deltaTime);
//
//     if (mBigAsteroids.empty() && mSmallAsteroids.empty() && mGameState == GameState::Playing) {
//         mGameState = GameState::Waiting;
//         mPauseTime = SDL_GetTicks();
//     }
//
//     if (mGameState == GameState::Waiting) {
//         if (SDL_GetTicks() - mPauseTime >= 1000) {
//             mShip->Reset();
//             CreateAsteroids();
//             mGameState = GameState::Playing;
//         }
//     }
// }

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
    {
        //SDL_Log("Deleting actor: %p (%s)", actor, typeid(*actor).name());
        delete actor;
    }

    if (!mAsteroids.empty())
        orderAsteroids();
}

void Game::CreateParticles(Asteroid *ast, float min, float max) {
    for (int i = 0; i < 20; i++) {
        Vector2 dir = Random::GetVector(Vector2(-1.0f, -1.0f), Vector2(1.0f, 1.0f));
        dir.Normalize();
        Vector2 force = dir * Random::GetFloatRange(min, max);

        new Particle(this, ast->GetPosition(), force);
    }
}

void Game::AddAsteroid(Asteroid* ast)
{
    // if (ast->GetSize() == AsteroidSize::Large)
    //     mBigAsteroids.emplace_back(ast);
    // else
    //     mSmallAsteroids.emplace_back(ast);

    mAsteroids.emplace_back(ast);
}

// void Game::RemoveAsteroid(Asteroid* ast)
// {
//     if (ast->GetSize() == AsteroidSize::Large) {
//         auto iter = std::find(mBigAsteroids.begin(), mBigAsteroids.end(), ast);
//         if (iter != mBigAsteroids.end()) {
//             if (ast->GetSize() == AsteroidSize::Large) {
//                 Vector2 pos = ast->GetPosition();
//                 for (int i=0; i<3; i++) {
//                     Vector2 offset = Random::GetVector(Vector2(-10.0f, -10.0f), Vector2(10.0f, 10.0f));
//                     auto *newSmallAst = new Asteroid(this, AsteroidSize::Small, pos+offset);
//                 }
//             }
//             ast->SetState(ActorState::Destroy);
//             std::iter_swap(iter, mBigAsteroids.end() - 1);
//             mBigAsteroids.pop_back();
//
//             CreateParticles(ast, 600, 1000);
//
//         }
//         else
//             SDL_Log("Attempting to remove asteroid not in list");
//     }
//     else {
//         auto iter = std::find(mSmallAsteroids.begin(), mSmallAsteroids.end(), ast);
//         if (iter != mSmallAsteroids.end()) {
//             if (ast->GetSize() == AsteroidSize::Large) {
//                 Vector2 pos = ast->GetPosition();
//                 for (int i=0; i<1; i++) {
//                     Vector2 offset = Random::GetVector(Vector2(-10.0f, -10.0f), Vector2(10.0f, 10.0f));
//                     Asteroid *newSmallAst = new Asteroid(this, AsteroidSize::Small, pos+offset);
//                 }
//             }
//             ast->SetState(ActorState::Destroy);
//             std::iter_swap(iter, mSmallAsteroids.end() - 1);
//             mSmallAsteroids.pop_back();
//
//             CreateParticles(ast, 300, 500);
//         }
//         else
//             SDL_Log("Attempting to remove asteroid not in list");
//     }
// }

void Game::RemoveAsteroid(Asteroid* ast)
{
    auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
        if (iter != mAsteroids.end()) {
            Vector2 pos = ast->GetPosition();
            bool isLarge = (ast->GetSize() == AsteroidSize::Large);

            ast->SetState(ActorState::Destroy);
            CreateParticles(ast, 600, 1000);

            std::iter_swap(iter, mAsteroids.end() - 1);
            mAsteroids.pop_back();

            if (isLarge) {
                for (int i=0; i<3; i++) {
                    Vector2 offset = Random::GetVector(Vector2(-10.0f, -10.0f), Vector2(10.0f, 10.0f));
                    auto *newSmallAst = new Asteroid(this, AsteroidSize::Small, pos+offset);
                }
            }
        }
        else
            SDL_Log("Attempting to remove asteroid not in list");
}

float Game::GetWrappedDelta(const float p1, const float p2, const float limit) {
    float dx = p2 - p1;
    if (dx > limit / 2.0f) {
        dx -= limit;
    } else if (dx < -limit / 2.0f) {
        dx += limit;
    }
    return dx;
}

float Game::GetWrappedDistanceSq(const Actor* a, const Actor* b) const {
    const float dx = GetWrappedDelta(a->GetPosition().x, b->GetPosition().x, static_cast<float>(mWindowWidth));
    const float dy = GetWrappedDelta(a->GetPosition().y, b->GetPosition().y, static_cast<float>(mWindowHeight));
    return dx*dx + dy*dy;
}

void Game::orderAsteroids () {
    std::sort(mAsteroids.begin(), mAsteroids.end(), [this](const Asteroid* a, const Asteroid* b) {
        const float distA = GetWrappedDistanceSq(mShip, a);
        const float distB = GetWrappedDistanceSq(mShip, b);
        return distA < distB;
    });
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
                  return a->GetDrawOrder() < b->GetDrawOrder();
              });
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::GenerateOutput()
{
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    for (auto drawable : mDrawables)
    {
        drawable->Draw(mRenderer);
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);

    const std::vector<float> state = GetObservationSpace();
    SDL_Log("state: %d", state.size());
    for (auto s : state)
        SDL_Log("%f", s);
}

void Game::Shutdown()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::DeleteAsteroids() {
    for (int i = mAsteroids.size() - 1; i >= 0; i--) {
        SDL_Log("Deleting asteroid %d", i);
        mAsteroids[i]->SetState(ActorState::Destroy);
        mAsteroids.pop_back();
    }
}

void Game::DeleteActors() {
    for (int i = mActors.size()-1; i >= 0; i--) {
        mActors[i]->SetState(ActorState::Destroy);
        mActors.pop_back();
    }
    mDrawables.clear();
}

void Game::Reset() {
    mShip->Reset();
    DeleteAsteroids();
    DeleteActors();
    InitializeActors();
    mStepsDone = 0;
}

/* 0: Pos x da nave
 * 1: Pos y da nave
 * 2: Raio da nave
 * 3: cos(angulo) da nave (Actor::GetForward)
 * 4: sen(angulo) da nave (Actor::GetForward)
 * 5: vel x da nave
 * 6: vel y da nave
 * 7: cooldown do tiro da nave
 * de 8 a 32 para os 5 asteroids mais proximos
 * 8: pos relativa x do asteroid
 * 9: pos relativa y do asteroid
 * 10: raio do asteroide
 * 11: vel x do asteroid
 * 12: vel y do asteroid
 */
std::vector<float> Game::GetObservationSpace() const {
    std::vector<float> states;
    //Dados da nave
    states.emplace_back(mShip->GetPosition().x / static_cast<float>(mWindowWidth));
    states.emplace_back(mShip->GetPosition().y / static_cast<float>(mWindowHeight));
    states.emplace_back(mShip->GetComponent<CircleColliderComponent>()->GetRadius() / MAX_RADIUS);
    states.emplace_back(mShip->GetForward().x);
    states.emplace_back(mShip->GetForward().y);
    states.emplace_back(mShip->GetComponent<RigidBodyComponent>()->GetVelocity().x / MAX_SHIP_VELOCITY);
    states.emplace_back(mShip->GetComponent<RigidBodyComponent>()->GetVelocity().y / MAX_SHIP_VELOCITY);
    states.emplace_back(mShip->GetLaserCoolDown() / MAX_LASER_COOLDOWN);

    //Dados dos 5 asteroids mais próximos
    for (int i=0; i<5; i++) {
        states.emplace_back(GetWrappedDelta(mShip->GetPosition().x, mAsteroids[i]->GetPosition().x, static_cast<float>(mWindowWidth)) / static_cast<float>(mWindowWidth));
        states.emplace_back(GetWrappedDelta(mShip->GetPosition().y, mAsteroids[i]->GetPosition().y, static_cast<float>(mWindowHeight)) / static_cast<float>(mWindowHeight));
        states.emplace_back(mAsteroids[i]->GetComponent<CircleColliderComponent>()->GetRadius() / MAX_RADIUS);
        states.emplace_back(mAsteroids[i]->GetComponent<RigidBodyComponent>()->GetVelocity().x / MAX_ASTEROID_VELOCITY);
        states.emplace_back(mAsteroids[i]->GetComponent<RigidBodyComponent>()->GetVelocity().y / MAX_ASTEROID_VELOCITY);
    }
    return states;
}

float Game::CalculateReward() {
    if (mShip->GetIsDead()) {
        return -10.0f;
    }
    if (mAsteroidDestroyed)
}

std::tuple<std::vector<float>, float, bool, bool> Game::Step(int action) {
    ApplyAction(static_cast<Action>(action));

    ProcessInput();
    UpdateGame();
    if (mVisualize)
        GenerateOutput();

    mStepsDone++;

    bool terminated = mShip->GetIsDead() || mAsteroids.empty();
    bool truncated = mStepsDone >= MAX_STEPS;

    return std::make_tuple(GetObservationSpace(), CalculateReward(), terminated, truncated);
}
