#include <algorithm>
#include <iostream>
#include <cmath>
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


            const int NUM_COLORS = 5;
            static float color[NUM_COLORS][3] = { {0,0,255}, {0, 255, 255}, {0,255,0}, {255,255,0}, {255,0,0} };
            // A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.
            
            int idx1;        // |-- Our desired color will be between these two indexes in "color".
            int idx2;        // |
            float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our val

            if (temp <= 0)
                idx1 = idx2 = 0;   // accounts for an input <=0
            else if (temp >= 1)
                idx1 = idx2 = NUM_COLORS-1;    // accounts for an input >=0
            else
            {
                temp = temp * (NUM_COLORS-1);        // Will multiply value by 3.
                idx1  = std::floor(temp);                  // Our desired color will be after this index.
                idx2  = idx1+1;                        // ... and before this index (inclusive).
                fractBetween = temp - float(idx1);    // Distance between the two indexes (0-1).
            }
                
            int R   = (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0];
            int G = (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1];
            int B  = (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2];

            cell_rect.setFillColor(sf::Color(R, G, B));
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