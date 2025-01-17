#include "HeatMapGradient.hpp"

#include <cmath>

HeatMapGradient::HeatMapGradient(std::vector<sf::Color> _colors)
    : colors(_colors)
{

}

sf::Color HeatMapGradient::mapdoubleToColor(double val, double min, double max)
{
    const int NUM_COLORS = colors.size();
    
    int idx1;        // |-- Our desired color will be between these two indexes in "color".
    int idx2;        // |
    double fractBetween = 0;  // Fraction between "idx1" and "idx2" where our val

    double normalizedVal = (val - min) / (max - min);

    if (normalizedVal <= 0)
        idx1 = idx2 = 0;   // accounts for an input <=0
    else if (normalizedVal >= 1)
        idx1 = idx2 = NUM_COLORS-1;    // accounts for an input >=0
    else
    {
        normalizedVal = normalizedVal * (NUM_COLORS-1);        // Will multiply value by 3.
        idx1  = std::floor(normalizedVal);                  // Our desired color will be after this index.
        idx2  = idx1+1;                        // ... and before this index (inclusive).
        fractBetween = normalizedVal - double(idx1);    // Distance between the two indexes (0-1).
    }
        
    int R = (colors.at(idx2).r - colors.at(idx1).r) * fractBetween + colors.at(idx1).r;
    int G = (colors.at(idx2).g - colors.at(idx1).g) * fractBetween + colors.at(idx1).g;
    int B = (colors.at(idx2).b - colors.at(idx1).b) * fractBetween + colors.at(idx1).b;

    return sf::Color(R, G, B);
}
