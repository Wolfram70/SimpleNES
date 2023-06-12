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

#define _XOPEN_SOURCE_EXTENDED 1

#define COMPORT 2016
#define SYNCPORT 2023

#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <chrono>

Bus nes;
std::string save_file;
bool joystick = false;
int players[] = {-1, -1};
int player0 = 0;
sf::RenderWindow window(sf::VideoMode(256.0f, 240.0f), "NES Emulator");

//communication for multiplayer
bool multi = false;
int mySocket;
struct sockaddr_in myAddr;
int otherSocket;
struct sockaddr_in otherAddr;
int syncSocketServ;
struct sockaddr_in syncAddrServ;
int syncSocket;
struct sockaddr_in syncAddr;
bool synced = false;
int delay = 0;

std::thread commThread;
std::thread syncThread;

float sound(int channel, float globalTime, float timeStep)
{
    while((!synced) && multi);
    while(!nes.clock());
    return static_cast<float>(nes.audio_sample);
}

void singleSys(sf::Event& event)
{
    //TODO: Fix problem with player0 = 1, keyboard input buffering???
    if(!joystick)
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                NES::Sound::destroy();
                //save the prg ram to a file
                nes.cartridge->save(save_file);
                window.close();
                //close the connection if multiplayer enabled
                if(multi)
                {
                    close(otherSocket);
                    close(mySocket);
                    close(syncSocketServ);
                    close(syncSocket);
                }
                multi = false;
                //wait for the thread to finish
                commThread.join();
                syncThread.join();
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
                //close the connection if multiplayer enabled
                if(multi)
                {
                    close(otherSocket);
                    close(mySocket);
                    close(syncSocketServ);
                    close(syncSocket);
                }
                multi = false;
                //wait for the thread to finish
                commThread.join();
                syncThread.join();
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
                    nes.controller[player0] &= ~0x10;
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
                        nes.controller[(player0 + 1) % 2] &= ~0x10;
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

void multiSys(sf::Event& event)
{
    bool valid;
    if(!joystick)
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                NES::Sound::destroy();
                //save the prg ram to a file
                nes.cartridge->save(save_file);
                window.close();
                //close the connection if multiplayer enabled
                if(multi)
                {
                    close(otherSocket);
                    close(mySocket);
                    close(syncSocketServ);
                    close(syncSocket);
                }
                multi = false;
                //wait for the thread to finish
                commThread.join();
                syncThread.join();
                break;
            case sf::Event::KeyPressed:
                valid = true;
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
                        valid = false;
                        break;
                }
                if(valid)
                {
                    //send the input to the other player
                    // std::cout << "Sent input!!" << std::endl;
                    char out = (char) nes.controller[player0];
                    int status = send(otherSocket, &out, 1, 0);
                    if(status == 0)
                    {
                        std::cout << "Connection closed!" << std::endl;
                        multi = false;
                        break;
                    }
                    else if(status == -1)
                    {
                        std::cout << "Error sending data!" << std::endl;
                        multi = false;
                        break;
                    }
                }
                break;
            case sf::Event::KeyReleased:
                valid = true;
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
                        valid = false;
                        break;
                }
                if(valid)
                {
                    //send the input to the other player
                    // std::cout << "Sent input!!" << std::endl;
                    char out = (char) nes.controller[player0];
                    int status = send(otherSocket, &out, 1, 0);
                    if(status == 0)
                    {
                        std::cout << "Connection closed!" << std::endl;
                        multi = false;
                        break;
                    }
                    else if(status == -1)
                    {
                        std::cout << "Error sending data!" << std::endl;
                        multi = false;
                        break;
                    }
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
                //close the connection if multiplayer enabled
                if(multi)
                {
                    close(otherSocket);
                    close(mySocket);
                    close(syncSocketServ);
                    close(syncSocket);
                }
                multi = false;
                //wait for the thread to finish
                commThread.join();
                syncThread.join();
                break;
            default:
                uint8_t before = nes.controller[player0];
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
                    nes.controller[player0] &= ~0x10;
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

                if(before != nes.controller[player0])
                {
                    //send the input to the other player
                    // std::cout << "Sent input!!" << std::endl;
                    char out = (char) nes.controller[player0];
                    int status = send(otherSocket, &out, 1, 0);
                    if(status == 0)
                    {
                        std::cout << "Connection closed!" << std::endl;
                        multi = false;
                        break;
                    }
                    else if(status == -1)
                    {
                        std::cout << "Error sending data!" << std::endl;
                        multi = false;
                        break;
                    }
                }
                break;
        }
    }
}

