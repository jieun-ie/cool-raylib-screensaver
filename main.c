#include <raylib.h>
#include <stdint.h>

#define MAX_OBJECT 1024
#define RADIUS 1.0f
#define RECT_SIZE 1

#define MAX_SPEED 400
#define MIN_SPEED 100

typedef enum
{
    COLOR_RANDOM,
    COLOR_MAPPED,
} State;

typedef enum
{
   RECT,
   CIRCLE,
}Shape;

typedef enum
{
    white,
    red,
    green,
    blue,
    pink,
    yellow,
} Tints;

Color tint = WHITE;

Vector2 GetScreenSize()
{
    int32_t monitor = GetCurrentMonitor();
    int32_t width = GetMonitorWidth(monitor);
    int32_t height = GetMonitorHeight(monitor);

    Vector2 size = { (float)width, (float)height };

    return size;
}

void InitMonitor()
{
    int32_t monitor = GetCurrentMonitor();
    int32_t hz = GetMonitorRefreshRate(monitor);

    if (hz <= 0) // FOR SAFETY IF GETTING HZ FAILS
    {
        hz = 60;
    }

    SetTargetFPS(hz);
    return;
}

void LoadWindowConfig()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
}

void CheckAltTab()
{
    if ((IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) && IsKeyPressed(KEY_TAB))
    {
        MinimizeWindow();
    }
}

Camera2D InitCamera(Vector2 screen_size)
{
    Camera2D camera = { 0 };
    camera.target = (Vector2){ screen_size.x / 2.0f, screen_size.y / 2.0f };
    camera.offset = (Vector2){ screen_size.x / 2.0f, screen_size.y / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    return camera;
}

void HandleCameraZoom(Camera2D *camera)
{
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

        camera->offset = GetMousePosition();
        camera->target = mouseWorldPos;

        float zoom = 0.125f;
        camera->zoom += (wheel * zoom);

        if (camera->zoom < 1.0f) camera->zoom = 1.0f;
        if (camera->zoom > 50.0f) camera->zoom = 50.0f;
    }
}

Tints TintChange(Tints tints)
{
    switch (tints)
    {
        case white:
            tint = RED;
            return red;
        case red:
            tint = GREEN;
            return green;
        case green:
            tint = BLUE;
            return blue;
        case blue:
            tint = PINK;
            return pink;
        case pink:
            tint = YELLOW;
            return yellow;
        case yellow:
            tint = WHITE;
            return white;
        default:
            tint = WHITE;
            return white;
    }
}

int main()
{
    int speed[MAX_OBJECT];
    Color colors[MAX_OBJECT];
    Color color_state = WHITE;
    State state = COLOR_RANDOM;
    Shape shape = CIRCLE;
    Vector2 object[MAX_OBJECT];
    Vector2 screen_size;
    Tints titties = white;

    LoadWindowConfig();

    InitWindow(1280, 720, "Title");

    screen_size = GetScreenSize();

    SetWindowSize((int)screen_size.x, (int)screen_size.y);
    ToggleBorderlessWindowed();

    InitMonitor();

    Camera2D camera = InitCamera(screen_size);

    RenderTexture2D canvas = LoadRenderTexture((int)screen_size.x, (int)screen_size.y);

    for (int i = 0; i < MAX_OBJECT; i++)
    {
        object[i] = (Vector2){
            (float)GetRandomValue(0, (int)screen_size.x),
            (float)GetRandomValue(0, (int)screen_size.y)
        };
        speed[i] = GetRandomValue(MIN_SPEED, MAX_SPEED);
        colors[i] = (Color){
            (unsigned char)GetRandomValue(0, 255),
            (unsigned char)GetRandomValue(0, 255),
            (unsigned char)GetRandomValue(0, 255),
            255
        };
    }

    BeginTextureMode(canvas);
        ClearBackground(BLACK);
    EndTextureMode();

    while (!WindowShouldClose())
    {
        CheckAltTab();

        float delta = GetFrameTime();

        if (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT))
        {
            titties = TintChange(titties);
        }

        if (IsKeyPressed(KEY_TAB))
        {
            if (state == COLOR_RANDOM)
            {
                state = COLOR_MAPPED;
            }
            else
            {
                state = COLOR_RANDOM;
            }

            BeginTextureMode(canvas);
                ClearBackground(BLACK);
            EndTextureMode();
        }

        HandleCameraZoom(&camera);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouseDelta = GetMouseDelta();

            mouseDelta.x /= camera.zoom;
            mouseDelta.y /= camera.zoom;

            camera.target = (Vector2){ camera.target.x - mouseDelta.x, camera.target.y - mouseDelta.y };
        }

        if (IsKeyPressed(KEY_R))
        {
            camera = InitCamera(screen_size);
        }

        BeginTextureMode(canvas);
            if (IsKeyPressed(KEY_Q))
            {
                ClearBackground(BLACK);
                if (shape == CIRCLE)
                {
                    shape = RECT;
                }
                else
                {
                    shape = CIRCLE;
                }
            }

            if (IsKeyDown(KEY_SPACE))
            {
                ClearBackground(BLACK);
            }

            for (int i = 0; i < MAX_OBJECT; i++)
            {
                object[i].x += (speed[i] * GetRandomValue(-1, 1)) * delta;
                object[i].y += (speed[i] * GetRandomValue(-1, 1)) * delta;

                if (object[i].x > screen_size.x || object[i].y > screen_size.y || object[i].x < 0 || object[i].y < 0)
                {
                    object[i] = (Vector2){
                        (float)GetRandomValue(0, (int)screen_size.x),
                        (float)GetRandomValue(0, (int)screen_size.y)
                    };
                }

                switch (state)
                {
                    case COLOR_MAPPED:
                    {
                        unsigned char r = (unsigned char)((object[i].x / screen_size.x) * 255.0f);
                        unsigned char g = (unsigned char)((object[i].y / screen_size.y) * 255.0f);
                        color_state = (Color){ r, g, 150, 255 };
                        break;
                    }
                    case COLOR_RANDOM:
                        color_state = colors[i];
                        break;
                }

                switch (shape)
                {
                    case CIRCLE:
                    {
                        DrawCircleV(object[i], RADIUS, color_state);
                        break;
                    }
                    case RECT:
                    {
                        DrawRectangleV(object[i],
                        (Vector2){ RECT_SIZE + GetRandomValue(-2, 2), RECT_SIZE + GetRandomValue(-2, 2)}, color_state);
                        break;
                    }
                }
            }
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
                DrawTextureRec(
                    canvas.texture,
                    (Rectangle){ 0, 0, (float)canvas.texture.width, (float)-canvas.texture.height },
                    (Vector2){ 0, 0 },
                    tint
                );
            EndMode2D();
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    CloseWindow();

    return 0;
}
