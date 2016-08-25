#include <iostream>
#include <deque>
#include <vector>
#include <unistd.h>
#include <string>

#include "Node/node.hpp"

//The dimensions of the grid
const unsigned row = 12;
const unsigned column = 12;

/*
 * This function create the grid on which the algorithm has to find its way.
 * You can change the grid by changing the bidimensionnal array:
 *      3 is the ending node
 *      2 is the starting node
 *      1 is an Obstacle
 *      Any other number is a walkable node
 * To change the size of the grid you have to change row and column (and of course
 * fill the array properly).
 * You'll also have to change the h and v variables in the goToEnd function.
 */
std::vector<Node*> *makeGrid() {
    int grid[row][column] = {{3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}};

    std::vector<Node*> *gridVector = new std::vector<Node*>();
    for(unsigned i=0; i < row; i++) {
        for(unsigned j=0; j < column; j++) {
            Node *node = new Node(j, i, 50, 2);
            if(grid[i][j] == 1) { //Obstacle node
                node->setStyle(Node::Obstacle);
                node->setFillColor(sf::Color::Black);
            }
            else if(grid[i][j] == 2) { //Starting node
                node->setStyle(Node::Start);
                node->setFillColor(sf::Color::Yellow);
            }
            else if(grid[i][j] == 3) { //Ending node
                node->setStyle(Node::End);
                node->setFillColor(sf::Color::Blue);
            }
            else { //Walkable node
                node->setStyle(Node::Path);
                node->setFillColor(sf::Color::White);
            }
            gridVector->push_back(node);
        }
    }
    return gridVector;
}

/*
 * This function will trace the final shortest way found by the algorithm
 * and change the color of the nodes in Cyan.
 * It starts at the ending node, and trace back the way by looking at the
 * parent node of the node it's currently checking.
 */
void backTrace(Node* node) {
    if(node->getStyle() != Node::Start && node->getStyle() != Node::End)
        node->setFillColor(sf::Color::Cyan);
    if(node->getStyle() != Node::Start) {
        Node* previousNode = node->getParent();
        backTrace(previousNode);
    }
}

/*
 * This is the main function of the program.
 * Each time it's called, it will check the node which has not yet been checked
 * with the lowest fCost (define by gCost + hCost, wher gCost is the distance
 * from the starting node, and hCost is the distance to the ending node).
 * This node is the first element of the opened deque (which has been sorted).
 * Each time a node is checked, it is marked as "closed", colored in red and then
 * the function compute the hCost, gCost and fCost of each of the neighbours nodes.
 * Those neighbours are colored in green and set as "opened" if they weren't
 * already, and if they are, the function update it's fCost.
 */
bool goToEnd(const std::vector<Node*> *grid, std::deque<Node*> &opened,
            std::vector<Node*> &closed, int endX, int endY) {
    //If there is no possible way to go to the end
    if(opened.empty())
        return true;
    //We take the node with the lowest fCost and mark it as closed
    Node* currentNode = opened[0];
    opened.pop_front();
    closed.push_back(currentNode);
    //If the ending has been found, the program can backtrace
    if(currentNode->getStyle() == Node::End) {
        backTrace(currentNode);
        return true;
    }
    //Set the color of the node to red
    if(currentNode->getStyle() != Node::Start)
        currentNode->setFillColor(sf::Color::Red);
    //Those variables are juste to simplify the reading
    const int x = currentNode->getX();
    const int y = currentNode->getY();

    //We store the neighbours of the current node in the neighbours vector
    std::vector<Node*> neighbours;
    if(y != row-1) {
        neighbours.push_back((*grid)[(y+1)*column+x]);
        if(x != column-1)
            neighbours.push_back((*grid)[(y+1)*column+x+1]);
        if(x != 0)
            neighbours.push_back((*grid)[(y+1)*column+x-1]);
    }
    if(y != 0) {
        neighbours.push_back((*grid)[(y-1)*column+x]);
        if(x != column-1)
            neighbours.push_back((*grid)[(y-1)*column+x+1]);
        if(x != 0)
            neighbours.push_back((*grid)[(y-1)*column+x-1]);
    }
    if(x != 0)
        neighbours.push_back((*grid)[(y)*column+x-1]);
    if(x != column-1)
        neighbours.push_back((*grid)[(y)*column+x+1]);

    bool alreadyOpened, alreadyClosed;
    int xEndDist, yEndDist, hCost, gCost, fCost;
    //For each neighbours
    for(unsigned i=0; i < neighbours.size(); i++) {
        alreadyOpened = false;
        alreadyClosed = false;
        //We check if the neighbour has already been opened
        for(unsigned j=0; j < opened.size(); j++) {
            if(neighbours[i] == opened[j])
                alreadyOpened = true;
        }
        //We check if the neighbour has already been closed
        for(unsigned j=0; j < closed.size(); j++) {
            if(neighbours[i] == closed[j])
                alreadyClosed = true;
        }
        //The gCost is the gCost form the parent node plus the distance whit it
        gCost = currentNode->getGCost();
        if(neighbours[i]->getX() == x || neighbours[i]->getY() == y)
            gCost += 10;
        else
            gCost += 14;
        //The hCost is simply the distance to the ending node
        xEndDist = abs(endX - neighbours[i]->getX());
        yEndDist = abs(endY - neighbours[i]->getY());
        //Euclidian distance
        hCost = int(10*sqrt(xEndDist*xEndDist+yEndDist*yEndDist));
        //We compute the fCost
        fCost = gCost+hCost;
        //If the node has not yet been closed, if its fCost has not yet been set
        //or if its fCost needs to be updated, we update it and set the parent
        //of the neighbour as the currentNode
        if(!alreadyClosed &&
            (neighbours[i]->getFCost() == 0 || neighbours[i]->getFCost() > fCost)) {
            if(neighbours[i]->getStyle() != Node::Start && neighbours[i]->getStyle() != Node::End)
                neighbours[i]->setFillColor(sf::Color::Green);
            neighbours[i]->setGCost(gCost);
            neighbours[i]->setHCost(hCost);
            neighbours[i]->setFCost(fCost);
            neighbours[i]->setParent(currentNode);
            //If the neighbour is not yet opened, we mark it as opened
            if(!alreadyOpened)
                opened.push_back(neighbours[i]);
        }
    }
    //The function didn't find the ending node yet
    return false;
}

