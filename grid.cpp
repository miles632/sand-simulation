#include "grid.hpp"

#include <math.h>
#include <array>
#include <algorithm>
#include <iostream>

Grid::Grid() {
    this->grid = {0};

    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            grid[x][y] = particle_t::airDefault();
        }
    }
}

void Grid::updateGrid() {
    // for (size_t x = 0; x < cols; ++x) {
    //     for (size_t y = 0; y < rows; ++y) {

    for (size_t y = height-1; y > 0; --y) {
        for (size_t x = 0; x < width; ++x) {
            switch(grid[x][y].variant) {
                case Air:
                    break;

                case Sand:
                    updateSand(x, y);
                    break;

                case Water:
                    updateWater(x, y);
                    break;
                case Wood:
                    break;

                case Stone:
                    break;
            }
        }
    }
}

void Grid::renderGrid() const {
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            DrawRectangle(x * PIXEL_LEN, y * PIXEL_LEN, PIXEL_LEN, PIXEL_LEN, grid[x][y].color);
            // DrawRectangleLines(x * PIXEL_LEN, y * PIXEL_LEN, PIXEL_LEN, PIXEL_LEN, BLACK);
        }
    }
}

void Grid::updateSand(int x, int y) {
    particle_t* p = &grid[x][y];

    if (!p->is_freefalling && movable(x, y+1) && inbounds(x, y+1)) {
        p->is_freefalling = true;
    }

    if (p->is_freefalling) {
        p->velocity.y += 5 * GetFrameTime();

        int deltaVy = y + static_cast<int>(p->velocity.y);

        for (int iy = y + 1; iy <= deltaVy; iy++) {
            if (movable(x, iy) && inbounds(x, iy)) {
                swapWith(x, iy-1, x, iy);
            } else {
                p = &grid[x][iy];

                // ground reached, done frefalling
                p->is_freefalling = false;

                // convert vertical into horizontal
                if (rand() % 2 == 0) {
                    // convertVerticalV(p->velocity);
                    p->velocity.x = p->velocity.y / 2;
                } else {
                    p->velocity.x = -(p->velocity.y / 2);
                }

                break;
            }
        }
    }

    if (p->velocity.x == 0 || static_cast<float>(p->velocity.x) < 1) {
        return;
    } else {
        int xModifier = p->velocity.x < 0 ? -1 : 1;
        int deltaVx = x + static_cast<int>(p->velocity.x);

        float newVx = p->velocity.x;

        // may god have mercy on me
        for (int ix = (xModifier == 1 ? x + 1 : -x - 1);
            xModifier == 1 ? ix <= deltaVx : ix <= x-1;
            xModifier == 1 ? ix++ : ix--)
        {
            if (movable(ix, y) && inbounds(ix, y)) {
                swapWith(xModifier == 1 ? ix-1 : ix+1, y, ix, y);
            if (!movable(ix, y+1)) {
                // newVx /= FRICTION_CONST;
                p->velocity.x /= 3.5;
            }
            } else {
                p->velocity.x = 0.0;
                break;
            }
        }
    }

    // if (p->velocity.x < 0) {
    //     traverseMatrixSlope()
    // }
}

void Grid::updateWater(int x, int y) {
    // check down
    if (y < height - 1 && grid[x][y+1].state == State::Gas) {
        swapWith(x, y, x, y+1);
        return;
    }


    // check right and left
     if (rand() % 2 == 0) {
         for (int i = 1; i <= WATER_DISPERSE_RATE; i++) {
             if (x-i >= 0 && grid[x-i][y].state == State::Gas) {
                 swapWith((x - i) + 1, y, x - i, y);
             } else {
                 return;
             }
         }
     } else {
         for (int i = 1; i <= WATER_DISPERSE_RATE; i++) {
             if (x + i < width -1 && grid[x+i][y].state == State::Gas) {
                 swapWith((x + i) - 1, y, x + i, y);
             } else {
                 return;
             }
         }
     }

    // downleft and downright
    if (rand() % 2 == 0) {
        //downleft
        if (x > 0 && y < height - 1 && grid[x-1][y+1].state == State::Gas) {
            swapWith(x, y, x-1, y+1);
            return;
        }
    } else {
        // downright
        if (x < width - 1 && y < height - 1 && grid[x+1][y+1].state == State::Gas) {
            swapWith(x, y, x+1, y+1);
            return;
        }
    }
}

