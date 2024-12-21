#ifndef HEATMAP_H
#define HEATMAP_H

#include <vector>
#include <SFML/Graphics.hpp>

class HeatMap
{
    public:
        HeatMap(int stepSize, int mapSize);
        ~HeatMap();

        void setDistribution(std::vector<float> distribution);
        void draw(sf::RenderWindow &window);

    private:
        int stepSize, mapSize;

        std::vector<sf::RectangleShape *> rectangles;
        void initMap();
};

#endif