void takeInput(sf::Event& event)
{
    if(multi)
    {
        multiSys(event);
    }
    else
    {
        singleSys(event);
    }
}

void syncEachOther(int syncSocket)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(delay));
    uint32_t myClockCount, othClockCount, othClockCountPrev = 0, myClockCountPrev = 0;
    uint8_t ram_buf[2048];
    int status;

    while(true)
    {
        if(!multi) break;
        //every 5 seconds sync up
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        if(dur.count() >= 1)
        {
            synced = false;
            char p_sync = 51;

            //if server
            if(player0 == 0)
            {
                //sync clocks
                myClockCount = nes.nSystemClockCounter;
                status = send(syncSocket, &myClockCount, sizeof(myClockCount), 0);
                status = recv(syncSocket, &othClockCount, sizeof(othClockCount), 0);

                if(status == 0)
                {
                    std::cout << "Sync connection closed!" << std::endl;
                    multi = false;
                    break;
                }
                else if(status == -1)
                {
                    std::cout << "Error sending data! (sync)" << std::endl;
                    multi = false;
                    break;
                }

                if(othClockCountPrev > othClockCount)
                {
                    //overflow
                    if(myClockCount > othClockCount && myClockCount >= myClockCountPrev)
                    {
                        while(myClockCount != othClockCount)
                        {
                            nes.clock();
                            myClockCount++;
                        }
                    }
                    else if(myClockCount < othClockCount && myClockCount < myClockCountPrev)
                    {
                        while(myClockCount != othClockCount)
                        {
                            nes.clock();
                            myClockCount++;
                        }
                    }
                }
                else
                {
                    if(myClockCount < othClockCount && myClockCount >= myClockCountPrev)
                    {
                        while(myClockCount != othClockCount)
                        {
                            nes.clock();
                            myClockCount++;
                        }
                    }
                }

                othClockCountPrev = othClockCount;
                myClockCountPrev = myClockCount;

                //sync ram
                for(int i = 0; i < 2048; i++)
                    ram_buf[i] = nes.cpu_ram[i];
                status = send(syncSocket, (char*) ram_buf, 2048, 0);
                status = recv(syncSocket, &p_sync, 1, 0);

                if(status == 0)
                {
                    std::cout << "Sync connection closed!" << std::endl;
                    multi = false;
                    break;
                }
                else if(status == -1)
                {
                    std::cout << "Error sending data! (sync)" << std::endl;
                    multi = false;
                    break;
                }

                //sync time
                auto syncStart = std::chrono::high_resolution_clock::now();
                status = send(syncSocket, &p_sync, 1, 0);
                status = recv(syncSocket, &p_sync, 1, 0);
                auto syncEnd = std::chrono::high_resolution_clock::now();

                auto syncDur = std::chrono::duration_cast<std::chrono::microseconds>(syncEnd - syncStart);
                delay = syncDur.count() / 2;

                //send delay to other player
                status = send(syncSocket, &delay, sizeof(delay), 0);

                if(status == 0)
                {
                    std::cout << "Sync connection closed!" << std::endl;
                    multi = false;
                    break;
                }
                else if(status == -1)
                {
                    std::cout << "Error sending data! (sync)" << std::endl;
                    multi = false;
                    break;
                }

                //wait for delay time
                std::this_thread::sleep_for(std::chrono::microseconds(delay));
            }
            else //if client
            {
                //sync clocks
                myClockCount = nes.nSystemClockCounter;
                status = recv(syncSocket, &othClockCount, sizeof(othClockCount), 0);
                status = send(syncSocket, &myClockCount, sizeof(myClockCount), 0);

                if(status == 0)
                {
                    std::cout << "Sync connection closed!" << std::endl;
                    multi = false;
                    break;
                }
                else if(status == -1)
                {
                    std::cout << "Error sending data! (sync)" << std::endl;
                    multi = false;
                    break;
                }

                if(othClockCountPrev > othClockCount)
                {
                    //overflow
                    if(myClockCount > othClockCount && myClockCount >= myClockCountPrev)
                    {
                        while(myClockCount != othClockCount)
                        {
                            nes.clock();
                            myClockCount++;
                        }
                    }
                    else if(myClockCount < othClockCount && myClockCount < myClockCountPrev)
                    {
                        while(myClockCount != othClockCount)
                        {
                            nes.clock();
                            myClockCount++;
                        }
                    }
                }
                else
                {
                    if(myClockCount < othClockCount && myClockCount >= myClockCountPrev)
                    {
                        while(myClockCount != othClockCount)
                        {
                            nes.clock();
                            myClockCount++;
                        }
                    }
                }

                othClockCountPrev = othClockCount;
                myClockCountPrev = myClockCount;

                //sync ram
                status = recv(syncSocket, (char*) ram_buf, 2048, 0);
                for(int i = 0; i < 2048; i++)
                    nes.cpu_ram[i] = ram_buf[i];
                status = send(syncSocket, &p_sync, 1, 0);

                if(status == 0)
                {
                    std::cout << "Sync connection closed!" << std::endl;
                    multi = false;
                    break;
                }
                else if(status == -1)
                {
                    std::cout << "Error sending data! (sync)" << std::endl;
                    multi = false;
                    break;
                }

                //sync time
                status = recv(syncSocket, &p_sync, 1, 0);
                status = send(syncSocket, &p_sync, 1, 0);

                //recieve delay from other player
                status = recv(syncSocket, &delay, sizeof(delay), 0);

                if(status == 0)
                {
                    std::cout << "Sync connection closed!" << std::endl;
                    multi = false;
                    break;
                }
                else if(status == -1)
                {
                    std::cout << "Error receiving data! (sync)" << std::endl;
                    multi = false;
                    break;
                }
            }

            synced = true;
            std::cout << "Synced! (with delay of " << delay << "mus)" <<  std::endl;

            start = std::chrono::high_resolution_clock::now();
        }
    }
    // std::cout << "Sync Thread End!" << std::endl;
}