void Grid::mouseInput(ParticleVariant& selectedP) {
    Vector2 mousePos = GetMousePosition();
    bool mouseHeld = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (IsKeyPressed(KEY_ONE)) selectedP = ParticleVariant::Sand;
    if (IsKeyPressed(KEY_TWO)) selectedP = ParticleVariant::Water;
    if (IsKeyPressed(KEY_THREE)) selectedP = ParticleVariant::Wood;
    if (IsKeyPressed(KEY_FOUR)) selectedP = ParticleVariant::Stone;

    if (mouseHeld || mousePressed) {
        int x = ((int)mousePos.x)/PIXEL_LEN;
        int y = ((int)mousePos.y)/PIXEL_LEN;


        for (int i = -CURSOR_RADIUS; i <= CURSOR_RADIUS; i++) {
            for (int j = -CURSOR_RADIUS; j <= CURSOR_RADIUS; j++) {

                int xNew =  x + (rand() % (2*CURSOR_RADIUS+1) - CURSOR_RADIUS);
                int yNew =  y + (rand() % (2*CURSOR_RADIUS+1) - CURSOR_RADIUS);

                if (xNew < 0 || xNew  > width || yNew < 0 || yNew > height)
                    continue;

                switch (selectedP) {
                    case ParticleVariant::Water:
                        if (grid[xNew][yNew].state == State::Gas) {
                            grid[xNew][yNew] = particle_t::waterDefault();
                        }
                    break;

                    case ParticleVariant::Sand:
                        if (grid[xNew][yNew].state == State::Gas) {
                            grid[xNew][yNew] = particle_t::sandDefault();
                        }
                        break;

                    case ParticleVariant::Stone:
                        if (grid[xNew][yNew].state == State::Gas) {
                            grid[xNew][yNew] = particle_t::stoneDefault();
                        }
                        break;

                    default:
                        break;
                }
            }
        }

        if (x >= 0 && x < width && y >= 0 && y < height) {
            switch (selectedP) {
                case ParticleVariant::Water:
                    grid[x][y] = particle_t::waterDefault();
                break;
                case ParticleVariant::Sand:
                    grid[x][y] = particle_t::sandDefault();
                break;
                default:
                    break;
            }
        }
    }
}

void Grid::swapWith(int x, int y, int x2, int y2) {
    particle_t particle_tbs = grid[x2][y2];
    grid[x2][y2] = grid[x][y];
    grid[x][y] = particle_tbs;
}

bool Grid::movable(int x, int y) const {
    if (grid[x][y].state == State::Gas || grid[x][y].state == State::Liquid) return true;

    return false;
}

bool Grid::inbounds(int x, int y) const {
    if (x < 0 || x >= width - 1 || y < 0 || y >= height - 1) return false;

    return true;
}

void Grid::convertVerticalV(Vector2& p) {
    float yAbs = std::max(abs(p.y) / 31, static_cast<float>(105));
    p.x = p.x < 0 ? -yAbs: +yAbs;
}


// //traverse shortest path and apply the given lambda
// void Grid::traverseMatrixSlope(int x, int y, int x2, int y2, std::function<void(int xLambda, int yLambda, int x1, int y1)> &fn) {
//
//     if (x == x2 && y == y2) return;
//
//     int xDelta = x - x2;
//     int yDelta = y - y2;
//
//     int xModifier = xDelta < 0 ? 1 : -1;
//     int yModifier = yDelta < 0 ? 1 : -1;
//
//     bool xDeltaLarger = abs(xDelta) > abs(yDelta);
//
//     int longerSide = std::max(abs(xDelta), abs(yDelta));
//     int shorterSide = std::min(abs(xDelta), abs(yDelta));
//
//     const auto slope = (shorterSide == 0 || longerSide == 0) ? 0 : static_cast<float>(shorterSide / longerSide);
//
//     int sside; // shorter side to be computed per iteration
//     int xPrev = x;
//     int yPrev = y;
//     for (int i = 1; i<= longerSide; i++) {
//         sside = round(i * slope);
//
//         int xIncrease, yIncrease;
//
//         if (xDeltaLarger) {
//             xIncrease = i;
//             yIncrease = sside;
//         } else {
//             yIncrease = i;
//             xIncrease = sside;
//         }
//
//         int xCurrent = x + (xIncrease * xModifier);
//         int yCurrent = y + (yIncrease * yModifier);
//
//         if(inbounds(xCurrent, yCurrent) && movable(xCurrent, yCurrent)) {
//             fn(xCurrent, yCurrent, xPrev, yPrev);
//
//             xPrev = xCurrent;
//             yPrev = yCurrent;
//         }
//     }
// }

void Grid::traverseMatrixSlope(int x, int y, int x2, int y2, std::function<void(int xLambda, int yLambda, int x1, int y1)> &fn) {
    // If the two points are the same no need to iterate. Just run the provided function
    // if (pos1.epsilonEquals(pos2)) {
    //     function();
    //     return;
    // }

    if (x == x2 && y == y2) {
        // function();
        return;
    }

    int xDiff = x - x2;
    int yDiff = y - y2;
    bool xDiffIsLarger = std::abs(xDiff) > std::abs(yDiff);

    int xModifier = xDiff < 0 ? 1 : -1;
    int yModifier = yDiff < 0 ? 1 : -1;

    int longerSideLength = std::max(std::abs(xDiff), std::abs(yDiff));
    int shorterSideLength = std::min(std::abs(xDiff), std::abs(yDiff));
    float slope = (shorterSideLength == 0 || longerSideLength == 0) ? 0 : (static_cast<float>(shorterSideLength) / longerSideLength);

    int shorterSideIncrease;
    int prevX = x;
    int prevY = y;

    for (int i = 1; i <= longerSideLength; ++i) {
        shorterSideIncrease = std::round(i * slope);
        int yIncrease, xIncrease;
        if (xDiffIsLarger) {
            xIncrease = i;
            yIncrease = shorterSideIncrease;
        } else {
            yIncrease = i;
            xIncrease = shorterSideIncrease;
        }
        int currentY = y + (yIncrease * yModifier);
        int currentX = x + (xIncrease * xModifier);
        if (inbounds(currentX, currentY)) {
            this->swapWith(currentX, currentY, prevX, prevY);

            prevX = currentX;
            prevY = currentY;
        }
    }
}

