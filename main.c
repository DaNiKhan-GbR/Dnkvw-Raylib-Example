//////////////////////////////////////////////////////////////////////////////////////
//  BSD 3-Clause License                                                            //
//                                                                                  //
//  Copyright (c) 2020, Daniel Klintworth and Nicolas Hollmann                      //
//  All rights reserved.                                                            //
//                                                                                  //
//  Redistribution and use in source and binary forms, with or without              //
//  modification, are permitted provided that the following conditions are met:     //
//                                                                                  //
//  * Redistributions of source code must retain the above copyright notice, this   //
//    list of conditions and the following disclaimer.                              //
//                                                                                  //
//  * Redistributions in binary form must reproduce the above copyright notice,     //
//    this list of conditions and the following disclaimer in the documentation     //
//    and/or other materials provided with the distribution.                        //
//                                                                                  //
//  * Neither the name of the copyright holder nor the names of its                 //
//    contributors may be used to endorse or promote products derived from          //
//    this software without specific prior written permission.                      //
//                                                                                  //
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     //
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       //
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  //
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    //
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      //
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      //
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      //
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   //
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   //
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************************
*
*   Dnkvw Raylib Minimal Example
*   Demonstrates the simplest possible use of dnkvw.
*
*   Based on raylib [core] example - 3d camera first person
*   The example was modified to use the DaNiKhan V-Window Library.
*
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*   
*   This software is provided "as-is", without any express or implied warranty. In no event 
*   will the authors be held liable for any damages arising from the use of this software.
*   
*   Permission is granted to anyone to use this software for any purpose, including commercial 
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*   
*     1. The origin of this software must not be misrepresented; you must not claim that you 
*     wrote the original software. If you use this software in a product, an acknowledgment 
*     in the product documentation would be appreciated but is not required.
*   
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*   
*     3. This notice may not be removed or altered from any source distribution.
*
********************************************************************************************/

#include <stdio.h>
#include <raylib.h>
#include <dnkvw/dnkvw.h>
#include "raymath.h"

#define MAX_COLUMNS 20

int main(void)
{
    // Constants
    //--------------------------------------------------------------------------------------
    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int cameraId = 0;
    const float nearPlane = 0.1f;
    const float farPlane = 100.0f;

    // Initialization
    //--------------------------------------------------------------------------------------

    // Try to enable VSync and MSAA.
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);

    // Create a raylib window.
    InitWindow(windowWidth, windowHeight, "DaNiKhan V-Window Raylib example");

    // Create a new dnkvw Context.
    IDnkvwHandle dnkvw = dnkvw_createContext();

    // Select a face tracker. DNN is the most stable.
    dnkvw_selectDnnTracker(dnkvw);

    // Tell dnkvw the aspect ratio and the near plane of the viewing frustum.
    dnkvw_configureFrustum(dnkvw, windowWidth / (float)windowHeight, nearPlane);

    // Start the tracking (needs camera access).
    dnkvw_startTracking(dnkvw, cameraId);

    const int monitorWidth = GetMonitorWidth(0);
    const int monitorHeight = GetMonitorHeight(0);

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

    // Set a first person camera mode
    SetCameraMode(camera, CAMERA_FIRST_PERSON);
    
    // Set our game to run at 60 frames-per-second
    SetTargetFPS(60);

    // Main game loop
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector3 eyeOffset;
        float dnkvwFps, left, right, top, bottom;
        char fpsText[50];

        // Load all calculated values into local variables.
        dnkvw_loadEyeOffset(dnkvw, &eyeOffset.x, &eyeOffset.y, &eyeOffset.z);
        dnkvw_loadFrustum(dnkvw, &left, &right, &top, &bottom);
        dnkvw_loadFps(dnkvw, &dnkvwFps);

    	// Prepare the FPS output string.
        snprintf(fpsText, 50, "FPS: %2d Tracking FPS: %02.2f", GetFPS(), dnkvwFps);
        
        UpdateCamera(&camera);                  

        if (IsKeyPressed(KEY_SPACE))
        {
            // The camera calibration resets the tracking origin.
            // The user should sit straight in front of the camera and not move for a second.
            dnkvw_calibrate(dnkvw);
        }

        if (IsKeyPressed(KEY_F2))
        {
            ToggleFullscreen();

            if (IsWindowFullscreen())
            {
                printf("Monitor Count: %d Width: %d Height: %d", GetMonitorCount(), monitorWidth, monitorHeight);
                SetWindowSize(
                    monitorWidth,
                    monitorHeight
                );
            }
            else
            {
                SetWindowSize(windowWidth, windowHeight);
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // 3D World
            //----------------------------------------------------------------------------------

            BeginMode3D(camera);
                
                // Save old camera position and apply the tracked eye offset.
                Vector3 oldPos = camera.position;
                camera.position = Vector3Add(oldPos, eyeOffset);

                // Use raymath to calculate a projection matrix using the tracked values.
                Matrix projection = MatrixFrustum(left, right, bottom, top, nearPlane, farPlane);

                // Configure the camera to use the calculated projection matrix.
                SetMatrixProjection(projection); 

                // Draw the room
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
                
                // Reset position to prevent unwanted drifting.
                // The eye offset should only be active for rendering and not be used for
                // movement calculations.
                camera.position = oldPos;

            EndMode3D();
            
            // GUI
            //----------------------------------------------------------------------------------
            DrawRectangle( 10, 10, 220, 130, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 220, 130, BLUE);

            DrawText("Controls:", 20, 20, 10, BLACK);
            DrawText("- Move with keys: W, A, S, D", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse move to look around", 40, 60, 10, DARKGRAY);
            DrawText("- SPACE to calibrate tracking", 40, 80, 10, DARKGRAY);
            DrawText("- F2 for fullscreen", 40, 100, 10, DARKGRAY);
            DrawText(fpsText, 40, 120, 10, DARKGRAY);

        EndDrawing();
    }

    // Cleanup
    //--------------------------------------------------------------------------------------

    // We need to stop the tracking. This also stops the camera access.
    dnkvw_stopTracking(dnkvw);

    // Free the used memory of the dnkvw context. Only use this method, never use free()!
    dnkvw_freeContext(&dnkvw);

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}
