/*
 * File: move.cpp
 * Author: Alessandra Gorla
 * Date: November 22, 2023
 * Description: Move module. It offers the functionality to let the player move.
 */

#include "move.h"

Coordinate add(Coordinate a, Coordinate b) {
    Coordinate c = { 0, 0 };
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

Coordinate mul(float s, Coordinate a) {
    Coordinate b = { 0, 0 };
    b.x = s * a.x;
    b.y = s * a.y;
    return b;
}

Coordinate move(Coordinate position, Coordinate velocity, float delta) {
    return add(position, mul(delta, velocity));
}