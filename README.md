It sucks.

There's nothing to read before opening, maybe basic controls:

1. Space to clear background
2. Q to change the "shape" - Circle / Rectangle
3. Tab changes the color - From random RGBA value to mapped by screen width and height
4. R to reset camera
5. Left click to drag the camera around
6. Mousewheel to zoom in / out
7. Shift to change Texture tint

Build:
/gcc main.c -o main.exe -Wall -Iinclude -Llib -lraylib -lgdi32 -lwinmm && .\main.exe
