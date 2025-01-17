#ifndef HEATMAP_H
#define HEATMAP_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "eigen3/Eigen/Dense"
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "PDE.hpp"
#include "PDESolver.hpp"
#include "HeatMapGradient.hpp"

class HeatMap
{
    public:
        HeatMap(int stepSize, int mapSize, sf::RenderWindow *ctx);
        ~HeatMap();

        void setCellTemperature(int i, int j, float temperature);
        void setGradient(HeatMapGradient &gradient);
        void applyHeatAtPoint(int x, int y, int kernelSize, float strength, float heatMultiplier);
        void initPDE();

        void draw();
        void print();
        float getPointOnDrawing(int i, int j);
        void clearDrawing();

        void simulate_Start();
        void simulate_Stop();
        void simulate_Toggle();
        void simulate_ManualStep();
        void simulate_ThreadedLoop();

        float source_fn(std::vector<int> x, float t);

    private:
        int stepSize, mapSize;
        std::shared_ptr<PDE::SpatialMesh> shared_mesh_ptr;
        std::unique_ptr<HeatEquationSolver> solver;
        std::unique_ptr <PDE::HeatEquationProblem> heatEq;

        HeatMapGradient* gradient;

        bool simulationRunning;

        sf::RenderWindow* window_ctx;
        Eigen::MatrixXf drawingMatrix;
        Eigen::MatrixXf tempDrawing;

        std::thread updateThread;
        std::mutex mutex;

        void initMap();
};

#endif