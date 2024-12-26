#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "HeatMap.hpp"
#include "PDE.hpp"

int main()
{

    PDE::SpatialMesh mesh(std::vector<float>(2.f, 1.f), 4);

    sf::RenderWindow window(sf::VideoMode(1960, 1280), "SFML works!", sf::Style::Close);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    int mapSize = 20;

    HeatMap heatMap((1280) / mapSize, mapSize, &window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {

                int mouseX = event.mouseButton.x * mapSize / 1280;
                int mouseY = event.mouseButton.y * mapSize / 1280;
                auto button = event.mouseButton.button;

                int kernelSize = 11;
                float sigma = 2;
                float kernel[kernelSize][kernelSize];
                double sum = 0.0;
                int center = kernelSize / 2;

                for (int i = 0; i < kernelSize; ++i)
                {
                    for (int j = 0; j < kernelSize; ++j)
                    {
                        double x = i - center;
                        double y = j - center;
                        kernel[i][j] = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
                        sum += kernel[i][j];
                    }
                }

                for (int i = 0; i < kernelSize; ++i)
                {
                    for (int j = 0; j < kernelSize; ++j)
                    {
                        double x = mouseX - center + i;
                        double y = mouseY - center + j;
                        if (x < mapSize && y < mapSize && x >= 0 && y >= 0) {
                            if (button == sf::Mouse::Button::Left)
                            {
                                heatMap.incrementCellTemperature(x, y, 1.f * 0.5f * kernel[i][j]);
                            } else if (button == sf::Mouse::Button::Right) {
                                heatMap.incrementCellTemperature(x, y, -1.f * 0.5f * kernel[i][j]);
                            }
                        }
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::K)
                {
                    heatMap.simulate();
                }
            }
        }

        window.clear();
        // heatMap.simulate();
        heatMap.draw();
        window.display();


        /*
            Settings:
            - Initial heat distribution
            - Diffusion Coefficient
            - Number of steps
            - Time step
            - Method
            - Show degrees
            - Boundary conditions

            Capabilities:
            - Manual step
            - Add heat
        */
    }

    return 0;
}