void waitForInput(int otherSocket)
{
    char buf;
    std::cout << "Logging for " << (player0 + 1) % 2 << " input!" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(delay));
    while (true)
    {
        if(!multi) break;

        int status = recv(otherSocket, &buf, 1, 0);
        if(status == 0)
        {
            std::cout << "Connection closed!" << std::endl;
            multi = false;
            break;
        }
        else if(status == -1)
        {
            std::cout << "Error receiving data!" << std::endl;
            multi = false;
            break;
        }

        // std::cout << "Recieved input!" << std::endl;
        nes.controller[(player0 + 1) % 2] = (uint8_t) buf;
    }
    // std::cout << "Input Logging Thread End!" << std::endl;
}

bool askUserMulti()
{
    std::cout << "Would you like to play multiplayer? (y/n)" << std::endl;
    char c;
    std::cin >> c;
    if(c == 'y')
    {
        std::cout << "Would like to join a game or host a game? (j/h)" << std::endl;
        std::cout << "NOTE: Depending on the choice, you will not be able to utlize the controller for the other player." << std::endl;
        std::cin >> c;

        if(c == 'j')
        {
            std::cout << "Enter the IP address of the host: ";
            std::string ip;
            std::cin >> ip;
            std::cout << "Connecting to " << ip << std::endl;

            player0 = 1;

            //connect to the host
            otherSocket = socket(AF_INET, SOCK_STREAM, 0);

            bzero((char*)&otherAddr, sizeof(otherAddr));
            otherAddr.sin_family = AF_INET;
            otherAddr.sin_port = htons(COMPORT);
            otherAddr.sin_addr.s_addr = inet_addr(ip.c_str());

            int status = connect(otherSocket, (struct sockaddr*)&otherAddr, sizeof(otherAddr));

            if(status == -1)
            {
                std::cout << "Error connecting to host!" << std::endl;
                return askUserMulti();
            }

            std::cout << "Connected!!" << std::endl;

            //create the sync socket
            syncSocket = socket(AF_INET, SOCK_STREAM, 0);

            bzero((char*)&syncAddr, sizeof(syncAddr));
            syncAddr.sin_family = AF_INET;
            syncAddr.sin_port = htons(SYNCPORT);
            syncAddr.sin_addr.s_addr = inet_addr(ip.c_str());

            status = connect(syncSocket, (struct sockaddr*)&syncAddr, sizeof(syncAddr));

            if(status == -1)
            {
                std::cout << "Error connecting to host sync!" << std::endl;
                return askUserMulti();
            }

            std::cout << "Connected to sync socket!" << std::endl;

            //recieve and send sync packet
            char p_sync = 51;

            status = recv(syncSocket, &p_sync, 1, 0);
            status = send(syncSocket, &p_sync, 1, 0);

            //recieve delay from other player
            status = recv(syncSocket, &delay, sizeof(delay), 0);

            synced = true;
            std::cout << "Synced! (with delay of " << delay << "mus)" <<  std::endl;

            //start the game and sync threads
            commThread = std::thread(waitForInput, otherSocket);
            syncThread = std::thread(syncEachOther, syncSocket);
        }
        else if(c == 'h')
        {
            std::cout << "Hosting on port " << COMPORT << std::endl;
            std::cout << "Sync port is " << SYNCPORT << std::endl;

            player0 = 0;

            //host the game
            mySocket = socket(AF_INET, SOCK_STREAM, 0);

            bzero((char*)&myAddr, sizeof(myAddr));
            myAddr.sin_family = AF_INET;
            myAddr.sin_port = htons(COMPORT);

            bind(mySocket, (struct sockaddr*)&myAddr, sizeof(myAddr));

            listen(mySocket, 1);

            bzero((char*)&otherAddr, sizeof(otherAddr));
            socklen_t otherAddrSize = sizeof(otherAddr);

            //create the sync socket
            syncSocketServ = socket(AF_INET, SOCK_STREAM, 0);

            bzero((char*)&syncAddrServ, sizeof(syncAddrServ));
            syncAddrServ.sin_family = AF_INET;
            syncAddrServ.sin_port = htons(SYNCPORT);

            bind(syncSocketServ, (struct sockaddr*)&syncAddrServ, sizeof(syncAddrServ));

            listen(syncSocketServ, 1);

            bzero((char*)&syncAddr, sizeof(syncAddr));
            socklen_t syncAddrSize = sizeof(syncAddr);

            //accept connections

            otherSocket = accept(mySocket, (struct sockaddr*)&otherAddr, &otherAddrSize);

            if(otherSocket == -1)
            {
                std::cout << "Error accepting connection!" << std::endl;
                return askUserMulti();
            }

            syncSocket = accept(syncSocketServ, (struct sockaddr*)&syncAddr, &syncAddrSize);

            if(syncSocket == -1)
            {
                std::cout << "Error accepting sync connection!" << std::endl;
                return askUserMulti();
            }

            //measure time between send and recieve of sync over syncSocket
            char p_sync = 51;

            auto start = std::chrono::high_resolution_clock::now();
            int status = send(syncSocket, &p_sync, 1, 0);
            status = recv(syncSocket, &p_sync, 1, 0);
            auto end = std::chrono::high_resolution_clock::now();

            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            delay = dur.count() / 2;

            //send delay to other player
            status = send(syncSocket, &delay, sizeof(delay), 0);

            //wait for delay time
            std::this_thread::sleep_for(std::chrono::microseconds(delay));

            synced = true;
            std::cout << "Synced! (with delay of " << delay << "mus)" <<  std::endl;

            //start the game and sync threads
            commThread = std::thread(waitForInput, otherSocket);
            syncThread = std::thread(syncEachOther, syncSocket);
        }
        else
        {
            std::cout << "Invalid input!" << std::endl;
            return askUserMulti();
        }

        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
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

    multi = askUserMulti();
    NES::Sound::setUserFunc(sound);

    save_file = std::string(argv[1]).substr(0, std::string(argv[1]).size() - 4) + ".sav";

    while (window.isOpen())
    {   
        sf::Event event;
        while(window.pollEvent(event))
        {
            takeInput(event);
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
