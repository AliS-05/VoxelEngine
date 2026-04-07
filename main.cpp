#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>
#include "terrain.hpp"

#define SCREEN_WIDTH 1250
#define SCREEN_HEIGHT 770
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)
#define FPS 240
#define UNIT 1
#define VELOCITY 100.0f
#define SPEED 2
#define REACH 3.0f
float velocityY = 0.0f;
const float GRAVITY = 9.8f;
const float FLOOR_Y = 0.0f;

static Camera3D camera = { 0 };

void initCamera(){
	camera.position = (Vector3){ 0.0f, 10.0f, 10.0f }; //3d position
	camera.target =   (Vector3){ 0.0f, 0.0f, 0.0f }; //target camera at origin
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; 
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE; // camera mode type
}

void updatePos(Vector3& coords, float dt){
	velocityY += GRAVITY * dt;
	coords.y -= velocityY * dt; 	
	if(coords.y < FLOOR_Y) { coords.y = 10; velocityY = 0; }
}

Chunk generateChunk(){
	Chunk c;
	for(int x = 0; x < 16; x++)
		for(int y = 0; y < 16; y++)
			for(int z = 0; z < 16; z++)
				c.blocks[x][y][z] = (y < 8) ? 1 : 0;  // only draw bottom half
	return c;
}


int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib locked in super game");
	SetWindowPosition(1280 - SCREEN_WIDTH / 2, 720 - SCREEN_HEIGHT / 2);
	initCamera();
	SetTargetFPS(FPS);
	DisableCursor(); //so cursor doesnt go off screen

	Vector3 cubePosition1 = { 0.0f, 10.0f, 0.0f };
	int index = 0;
	std::vector<Color> colors = { RED, BLUE , GREEN };
	Chunk c = generateChunk();
	while (!WindowShouldClose()) {  //esc to exit
		float dt = GetFrameTime();
		UpdateCamera(&camera, CAMERA_FREE);
		//updatePos(cubePosition1, dt);

		//drawing crosshair
		int crosshairSize = 50;
		DrawLine(CENTER_X - crosshairSize, CENTER_Y, CENTER_X + crosshairSize, CENTER_Y, WHITE);
		DrawLine(CENTER_X, CENTER_Y - crosshairSize, CENTER_X, CENTER_Y + crosshairSize, WHITE);

		BeginDrawing();
		ClearBackground(BLACK); 
		BeginMode3D(camera);
		
		Ray ray = GetScreenToWorldRay({CENTER_X, CENTER_Y}, camera); //passing Vector2 with center coords
		DrawLine3D(Vector3Add(ray.position, (Vector3){0, -1.0f, 0}), Vector3Add(ray.position, Vector3Scale(ray.direction, REACH)), PINK);

		for(int x = 0; x < 16; x++){
			for(int y = 0; y < 16; y++){
				for(int z = 0; z < 16; z++){
						if(c.blocks[x][y][z] != 0){
							DrawCube((Vector3){x, y, z}, 1, 1, 1, colors[index]);
						}
					}
			}
		}

		//DrawCube(cubePosition1, 3.0f, 3.0f, 3.0f, colors[index]);

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			index == 2 ? index = 0 : index++; //staying in bounds of color vec
		}

		DrawGrid(100, 1.0f);
		EndMode3D();
		EndDrawing();
	}
	CloseWindow();        
	return 0;
}
