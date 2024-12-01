#ifndef PROJECT_FLOODFILL_H
#define PROJECT_FLOODFILL_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
// every bit in represent information
#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 16

#define FRONT 32
#define BACK 64
#define RIGHT 128
#define LEFT 256

void move(short *Direction, short x, short y, short toX, short toY);
void updateCoordinates(short Direction, short Wall, short x, short y, short *toX, short *toY);
void floodFill(short x, short y, short *Direction, int visited[][17], short data[]);
short getDirection(short data);
short BFS(short source, short targets[4], short Direction, short data[]);
void doIt();
int checkWall(int wall);

typedef struct
{
    int front, rear, size;
    unsigned capacity;
    int *array;
} Queue;

typedef struct
{
    int top;
    unsigned capacity;
    int *array;
} Stack;

Queue *createQueue(unsigned capacity);
int isQueueFull(Queue *queue);
int isQueueEmpty(Queue *queue);
void enqueue(Queue *queue, int item);
int dequeue(Queue *queue);
void getAvailable(short data, short Direction, short x, short y, int visited[], short parent[], Queue *queue);
int front(Queue *queue);

Stack *createStack(unsigned capacity);
int isStackFull(Stack *stack);
int isStackEmpty(Stack *stack);
void push(Stack *stack, int item);
int pop(Stack *stack);

#endif // PROJECT_FLOODFILL_H
