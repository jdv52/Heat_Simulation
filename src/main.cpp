#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "HeatMap.hpp"
#include "PDE.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1960, 1280), "SFML works!", sf::Style::Close);

    int mapSize = 20;

    HeatMapGradient gradient(
        std::vector<sf::Color>(
            {
                sf::Color::Blue, sf::Color::Cyan, sf::Color::Green, sf::Color::Yellow, sf::Color::Red, sf::Color::White
            }
        )
    );

    HeatMap heatMap((1280) / mapSize, mapSize, &window);
    heatMap.setGradient(gradient);

    bool mouseDown;
    float scaleMultiplier = 1.0;

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
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    scaleMultiplier = 1.0;
                    mouseDown = true;
                } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                    scaleMultiplier = -1.0;
                    mouseDown = true;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                mouseDown = false;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::K)
                {
                    heatMap.simulate_Toggle();
                }
            }
        }

        if (mouseDown) {
            int mouseX = sf::Mouse::getPosition(window).x * mapSize / 1280;
            int mouseY =  sf::Mouse::getPosition(window).y * mapSize / 1280;

            int kernelSize = 21;
            float strength = 0.4;

            // std::cout << mouseX << " " << mouseY << "\n";

            // if (button == sf::Mouse::Button::Left) {
            heatMap.applyHeatAtPoint(mouseX, mouseY, kernelSize, strength, 1000.0 * scaleMultiplier);
            // } else if (button == sf::Mouse::Button::Right) {
                // heatMap.applyHeatAtPoint(mouseX, mouseY, kernelSize, strength, 3000.0);
            // }
        }

        window.clear();
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

    heatMap.simulate_Stop();

    return 0;
}