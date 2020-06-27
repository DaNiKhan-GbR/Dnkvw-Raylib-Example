/*******************************************************************************************
*
*   Based on raylib [core] example - 3d camera first person
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stdio.h>
#include <raylib.h>
#include <dnkvw/dnkvw.h>
#include "raymath.h"

#define MAX_COLUMNS 20

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "DaNiKhan V-Window Raylib example");
    IDnkvwHandle dnkvw = dnkvw_createContext();
    dnkvw_selectDnnTracker(dnkvw);
    dnkvw_configureFrustum(dnkvw, screenWidth / (float)screenHeight, 1.0f);
    dnkvw_startTracking(dnkvw, 0); // 0 is the camera ID

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;

    // Generates some random columns
    float heights[MAX_COLUMNS] = { 0.0f };
    Vector3 positions[MAX_COLUMNS] = { 0 };
    Color colors[MAX_COLUMNS] = { 0 };

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){ GetRandomValue(-15, 15), heights[i]/2, GetRandomValue(-15, 15) };
        colors[i] = (Color){ GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255 };
    }

    SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode
    
    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector3 eyeOffset;
        float dnkvwFps, left, right, top, bottom;
        char fpsText[50];
        dnkvw_loadEyeOffset(dnkvw, &eyeOffset.x, &eyeOffset.y, &eyeOffset.z);
        dnkvw_loadFrustum(dnkvw, &left, &right, &top, &bottom);
        dnkvw_loadFps(dnkvw, &dnkvwFps);

        snprintf(fpsText, 50, "FPS: %2d Tracking FPS: %02.2f", GetFPS(), dnkvwFps);
        
        UpdateCamera(&camera);                  // Update camera

        if (IsKeyPressed(KEY_SPACE))
        {
            dnkvw_calibrate(dnkvw);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                Vector3 oldPos = camera.position;
                camera.position = Vector3Add(oldPos, eyeOffset);
                Matrix projection = MatrixFrustum(left, right, bottom, top, 1.0f, 100.0f);
                SetMatrixProjection(projection); 

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

                // Draw some cubes around
                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
                }
                
                // Reset position
                camera.position = oldPos;

            EndMode3D();

            DrawRectangle( 10, 10, 220, 90, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 220, 90, BLUE);

            DrawText("First person camera default controls:", 20, 20, 10, BLACK);
            DrawText("- Move with keys: W, A, S, D", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse move to look around", 40, 60, 10, DARKGRAY);
            DrawText(fpsText, 40, 80, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    dnkvw_stopTracking(dnkvw);
    dnkvw_freeContext(&dnkvw);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
