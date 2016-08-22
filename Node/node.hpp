#ifndef _NODE_HPP_
#define _NODE_HPP_ 1

#include <SFML/Graphics.hpp>

class Node : public sf::RectangleShape {
    public:
        enum Style {Start, End, Path, Obstacle};

        Node(int x, int y, int size, int thickness) {
            setSize(sf::Vector2f(size-thickness, size-thickness));
            setOutlineColor(sf::Color::Black);
            setOutlineThickness(thickness);
            setOrigin(10, 10);

            this->x = x;
            this->y = y;
            setPosition(x*(getSize().x + getOutlineThickness()) + getOrigin().x,
                        y*(getSize().y + getOutlineThickness()) + getOrigin().y);
            fCost = -1;
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
        int x, y;
        int gCost, hCost, fCost;
        Node* parent;
        Style style;
};

#endif
