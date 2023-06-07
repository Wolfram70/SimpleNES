#include <iostream>
#include <memory>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Joystick.hpp>

#include "../include/Bus.h"
#include "../include/Sound.h"

Bus nes;
std::string save_file;
bool joystick = false;
int players[] = {-1, -1};
int player0 = 0;

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
    nes.controller[1] = 0x00;
    window.setFramerateLimit(60);

    sf::Joystick::update();
    int joycount = 0;

    for(int i = 0; i < sf::Joystick::Count; i++)
    {
        if(sf::Joystick::isConnected(i))
        {
            std::cout << "Joystick connected on port " << i << " and set to player " << joycount << std::endl;
            joystick = true;
            players[joycount++] = i;
            if(joycount >= 2) break;
        }
    }

    save_file = std::string(argv[1]).substr(0, std::string(argv[1]).size() - 4) + ".sav";

    while (window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(!joystick)
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                        NES::Sound::destroy();
                        //save the prg ram to a file
                        nes.cartridge->save(save_file);
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        switch(event.key.code)
                        {
                            case sf::Keyboard::Key::X:
                                nes.controller[player0] |= 0x80;
                                break;
                            case sf::Keyboard::Key::Z:
                                nes.controller[player0] |= 0x40;
                                break;
                            case sf::Keyboard::Key::A:
                                nes.controller[player0] |= 0x20;
                                break;
                            case sf::Keyboard::Key::S:
                                nes.controller[player0] |= 0x10;
                                break;
                            case sf::Keyboard::Key::Up:
                                nes.controller[player0] |= 0x08;
                                nes.controller[player0] &= ~0x04;
                                break;
                            case sf::Keyboard::Key::Down:
                                nes.controller[player0] |= 0x04;
                                nes.controller[player0] &= ~0x08;
                                break;
                            case sf::Keyboard::Key::Left:
                                nes.controller[player0] |= 0x02;
                                nes.controller[player0] &= ~0x01;
                                break;
                            case sf::Keyboard::Key::Right:
                                nes.controller[player0] |= 0x01;
                                nes.controller[player0] &= ~0x02;
                                break;
                            case sf::Keyboard::Key::R:
                                nes.reset();
                                break;
                            case sf::Keyboard::Key::P:
                                //save the prg ram to a file
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
                                nes.controller[player0] &= ~0x80;
                                break;
                            case sf::Keyboard::Key::Z:
                                nes.controller[player0] &= ~0x40;
                                break;
                            case sf::Keyboard::Key::A:
                                nes.controller[player0] &= ~0x20;
                                break;
                            case sf::Keyboard::Key::S:
                                nes.controller[player0] &= ~0x10;
                                break;
                            case sf::Keyboard::Key::Up:
                                nes.controller[player0] &= ~0x08;
                                break;
                            case sf::Keyboard::Key::Down:
                                nes.controller[player0] &= ~0x04;
                                break;
                            case sf::Keyboard::Key::Left:
                                nes.controller[player0] &= ~0x02;
                                break;
                            case sf::Keyboard::Key::Right:
                                nes.controller[player0] &= ~0x01;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                        NES::Sound::destroy();
                        //save the prg ram to a file
                        nes.cartridge->save(save_file);
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        if(players[1] == -1)
                        {
                            switch(event.key.code)
                            {
                                case sf::Keyboard::Key::X:
                                    nes.controller[(player0 + 1) % 2] |= 0x80;
                                    break;
                                case sf::Keyboard::Key::Z:
                                    nes.controller[(player0 + 1) % 2] |= 0x40;
                                    break;
                                case sf::Keyboard::Key::A:
                                    nes.controller[(player0 + 1) % 2] |= 0x20;
                                    break;
                                case sf::Keyboard::Key::S:
                                    nes.controller[(player0 + 1) % 2] |= 0x10;
                                    break;
                                case sf::Keyboard::Key::Up:
                                    nes.controller[(player0 + 1) % 2] |= 0x08;
                                    nes.controller[(player0 + 1) % 2] &= ~0x04;
                                    break;
                                case sf::Keyboard::Key::Down:
                                    nes.controller[(player0 + 1) % 2] |= 0x04;
                                    nes.controller[(player0 + 1) % 2] &= ~0x08;
                                    break;
                                case sf::Keyboard::Key::Left:
                                    nes.controller[(player0 + 1) % 2] |= 0x02;
                                    nes.controller[(player0 + 1) % 2] &= ~0x01;
                                    break;
                                case sf::Keyboard::Key::Right:
                                    nes.controller[(player0 + 1) % 2] |= 0x01;
                                    nes.controller[(player0 + 1) % 2] &= ~0x02;
                                    break;
                                case sf::Keyboard::Key::R:
                                    nes.reset();
                                    break;
                                case sf::Keyboard::Key::P:
                                    //save the prg ram to a file
                                    if(nes.cartridge->save(save_file))
                                        std::cout << "Saved to " << save_file << std::endl;
                                    else
                                        std::cout << "Failed to save to " << save_file << std::endl;
                                    break;
                                default:
                                    break;
                            }
                        }
                        break;
                    case sf::Event::KeyReleased:
                        if(players[1] == -1)
                        {
                            switch(event.key.code)
                            {
                                case sf::Keyboard::Key::X:
                                    nes.controller[(player0 + 1) % 2] &= ~0x80;
                                    break;
                                case sf::Keyboard::Key::Z:
                                    nes.controller[(player0 + 1) % 2] &= ~0x40;
                                    break;
                                case sf::Keyboard::Key::A:
                                    nes.controller[(player0 + 1) % 2] &= ~0x20;
                                    break;
                                case sf::Keyboard::Key::S:
                                    nes.controller[(player0 + 1) % 2] &= ~0x10;
                                    break;
                                case sf::Keyboard::Key::Up:
                                    nes.controller[(player0 + 1) % 2] &= ~0x08;
                                    break;
                                case sf::Keyboard::Key::Down:
                                    nes.controller[(player0 + 1) % 2] &= ~0x04;
                                    break;
                                case sf::Keyboard::Key::Left:
                                    nes.controller[(player0 + 1) % 2] &= ~0x02;
                                    break;
                                case sf::Keyboard::Key::Right:
                                    nes.controller[(player0 + 1) % 2] &= ~0x01;
                                    break;
                                default:
                                    break;
                            }
                        }
                        break;
                    default:
                        //joystick controls
                        sf::Joystick::update();
                        if(sf::Joystick::isButtonPressed(players[0], 0))
                            nes.controller[player0] |= 0x80;
                        else
                            nes.controller[player0] &= ~0x80;
                        if(sf::Joystick::isButtonPressed(players[0], 1))
                            nes.controller[player0] |= 0x40;
                        else
                            nes.controller[player0] &= ~0x40;
                        if(sf::Joystick::isButtonPressed(players[0], 6))
                            nes.controller[player0] |= 0x20;
                        else
                            nes.controller[player0] &= ~0x20;
                        if(sf::Joystick::isButtonPressed(players[0], 7))
                            nes.controller[player0] |= 0x10;
                        else
                            nes.controller[0] &= ~0x10;
                        if(sf::Joystick::getAxisPosition(players[0], sf::Joystick::Axis::Y) < -50)
                        {
                            nes.controller[player0] |= 0x08;
                            nes.controller[player0] &= ~0x04;
                        }
                        else if(sf::Joystick::getAxisPosition(players[0], sf::Joystick::Axis::Y) > 50)
                        {
                            nes.controller[player0] |= 0x04;
                            nes.controller[player0] &= ~0x08;
                        }
                        else
                        {
                            nes.controller[player0] &= ~0x08;
                            nes.controller[player0] &= ~0x04;
                        }
                        if(sf::Joystick::getAxisPosition(players[0], sf::Joystick::Axis::X) < -50)
                        {
                            nes.controller[player0] |= 0x02;
                            nes.controller[player0] &= ~0x01;
                        }
                        else if(sf::Joystick::getAxisPosition(players[0], sf::Joystick::Axis::X) > 50)
                        {
                            nes.controller[player0] |= 0x01;
                            nes.controller[player0] &= ~0x02;
                        }
                        else
                        {
                            nes.controller[player0] &= ~0x02;
                            nes.controller[player0] &= ~0x01;
                        }

                        if(players[1] >= 0)
                        {
                            if(sf::Joystick::isButtonPressed(players[1], 0))
                                nes.controller[(player0 + 1) % 2] |= 0x80;
                            else
                                nes.controller[(player0 + 1) % 2] &= ~0x80;
                            if(sf::Joystick::isButtonPressed(players[1], 1))
                                nes.controller[(player0 + 1) % 2] |= 0x40;
                            else
                                nes.controller[(player0 + 1) % 2] &= ~0x40;
                            if(sf::Joystick::isButtonPressed(players[1], 6))
                                nes.controller[(player0 + 1) % 2] |= 0x20;
                            else
                                nes.controller[(player0 + 1) % 2] &= ~0x20;
                            if(sf::Joystick::isButtonPressed(players[1], 7))
                                nes.controller[(player0 + 1) % 2] |= 0x10;
                            else
                                nes.controller[0] &= ~0x10;
                            if(sf::Joystick::getAxisPosition(players[1], sf::Joystick::Axis::Y) < -50)
                            {
                                nes.controller[(player0 + 1) % 2] |= 0x08;
                                nes.controller[(player0 + 1) % 2] &= ~0x04;
                            }
                            else if(sf::Joystick::getAxisPosition(players[1], sf::Joystick::Axis::Y) > 50)
                            {
                                nes.controller[(player0 + 1) % 2] |= 0x04;
                                nes.controller[(player0 + 1) % 2] &= ~0x08;
                            }
                            else
                            {
                                nes.controller[(player0 + 1) % 2] &= ~0x08;
                                nes.controller[(player0 + 1) % 2] &= ~0x04;
                            }
                            if(sf::Joystick::getAxisPosition(players[1], sf::Joystick::Axis::X) < -50)
                            {
                                nes.controller[(player0 + 1) % 2] |= 0x02;
                                nes.controller[(player0 + 1) % 2] &= ~0x01;
                            }
                            else if(sf::Joystick::getAxisPosition(players[1], sf::Joystick::Axis::X) > 50)
                            {
                                nes.controller[(player0 + 1) % 2] |= 0x01;
                                nes.controller[(player0 + 1) % 2] &= ~0x02;
                            }
                            else
                            {
                                nes.controller[(player0 + 1) % 2] &= ~0x02;
                                nes.controller[(player0 + 1) % 2] &= ~0x01;
                            }
                        }
                        break;
                }
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
