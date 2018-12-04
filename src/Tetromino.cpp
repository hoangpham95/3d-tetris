#include <Tetromino.h>

std::vector<Cube*> Tetromino::getCubes() { return m_Cubes; }

void Tetromino::genCubeCoords(float* cubeCoords, int board_h) {
  for (int i = 0; i < 4; i++) {
    m_Cubes.push_back(new Cube(cubeCoords[4 * i],
                               board_h - cubeCoords[4 * i + 1],
                               cubeCoords[4 * i + 2]));
  }
}

Tetromino::Tetromino(Shape s, int board_h) {
  float* cubeCoords;
  if (s == L) {
    m_Center = new Cube(0.5f, board_h + 1.5f, 0);
    float cubeCoords[12] = {0.0f, -1.0f, 0.0f, 0.0f, -2.0f, 0.0f,
                            1.0f, -1.0f, 0.0f, 1.0f, -2.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else if (s == Z) {
    m_Center = new Cube(1.0f, board_h + 1.0f, 0);
    float cubeCoords[12] = {0.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
                            2.0f, -1.0f, 0.0f, 3.0f, -1.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else if (s == I) {
    m_Center = new Cube(1.0f, -1.0f, 0);
    float cubeCoords[12] = {0.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
                            2.0f, -1.0f, 0.0f, 2.0f, -2.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else if (s == J) {
    m_Center = new Cube(1.0f, -1.0f, 0);
    float cubeCoords[12] = {0.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
                            2.0f, -1.0f, 0.0f, 0.0f, -2.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else if (s == O) {
    m_Center = new Cube(1.0f, -1.0f, 0);
    float cubeCoords[12] = {0.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
                            2.0f, -1.0f, 0.0f, 1.0f, -2.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else if (s == S) {
    m_Center = new Cube(1.0f, -1.0f, 0);
    float cubeCoords[12] = {0.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
                            1.0f, -2.0f, 0.0f, 2.0f, -2.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else if (s == T) {
    m_Center = new Cube(1.0f, -1.0f, 0);
    float cubeCoords[12] = {0.0f, -2.0f, 0.0f, 1.0f, -2.0f, 0.0f,
                            1.0f, -1.0f, 0.0f, 2.0f, -1.0f, 0.0f};
    genCubeCoords(cubeCoords, board_h);
  } else {
    return;
  }
}

void Tetromino::Move(Direction d) {
  switch (d) {
    case D_DOWN:
      m_Center->m_y -= 1;
      for (int i = 0; i < m_Cubes.size(); i++) {
        m_Cubes[i]->m_y -= 1;
      }
      break;
    case D_LEFT:
      m_Center->m_x -= 1;
      for (int i = 0; i < m_Cubes.size(); i++) {
        m_Cubes[i]->m_x -= 1;
      }
      break;
    case D_RIGHT:
      m_Center->m_x += 1;
      for (int i = 0; i < m_Cubes.size(); i++) {
        m_Cubes[i]->m_x += 1;
      }
    case D_NONE:
      break;
    default:
      break;
  }
}

void Tetromino::Rotate(Rotation r) {
  switch (r) {
    case R_CCW:
      for (int i = 0; i < m_Cubes.size(); i++) {
        Cube* cur = m_Cubes[i];
        cur->m_x = m_Center->m_x + (m_Center->m_y - cur->m_y);
        cur->m_y = m_Center->m_y + (cur->m_x - m_Center->m_x);
      }
    case R_CW:
      Rotate(R_CCW);
      Rotate(R_CCW);
      Rotate(R_CCW);
    default:
      break;
  }
}
