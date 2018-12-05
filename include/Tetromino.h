#ifndef TETROMINO_H
#define TETROMINO_H

#include <Constants.h>
#include <Cube.hpp>

class Tetromino {
 public:
  Tetromino(Shape, const int&);

  std::vector<Cube*> getCubes();
  void Move(Direction);
  void Rotate(Rotation);

 private:
  std::vector<Cube*> m_Cubes;
  Cube* m_Center;

  void genCubeCoords(float* cubeCoords, const int& board_h, float* color);

  float RED[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  float GREEN[4] = {0.0f, 1.0f, 0.0f, 1.0f};
  float BLUE[4] = {0.0f, 0.0f, 1.0f, 1.0f};
  float PURPLE[4] = {0.5f, 0.0f, 0.5f, 1.0f};
  float CYAN[4] = {0.0f, 0.5f, 0.5f, 1.0f};
  float ORANGE[4] = {1.0f, 0.65f, 0.0f, 1.0f};
  float PINK[4] = {0.5f, 0.0f, 0.0f, 1.0f};
};
#endif