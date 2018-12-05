//
//  TetrisWorld.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "TetrisWorld.hpp"

TetrisWorld::TetrisWorld(unsigned int x, unsigned int y, unsigned int z)
    : board_x(x), board_y(y), board_z(z) {
  GenNextFigure();
}

TetrisWorld::~TetrisWorld() {}

// tetris will always going down on each tick
void TetrisWorld::Update(Direction d, Rotation r) {
  // need to talk to Zachary about the coordinates of the cube
  Rotate(r);
  Move(d);
}

int TetrisWorld::GetPoints() { return points; }

bool TetrisWorld::IsEndGame() { return isGameOver; }

void TetrisWorld::Move(Direction d) {
  switch (d) {
    case D_DOWN:
      if (IsColliding()) {
        Merge();
      } else {
        m_CurrentMovingCubes->Move(d);
      }
      break;
    case D_LEFT:
      //      std::cout << "Move left" << std::endl;
      for (Cube* curr : m_CurrentMovingCubes->getCubes()) {
        if (curr->m_x == 0) {
          return;
        } else {
          for (Cube* c : m_Cubes) {
            if (c->m_y == curr->m_y && c->m_x + 1 == curr->m_x) {
              return;
            }
          }
        }
      }

      m_CurrentMovingCubes->Move(d);
      break;
    case D_RIGHT:
      for (Cube* curr : m_CurrentMovingCubes->getCubes()) {
        if (curr->m_x == board_x - 1) {
          return;
        } else {
          for (Cube* c : m_Cubes) {
            if (c->m_y == curr->m_y && c->m_x - 1 == curr->m_x) {
              return;
            }
          }
        }
      }

      m_CurrentMovingCubes->Move(d);
      break;
    default:
      break;
  }
}

void TetrisWorld::Rotate(Rotation r) { m_CurrentMovingCubes->Rotate(r); }

bool TetrisWorld::IsColliding() {
  for (Cube* moving : m_CurrentMovingCubes->getCubes()) {
    if (moving->m_y == 0) {
      std::cout << "Collide 1" << std::endl;
      return true;
    } else {
      for (Cube* current : m_Cubes) {
        if (current->m_x == moving->m_x && current->m_y + 1 == moving->m_y) {
          std::cout << "Collide 2" << std::endl;
          return true;
        }
      }
    }
  }
  for (Cube* c : m_CurrentMovingCubes->getCubes()) {
    std::cout << "Not colliding: " << c << std::endl;
  }
  std::cout << "======================================" << std::endl;

  return false;
}

void TetrisWorld::Merge() {
  //  std::cout << "Merge" << std::endl;
  for (Cube* next : m_CurrentMovingCubes->getCubes()) {
    m_Cubes.push_back(next);
  }

  int streak = 1;
  while (Collapse()) {
    points += board_x * 10 * streak;
    streak += 1;
  }

  streak = 0;
  float highest_y = 0.0f;
  for (Cube* c : m_Cubes) {
    highest_y = std::max(highest_y, c->m_y);
  }

  if (highest_y >= board_y) {
    isGameOver = true;
  } else {
    GenNextFigure();
  }
}

// generate next figure, need to talk to Zachary
void TetrisWorld::GenNextFigure() {
  //  std::cout << "Gen next tetromino" << std::endl;
  static std::random_device rd;
  static std::mt19937_64 gen(rd());
  std::uniform_int_distribution<> dis(0, 7);
  int shape = dis(gen);
  m_CurrentMovingCubes = new Tetromino(static_cast<Shape>(shape), board_y);
}

bool compare(Cube* x, Cube* y) {
  if (x->m_y == y->m_y) return x->m_x < y->m_y;

  return x->m_y < y->m_y;
}

bool TetrisWorld::Collapse() {
  std::sort(m_Cubes.begin(), m_Cubes.end(), compare);

  for (int i = 0; i < board_y; i++) {
    int count = 0;
    for (Cube* c : m_Cubes) {
      if (c->m_y == i) count++;
    }

    if (count == board_x) {
      std::cout << "Collapsed:" << count << std::endl;
      std::vector<Cube*> newV;
      for (int j = 0; j < m_Cubes.size(); j++) {
        Cube* rowJ = m_Cubes[j];
        if (rowJ->m_y > i) {
          rowJ->m_y -= 1;
          newV.push_back(rowJ);
        } else if (rowJ->m_y < i) {
          newV.push_back(rowJ);
        }
      }

      m_Cubes = newV;
      return true;
    }
  }
  std::cout << "Not collapse on any row" << std::endl;
  return false;
}

std::vector<Cube*> TetrisWorld::GetAllCubes() {
  std::vector<Cube*> res;

  for (Cube* c : m_Cubes) {
    res.push_back(c);
  }

  for (Cube* c : m_CurrentMovingCubes->getCubes()) {
    res.push_back(c);
  }

  return res;
}
