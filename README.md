# Terrain generation and finding a path between 2 points

## How to run
- Clone the repository
- Use the command `make`
- Use the command `./asdf`
- Open `Terrain.ppm` 
 > Note: Not all image editors support .ppm, I use GIMP

## How does it work
### Diamond-Square generates a height map
### Pathfinding algorithm finds path between 2 points in the generated height map

## Algorithms
### Diamond Square
![Image of DS](https://upload.wikimedia.org/wikipedia/commons/thumb/b/bf/Diamond_Square.svg/2560px-Diamond_Square.svg.png)
At it's essense it's a recursive algorithm that has 2 steps:
- Diamond step
  - Takes four corners and averages them then adds a random value
- Square step
  - Takes three edges and averages them then adds a random value

### Pathfinding using BFS
![GIF](https://upload.wikimedia.org/wikipedia/commons/4/46/Animated_BFS.gif)  
Animated example of a breadth-first search.  
**Black:** _explored_, **grey:** _queued to be explored later on_

### Setup
Add the start cell to the queue.

### Recursive part
1. Pop first element from the queue.
2. Check which of its adjacent cells are valid according to the [rules](https://github.com/AlexOgn/ProektASD/tree/main#rules-for-pathfinding).
3. Add all valid cells to the queue.
4. Save current coordinates to the `previous` array, which is used to know where we came from.

### Generating the path
1. Start at the end point
2. Iterate through the `previous` array until we get to the start coordinates
3. Reverse the path so that it's from start to end

### Rules for pathfinding
1. **The difference in height between points in the path cannot be greater than 5.**
2. **If path includes water, a boat is used which has a certain durability.**
3. **If the durability becomes 0 it cannot be used anymore and the path has to go around water.**
4. **The difference in height doesn't matter if a boat is being used.**
5. **We then generate 2 paths:**
    * **A path where use of boat is permitted**
    * **A path where use of boat is prohibited**
6. **Finally, we compare the length of each path and choose the shorter one**


