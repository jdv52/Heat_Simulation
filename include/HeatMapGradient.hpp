#ifndef HEATMAP_GRADIENT_H
#define HEATMAP_GRADIENT_H

#include <vector>

#include <SFML/Graphics.hpp>

class HeatMapGradient
{
    public:

        HeatMapGradient(std::vector<sf::Color> _colors);

        sf::Color mapFloatToColor(float val, float min = 0, float max = 1);

    private:
        std::vector<sf::Color> colors;

};

#endif