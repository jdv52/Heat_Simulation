#include "HeatMap.hpp"

HeatMap::HeatMap(int stepSize, int mapSize)
{
    this->stepSize = stepSize;
    this->mapSize = mapSize;

    this->rectangles.reserve(mapSize * mapSize);

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            rectangles.push_back(new sf::RectangleShape(sf::Vector2f(stepSize, stepSize)));
        }
    }

    initMap();
}

HeatMap::~HeatMap()
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            delete (rectangles.at(i * mapSize + j));
        }
    }
}

void HeatMap::draw(sf::RenderWindow &window)
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            window.draw(sf::RectangleShape(*rectangles.at(i * mapSize + j)));
        }
    }
}

void HeatMap::initMap()
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            rectangles.at(i * mapSize + j)->setFillColor(sf::Color::White);
            rectangles.at(i * mapSize + j)->setPosition(sf::Vector2f(i * stepSize + 0, j * stepSize + 0));
            rectangles.at(i * mapSize + j)->setOutlineThickness(1.0f);
            rectangles.at(i * mapSize + j)->setOutlineColor(sf::Color::Black);
        }
    }
}