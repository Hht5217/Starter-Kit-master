# Part 2

## Video Demo

https://youtu.be/3xr7_prbOUY

## Minimum Requirements

### Completed

1. Randomly initialize objects on the map.
2. Randomly initialize alien and zombies attributes.
3. Alien and zombie(s) can attack each other.
4. Alien stops moving after hitting specific objects.
5. Rock object reveals hidden objects.
6. All required commands implemented.


### To Do

Completed

## Additional Features

None

## Contributions

### Tan Hong Han

1. Modified the gameboard.
2. Managed the display of in-game character's attributes.
3. Managed the movement of the characters.
4. Managed the SAVE & LOAD part.

### Anadani Muaaz

1. Designed the main menu.
2. Managed the initialization of in-game character's attributes.

## Problems Encountered & Solutions

1. Program reasking for command from previous function
- Solved by adding "break;" to while loops.

2. Type error exception when asking for user input
- Solved by using while loop.

3. Zombies attributes keep changing due to rand() and recalling of functions
- Solved by initializing value before game start and passing objects array by reference between functions.

4. Objects moving out of board range.
- Solved by checking position of objects before proceeding to next step (ie. checking if object has reached border).

5. Zombie is trapped by other objects on the board.(i.e. it is cornered and the other surrounding spaces are occupied)
- Solved by checking object surrounding zombie before proceeding.