/*
 * This class is just there to add in a vector the texts that will be displayed
 * The template is necessary so that we can call it with a deque or a vector
 * (or any other container with the needed function).
 */
template<class T>
void displayContainer(T &container, std::vector<sf::Text*> &texts, sf::Font &font) {
    int costs[3];
    int x, y;
    sf::Text* costText;
    for(unsigned i=0; i < container.size(); i++) {
        //This condition is either usefull to optimize the program and to prevent
        //some parasit numbers to appear (the costs of the obstacles).
        if(container[i]->getStyle() == Node::Path) {
            x = container[i]->getX();
            y = container[i]->getY();
            costs[0] = container[i]->getGCost();
            costs[1] = container[i]->getHCost();
            costs[2] = container[i]->getFCost();
            for(unsigned j=0; j < 3; j++) {
                costText = new sf::Text();
                costText->setString(std::to_string(costs[j]));
                costText->setFont(font);
                costText->setCharacterSize(15);
                costText->setColor(sf::Color::Black);
                if(j==0)
                    costText->setPosition(x*50, y*50);
                if(j==1)
                    costText->setPosition(x*50+25, y*50);
                if(j==2)
                    costText->setPosition(x*50+10, y*50+30);
                texts.push_back(costText);
            }
        }
    }
}

/*
 * In this function, we delete the vector text of the precedent loop and update it.
 */
void displayInfo(std::deque<Node*> &opened, std::vector<Node*> &closed,
                std::vector<sf::Text*> &texts, sf::Font &font) {
    for(sf::Text* text : texts)
        delete text;
    texts.clear();
    displayContainer<std::deque<Node*>>(opened, texts, font);
    displayContainer<std::vector<Node*>>(closed, texts, font);
}

/*
 * This a comparison function used for the sort function of the STL.
 * It compares two node. The "lower" one is the one with the lowest fCost.
 * For equal fCost, the lower one is the one with the lower hCost.
 * For equal hCost, the node are "equal"
 */
bool cmp(Node* node1, Node* node2) {
    if(node1->getFCost() != node2->getFCost())
        return node1->getFCost() < node2->getFCost();
    else
        return node1->getHCost() < node2->getHCost();
}

/*
 * In the main function we call all the other function and display the results
 * in a window create with SFML.
 */
int main() {
    //We create the grid and initialize the opened and closed container
    const std::vector<Node*> *grid = makeGrid();
    std::deque<Node*> opened;
    std::vector <Node*> closed;
    //We check if there is an ending node, we set the obstacle as closed
    //and mark the started node as opened
    int xEnd(-1), yEnd;
    for(unsigned i=0; i < grid->size(); i++) {
        if((*grid)[i]->getStyle() == Node::Start)
            opened.push_back((*grid)[i]);
        else if((*grid)[i]->getStyle() == Node::End) {
            xEnd = (*grid)[i]->getX();
            yEnd = (*grid)[i]->getY();
        }
        else if((*grid)[i]->getStyle() == Node::Obstacle)
            closed.push_back((*grid)[i]);
    }
    //Function to create a window with SFML
    sf::RenderWindow window(sf::VideoMode(600, 600), "A* algorithm");
    //Needed in SFML to display text
    sf::Font font;
    if(!font.loadFromFile("Font/arial.ttf"))
        std::cout << "Unable to load font" << std::endl;
    //This vector will contain the texts that will be displayed
    std::vector<sf::Text*> texts;
    //This is the main loop, in which we update everything in each loop
    bool finish = false;
    while(window.isOpen()) {
        //If there is an ending node and the algorithm is not finished, we call
        //the goToEnd function to find the shortest path to the ending node
        if(xEnd >= 0 && !finish)
            finish = goToEnd(grid, opened, closed, xEnd, yEnd);
        //We sort the opened deque so that it is easy to find the next node to check
        std::sort(opened.begin(), opened.end(), cmp);
        //We update what need to be displayed
        displayInfo(opened, closed, texts, font);
        //If the user click on the close button, the window is closed at the end
        //of the loop
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        //We display the nodes and the texts
        window.clear();
        for(Node* node : *grid)
            window.draw(*node);
        for(sf::Text* text : texts)
            window.draw(*text);
        window.display();
        //Just to slow down the loop
        //usleep(100000);
    }
    //We desallocate everything
    for(sf::Text* text : texts)
        delete text;
    for(Node* node : *grid)
        delete node;
    delete grid;

    return 0;
}
