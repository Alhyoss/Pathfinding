#include <iostream>
#include <deque>
#include <vector>
#include <unistd.h>
#include <string>

#include "Node/node.hpp"

std::vector<Node*> *makeGrid() {
    unsigned x(13), y(12);
    int grid[y][x] = {{3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}};

    std::vector<Node*> *gridVector = new std::vector<Node*>();
    for(unsigned i=0; i < y; i++) {
        for(unsigned j=0; j < x; j++) {
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

void backTrace(Node* node) {
    if(node->getStyle() != Node::Start && node->getStyle() != Node::End)
        node->setFillColor(sf::Color::Cyan);
    if(node->getStyle() != Node::Start) {
        Node* previousNode = node->getParent();
        backTrace(previousNode);
    }
}

bool goToEnd(std::vector<Node*> *grid, std::deque<Node*> &opened, std::vector<Node*> &closed,
            int endX, int endY) {
    Node* currentNode = opened[0]; //Node with the minimal F Cost
    if(opened.empty())
        return true;
    opened.pop_front();
    closed.push_back(currentNode);
    if(currentNode->getStyle() == Node::End) {
        backTrace(currentNode);
        return true;
    }
    if(currentNode->getStyle() != Node::Start)
        currentNode->setFillColor(sf::Color::Red);
    int x = currentNode->getX();
    int y = currentNode->getY();
    int h(13), v(12);
    std::vector<Node*> neighbours;
    if(y%(v-1) != 0 && x%(h-1) != 0) {
        neighbours.push_back((*grid)[(y-1)*h+x-1]);
        neighbours.push_back((*grid)[(y-1)*h+x]);
        neighbours.push_back((*grid)[(y-1)*h+x+1]);
        neighbours.push_back((*grid)[(y)*h+x-1]);
        neighbours.push_back((*grid)[(y)*h+x+1]);
        neighbours.push_back((*grid)[(y+1)*h+x-1]);
        neighbours.push_back((*grid)[(y+1)*h+x]);
        neighbours.push_back((*grid)[(y+1)*h+x+1]);
    }
    else if(y == (v-1) && x%(h-1) != 0) {
        neighbours.push_back((*grid)[(y-1)*h+x-1]);
        neighbours.push_back((*grid)[(y-1)*h+x]);
        neighbours.push_back((*grid)[(y-1)*h+x+1]);
        neighbours.push_back((*grid)[(y)*h+x-1]);
        neighbours.push_back((*grid)[(y)*h+x+1]);
    }
    else if(y == 0 && x%(h-1) != 0) {
        neighbours.push_back((*grid)[(y)*h+x-1]);
        neighbours.push_back((*grid)[(y)*h+x+1]);
        neighbours.push_back((*grid)[(y+1)*h+x-1]);
        neighbours.push_back((*grid)[(y+1)*h+x]);
        neighbours.push_back((*grid)[(y+1)*h+x+1]);
    }
    else if(x == 0 && y%(v-1) != 0) {
        neighbours.push_back((*grid)[(y-1)*h+x]);
        neighbours.push_back((*grid)[(y-1)*h+x+1]);
        neighbours.push_back((*grid)[(y)*h+x+1]);
        neighbours.push_back((*grid)[(y+1)*h+x]);
        neighbours.push_back((*grid)[(y+1)*h+x+1]);
    }
    else if(x == (h-1) && y%(v-1) != 0) {
        neighbours.push_back((*grid)[(y-1)*h+x-1]);
        neighbours.push_back((*grid)[(y-1)*h+x]);
        neighbours.push_back((*grid)[(y)*h+x-1]);
        neighbours.push_back((*grid)[(y+1)*h+x-1]);
        neighbours.push_back((*grid)[(y+1)*h+x]);
    }
    else if(x == (h-1) && y == (v-1)) {
        neighbours.push_back((*grid)[(y-1)*h+x-1]);
        neighbours.push_back((*grid)[(y-1)*h+x]);
        neighbours.push_back((*grid)[(y)*h+x-1]);
    }
    else if(x == (h-1) && y == 0) {
        neighbours.push_back((*grid)[(y)*h+x-1]);
        neighbours.push_back((*grid)[(y+1)*h+x-1]);
        neighbours.push_back((*grid)[(y+1)*h+x]);
    }
    else if(x == 0 && y == 0) {
        neighbours.push_back((*grid)[(y)*h+x+1]);
        neighbours.push_back((*grid)[(y+1)*h+x]);
        neighbours.push_back((*grid)[(y+1)*h+x+1]);
    }
    else if(x == 0 && y == (v-1)) {
        neighbours.push_back((*grid)[(y-1)*h+x]);
        neighbours.push_back((*grid)[(y-1)*h+x+1]);
        neighbours.push_back((*grid)[(y)*h+x+1]);
    }

    for(unsigned i=0; i < neighbours.size(); i++) {
        bool alreadyOpened = false;
        bool alreadyClosed = false;
        for(unsigned j=0; j < opened.size(); j++) {
            if(neighbours[i] == opened[j])
                alreadyOpened = true;
        }
        for(unsigned j=0; j < closed.size(); j++) {
            if(neighbours[i] == closed[j])
                alreadyClosed = true;
        }
        int xEndDist = abs(endX - neighbours[i]->getX());
        int yEndDist = abs(endY - neighbours[i]->getY());
        int hCost = 10*abs(xEndDist - yEndDist);
        int gCost = currentNode->getGCost();
        if(neighbours[i]->getX() == x || neighbours[i]->getY() == y)
            gCost += 10;
        else
            gCost += 14;
        if(xEndDist < yEndDist)
            hCost += xEndDist*14;
        else
            hCost += yEndDist*14;
        int fCost = gCost+hCost;
        if(!alreadyClosed &&
            (neighbours[i]->getFCost() < 0 || neighbours[i]->getFCost() > fCost)) {
            if(neighbours[i]->getStyle() != Node::Start && neighbours[i]->getStyle() != Node::End)
                neighbours[i]->setFillColor(sf::Color::Green);
            neighbours[i]->setGCost(gCost);
            neighbours[i]->setHCost(hCost);
            neighbours[i]->setFCost(fCost);
            neighbours[i]->setParent(closed[closed.size()-1]);
            if(!alreadyOpened)
                opened.push_back(neighbours[i]);
        }
    }
    return false;
}

template<class T>
void displayContainer(T &container, std::vector<sf::Text*> &texts, sf::Font &font) {
    int costs[3];
    int x, y;
    sf::Text* costText;
    for(unsigned i=0; i < container.size(); i++) {
        costs[3] = {container[i]->getGCost(), container[i]->getHCost(),
                        container[i]->getGCost() + container[i]->getHCost()};
        x = container[i]->getX();
        y = container[i]->getY();
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

void displayInfo(std::deque<Node*> &opened, std::vector<Node*> &closed,
                std::vector<sf::Text*> &texts, sf::Font &font) {
    for(sf::Text* text : texts)
        delete text;
    texts.clear();
    displayContainer<std::deque<Node*>>(opened, texts, font);
    displayContainer<std::vector<Node*>>(closed, texts, font);
}

bool cmp(Node* node1, Node* node2) {
    if(node1->getFCost() < node2->getFCost())
        return true;
    else if(node1->getFCost() > node2->getFCost())
        return false;
    else if(node1->getHCost() < node2->getHCost())
        return true;
    return false;
}

int main() {

    std::vector<Node*> *grid = makeGrid();
    std::deque<Node*> opened;
    std::vector <Node*> closed;
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

    sf::RenderWindow window(sf::VideoMode(650, 600), "A* algorithm");

    sf::Font font;
    if(!font.loadFromFile("Font/arial.ttf"))
        std::cout << "Unable to load font" << std::endl;

    std::vector<sf::Text*> texts;
    bool finish = false;
    while(window.isOpen()) {
        if(xEnd >= 0 && !finish)
            finish = goToEnd(grid, opened, closed, xEnd, yEnd);

        std::sort(opened.begin(), opened.end(), cmp);
        displayInfo(opened, closed, texts, font);

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for(Node* node : *grid)
            window.draw(*node);
        for(sf::Text* text : texts)
            window.draw(*text);
        window.display();
        usleep(500000);
    }

    for(sf::Text* text : texts)
        delete text;
    for(Node* node : *grid)
        delete node;
    delete grid;

    return 0;
}
