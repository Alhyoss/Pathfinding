#ifndef _NODE_HPP_
#define _NODE_HPP_ 1

#include <SFML/Graphics.hpp>

/*
 * The Node class derived from RectangleShape from SFML. Therefore it can
 * use lots of usefull function to display it easily (such as setFillColor, ...).
 * An instance of this class is a square on the displayed grid.
 * It contains informations such as the type of node (Path, starting node, ending
 * node or obstacle), its hCost, gCost and fCost and its parent.
 */
class Node : public sf::RectangleShape {
    public:
        //The type of the node
        enum Style {Start, End, Path, Obstacle};
        /*
         * x and y are the indexes in the bidimensional array of the rectangular
         * node, size is its size (Really) and thickness is the thickness of its
         * outline (in black, to see the different nodes)
         */
        Node(int x, int y, int size, int thickness) {
            //We set the rectangular shape
            setSize(sf::Vector2f(size-thickness, size-thickness));
            setOutlineColor(sf::Color::Black);
            setOutlineThickness(thickness);
            setOrigin(10, 10);
            //We set its position
            this->x = x;
            this->y = y;
            setPosition(x*(getSize().x + getOutlineThickness()) + getOrigin().x,
                        y*(getSize().y + getOutlineThickness()) + getOrigin().y);
            //fCost to zero so that we know it hasn't been checked
            fCost = 0;
        }

        void setFCost(int f) {
            fCost = f;
        }

        int getFCost() {
            return fCost;
        }

        void setGCost(int g) {
            gCost = g;
        }

        int getGCost() {
            return gCost;
        }

        void setHCost(int h) {
            hCost = h;
        }

        int getHCost() {
            return hCost;
        }

        void setStyle(Style s) {
            style = s;
        }

        Style getStyle() {
            return style;
        }

        void setParent(Node* p) {
            parent = p;
        }

        Node* getParent() {
            return parent;
        }

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }

    protected:
        int x, y;//the indexes in the bidimensionnal array in the makeGrid function
        int gCost, hCost, fCost;
        Node* parent;
        Style style;
};

#endif
