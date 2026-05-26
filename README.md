# VoxelEngine

A Minecraft-style voxel engine built in C++ using Raylib. Implements chunk-based 
world rendering, raycasting-based block selection, and first-person camera controls.

## Features

- Chunk-based world representation (16x16x16 blocks per chunk)
- Raycasting for block targeting and highlighting
- Left click to destroy blocks, right click to place blocks
- Free camera with 240 FPS target
- Crosshair overlay

## Dependencies
- Raylib

## Building

Build on WSL, Run on Windows through .exe (yes weird I know)

```bash
./build.sh
```

## Controls

- Mouse: Look Around
- WASD: Move
- Right Click: Place Block
- Left Click: Destroy Block
- ESC: Exit 
