#pragma once

#include <variant>

#include "raylib.h"
#include <raymath.h>

#define ID_AIR 0
#define ID_WATER 1
#define ID_SAND 2
#define ID_STONE 3

#define AIR_COLOR GRAY
#define SAND_COLOR YELLOW
#define WATER_COLOR BLUE
#define WOOD_COLOR BROWN
#define STONE_COLOR DARKGRAY

constexpr int WATER_DISPERSE_RATE = 10;

enum class State {
    Liquid,
    SolidImmovable,
    SolidMovable,
    Gas,
};

enum ParticleVariant {
    Air = 0x00,
    Sand,
    Water,
    Wood,
    Stone,
};

struct particle_t {
    float lifetime;
    Vector2 velocity;    
    Color color; 
    bool is_freefalling;

    State state;
    ParticleVariant variant;

    static particle_t airDefault() {
        return particle_t{0.0f, Vector2One(), AIR_COLOR, true, State::Gas, ParticleVariant::Air};
    }
    static particle_t sandDefault() {
        return particle_t{0.0f, Vector2One(), SAND_COLOR, true, State::SolidMovable, ParticleVariant::Sand};
    }
    static particle_t waterDefault() {
        return particle_t{0.0f, Vector2One(), WATER_COLOR, true, State::Liquid, ParticleVariant::Water};
    }
    static particle_t stoneDefault() {
        return particle_t{0.0f, Vector2One(), STONE_COLOR, true, State::SolidImmovable, ParticleVariant::Stone};
    }
};