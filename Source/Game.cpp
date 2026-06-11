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

Game::Game(const bool visualize, int difficulty)
        :mVisualize(visualize)
        ,mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mWindowWidth(1024)
        ,mWindowHeight(768)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mShip(nullptr)
        ,mGameState(GameState::Playing)
        ,mAsteroidDestroyedReward(1.0f)
        ,mDeathReward(-10.0f)
        ,mAllAsteroidsDestroyedReward(10.0f)
{
        if (difficulty == 1) {
            mInitialAsteroidsNumber = 10;
            mAllowSplitAsteroids = false;
            mTimeReward = 0.0f;
            mLasersMissedReward = -0.05f;
            mProximityAndDirectionReward = 0.0f;
        }
        else if (difficulty == 2) {
            mInitialAsteroidsNumber = 1;
            mAllowSplitAsteroids = false;
            mTimeReward = -0.004f;
            mLasersMissedReward = -0.01f;
            mProximityAndDirectionReward = 0.05f;
        }
        else {
            mInitialAsteroidsNumber = 10;
            mAllowSplitAsteroids = true;
            mTimeReward = -0.05f;
            mLasersMissedReward = -0.5f;
            mProximityAndDirectionReward = 0.01f;
        }
}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    Uint32 windowFlags = mVisualize ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN;

    mWindow = SDL_CreateWindow("TP2: Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, windowFlags);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
    if (mVisualize) {
        renderFlags |= SDL_RENDERER_PRESENTVSYNC;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, renderFlags);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    //InitializeActors();

    return true;
}

void Game::CreateAsteroids() {
    for (int i=0; i < mInitialAsteroidsNumber; i++) {
        auto* ast = new Asteroid(this, AsteroidSize::Large);
    }
}

void Game::InitializeActors()
{
    mShip = new Ship(this, 20);
    mShip->SetPosition(Vector2(mWindowWidth / 2.0f, mWindowHeight / 2.0f));
    
    CreateAsteroids();

}

// void Game::RunLoop()
// {
//     GenerateOutput();
//     while (mIsRunning)
//     {
//         ProcessInput();
//         if (!mWaitingForAction) {
//             UpdateGame();
//             GenerateOutput();
//         }
//     }
// }

void Game::RunLoop() {
    Reset();
    while (mIsRunning) {
        auto [obs, reward, terminated, truncated, stats] = Step(1);
        auto [obs2, reward2, terminated2, truncated2, stats2] = Step(2);

        if (terminated or terminated2 or truncated or truncated2) {
            Reset();
        }

    }
}

