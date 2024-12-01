#include "API.h"
#include "floodfill.h"

#define MAX_SIZE 17
#define TOTAL_CELLS 257

char color = 'B';
int numVisited = 0;

// function to move the mouse from one cell to another
void move(short *Direction, short y, short x, short toY, short toX)
{
    API_setColor(x - 1, 16 - y, color); // set the color of the current cell
    if (toY == y)
    { // same row
        if (x > toX)
        { // go to west
            // turn the mouse according to the current facing direction
            if (*Direction == NORTH)
            {
                API_turnLeft();
            }
            else if (*Direction == SOUTH)
            {
                API_turnRight();
            }
            else if (*Direction == EAST)
            {
                API_turnRight();
                API_turnRight();
            }
            *Direction = WEST;
        }
        else
        { // go to east
            // turn the mouse according to the current facing direction
            if (*Direction == NORTH)
            {
                API_turnRight();
            }
            else if (*Direction == SOUTH)
            {
                API_turnLeft();
            }
            else if (*Direction == WEST)
            {
                API_turnRight();
                API_turnRight();
            }
            *Direction = EAST;
        }
    }

    else if (y > toY)
    { // go to north
        if (*Direction == SOUTH)
        {
            API_turnRight();
            API_turnRight();
        }
        else if (*Direction == WEST)
        {
            API_turnRight();
        }
        else if (*Direction == EAST)
        {
            API_turnLeft();
        }
        *Direction = NORTH;
    }

    else
    { // go to south
        if (*Direction == NORTH)
        {
            API_turnRight();
            API_turnRight();
        }
        else if (*Direction == WEST)
        {
            API_turnLeft();
        }
        else if (*Direction == EAST)
        {
            API_turnRight();
        }
        *Direction = SOUTH;
    }
    API_moveForward();
}

// Update the coordinates of the cell we want to move to (next cell)
void updateCoordinates(short Direction, short Wall, short y, short x, short *toY, short *toX)
{
    *toY = y;
    *toX = x;

    // check the direction and the wall to update the coordinates
    switch (Wall)
    {
    case 1: // Front wall is open
        // check the current facing direction
        if (Direction == NORTH)
            *toY -= 1;
        else if (Direction == SOUTH)
            *toY += 1;
        else if (Direction == WEST)
            *toX -= 1;
        else if (Direction == EAST)
            *toX += 1;
        break;

    case 2: // Right wall is open
        // check the current facing direction
        if (Direction == NORTH)
            *toX += 1;
        else if (Direction == SOUTH)
            *toX -= 1;
        else if (Direction == WEST)
            *toY -= 1;
        else if (Direction == EAST)
            *toY += 1;
        break;

    case 3: // Left wall is open
        // check the current facing direction
        if (Direction == NORTH)
            *toX -= 1;
        else if (Direction == SOUTH)
            *toX += 1;
        else if (Direction == WEST)
            *toY += 1;
        else if (Direction == EAST)
            *toY -= 1;
        break;

    default:
        break;
    }
}

// Function to check the corresponding wall
int checkWall(int wall)
{
    switch (wall)
    {
    case 1: // check front wall
        return API_wallFront();
        break;
    case 2: // check right wall
        return API_wallRight();
        break;
    case 3: // check left wall
        return API_wallLeft();
        break;
    default:
        break;
    }
    return 1;
}

// Floodfill algorithm to explore the maze
void floodFill(short y, short x, short *Direction, int visited[][17], short data[])
{
    visited[y][x] = 1; // Mark the current cell as visited

    numVisited++;

    if (numVisited == 256)
    {
        color = 'y';
    }

    // find the node number of the current cell
    short node = (short)((y - 1) * 16 + x);

    // Update data for the current cell (add the surrounding walls and the direction we came from)
    data[node] = (short)(data[node] | *Direction) | BACK;
    data[node] |= (!API_wallFront()) ? FRONT : 0;
    data[node] |= (!API_wallRight()) ? RIGHT : 0;
    data[node] |= (!API_wallLeft()) ? LEFT : 0;

    short toY, toX;

    // Check all the surrounding cells
    for (int i = 0; i < 9; i++)
    {
        if (!checkWall(i % 3 + 1))
        { // Function to check corresponding wall
            updateCoordinates(*Direction, i % 3 + 1, y, x, &toY, &toX);
            if (!visited[toY][toX])
            {
                move(Direction, y, x, toY, toX);
                floodFill(toY, toX, Direction, visited, data);
                move(Direction, toY, toX, y, x); // Move back to previous position
            }
        }
    }
}

// get the direction of state that the mouse entered the cell with
short getDirection(short data)
{
    switch (data & 0x1F)
    {       // 0x1F = 0001 1111
    case 1: // 0000 0001 (was entered from north)
        return NORTH;
    case 2: // 0000 0010 (was entered from south)
        return SOUTH;
    case 4: // 0000 0100 (was entered from east)
        return EAST;
    case 16: // 0001 0000 (was entered from west)
        return WEST;
    default:
        break;
    }
}

