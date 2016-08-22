This is a pathfinding algorithm in C++.

It uses the SFML library that you can download here : http://www.sfml-dev.org/download/sfml/2.4.0/

The algorithm used is the A* algorithm.

It worked quite well, but it is yet to be optimized a lot, especially the goToEnd function which is really repetitive.

You can change the grid by changing the bidimensionnal array in the makeGrid function.
        - 0 is a walkable Node
        - 1 is a Obstacle
        - 2 is the starting node
        - 3 is the ending node
