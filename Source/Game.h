// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include "Actors/Ship.h"
#include "Actors/Asteroid.h"
#include <typeinfo>

enum class GameState
{
    Playing,
    Waiting
};

class Game
{
public:
    Game(int windowWidth, int windowHeight, int difficulty=1);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    void CreateAsteroids();
    void CreateParticles(Asteroid *ast, float min, float max);

    // Actor functions
    void InitializeActors();
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Draw functions
    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    // Game-specific (add/remove asteroid)
    void AddAsteroid(class Asteroid* ast);
    void RemoveAsteroid(class Asteroid* ast);
    std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

    void SetLasersMissed(bool state) { mLaserMissedInTheStep = state; }

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void DeleteActors();
    void Reset();
    static float GetWrappedDelta(float p1, float p2, float limit);
    float GetWrappedDistanceSq(const Actor* a, const Actor* b) const;
    void orderAsteroids();
    bool CalculateDistanceAndDirectionToTheNearestAsteroid(float distanceLimit, float dotProductLimit);
    float CalculateReward();

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    // Game-specific
    class Ship* mShip;
    std::vector<class Asteroid*> mAsteroids;
    int mCurrentAsteroidsNumber = 0;
    int mDifficulty;

    //Track the pauses between the asteroids waves
    GameState mGameState;
    Uint32 mPauseTime = 0;

    bool mAllowSplitAsteroids;
    int mInitialAsteroidsNumber;
    bool mAsteroidDestroyed = false;
    bool mLaserMissedInTheStep = false;

    float mTimeReward;
    float mDeathReward;
    float mAsteroidDestroyedReward;
    float mLasersMissedReward;
    float mAllAsteroidsDestroyedReward;
    float mProximityAndDirectionReward;

};
