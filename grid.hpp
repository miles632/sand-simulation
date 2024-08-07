#pragma once

#include <array>
#include <functional>
#include "cell.hpp"


const int SCREEN_W = 1280; 
const int SCREEN_H = 720;

const int PIXEL_LEN = 5;

//array height and width
constexpr int height = SCREEN_H / PIXEL_LEN;
constexpr int width = SCREEN_W / PIXEL_LEN;

typedef std::array<std::array<particle_t, height>, width> gridMatrix;

struct Grid {
    gridMatrix grid;

    Grid();
    static constexpr float FRICTION_CONST = 1.3;
    static constexpr float GRAVITY_CONST = 1.5; // per second
    static constexpr int CURSOR_RADIUS = 10;

    void mouseInput(ParticleVariant& selectedP);
    void updateGrid();
    void renderGrid() const;

    constexpr void clearGrid();

    bool inbounds(int x, int y) const;
    bool movable(int x, int y) const;
    void swapWith(int x, int y, int x2, int y2);

    std::function<void(int ,int, int, int)> FnSwapP = [this] (int xLambda, int yLambda, int x1, int y1) {
        swapWith(x1, y1, xLambda, yLambda);
    };

    void convertVerticalV(Vector2& p);
    void traverseMatrixSlope(int x, int y, int x2, int y2, std::function<void(int xLambda, int yLambda, int x1, int y1)> &fn);

    void updateSand(int x, int y);
    void updateWater(int x, int y);


};

inline bool inbounds(int x, int y) {
    if (x < 0 || x >= width - 1 || y < 0 || y >= height - 1) {
        return false;
    } else {
        return true;
    }

}
