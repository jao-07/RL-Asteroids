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
            // # Ações Discretas(0: Dir, 1: Esq, 2: Tiro, 3: Frente, 4: Nada)
            int acaoSimulada = 2;
            auto [obs, reward, terminated, truncated, stats] = game.Step(acaoSimulada);
            totalReward += reward;

            if (terminated || truncated)
            {
                SDL_Log("Episódio %d terminou! Recompensa Total: %f", episodeCount, totalReward);
                SDL_Log("Stats: ShotsFired: %d\nshotsHit: %d\nsurvivalTime: %d\nvictory: %d", std::get<1>(stats), std::get<2>(stats), std::get<3>(stats), std::get<4>(stats));
                game.Reset();

                episodeCount++;
                totalReward = 0.0f;
            }
        }
    }

    game.Shutdown();
    return 0;
}
