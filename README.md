# 3d-tetris
3D Tetris with SDL and OpenGL

## Issue
- one of the L is not rotating correctly
- the cube unit generation is not random, use mt19

## Note that there are some name change for consistency purposes
In `Cube` class
- `UpdateNeighborLocation` -> `UpdateCubeUnitLocation`
- `RotateNeighborX` -> `RotateCubeUnitOverX`
- `RotateNeighborY` -> `RotateCubeUnitOverY`
- `RotateNeighborZ` -> `RotateCubeUnitOverZ`
- `PrintNeighborLocationAsCSV` -> `PrintCubeUnitLocationAsCSV`
- `ExportNeighborLocationAsCSV` -> `ExportCubeUnitLocationAsCSV`

## Terminology 
- Cube: a single cube in 3d-tetris
- Cube Unit: a set of connected cubes; the Cube Unit of a cube is all the connected cubes including itself
- x,y,z: the x, y, z axises in the OpenGL sense

## Road map 
- (Complete) Create of `ObjectCube.cpp` that inherits `Object.cpp` (same as `Sphere.cpp`) (Zachary)
- Create `SceneNodeCube.cpp` that inherits `SceneNode.cpp`. This `SceneNodeCube` class will use a simpler shader, each of them should has a `Cube` (for location and color) and `ObjectCube` (for rendering the Cube). Each local transformation of `SceneNodeCube` should be relative to the TetrisWorld.
- Implementation of `TetrisWorld.cpp` that has the rules and mechanisms of the game. (Hoang)
- Next meetup on Friday (Nov 30) for further discussion.
-
