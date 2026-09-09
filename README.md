### Controls

* <kbd>Space</kbd> Clear background
* <kbd>Q</kbd> Toggle shape (*Circle* / *Rectangle*)
* <kbd>Tab</kbd> Toggle color mode (*Random RGBA* / *Screen-mapped*)
* <kbd>R</kbd> Reset camera
* <kbd>Shift</kbd> Cycle texture tint
* <kbd>Left Click + Drag</kbd> Pan camera
* <kbd>Mouse Wheel</kbd> Zoom in / out

### Build & Run

```bash
gcc main.c -o main.exe -Wall -Iinclude -Llib -lraylib -lgdi32 -lwinmm && .\main.exe
