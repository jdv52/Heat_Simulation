#include <algorithm>
#include <iostream>
#include <ctime>

#include "HeatMap.hpp"

float f(float x, float y, float z)
{
    return 0.0f;
}

HeatMap::HeatMap(int stepSize, int mapSize, sf::RenderWindow *ctx) 
    : mesh(std::vector<float>(2, 1.0f), mapSize),
        heatEq(0.000001f, &mesh, (PDE::Function_handle)f)
{
    this->stepSize = stepSize;
    this->mapSize = mapSize;
    this->window_ctx = ctx;

    initMap();
}

HeatMap::~HeatMap()
{
    delete solver;
}

void HeatMap::setCellTemperature(int i, int j, float temperature)
{
    mesh.setFValAtMeshPoint(std::vector<int>({i, j}), std::clamp(temperature, 0.0f, 1.0f));
}

void HeatMap::incrementCellTemperature(int i, int j, float incrementAmount)
{
    float temp = mesh.getFValAtMeshPoint(std::vector<int>({i, j}));
    float new_temp = temp + incrementAmount;
    setCellTemperature(i, j, new_temp);
}

void HeatMap::setGradient(HeatMapGradient &_gradient)
{
    this->gradient = &_gradient;
}

void HeatMap::draw()
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            auto cell_rect = sf::RectangleShape(sf::Vector2f(stepSize, stepSize));
            cell_rect.setPosition(sf::Vector2f(i * stepSize + 0, j * stepSize + 0));
            cell_rect.setOutlineThickness(1.0f);
            cell_rect.setOutlineColor(sf::Color::Black);

            float temp = mesh.getFValAtMeshPoint(std::vector<int>({i, j}));

            cell_rect.setFillColor(gradient->mapFloatToColor(temp));
            window_ctx->draw(cell_rect);    
        }
    }
}

void HeatMap::print()
{
    mesh.printMesh();
}

void HeatMap::initMap()
{
    solver = new ForwardDifference(0.001);

    for (int i = 0; i < mapSize * mapSize; ++i)
    {
        mesh.setFValAtIdx(i, 0.0f);
    }
}

void HeatMap::simulate()
{
    solver->solve(heatEq);
}