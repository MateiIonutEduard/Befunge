#include <stdio.h>

#define PRINT(X) (X) = (((X) < 0x20 || (X) > 0x7E) ? 0x20 : (X))
#define NUM(X) (((X) <= 0x39) ? (X) - 0x30 : 10 + (X) - 0x61)
#define ABS(X) (((X) < 0) ? (-(X)) : (X))

typedef struct node
{
	int val;
	struct node* next;
}node;

typedef struct
{
	node* top;
	node* bottom;
}stack;

typedef struct
{
	int dx;
	int dy;
}cursor;

typedef struct {
	int rows;
	int cols;
	char** space;
} CPU;

typedef enum {
	Off, 
	Single, 
	Multi
}state;

#ifdef __cplusplus
extern "C" {
#endif
	stack* alloc();
	void push(stack* stack, int val);
	int pop(stack* stack);
	void destroy(stack* stack);
	void up(cursor* ptr);
	void down(cursor* ptr);
	void left(cursor* ptr);
	void right(cursor* ptr);
	void rel_left(cursor* ptr);
	void rel_right(cursor* ptr);
	void reflect(cursor* ptr);
	int execute(stack* stack, cursor* ptr, CPU* is, int pos_row, int pos_col);
#ifdef __cplusplus
}
#endif
