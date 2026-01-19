#include "raylib.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define FPS 60
#define UNIT 1 // 1 foot
#define VELOCITY 100.0f

float velocityY = 0.0f;
const float GRAVITY = 9.8f;
const float FLOOR_Y = 0.0f;

void updatePos(Vector3& coords, float dt){
	// just move down 1 unit for now
	//coords.SetY(coords.GetY() - 1);
	velocityY += GRAVITY * dt;

	coords.y -= velocityY * dt; //  (1 / 9.8 * 0.16 )
	if(coords.y < FLOOR_Y) { coords.y = FLOOR_Y; velocityY = 0; }
}
int main() {
    // Initialization: Set window width, height, and title
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    InitWindow(screenWidth, screenHeight, "raylib locked in super game");

    // Set game to run at 60 frames-per-second
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 20.0f }; //3d position
    camera.target =   (Vector3){ 0.0f, 0.0f, 0.0f }; //target camera at origin
    //the direction your hair is facing - ChatGPT
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; //Camera up vector ? "rotation towards target"
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE; // camera mode type

    // x, y ,z
    Vector3 cubePosition1 = { 0.0f, 10.0f, 0.0f };
    //Vector3 cubePosition2 = { 2.0f, 0.0f, 0.0f };   


    SetTargetFPS(FPS);
    DisableCursor();

    int index = 0;
    std::vector<Color> colors = { RED, BLUE , GREEN };

    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        // 1. Update logic 
	//UpdateCamera(&camera, CAMERA_ORBITAL);
	float dt = GetFrameTime();

	updatePos(cubePosition1, dt);
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		index == 2 ? index = 0 : index++;
	}
	// Movement parameters
	const float cameraSpeed = 5.0f; // units per second

	// Compute forward vector (direction camera is looking)
	Vector3 forward = { 
	    camera.target.x - camera.position.x,
	    camera.target.y - camera.position.y,
	    camera.target.z - camera.position.z
	};

	// Normalize forward vector
	float length = sqrtf(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z);
	forward.x /= length; forward.y /= length; forward.z /= length;

	// Compute right vector (perpendicular to forward and up)
	Vector3 right = {
	    forward.z * camera.up.y - forward.y * camera.up.z,
	    forward.x * camera.up.z - forward.z * camera.up.x,
	    forward.y * camera.up.x - forward.x * camera.up.y
	};

	// Normalize right vector
	length = sqrtf(right.x*right.x + right.y*right.y + right.z*right.z);
	right.x /= length; right.y /= length; right.z /= length;

	// WASD movement
	if (IsKeyDown(KEY_W)) {
	    camera.position.x += forward.x * cameraSpeed * dt;
	    camera.position.y += forward.y * cameraSpeed * dt;
	    camera.position.z += forward.z * cameraSpeed * dt;

	    camera.target.x += forward.x * cameraSpeed * dt;
	    camera.target.y += forward.y * cameraSpeed * dt;
	    camera.target.z += forward.z * cameraSpeed * dt;
	}

	if (IsKeyDown(KEY_S)) {
	    camera.position.x -= forward.x * cameraSpeed * dt;
	    camera.position.y -= forward.y * cameraSpeed * dt;
	    camera.position.z -= forward.z * cameraSpeed * dt;

	    camera.target.x -= forward.x * cameraSpeed * dt;
	    camera.target.y -= forward.y * cameraSpeed * dt;
	    camera.target.z -= forward.z * cameraSpeed * dt;
	}

	if (IsKeyDown(KEY_D)) {
	    camera.position.x -= right.x * cameraSpeed * dt;
	    camera.position.y -= right.y * cameraSpeed * dt;
	    camera.position.z -= right.z * cameraSpeed * dt;

	    camera.target.x -= right.x * cameraSpeed * dt;
	    camera.target.y -= right.y * cameraSpeed * dt;
	    camera.target.z -= right.z * cameraSpeed * dt;
	}

	if (IsKeyDown(KEY_A)) {
	    camera.position.x += right.x * cameraSpeed * dt;
	    camera.position.y += right.y * cameraSpeed * dt;
	    camera.position.z += right.z * cameraSpeed * dt;

	    camera.target.x += right.x * cameraSpeed * dt;
	    camera.target.y += right.y * cameraSpeed * dt;
	    camera.target.z += right.z * cameraSpeed * dt;
	}

        BeginDrawing();
	ClearBackground(BLACK); // Clear the frame with a solid color
	
	BeginMode3D(camera);

	DrawCube(cubePosition1, 5.0f, 5.0f, 5.0f, colors[index]);
	//DrawCube(cubePosition2, 2.0f, 2.0f, 2.0f, colors[index]);
	//DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, WHITE);

	DrawGrid(10, 1.0f);

	EndMode3D();
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
