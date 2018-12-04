//
//  TetrisWorld.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/20/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "TetrisWorld.hpp"

TetrisWorld::TetrisWorld(unsigned int x, unsigned int y, unsigned int z)
    : board_x(x), board_y(y), board_z(z) {}

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
      for (int i = 0; i < m_CurrentMovingCubes->getCubes().size(); i++) {
        Cube* curr = m_CurrentMovingCubes->getCubes()[i];
        if (curr->m_x == 0) {
          return;
        } else {
          for (int j = 0; i < m_Cubes.size(); j++) {
            Cube* c = m_Cubes[j];
            if (c->m_y == curr->m_y && c->m_x + 1 == curr->m_x) {
              return;
            }
          }
        }
      }

      m_CurrentMovingCubes->Move(d);
      break;
    case D_RIGHT:
      for (int i = 0; i < m_CurrentMovingCubes->getCubes().size(); i++) {
        Cube* curr = m_CurrentMovingCubes->getCubes()[i];
        if (curr->m_x == board_x - 1) {
          return;
        } else {
          for (int j = 0; i < m_Cubes.size(); j++) {
            Cube* c = m_Cubes[j];
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
  float min_y = INT_MAX, min_x = 0;
  for (int i = 0; i < m_CurrentMovingCubes->getCubes().size(); i++) {
    Cube* current = m_CurrentMovingCubes->getCubes()[i];
    if (current->m_y < board_y) {
      min_y = current->m_y;
      min_x = current->m_x;
    }
  }

  for (int i = 0; i < m_Cubes.size(); i++) {
    Cube* current = m_Cubes[i];
    if (current->m_x == min_x && current->m_y + 1 == min_y) {
      return true;
    }
  }

  return m_Cubes.size() == 0;
}

void TetrisWorld::Merge() {
  while (!m_CurrentMovingCubes->getCubes().empty()) {
    m_Cubes.push_back(m_CurrentMovingCubes->getCubes().back());
    m_CurrentMovingCubes->getCubes().pop_back();
  }

  int streak = 1;
  while (Collapse()) {
    points += board_x * 10 * streak;
    streak += 1;
  }

  streak = 0;
  float highest_y = 0.0f;
  for (int i = 0; i < m_Cubes.size(); i++) {
    highest_y = std::max(highest_y, m_Cubes[i]->m_y);
  }

  if (highest_y >= board_y) {
    isGameOver = true;
  } else {
    GenNextFigure();
  }
}

// generate next figure, need to talk to Zachary
void TetrisWorld::GenNextFigure() {
  delete m_CurrentMovingCubes;
  int shape = rand() % 7;
  m_CurrentMovingCubes = new Tetromino(static_cast<Shape>(shape), board_y);
}

bool compare(Cube* x, Cube* y) {
  if (x->m_y == y->m_y) return x->m_x < y->m_y;

  return x->m_y < y->m_y;
}

bool TetrisWorld::Collapse() {
  std::sort(m_Cubes.begin(), m_Cubes.end(), compare);

  int count = 0;
  for (int i = 0; i < m_Cubes.size(); i++) {
    if (m_Cubes[i]->m_y == 0) count++;
  }

  if (count == board_x) {
    for (int i = 0; i < board_x; i++) {
      m_Cubes.erase(m_Cubes.begin());
    }

    for (int j = 0; j < m_Cubes.size(); j++) {
      m_Cubes[j]->m_y -= 1;
    }

    return true;
  }

  return false;
}