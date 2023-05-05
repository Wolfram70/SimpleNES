#include <iostream>
#include <memory>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../include/Bus.h"
#include "../include/Sound.h"

Bus nes;

float sound(int channel, float globalTime, float timeStep)
{
    while(!nes.clock());
    return static_cast<float>(nes.audio_sample);
}

void updateWithAudio()
{

}

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

    std::shared_ptr<Cartridge> cartridge = std::make_shared<Cartridge>(argv[1]);
    std::string save_file;
    if(!cartridge->getValid())
        return 0;
    
    nes.insert_cartridge(cartridge);
    nes.setSampleFrequency(44100);
    nes.reset();

    sf::Texture texture;
    texture.create(256, 240);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::Vector2f targetSize(256.0f, 240.0f);

    NES::Sound::create(44100, 1, 8, 512);
    NES::Sound::setUserFunc(sound);

    nes.controller[0] = 0x00;

    while (window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    NES::Sound::destroy();
                    //save the prg ram to a file
                    save_file = std::string(argv[1]).substr(0, std::string(argv[1]).size() - 4) + ".sav";
                    nes.cartridge->save(save_file);
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code)
                    {
                        case sf::Keyboard::Key::X:
                            nes.controller[0] |= 0x80;
                            break;
                        case sf::Keyboard::Key::Z:
                            nes.controller[0] |= 0x40;
                            break;
                        case sf::Keyboard::Key::A:
                            nes.controller[0] |= 0x20;
                            break;
                        case sf::Keyboard::Key::S:
                            nes.controller[0] |= 0x10;
                            break;
                        case sf::Keyboard::Key::Up:
                            nes.controller[0] |= 0x08;
                            nes.controller[0] &= ~0x04;
                            break;
                        case sf::Keyboard::Key::Down:
                            nes.controller[0] |= 0x04;
                            nes.controller[0] &= ~0x08;
                            break;
                        case sf::Keyboard::Key::Left:
                            nes.controller[0] |= 0x02;
                            nes.controller[0] &= ~0x01;
                            break;
                        case sf::Keyboard::Key::Right:
                            nes.controller[0] |= 0x01;
                            nes.controller[0] &= ~0x02;
                            break;
                        case sf::Keyboard::Key::R:
                            nes.reset();
                            break;
                        case sf::Keyboard::Key::P:
                            //save the prg ram to a file
                            save_file = std::string(argv[1]).substr(0, std::string(argv[1]).size() - 4) + ".sav";
                            if(nes.cartridge->save(save_file))
                                std::cout << "Saved to " << save_file << std::endl;
                            else
                                std::cout << "Failed to save to " << save_file << std::endl;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch(event.key.code)
                    {
                        case sf::Keyboard::Key::X:
                            nes.controller[0] &= ~0x80;
                            break;
                        case sf::Keyboard::Key::Z:
                            nes.controller[0] &= ~0x40;
                            break;
                        case sf::Keyboard::Key::A:
                            nes.controller[0] &= ~0x20;
                            break;
                        case sf::Keyboard::Key::S:
                            nes.controller[0] &= ~0x10;
                            break;
                        case sf::Keyboard::Key::Up:
                            nes.controller[0] &= ~0x08;
                            break;
                        case sf::Keyboard::Key::Down:
                            nes.controller[0] &= ~0x04;
                            break;
                        case sf::Keyboard::Key::Left:
                            nes.controller[0] &= ~0x02;
                            break;
                        case sf::Keyboard::Key::Right:
                            nes.controller[0] &= ~0x01;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        
        updateWithAudio();

        targetSize = window.getView().getSize();

        texture.update(nes.ppu.getScreen());
        sprite.setScale(targetSize.x / sprite.getLocalBounds().width, targetSize.y / sprite.getLocalBounds().height);

        window.clear();

        window.draw(sprite);

        window.display();
    }

    return 0;
}
