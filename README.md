# SimpleNES
A simple NES Emulator. <br>
Thanks to <a href="https://www.youtube.com/@javidx9">javidx9</a> for the wonderful tutorial (and the opcode table). <br>
## Usage
Needs the SFML library to work, can be installed with:
```
sudo apt-get install libsfml-dev
```
Build with:
```
g++ src/*.cpp -lsfml-graphics -lsfml-system -lsfml-window -o <filename>
```
Use with:
```
./<filename> <rom_filename>
```
