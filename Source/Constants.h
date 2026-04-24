//
// Created by joaovecruz on 16/04/2026.
//

#pragma once

enum class GameState
{
    Playing,
    Waiting
};

enum class Action {
    Right,
    Left,
    Shoot,
    Forward,
    Nothing
};

enum class ActorState
{
    Active,
    Paused,
    Destroy
};
