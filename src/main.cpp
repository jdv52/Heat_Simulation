#include <SFML/Graphics.hpp>

#include "HeatMap.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1960, 1280), "SFML works!", sf::Style::Close);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    int mapSize = 100;

    HeatMap heatMap((1280 - 0 * 2) / mapSize, mapSize);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        heatMap.draw(window);
        window.display();


        /*
            Settings:
            - Initial heat distributionn
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