void enqueueIfNotVisited(short toY, short toX, int visited[], short from, short parent[], Queue *queue)
{
    short to = (toY - 1) * 16 + toX;
    if (!visited[to])
    {
        enqueue(queue, to);
        parent[to] = from;
        visited[to] = 1;
    }
}

// get the available cells to move to (get the non visited cells)
void getAvailable(short data, short Direction, short y, short x, int visited[], short parent[], Queue *queue)
{
    short from = (y - 1) * 16 + x;

    if (data & FRONT)
    {
        short toY = (Direction == NORTH) ? y - 1 : (Direction == SOUTH) ? y + 1
                                                                        : y;
        short toX = (Direction == WEST) ? x - 1 : (Direction == EAST) ? x + 1
                                                                      : x;
        enqueueIfNotVisited(toY, toX, visited, from, parent, queue);
    }

    if (data & BACK)
    {
        short toY = (Direction == NORTH) ? y + 1 : (Direction == SOUTH) ? y - 1
                                                                        : y;
        short toX = (Direction == WEST) ? x + 1 : (Direction == EAST) ? x - 1
                                                                      : x;
        enqueueIfNotVisited(toY, toX, visited, from, parent, queue);
    }

    if (data & RIGHT)
    {
        short toY = (Direction == WEST) ? y - 1 : (Direction == EAST) ? y + 1
                                                                      : y;
        short toX = (Direction == NORTH) ? x + 1 : (Direction == SOUTH) ? x - 1
                                                                        : x;
        enqueueIfNotVisited(toY, toX, visited, from, parent, queue);
    }

    if (data & LEFT)
    {
        short toY = (Direction == WEST) ? y + 1 : (Direction == EAST) ? y - 1
                                                                      : y;
        short toX = (Direction == NORTH) ? x - 1 : (Direction == SOUTH) ? x + 1
                                                                        : x;
        enqueueIfNotVisited(toY, toX, visited, from, parent, queue);
    }
}

// Breadth First Search algorithm to find the shortest path from the start to the goal
short BFS(short source, short targets[4], short Direction, short data[])
{
    Queue *q = createQueue(257);
    int visited[257] = {0};
    short parent[257]; // to store the parent of each node
    memset(parent, -1, sizeof(parent));

    enqueue(q, source);
    visited[source] = 1;
    short target = -1;
    while (!isQueueEmpty(q))
    { // while not reached the target
        short p = (short)dequeue(q);
        short a = (short)((p + 15) / 16);    // y coordinate (row)
        short b = (short)((p - 1) % 16 + 1); // x coordinate (column)

        if (p == targets[0] || p == targets[1] || p == targets[2] || p == targets[3])
        {
            target = p;
            API_setText(b - 1, 16 - a, "GOAL");
            API_setColor(b - 1, 16 - a, 'r');
            break; // Early exit if target is reached
        }
        getAvailable(data[p], getDirection(data[p]), a, b, visited, parent, q);
    }

    // Backtrack from the target to the source to find the path
    Stack *stack = createStack(257);
    for (short current = target; current != -1; current = parent[current])
    {
        push(stack, current);
    }

    short y = 16, x = 1;
    short size_of_stack = stack->top + 1;
    char str[5];
    while (!isStackEmpty(stack))
    {
        short node = (short)pop(stack);
        short a = (short)((node + 15) / 16);
        short b = (short)((node - 1) % 16 + 1);

        if (node != source)
        { // Avoid moving from the starting position
            move(&Direction, y, x, a, b);
            // set the text of the current cell to the node number
            sprintf(str, "%d", stack->top + 2); // convert the number of steps to string
            API_setText(x - 1, 16 - y, str);    // set the text of the current cell to the steps number
        }
        y = a;
        x = b;
    }
    return size_of_stack; // return the number of steps needed to reach the goal
}

// function to print on the run output in the simulator
void printOutput(char *str)
{
    fprintf(stderr, "%s\n", str);
    fflush(stderr);
}

// Function to start the floodfill and the BFS
void doIt()
{
    printOutput("Started running...");
    // initial direction
    short Direction = NORTH;
    // to check the visited cells while discovering the maze in the floodfill
    int visited[MAX_SIZE][MAX_SIZE];
    /* store the STATE of every cell in the maze
     * I mean by the state the direction it entered the cell with and the open walls */
    short data[TOTAL_CELLS];
    short end_points[4] = {120, 121, 136, 137}; // the end points of the maze
    memset(visited, 0, sizeof(visited));
    memset(data, 0, sizeof(data));

    API_setText(0, 0, "START");
    printOutput("Started floodfill to explore the maze...");
    // explore the maze
    floodFill(16, 1, &Direction, visited, data);
    printOutput("Back to starting point...");

    API_setColor(0, 0, 'r');

    color = 'G';
    data[241] &= ~BACK;

    printOutput("Started BFS to find the shortest path from start to the goal...");
    // find the shortest path from the start to the goal
    short number_steps = BFS(241, end_points, Direction, data);
    char str[5];
    sprintf(str, "%d", number_steps); // convert the number of steps to string
    sprintf(str, "%s steps needed to reach the goal...", str);
    printOutput(str);
    printOutput("Finished running...");
}