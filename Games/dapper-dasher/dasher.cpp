#include "raylib.h"

struct AnimData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.position.y >= windowHeight - data.rectangle.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        data.rectangle.x = data.frame * data.rectangle.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }

    return data;
}

int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    // Scarfy variables
    int velocity{0};
    bool isInAir{false};
    const int jumpVelocity{-600};
    bool collision{};

    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");

    AnimData scarfyData;

    // rectangle
    scarfyData.rectangle.width = scarfyTexture.width / 6;
    scarfyData.rectangle.height = scarfyTexture.height;
    scarfyData.rectangle.x = 0;
    scarfyData.rectangle.y = 0;

    // position
    scarfyData.position.x = windowDimensions[0] / 2 - scarfyData.rectangle.width / 2;
    scarfyData.position.y = windowDimensions[1] - scarfyData.rectangle.width;

    // frames
    scarfyData.frame = 0;

    // running time and update time
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    // Nebula Variables
    int nebulaVelocity{-200};

    Texture2D nebulaTexture = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rectangle.x = 0.0;
        nebulae[i].rectangle.y = 0.0;
        nebulae[i].rectangle.width = nebulaTexture.width / 8;
        nebulae[i].rectangle.height = nebulaTexture.height / 8;

        nebulae[i].position.y = windowDimensions[1] - nebulaTexture.height / 8;
        nebulae[i].position.x = windowDimensions[0] + i * 300;

        nebulae[i].frame = 0;

        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
    }

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float backgroundXPosition{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float midgroundXPosition{};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float foregroundXPosition{};

    float finishLine{nebulae[sizeOfNebulae - 1].position.x};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time
        const float deltaTime{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // draw background
        backgroundXPosition -= 20 * deltaTime;
        midgroundXPosition -= 40 * deltaTime;
        foregroundXPosition -= 80 * deltaTime;

        if (backgroundXPosition <= -background.width * 2)
        {
            backgroundXPosition = 0.0;
        }
        if (midgroundXPosition <= -background.width * 2)
        {
            midgroundXPosition = 0.0;
        }
        if (foregroundXPosition <= -background.width * 2)
        {
            foregroundXPosition = 0.0;
        }

        Vector2 background1Position{backgroundXPosition, 0.0};
        Vector2 background2Position{backgroundXPosition + background.width * 2, 0.0};
        DrawTextureEx(background, background1Position, 0.0, 2.0, WHITE);
        DrawTextureEx(background, background2Position, 0.0, 2.0, WHITE);

        Vector2 midground1Position{midgroundXPosition, 0.0};
        Vector2 midground2Position{midgroundXPosition + background.width * 2, 0.0};
        DrawTextureEx(midground, midground1Position, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, midground2Position, 0.0, 2.0, WHITE);

        Vector2 foreground1Position{foregroundXPosition, 0.0};
        Vector2 foreground2Position{foregroundXPosition + background.width * 2, 0.0};
        DrawTextureEx(foreground, foreground1Position, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, foreground2Position, 0.0, 2.0, WHITE);

        // perform ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            // rectangle on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is on the air
            velocity += gravity * deltaTime;
            isInAir = true;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity = jumpVelocity;
        }

        // Update Nebulas  position
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].position.x += nebulaVelocity * deltaTime;
        }

        // update finish line
        finishLine += nebulaVelocity * deltaTime;

        // Update scarfyTexture position
        scarfyData.position.y += velocity * deltaTime;

        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, deltaTime, 5);
        }

        // Update nebulas animations
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], deltaTime, 7);
        }

     
        for (AnimData nebula : nebulae)
        {
            float padding{50};
            Rectangle nebulaRectangle{
                nebula.position.x + padding,
                nebula.position.y + padding,
                nebula.rectangle.width - 2 * padding,
                nebula.rectangle.height - 2 * padding};

            Rectangle scarfRectangle{
                scarfyData.position.x,
                scarfyData.position.y,
                scarfyData.rectangle.width,
                scarfyData.rectangle.height};

            if (CheckCollisionRecs(nebulaRectangle, scarfRectangle))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0]/4,windowDimensions[1]/2,40,WHITE);
        }
        else if (scarfyData.position.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0]/4,windowDimensions[1]/2,40,WHITE);
        }
        
        else
        {
            // Draw Nebulae;
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebulaTexture, nebulae[i].rectangle, nebulae[i].position, WHITE);
            }

            // Draw scarfyTexture
            DrawTextureRec(scarfyTexture, scarfyData.rectangle, scarfyData.position, WHITE);
        }

            // stop drawing
        EndDrawing();
    }
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    CloseAudioDevice();
    CloseWindow();
}