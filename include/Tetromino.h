#ifndef TETROMINO_H
#define TETROMINO_H

#include <Constants.h>
#include <Cube.hpp>

class Tetromino {
 public:
  Tetromino(Shape, int);

  std::vector<Cube*> getCubes();
  void Move(Direction);
  void Rotate(Rotation);

 private:
  std::vector<Cube*> m_Cubes;
  Cube* m_Center;

  void genCubeCoords(float* cubeCoords, int board_h);
};
#endif