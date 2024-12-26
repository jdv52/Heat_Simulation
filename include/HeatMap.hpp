#ifndef HEATMAP_H
#define HEATMAP_H

#include <vector>
#include <SFML/Graphics.hpp>

#include "PDE.hpp"
#include "PDESolver.hpp"
#include "HeatMapGradient.hpp"

class HeatMap
{
    public:
        HeatMap(int stepSize, int mapSize, sf::RenderWindow *ctx);
        ~HeatMap();

        void setCellTemperature(int i, int j, float temperature);
        void incrementCellTemperature(int i, int j, float incrementAmount);
        void setGradient(HeatMapGradient &gradient);

        void draw();
        void print();

        void simulate();

    private:
        int stepSize, mapSize;
        PDE::SpatialMesh mesh;
        HeatEquationSolver* solver;
        PDE::HeatEquationProblem heatEq;

        HeatMapGradient* gradient;

        sf::RenderWindow* window_ctx;

        void initMap();

        /**
         * @brief The initial distribution of the heat map
         * 
         * Takes in an (i, j) coordinate and outputs a floating point value
         * indicating the amount of heat energy at (i ,j). Heat energy is sampled
         * from the discretized heat map 
         * 
         * @param i spatial domain coordinate along the horizontal aaxis
         * @param j spatial domain coordinate along the a
         * @return heat energy at (i, j) as a float
         */
        float distribution_fn(float i, float j);
};

#endif