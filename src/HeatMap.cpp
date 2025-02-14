#include <algorithm>
#include <iostream>
#include <ctime>
#include <functional>

#include "HeatMap.hpp"


HeatMap::HeatMap(int stepSize, int mapSize, sf::RenderWindow *ctx) 
    : shared_mesh_ptr(std::make_shared<PDE::SpatialMesh>(std::vector<double>(2, 1.0), mapSize)),
        drawingMatrix(mapSize, mapSize)
{
    this->stepSize = stepSize;
    this->mapSize = mapSize;
    this->window_ctx = ctx;

    clearDrawing();

    simulationRunning = false;

    initMap();
    initPDE();
}

HeatMap::~HeatMap()
{
    
}

void HeatMap::setCellTemperature(int i, int j, double temperature)
{
    shared_mesh_ptr->setFValAtMeshPoint(std::vector<int>({i, j}), std::clamp(temperature, 0.0, 1.0));
}

void HeatMap::setGradient(HeatMapGradient &_gradient)
{
    this->gradient = &_gradient;
}

void HeatMap::applyHeatAtPoint(int x, int y, int kernelSize, double strength, double heatMultiplier)
{
    double sum = 0;
    double sigma = 1 / strength;
    int center = kernelSize / 2;

    double kernel[kernelSize][kernelSize];

    for (int i = 0; i < kernelSize; ++i)
    {
        for (int j = 0; j < kernelSize; ++j)
        {
            int _x = i - center;
            int _y = j - center;
            double val = exp(-(_x * _x + _y * _y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);

            kernel[i][j] = val;

            sum += val;
        }
    }

    for (int i = 0; i < kernelSize; ++i)
    {   
        for (int j = 0; j < kernelSize; ++j)
        {
            int matrixY = i - center + x;
            int matrixX = j - center + y;

            if (matrixX > 0 && matrixX < mapSize - 1 && matrixY > 0 && matrixY < mapSize - 1)
                drawingMatrix(matrixX, matrixY) += kernel[i][j] * heatMultiplier / sum;
        }
    }
}

double HeatMap::source_fn(std::vector<int> x, double t)
{
    int i = x.at(0);
    int j = x.at(1);

    return drawingMatrix(i, j);
}


void HeatMap::draw()
{
    std::lock_guard<std::mutex> lock(mutex); 

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            auto cell_rect = sf::RectangleShape(sf::Vector2f(stepSize, stepSize));
            cell_rect.setPosition(sf::Vector2f(i * stepSize + 0, j * stepSize + 0));
            cell_rect.setOutlineThickness(1.0);
            cell_rect.setOutlineColor(sf::Color::Black);

            double temp = shared_mesh_ptr->getFValAtMeshPoint(std::vector<int>({i, j}));

            cell_rect.setFillColor(gradient->mapdoubleToColor(temp, -1000, 1000));
            window_ctx->draw(cell_rect);    
        }
    }
}

void HeatMap::print()
{
    shared_mesh_ptr->printMesh();
}

void HeatMap::initMap()
{
    std::cout << "Initializing map and solver...\n";
    solver = std::make_unique<ForwardDifference>(0.01);

    for (int i = 0; i < mapSize * mapSize; ++i)
    {
        shared_mesh_ptr->setFValAtIdx(i, 0.0);
    }
}

void HeatMap::initPDE()
{
    PDE::Function_handle fn = [this](std::vector<int> x, double t) {
        return this->source_fn(x, t);
    };

    heatEq = std::make_unique<PDE::HeatEquationProblem>(1e-2, shared_mesh_ptr, fn);
}

void HeatMap::simulate_ManualStep()
{
    solver->solve(*heatEq);
    clearDrawing();
}

double HeatMap::getPointOnDrawing(int i, int j)
{
    return drawingMatrix(i, j);
}

void HeatMap::clearDrawing()
{
    drawingMatrix.setZero();
}

void HeatMap::simulate_Start()
{
    if (!simulationRunning)
    {
        std::cout << "Starting Simulation...\n";
        simulationRunning = true;
        updateThread = std::thread(&HeatMap::simulate_ThreadedLoop, this);
        std::cout << "Started!\n";
    }
}

void HeatMap::simulate_Stop()
{
     if (simulationRunning)
     {
        simulationRunning = false;
        std::cout << "Stopping Simulation.\n";
        if (updateThread.joinable())
        {
            updateThread.join();
        }
        
        std::cout << "Stopped!\n";
     }
}

void HeatMap::simulate_Toggle()
{
    if (simulationRunning)
        simulate_Stop();
    else
        simulate_Start();
}

void HeatMap::simulate_ThreadedLoop()
{
    while (simulationRunning)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            solver->solve(*heatEq);
            clearDrawing();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}