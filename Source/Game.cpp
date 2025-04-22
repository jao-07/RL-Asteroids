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

Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mShip(nullptr)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
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

    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1. (1 linha): Uma biblioteca Random.h foi incluída nesse projeto para a geração de números aleatórios.
    //  Utilize a função Random::Init para inicializar o gerador de números aleatórios (~1 linha).
    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    return true;
}

void Game::InitializeActors()
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 2.1 (2 linhas): Crie um objeto da classe Ship com 20 pixels de altura e o armazene no ponteiro mShip.
    //  Em seguida, posicione a nave (mShip->SetPosition) no meio da tela. Lembre-se que as variáveis mWindowWidth e
    //  mWindowHeight armazenam as dimensões da tela.
    mShip = new Ship(this, 20);
    mShip->SetPosition(Vector2(mWindowWidth / 2.0f, mWindowHeight / 2.0f));

    // TODO 2.2 (~4 linhas): Escreva um laço para instanciar 10 objetos da classe Asteroid, cada um com 80 pixels de raio.
    for (int i=0; i < 10; i++) {
        Asteroid* ast = new Asteroid(this, 80);
        //AddAsteroid(ast);
    }

}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
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

    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    // Update all actors and pending actors
    UpdateActors(deltaTime);
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
        delete actor;
    }
}

void Game::AddAsteroid(Asteroid* ast)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 3.1 (1 linhas): Adicione (emplace_back) o asteroide (ast) ao vetor de asteroids (mAsteroids).
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
    // TODO 3.1 (~4 linhas): Utilize a função std::find para procurar pelo asteroide ast no vetor de
    //  asteroides mAsteroids. Se o encontrou, remova-o do vetor de asteroids (mAsteroids.erase()).
    auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
    if (iter != mAsteroids.end()) {
        mAsteroids.erase(iter);
    }
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
