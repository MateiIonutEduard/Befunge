#include "befunge.h"

void (*direction[4]) (cursor*) = { up, down, left, right };

int execute(stack* stack, cursor* ptr, CPU* is, int pos_row, int pos_col) {
    static state state = Off;
    static int repeat = 1;

    static int skip_num = 0;
    static int toggle_exec = 0;

    static int jumping_back = 0;
    char command = *(*((is->space) + pos_row) + pos_col);

    if (skip_num > 0) {
        skip_num--;
        return 0;
    }

    if (jumping_back) {
        reflect(ptr);
        jumping_back = 0;
    }

    if (toggle_exec) {
        if (command == ';') toggle_exec = 0;
        return 0;
    }

    if (state == Single) {
        push(stack, (int)command);
        state = Off;
        return 0;
    }

    if (state == Multi) {
        if (command == '\"') {
            state = Off;
            return 0;
        }

        push(stack, (int)command);
        return 0;
    }

    int a = 0;
    for (int cx = 0; cx < repeat; cx++) {
        switch (command) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            push(stack, NUM(command));
            break;
        case '+':
            a = pop(stack);
            a += pop(stack);
            push(stack, a);
            break;
        case '-':
            a = pop(stack);
            a = -a;
            a += pop(stack);
            push(stack, a);
            break;
        case '*':
            a = pop(stack);
            a *= pop(stack);
            push(stack, a);
            break;
        case '/':
            a = pop(stack);
            if (a == 0) return -1;
            a = pop(stack) / a;
            push(stack, a);
            break;
        case '%':
            a = pop(stack);
            if (a == 0) return -1;
            a = pop(stack) % a;
            push(stack, a);
            break;
        case '!':
            a = pop(stack);
            a = (a) ? 0 : 1;
            push(stack, a);
            break;
        case '`':
            a = pop(stack);
            a = (pop(stack) > a) ? 1 : 0;
            push(stack, a);
            break;
        case '\'':
            state = Single;
            break;
        case '\"':
            state = Multi;
            break;
        case '.':
            printf("%i", pop(stack));
            break;
        case ',':
            printf("%c", (char)pop(stack));
            break;
        case ':':
            a = pop(stack);
            push(stack, a);
            push(stack, a);
            break;
        case '\\':
        {
            int b = pop(stack);
            a = pop(stack);
            push(stack, b);
            push(stack, a);
        }
        break;
        case '$':
            pop(stack);
            break;
        case 'k':
            repeat = pop(stack);
            repeat += (repeat > 0) ? 1 : 0;
            return 0;
        case '^':
            up(ptr);
            break;
        case 'v':
            down(ptr);
            break;
        case '<':
            left(ptr);
            break;
        case '>':
            right(ptr);
            break;
        case '[':
            rel_left(ptr);
            break;
        case ']':
            rel_right(ptr);
            break;
        case 'r':
            reflect(ptr);
            break;
        case '?':
            a = rand() % 4;
            (*(direction + a)) (ptr);
            break;
        case '_':
            a = pop(stack);
            if (!a) right(ptr);
            else left(ptr);
            break;
        case '|':
            a = pop(stack);
            if (!a) down(ptr);
            else up(ptr);
            break;
        case '#':
            skip_num = 1;
            break;
        case ';':
            toggle_exec = 1;
            break;
        case 'j':
            skip_num = pop(stack);
            if (skip_num < 0) {
                reflect(ptr);
                skip_num = ABS(skip_num);
                skip_num -= 2;
            }
            break;
        case '&':
            scanf("%d", &a);
            push(stack, a);
            break;
        case '~':
            scanf("%c", (char*)&a);
            push(stack, (char)a);
            break;
        case 'g':
        {
            a = pop(stack);
            int x = pop(stack);
            if (x >= is->cols || a >= is->rows) push(stack, 0);
            else push(stack, (char)*(*((is->space) + a) + x));
        }
        break;
        case 'p':
        {
            int y = pop(stack);
            int x = pop(stack);
            a = pop(stack);

            if (y >= is->rows) {
                is->space = (char**)realloc(is->space, sizeof(char*) * (y + 1));

                for (int i = is->rows; i < (y + 1); i++)
                    *((is->space) + i) = (char*)calloc((y + 1), 1);

                is->rows = y + 1;
            }

            if (x >= is->cols) {

                for (int i = 0; i < is->rows; i++)
                    *((is->space) + i) = (char*)realloc(*((is->space) + i), (x + 1));

                is->cols = x + 1;
            }

            *(*((is->space) + y) + x) = (char)a;
        }
        break;
        case '@':
            return 1;
        default:
            break;
        }
    }

    repeat = 1;

    return 0;
}