#pragma once

#define DEBUG false

#define LIMITELEMENTSCOLLISION 5U

#define GRAVITY 9.81f

enum Direction
{
UP = 0,
DOWN,
LEFT,
RIGHT,
FRONT,
BOTTOM,
UNDEFINED
};

enum CharacterState
{
IDLE = 0,
WALKING,
RUNNING,
JUMPING,
FALLING,
SLIDING
};

enum CharacterHealthState
{
SAFE,
RECOVERING,
DEAD
};

