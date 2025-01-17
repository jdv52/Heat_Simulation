#ifndef HEATMAP_GRADIENT_H
#define HEATMAP_GRADIENT_H

#include <vector>

#include <SFML/Graphics.hpp>

class HeatMapGradient
{
    public:

        HeatMapGradient(std::vector<sf::Color> _colors);

        sf::Color mapdoubleToColor(double val, double min = 0, double max = 1);

    private:
        std::vector<sf::Color> colors;

};

#endif