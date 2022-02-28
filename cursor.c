#include "befunge.h"

void up(cursor* ptr) {
    ptr->dx = 0;
    ptr->dy = -1;
}

void down(cursor* ptr) {
    ptr->dx = 0;
    ptr->dy = 1;
}

void left(cursor* ptr) {
    ptr->dx = -1;
    ptr->dy = 0;
}

void right(cursor* ptr) {
    ptr->dx = 1;
    ptr->dy = 0;
}

void rel_left(cursor* ptr) {
    ptr->dx ^= ptr->dy;
    ptr->dy ^= ptr->dx;
    ptr->dx ^= ptr->dy;
    ptr->dy = -(ptr->dy);
}

void rel_right(cursor* ptr) {
    ptr->dx ^= ptr->dy;
    ptr->dy ^= ptr->dx;
    ptr->dx ^= ptr->dy;
    ptr->dx = -(ptr->dx);
}

void reflect(cursor* ptr) {
    ptr->dx = -(ptr->dx);
    ptr->dy = -(ptr->dy);
}