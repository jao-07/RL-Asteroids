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

Game::Game(int windowWidth, int windowHeight, int difficulty)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mShip(nullptr)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mGameState(GameState::Playing)
        ,mDifficulty(difficulty)
        ,mDeathReward(-50.0f)
        ,mAsteroidDestroyedReward(10.0f)
        ,mAllAsteroidsDestroyedReward(50.0f)
{
    if (difficulty == 1) {
        mAsteroidsNumber = 2;
        mAllowSplitAsteroids = false;
        mTimeReward = 0.1f;
    }
    else if (difficulty == 2) {
        mAsteroidsNumber = 5;
        mAllowSplitAsteroids = true;
        mTimeReward = 0.0f;
        mLasersMissedReward = -0.1f;
    }
    else {
        mAsteroidsNumber = 10;
        mAllowSplitAsteroids = true;
        mTimeReward = 0.0f;
        mLasersMissedReward = -0.5f;
    }
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
    for (int i=0; i < mAsteroidsNumber; i++) {
        auto* ast = new Asteroid(this, AsteroidSize::Large);
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
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        if (mGameState == GameState::Playing)
            GenerateOutput();
        if (CalculateReward() != 0)
            SDL_Log("Reward: %f", CalculateReward());
    }
}

void Game::ProcessInput()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }
    mUpdatingActors = false;
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    // constexpr float deltaTime = 1.0f / 60.0f;

    mTicksCount = SDL_GetTicks();
    mAsteroidDestroyed = false;
    mLaserMissedInTheStep = false;

    UpdateActors(deltaTime);

    if (mGameState == GameState::Playing && (mAsteroids.empty() || mShip->GetIsDead())) {
        mGameState = GameState::Waiting;
        mPauseTime = SDL_GetTicks();
    }

    if (mGameState == GameState::Waiting) {
        if (SDL_GetTicks() - mPauseTime >= 1000) {
            Reset();
            mGameState = GameState::Playing;
        }
    }
}

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
    // SDL_Log("Asteroid Added - mAsteroids size: %d", mAsteroids.size());
}

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

        if (isLarge && mAllowSplitAsteroids) {
            for (int i=0; i<3; i++) {
                Vector2 offset = Random::GetVector(Vector2(-10.0f, -10.0f), Vector2(10.0f, 10.0f));
                auto *newSmallAst = new Asteroid(this, AsteroidSize::Small, pos+offset);
            }
        }
        mAsteroidDestroyed = true;
    }
    else
        SDL_Log("Attempting to remove asteroid not in list");
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

void Game::Reset() {
    DeleteActors();
    InitializeActors();
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

bool Game::CalculateDistanceAndDirectionToTheNearestAsteroid() {

}

float Game::CalculateReward() const {
    float reward = mTimeReward;

    if (mShip->GetIsDead())
        reward += mDeathReward;

    if (mAsteroidDestroyed)
        reward += mAsteroidDestroyedReward;

    if (mLaserMissedInTheStep)
        reward += mLasersMissedReward;

    if (mAsteroids.empty() && !mShip->GetIsDead())
        reward += mAllAsteroidsDestroyedReward;

    return reward;
}
