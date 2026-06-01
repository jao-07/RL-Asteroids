// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "Random.h"

// int main(int argc, char** argv)
// {
//     Game game = Game(true);
//     bool success = game.Initialize();
//     if (success)
//     {
//         game.RunLoop();
//     }
//     game.Shutdown();
//     return 0;
// }

int main(int argc, char** argv)
{
    auto game = Game(true, 1);

    if (game.Initialize())
    {
        int episodeCount = 0;
        float totalReward = 0.0f;

        game.Reset();
        while (game.IsRunning())
        {
            // int acaoSimulada = Random::GetIntRange(0,5);
            int acaoSimulada = 3;
            auto [obs, reward, terminated, truncated] = game.Step(acaoSimulada);
            totalReward += reward;

            if (terminated || truncated)
            {
                SDL_Log("Episódio %d terminou! Recompensa Total: %f", episodeCount, totalReward);
                game.Reset();

                episodeCount++;
                totalReward = 0.0f;
            }
        }
    }

    game.Shutdown();
    return 0;
}
