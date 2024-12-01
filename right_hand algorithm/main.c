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

    API_setColor(x, y, 'G');
}

int main()
{

    /*
    <<<<<<<<<<<<<<<That is how the right hand rule works>>>>>>>>>>>>>>>>>>>
        Always keep your right hand on the wall.
        If there's a wall on your right, go forward.
        If there's a wall in front, turn left.
        If there's a wall on all three sides (right, front, and left), turn around.
        Continue this pattern to navigate through the maze.
    */

    printf("Start The Right-Hand Rule ...\n");
    // Initialize position and facing direction
    // Assuming the starting point is (0,0) and the facing is 0, which represents north (of the facing directions above)

    x = 0;
    y = 0;
    facing = 0;

    while (!API_wasReset())
    {
        if (!API_wallRight())
        {
            // If no wall to the right, turn right and move forward and update the postion of robot
            turnRightAndUpdateFacing();
            moveForwardAndUpdatePosition();
        }
        else if (!API_wallFront())
        {
            // If no wall in front, move forward and update the postion of robot
            moveForwardAndUpdatePosition();
        }
        else if (!API_wallLeft())
        {
            // If no wall to the left, turn left and move forward and update the postion of robot
            turnLeftAndUpdateFacing();
            moveForwardAndUpdatePosition();
        }
        else
        {
            // If surrounded by walls on three sides, turn around (go back) and update the postion of robot
            turnRightAndUpdateFacing();
            turnRightAndUpdateFacing();
        }
    }

    return 0;
}
