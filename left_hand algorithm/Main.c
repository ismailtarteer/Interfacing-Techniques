#include <stdio.h>
#include "API.h"

/*
    facing directions:
    north:0
    east:1
    south:2
    west:3
*/

int x, y, facing;

void turnLeftAndUpdateFacing()
{
    API_turnLeft();            // turn left ,then update the facing
    facing = (facing + 3) % 4; // 0->3, 1->0, 2->1, 3->2 (look at the facing directions in the top)
}

void turnRightAndUpdateFacing()
{
    API_turnRight();           // turn right ,then update the facing
    facing = (facing + 1) % 4; // 0->1, 1->2, 2->3, 3->0 (look at the facing directions in the top)
}

void moveForwardAndUpdatePosition()
{
    API_moveForward();
    if (facing == 0) // it will go to the top, so y will increase by one
        y++;
    else if (facing == 1) // It will go to the right, so x will increase by one
        x++;
    else if (facing == 2) // It will go to the bottom, so y will decrease by one
        y--;
    else if (facing == 3) // It will go to the left, so x will decrease by one
        x--;

    API_setColor(x, y, 'Y'); // set current cell color to yellow
}

int main()
{
    /*
<<<<<<<<<<<<<<<That is how the left hand rule works>>>>>>>>>>>>>>>>>>>
        Always keep your left hand on the wall.
        If there's a wall on your left, go forward.
        If there's a wall in front, turn right.
        If there's a wall on all three sides (left, front, and right), turn around.
        Continue this pattern to navigate through the maze.
    */

    printf("Running Left-Hand Rule Algorithm...\n");
    // Initialize position and facing direction
    // Assuming the starting point is (0,0) and the facing is 0, which represents north (of the facing directions above)

    x = 0;
    y = 0;
    facing = 0;

    while (!API_wasReset())
    {
        if (!API_wallLeft())
        {
            // If no wall to the left, turn left and move forward and update the postion of robot
            turnLeftAndUpdateFacing();
            moveForwardAndUpdatePosition();
        }
        else if (!API_wallFront())
        {
            // If no wall in front, move forward and update the postion of robot
            moveForwardAndUpdatePosition();
        }
        else if (!API_wallRight())
        {
            // If no wall to the right, turn right and move forward and update the postion of robot
            turnRightAndUpdateFacing();
            moveForwardAndUpdatePosition();
        }
        else
        {
            // If surrounded by walls on three sides, turn around and update the postion of robot
            turnLeftAndUpdateFacing();
            turnLeftAndUpdateFacing();
        }
    }

    return 0;
}
