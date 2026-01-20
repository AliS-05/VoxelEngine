#include "raylib.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>
#define SCREEN_WIDTH 1250
#define SCREEN_HEIGHT 770
#define FPS 60
#define UNIT 1 // 1 foot
#define VELOCITY 100.0f
#define SPEED 2
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

void updateFreeCamera(Camera3D &camera, float dt, float sensitivity = 0.003f) {
    // Get mouse movement
    Vector2 mouseDelta = GetMouseDelta(); // change since last frame
    static float yaw = 0.0f;    // horizontal rotation
    static float pitch = 0.0f;  // vertical rotation

    yaw   += mouseDelta.x * sensitivity;
    pitch += -mouseDelta.y * sensitivity; // negative because screen Y is inverted

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f * DEG2RAD) pitch = 89.0f * DEG2RAD;
    if (pitch < -89.0f * DEG2RAD) pitch = -89.0f * DEG2RAD;

    // Compute forward vector
    Vector3 forward;
    forward.x = cosf(pitch) * sinf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * cosf(yaw);

    // Normalize just in case
    float len = sqrtf(forward.x*forward.x + forward.y*forward.y + forward.z*forward.z);
    forward.x /= len;
    forward.y /= len;
    forward.z /= len;

    // Update camera target
    camera.target.x = camera.position.x + forward.x;
    camera.target.y = camera.position.y + forward.y;
    camera.target.z = camera.position.z + forward.z;

    // Optional: move camera with arrow keys (just moves the camera in space)
    const float speed = 10.0f;
    Vector3 right = {
        forward.z * camera.up.y - forward.y * camera.up.z,
        forward.x * camera.up.z - forward.z * camera.up.x,
        forward.y * camera.up.x - forward.x * camera.up.y
    };
    // normalize right
    len = sqrtf(right.x*right.x + right.y*right.y + right.z*right.z);
    right.x /= len; right.y /= len; right.z /= len;

    if (IsKeyDown(KEY_W)) {
        camera.position.x += forward.x * speed * dt;
        camera.position.y += forward.y * speed * dt;
        camera.position.z += forward.z * speed * dt;
    }
    if (IsKeyDown(KEY_S)) {
        camera.position.x -= forward.x * speed * dt;
        camera.position.y -= forward.y * speed * dt;
        camera.position.z -= forward.z * speed * dt;
    }
    if (IsKeyDown(KEY_A)) {
        camera.position.x += right.x * speed * dt;
        camera.position.y += right.y * speed * dt;
        camera.position.z += right.z * speed * dt;
    }
    if (IsKeyDown(KEY_D)) {
        camera.position.x -= right.x * speed * dt;
        camera.position.y -= right.y * speed * dt;
        camera.position.z -= right.z * speed * dt;
    }
}


int main() {
    // Initialization: Set window width, height, and title
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    InitWindow(screenWidth, screenHeight, "raylib locked in super game");
    SetWindowPosition(0, 0);

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

    int index = 0;
    std::vector<Color> colors = { RED, BLUE , GREEN };

    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        // 1. Update logic 
	//UpdateCamera(&camera, CAMERA_ORBITAL);
	

	float dt = GetFrameTime();
	updateFreeCamera(camera, dt);
	updatePos(cubePosition1, dt);

	Vector2 mousePos = GetMousePosition();
	Vector2 mouseDelta = GetMouseDelta();
	//moveCamera(camera.position, mousePos);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		index == 2 ? index = 0 : index++;
	}
        BeginDrawing();
	ClearBackground(BLACK); // Clear the frame with a solid color
	
	BeginMode3D(camera);

	DrawCube(cubePosition1, 3.0f, 3.0f, 3.0f, colors[index]);
	//DrawCube(cubePosition2, 2.0f, 2.0f, 2.0f, colors[index]);
	//DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, WHITE);

	DrawGrid(100, 1.0f);

	EndMode3D();
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
