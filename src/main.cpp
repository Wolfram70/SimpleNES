#include <iostream>
#include <memory>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../include/Bus.h"

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(256.0f, 240.0f), "NES Emulator");

    if(argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <rom.nes>" << std::endl;
        return 0;
    }

    if(argc > 2)
    {
        std::cout << "Too many arguments!" << std::endl;
        return 0;
    }

    Bus nes;
    std::shared_ptr<Cartridge> cartridge = std::make_shared<Cartridge>(argv[1]);
    if(!cartridge->getValid())
        return 0;
    
    nes.insert_cartridge(cartridge);
    nes.reset();

    sf::Texture texture;
    texture.create(256, 240);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::Vector2f targetSize(256.0f, 240.0f);

    float framerate = 60.0f;
    float frametime = 1.0f / framerate;
    float currentelapsed = 0.0f;
    sf::Clock clock;


    //TEMP VAR START
    int palette_color = 0;
    int palette_index = 0;
    //TEMP VAR END

    while (window.isOpen())
    {
        nes.controller[0] = 0x00;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
            nes.controller[0] |= 0x80;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
            nes.controller[0] |= 0x40;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            nes.controller[0] |= 0x20;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            nes.controller[0] |= 0x10;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            nes.controller[0] |= 0x08;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            nes.controller[0] |= 0x04;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            nes.controller[0] |= 0x02;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            nes.controller[0] |= 0x01;
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        if(clock.getElapsedTime() < sf::seconds(frametime))
        {

        }
        else
        {
            clock.restart();
            do
            { 
                nes.clock();
            } while (!nes.ppu.frame_complete);
            do
            {
                nes.clock();
            } while (!nes.cpu.complete());
            nes.ppu.frame_complete = false;
        }

        targetSize = window.getView().getSize();

        texture.update(nes.ppu.getScreen());
        sprite.setScale(targetSize.x / sprite.getLocalBounds().width, targetSize.y / sprite.getLocalBounds().height);

        window.clear();

        window.draw(sprite);

        window.display();
    }

    return 0;
}