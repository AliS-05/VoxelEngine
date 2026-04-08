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
std::vector<Chunk*> chunkVector;
Chunk* lastChunk = nullptr;
int lastX = -1, lastY = -1, lastZ = -1;

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

Chunk generateChunk(int x, int z){
	Chunk c;
	c.worldX = x;
	c.worldZ = z;
	for(int x = 0; x < 16; x++)
		for(int y = 0; y < 16; y++)
			for(int z = 0; z < 16; z++)
				c.blocks[x][y][z] = (y < 8) ? 1 : 0;  // only draw bottom half
	return c;
}

Chunk* findChunk(int x, int z){
	for(Chunk* c : chunkVector){
		if(c->worldX == x && c->worldZ == z){
			return c;
		}
	}
	return nullptr;
}

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib locked in super game");
	SetWindowPosition(1280 - SCREEN_WIDTH / 2, 720 - SCREEN_HEIGHT / 2);
	initCamera();
	SetTargetFPS(FPS);
	DisableCursor(); //so cursor doesnt go off screen

	Vector3 cubePosition1 = { 0.0f, 10.0f, 0.0f };
	int index = 0;
	std::vector<Color> colors = { BEIGE, WHITE , DARKGRAY};
	Chunk c = generateChunk(0, 0); //corner at 0,0 expands into quadrant 1
	chunkVector.push_back(&c);
	Vector3 curRayPos;
	int chunkX;
	int chunkZ;

	while(!WindowShouldClose()) {  //esc to exit
		Chunk* curChunk = nullptr;
		float dt = GetFrameTime();
		UpdateCamera(&camera, CAMERA_FREE);
		//updatePos(cubePosition1, dt);
		Ray ray = GetScreenToWorldRay({CENTER_X, CENTER_Y}, camera); //passing Vector3 with center coords
		

		//highlight block currently being looked at
		for(float i = 0; i < REACH; i += 0.05f){
			curRayPos = Vector3Add(ray.position, Vector3Scale(ray.direction, i)); //'stepping' along Ray
			chunkX = (int)floor(curRayPos.x / 16.0f);
			chunkZ = (int)floor(curRayPos.z / 16.0f);

			curChunk = findChunk(chunkX, chunkZ);
			if(curChunk != nullptr){ 
				int localX = (int)floor(curRayPos.x) - chunkX * 16; // better alternative to simply using modulo
				int localY = (int)floor(curRayPos.y);
				int localZ = (int)floor(curRayPos.z) - chunkZ * 16;

				if(localX < 0 || localX >= 16 || 
					localY < 0 || localY >= 16 || 
					localZ < 0 || localZ >= 16) {
					continue; 	
				}
				if(curChunk->blocks[localX][localY][localZ] == 0) continue;

				// update previous block
				if(lastChunk != nullptr){
					if(lastChunk->blocks[lastX][lastY][lastZ] == 2)
						lastChunk->blocks[lastX][lastY][lastZ] = 1;
				}

				// highlighting new block 
				curChunk->blocks[localX][localY][localZ] = 2;

				lastChunk = curChunk;
				lastX = localX;
				lastY = localY;
				lastZ = localZ;

				break;
			}
		}

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			//DrawLine3D(Vector3Add(ray.position, (Vector3){0, -1.0f, 0}), Vector3Add(ray.position, Vector3Scale(ray.direction, REACH)), PINK);
			
			//traverse ray, Vector3Add(ray.position, Vector3Scale(ray.direction * i)) inside for loop (i < REACH)
			//if curRayPos.x // 16 == any chunk x && curRayPos.z // 16 == any chunk.worldZ then the ray is inside of that chunk
			//then use curRayPos.x, y and z % 16 to find local chunk block position
			for(float i = 0; i < REACH; i += 0.05f){
				curRayPos = Vector3Add(ray.position, Vector3Scale(ray.direction, i)); //'stepping' along Ray
				chunkX = (int)floor(curRayPos.x / 16.0f);
				chunkZ = (int)floor(curRayPos.z / 16.0f);

				curChunk = findChunk(chunkX, chunkZ);
				if(curChunk != nullptr){ 
					int localX = (int)floor(curRayPos.x) - chunkX * 16; // better alternative to simply using modulo
					int localY = (int)floor(curRayPos.y);
					int localZ = (int)floor(curRayPos.z) - chunkZ * 16;

					if(localX < 0 || localX >= 16 || 
						localY < 0 || localY >= 16 || 
					        localZ < 0 || localZ >= 16) {
						continue; // out of bounds, skip
					}

					if(curChunk->blocks[localX][localY][localZ] == 0) continue; //skip empty blocks so the raycast doesnt get stuck behind air
					else{ curChunk->blocks[localX][localY][localZ] = 0; break;}; //else solid block, set to 0
				}
			}
		}

		//rmb = place blocks
		//literally same exact thing but set to 1 ?
		if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
			//DrawLine3D(Vector3Add(ray.position, (Vector3){0, -1.0f, 0}), Vector3Add(ray.position, Vector3Scale(ray.direction, REACH)), PINK);
			
			//traverse ray, Vector3Add(ray.position, Vector3Scale(ray.direction * i)) inside for loop (i < REACH)
			//if curRayPos.x // 16 == any chunk x && curRayPos.z // 16 == any chunk.worldZ then the ray is inside of that chunk
			//then use curRayPos.x, y and z % 16 to find local chunk block position
			for(float i = 0; i < REACH; i += 0.05f){
				curRayPos = Vector3Add(ray.position, Vector3Scale(ray.direction, i)); //'stepping' along Ray
				chunkX = (int)floor(curRayPos.x / 16.0f);
				chunkZ = (int)floor(curRayPos.z / 16.0f);
				
				int recX;
				int recY;
				int recZ;

				curChunk = findChunk(chunkX, chunkZ);
				if(curChunk != nullptr){ 
					int localX = (int)floor(curRayPos.x) - chunkX * 16; // better alternative to simply using modulo (negatives)
					int localY = (int)floor(curRayPos.y);
					int localZ = (int)floor(curRayPos.z) - chunkZ * 16;

					  if(localX < 0 || localX >= 16 || 
					  	localY < 0 || localY >= 16 || 
					        localZ < 0 || localZ >= 16) {
						continue; // out of bounds, skip
					  }

					if(curChunk->blocks[localX][localY][localZ] == 0){
						//idea keep track of most recent empty block to update once i find solid block
						recX = localX;	
						recY = localY;
						recZ = localZ;
					}
					else{ //need to find face of the block selected and then place on top of that face
						curChunk->blocks[recX][recY][recZ] = 1; 
						break;
					} //else solid block, set to 0
				}
			}
		}

	
		BeginDrawing();
		ClearBackground(BLACK); 
		BeginMode3D(camera);
		
		for(int x = 0; x < 16; x++){
			for(int y = 0; y < 16; y++){
				for(int z = 0; z < 16; z++){
					if(c.blocks[x][y][z] == 1){
						Vector3 visualPos = { (float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f };
						DrawCube(visualPos, 1, 1, 1, colors[index]);
						DrawCubeWires(visualPos, 1, 1, 1, GRAY);

					//	DrawCube((Vector3){x, y, z}, 1, 1, 1, colors[index]);
					//	DrawCubeWires((Vector3){x, y, z}, 1, 1, 1, GRAY);
					}
					else if(c.blocks[x][y][z] == 2){
						Vector3 visualPos = { (float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f };
						DrawCube(visualPos, 1, 1, 1, PINK);
						DrawCubeWires(visualPos, 1, 1, 1, WHITE);
					}
				}
			}
		}
		

		if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
			index == 2 ? index = 0 : index++; //staying in bounds of color vec
		}

		//DrawCube(cubePosition1, 3.0f, 3.0f, 3.0f, colors[index]);
		DrawGrid(100, 1.0f);
		EndMode3D();

		//drawing crosshair last so it's visible at all times
		int crosshairSize = 25;
		DrawLine(CENTER_X - crosshairSize, CENTER_Y, CENTER_X + crosshairSize, CENTER_Y, WHITE);
		DrawLine(CENTER_X, CENTER_Y - crosshairSize, CENTER_X, CENTER_Y + crosshairSize, WHITE);

		EndDrawing();
	}
	CloseWindow();        
	return 0;
}
