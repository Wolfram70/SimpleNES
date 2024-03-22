# SimpleNES
A simple NES Emulator with multiplayer capability on LAN. <br>
Thanks to <a href="https://www.youtube.com/@javidx9">javidx9</a> for the wonderful tutorial (and the opcode table) that helped me get started. <br>
## Usage
Needs SFML and ALSA to work, can be installed with:
```
sudo apt-get install libsfml-dev
sudo apt-get install libasound2
```
Build with:
```
g++ src/*.cpp -lsfml-graphics -lsfml-system -lsfml-window -lasound -o <filename>
```
Use with:
```
./<filename> <rom_filename>
```
