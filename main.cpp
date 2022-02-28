#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "befunge.h"

int main(int argc, char** argv) {
    char* filename;
    FILE* in;

    CPU* cpu;
    char val;

    stack* stack;
    cursor ptr;

    srand((unsigned)time(0));
    int pos_row, pos_col;

    if (argc < 2) {
        printf("Too few arguments.");
        exit(-1);
    }
    else {
        filename = (char*)malloc(strlen(*(argv + 1)));
        filename = *(argv + 1);
    }

    in = fopen(filename, "r");

    if (!in) {
        printf("File not found.");
        free(filename);
        exit(-1);
    }


    cpu = (CPU*)malloc(sizeof(CPU));
    cpu->rows = 1;
    cpu->cols = 1;

    cpu->space = (char**)malloc(sizeof(char*));
    *((cpu->space) + 0) = (char*)calloc(cpu->cols, 1);

    pos_row = 0;
    pos_col = 0;
    val = 0;

    while ((val = fgetc(in)) != EOF) {
        if (pos_col == cpu->cols) {
            cpu->cols++;

            for (int row = 0; row < cpu->rows; row++) {
                *((cpu->space) + row) = (char*)realloc(*((cpu->space) + row), cpu->cols);
                *(*((cpu->space) + row) + pos_col) = 0x00;
            }
        }

        *(*((cpu->space) + pos_row) + pos_col) = val;
        pos_col++;

        if (val == '\n') {
            cpu->rows++;
            cpu->space = (char**)realloc(cpu->space, sizeof(char*) * cpu->rows);
            *((cpu->space) + (cpu->rows - 1)) = (char*)calloc(cpu->cols, 1);
            pos_row++;
            pos_col = 0;
        }
    }

    for (int r = 0; r < cpu->rows; r++) {
        for (int c = 0; c < cpu->cols; c++)
            PRINT(*(*((cpu->space) + r) + c));
    }

    // alloc stack
    stack = alloc();

    // set up cursor
    ptr.dx = 1;
    ptr.dy = 0;
    pos_row = 0;
    pos_col = 0;

    while (!(execute(stack, &ptr, cpu, pos_row, pos_col))) {
        pos_col += ptr.dx;
        pos_row += ptr.dy;

        if (pos_col < 0) pos_col = cpu->cols - 1;
        if (pos_col == cpu->cols) pos_col = 0;

        if (pos_row < 0) pos_row = cpu->rows - 1;
        if (pos_row == cpu->rows) pos_row = 0;
    }

    fclose(in);

    free(filename);
    for (int cx = 0; cx < cpu->rows; cx++) free(*((cpu->space) + cx));
    free(cpu->space);
    free(cpu);

    destroy(stack);
    free(stack);

    return 0;
}