void Game::ApplyAction(Action action) {
    mSelectedAction = action;
    mWaitingForAction = false;
    mFramesToProcess = 1;
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
    for (int i=mFramesToProcess; i>0; i--){
        for (auto actor : mActors){
            if (actor != nullptr)
                actor->ProcessInput(mSelectedAction);
         }

        constexpr float fixedDT = 16.0f / 1000.0f;
        UpdateActors(fixedDT);
        if (mVisualize) {
            GenerateOutput();

            static Uint32 tickAnterior = SDL_GetTicks();
            while (!SDL_TICKS_PASSED(SDL_GetTicks(), tickAnterior + 16));
            tickAnterior = SDL_GetTicks();
        }
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

    // if (!mCurrentAsteroidsNumber == 0)
    //     orderAsteroids();
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
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
    auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
    if (iter != mAsteroids.end()) {
        Vector2 pos = ast->GetPosition();
        bool isLarge = (ast->GetSize() == AsteroidSize::Large);

        ast->SetState(ActorState::Destroy);
        CreateParticles(ast, 600, 1000);

        // std::iter_swap(iter, mAsteroids.end() - 1);
        // mAsteroids.pop_back();
        *iter = nullptr;

        if (isLarge && mAllowSplitAsteroids) {
            for (int i=0; i<3; i++) {
                Vector2 offset = Random::GetVector(Vector2(-10.0f, -10.0f), Vector2(10.0f, 10.0f));
                auto *newSmallAst = new Asteroid(this, AsteroidSize::Small, pos+offset);
            }
        }
        mAsteroidDestroyed = true;
        mCurrentAsteroidsNumber--;
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

    if (a != nullptr && b != nullptr) {
        float dx = GetWrappedDelta(a->GetPosition().x, b->GetPosition().x, static_cast<float>(mWindowWidth));
        float dy = GetWrappedDelta(a->GetPosition().y, b->GetPosition().y, static_cast<float>(mWindowHeight));
        return dx*dx + dy*dy;
    }

    return 999999999.0f;
}

// void Game::orderAsteroids (std::vector<class Asteroid*>& asteroids) {
//     std::sort(asteroids.begin(), asteroids.end(), [this](const Asteroid* a, const Asteroid* b) {
//         const float distA = GetWrappedDistanceSq(mShip, a);
//         const float distB = GetWrappedDistanceSq(mShip, b);
//         return distA < distB;
//     });
// }

void Game::AddActor(Actor* actor) {
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

void Game::DeleteActors() {
    if (mActors.empty()) return;
    for (int i = mActors.size()-1; i >= 0; i--) {
        mActors[i]->SetState(ActorState::Destroy);
        mActors.pop_back();
    }
    mDrawables.clear();
    mAsteroids.clear();
}

std::vector<float> Game::Reset() {
    DeleteActors();
    mCurrentAsteroidsNumber = 0;
    InitializeActors();
    mStepsDone = 0;

    return GetObservationSpace();
}

/* 0: Pos x da nave
 * 1: Pos y da nave
 * 2: cos(angulo) da nave (Actor::GetForward)
 * 3: sen(angulo) da nave (Actor::GetForward)
 * 4: vel x da nave
 * 5: vel y da nave
 * 6: cooldown do tiro da nave
 * de 7 a 32 para os 15 asteroids mais proximos
 * 7: pos relativa x do asteroid
 * 8: pos relativa y do asteroid
 * 9: raio do asteroide
 * 10: vel x do asteroid
 * 11: vel y do asteroid
 */
std::vector<float> Game::GetObservationSpace() const {
    std::vector<float> states;
    states.reserve(85);
    //Dados da nave
    states.emplace_back(mShip->GetForward().x);
    states.emplace_back(mShip->GetForward().y);
    states.emplace_back(mShip->GetComponent<RigidBodyComponent>()->GetVelocity().x / MAX_SHIP_VELOCITY);
    states.emplace_back(mShip->GetComponent<RigidBodyComponent>()->GetVelocity().y / MAX_SHIP_VELOCITY);
    states.emplace_back(mShip->GetLaserCoolDown() / MAX_LASER_COOLDOWN);

    for (int i = 0; i < 10; i++) {
        if (i < mAsteroids.size() && mAsteroids[i] != nullptr) {
            float dx = GetWrappedDelta(mShip->GetPosition().x, mAsteroids[i]->GetPosition().x, static_cast<float>(mWindowWidth)) / (static_cast<float>(mWindowWidth) / 2.0f);
            float dy = GetWrappedDelta(mShip->GetPosition().y, mAsteroids[i]->GetPosition().y, static_cast<float>(mWindowHeight)) / (static_cast<float>(mWindowHeight) / 2.0f);
            float distance = std::sqrt(dx * dx + dy * dy);
            float dirX = 0.0f;
            float dirY = 0.0f;
            if (distance > 0.0001f){
                dirX = dx / distance;
                dirY = dy / distance;
            }
            states.emplace_back(dx);
            states.emplace_back(dy);
            states.emplace_back(distance / std::sqrt(2.0f));
            states.emplace_back(mShip->GetForward().x * dirX + mShip->GetForward().y * dirY);
            states.emplace_back(mShip->GetForward().x * dirY - mShip->GetForward().y * dirX);
            states.emplace_back(mAsteroids[i]->GetComponent<CircleColliderComponent>()->GetRadius() / MAX_RADIUS);
            states.emplace_back(mAsteroids[i]->GetComponent<RigidBodyComponent>()->GetVelocity().x / MAX_ASTEROID_VELOCITY);
            states.emplace_back(mAsteroids[i]->GetComponent<RigidBodyComponent>()->GetVelocity().y / MAX_ASTEROID_VELOCITY);
        }
        else {
            states.emplace_back(0.0f);
            states.emplace_back(0.0f);
            states.emplace_back(1.0f);
            states.emplace_back(0.0f);
            states.emplace_back(0.0f);
            states.emplace_back(0.0f);
            states.emplace_back(0.0f);
            states.emplace_back(0.0f);
        }
    }
    return states;
}

bool Game::CalculateDistanceAndDirectionToTheNearestAsteroid(float distanceLimit, float dotProductLimit) {
    if (mShip == nullptr) {
        return false;
    }

    Asteroid* nearestAsteroid = nullptr;
    float minDistanceSq = 999999999.0f;

    for (Asteroid* ast : mAsteroids) {
        if (ast != nullptr) {
            float distSq = GetWrappedDistanceSq(mShip, ast);
            if (distSq < minDistanceSq) {
                minDistanceSq = distSq;
                nearestAsteroid = ast;
            }
        }
    }

    if (nearestAsteroid == nullptr) {
        return false;
    }

    float dx = GetWrappedDelta(mShip->GetPosition().x, nearestAsteroid->GetPosition().x, static_cast<float>(mWindowWidth));
    float dy = GetWrappedDelta(mShip->GetPosition().y, nearestAsteroid->GetPosition().y, static_cast<float>(mWindowHeight));
    float distance = std::sqrt(dx*dx + dy*dy);
    if (distance <= distanceLimit) {
        Vector2 dirToAsteroid(dx, dy);
        if (distance > 0.0f) {
            dirToAsteroid.x /= distance;
            dirToAsteroid.y /= distance;
        }
        float dotProduct = (mShip->GetForward().x * dirToAsteroid.x) + (mShip->GetForward().y * dirToAsteroid.y);

        if (dotProduct > dotProductLimit) {
            return true;
        }
    }
    return false;
}

float Game::IsAimingAtAnyAsteroid(float distanceLimit) {

    float bestDot = 0.0f;
    for (Asteroid* ast : mAsteroids) {
        if (ast == nullptr)
            continue;

        float dx = GetWrappedDelta(mShip->GetPosition().x, ast->GetPosition().x, static_cast<float>(mWindowWidth));
        float dy = GetWrappedDelta(mShip->GetPosition().y, ast->GetPosition().y, static_cast<float>(mWindowHeight));

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance == 0.0f || distance > distanceLimit)
            continue;

        Vector2 dirToAsteroid(dx / distance, dy / distance);
        float dotProduct = (mShip->GetForward().x * dirToAsteroid.x) + (mShip->GetForward().y * dirToAsteroid.y);
        bestDot = std::max(bestDot, dotProduct);
    }

    return std::max(0.0f, bestDot*bestDot);
}

float Game::CalculateReward() {
    float reward = mTimeReward;

    if (mShip->GetIsDead())
        reward += mDeathReward;

    if (mAsteroidDestroyed)
        reward += mAsteroidDestroyedReward;

    if (mLaserMissedInTheStep)
        reward += mLasersMissedReward;

    if (mCurrentAsteroidsNumber == 0 && !mShip->GetIsDead())
        reward += mAllAsteroidsDestroyedReward;

    reward += IsAimingAtAnyAsteroid(500.0) * mProximityAndDirectionReward;
    // SDL_Log("Mira: %f", IsAimingAtAnyAsteroid(500.0));

    return reward;
}

std::tuple<std::vector<float>, float, bool, bool, std::tuple<bool, int, int, int, bool>> Game::Step(int action) {
    mAsteroidDestroyed = false;
    mLaserMissedInTheStep = false;
    ApplyAction(static_cast<Action>(action));

    ProcessInput();
    UpdateGame();

    mStepsDone++;

    bool terminated = mShip->GetIsDead() || mCurrentAsteroidsNumber == 0;
    bool truncated = mStepsDone >= MAX_STEPS;

    std::tuple stats{false, 0, 0, 0, false};
    if (terminated || truncated)
    {
        std::get<0>(stats) = true;
        std::get<1>(stats) = mLasersFired;
        std::get<2>(stats) = mLasersHit;
        std::get<3>(stats) = mStepsDone;
        std::get<4>(stats) = mCurrentAsteroidsNumber == 0;
    }
    std::vector<float> obs = GetObservationSpace();
    float reward = CalculateReward();
    std::tuple tuple = std::make_tuple(obs, reward, terminated, truncated, stats);

    return tuple;
}