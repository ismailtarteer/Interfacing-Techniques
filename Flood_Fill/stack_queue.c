#include "floodfill.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue *createQueue(unsigned capacity)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int *)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isQueueFull(Queue *queue)
{
    return (queue->size == queue->capacity);
}

int isQueueEmpty(Queue *queue)
{
    return (queue->size == 0);
}

void enqueue(Queue *queue, int item)
{
    if (isQueueFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(Queue *queue)
{
    if (isQueueEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(Queue *queue)
{
    if (isQueueEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

int rear(Queue *queue)
{
    if (isQueueEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

Stack *createStack(unsigned capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
    return stack;
}

int isStackFull(Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

int isStackEmpty(Stack *stack)
{
    return stack->top == -1;
}

void push(Stack *stack, int item)
{
    if (isStackFull(stack))
        return;
    stack->array[++stack->top] = item;
}

int pop(Stack *stack)
{
    if (isStackEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

int peek(Stack *stack)
{
    if (isStackEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}