# 3d-tetris
3D Tetris with SDL and OpenGL

## Note that there are some name change for consistency purposes
In `Cube` class
`UpdateNeighborLocation` -> `UpdateCubeUnitLocation`
`RotateNeighborX` -> `RotateCubeUnitOverX`
`RotateNeighborY` -> `RotateCubeUnitOverY`
`RotateNeighborZ` -> `RotateCubeUnitOverZ`
`PrintNeighborLocationAsCSV` -> `PrintCubeUnitLocationAsCSV`
`ExportNeighborLocationAsCSV` -> `ExportCubeUnitLocationAsCSV`

## Terminology 
- Cube: a single cube in 3d-tetris
- Cube Unit: a set of connected cubes; the Cube Unit of a cube is all the connected cubes including itself
- x,y,z: the x, y, z axises in the OpenGL sense
