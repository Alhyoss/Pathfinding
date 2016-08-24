This is a pathfinding algorithm in C++.

It uses the SFML library that you can download here : http://www.sfml-dev.org/download/sfml/2.4.0/

You can change the grid by changing the bidimensionnal array in the makeGrid function.

        - 0 is a walkable Node
        - 1 is a Obstacle
        - 2 is the starting node
        - 3 is the ending node

The algorithm used is the A* algorithm, which is the following :

    OPEN //set of nodes to be evaluated
    CLOSED //set of nodes already evaluated
    add the start to opened
    loop
        current = node in OPEN with lowest fCost
        remove current from OPEN
        add current to CLOSED

        if current is the ending node //path found
            return

        foreach neighbour of the current node
            if neighbour is not walkable or neighbour is in CLOSED
                skip to next neighbour

            if new path to neighbour is shorter or neighbour is not in OPEN
                set fCost of neighbour
                set parent of neighbour to current node
                if neighbour is not in OPEN
                    add neighbour to OPEN
