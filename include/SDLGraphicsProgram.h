// SDLGraphicsProgram
// This class is used to handel SDL windows and the OpenGL program within it
// The structure of this class is inspired by Mike Shah, instructor of CS4300
// The implementation is made suitable for rendering 3d-tetris
//
// Author: Mike Shah, Shanghao Zhong

#ifndef SDLGRAPHICSPROGRAM
#define SDLGRAPHICSPROGRAM

#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else  // This works for Mac
#include <SDL.h>
#endif

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>

// glm for handling model, view, projection matrices
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"

// std library
#include <TetrisWorld.hpp>
#include <iostream>

class SDLGraphicsProgram {
 public:
  // Constructor
  // It takes in the dimensions of the windows
  // It will initialized SDL, OpenGL, and other relevent data
  SDLGraphicsProgram(int w, int h, TetrisWorld wo);
  // Desctructor
  ~SDLGraphicsProgram();
  // Per frame update
  void update();
  // Renders shapes to the screen
  void render();
  // loop that runs forever
  void loop();
  // Get Pointer to Window
  SDL_Window* getSDLWindow();
  // Shader loading utility programs
  void printProgramLog(GLuint program);
  void printShaderLog(GLuint shader);
  // Helper Function to Query OpenGL information.
  void getOpenGLVersionInfo();

 private:
  TetrisWorld tetris;
  // Screen dimension constants
  int screenHeight;
  int screenWidth;
  // The window we'll be rendering to
  SDL_Window* gWindow;
  // OpenGL context
  SDL_GLContext gContext;
  // For now, we can have one shader.
  unsigned int shader;
  // Vertex Array Object
  GLuint VAOId;
  // Vertex Buffer
  GLuint vertexPositionBuffer;
  // Index Buffer Object
  GLuint indexBufferObject;
  // number of indices
  unsigned int m_numIdx;
  // bool indicated if the graph is filled or outlined
  bool m_isFilled{true};
  // the distance between two cubes that are single unit apart
  const GLfloat m_cubeDistance{1.0f};
  // the width of each cube
  const GLfloat m_cubeWidth{0.8};
  // the vertex positions and indices used to draw a single cube centered at
  // (0,0,0) with length m_cubeWidth;
  GLfloat* m_singleCubeVertice;
  GLshort* m_singleCubeIndices;
  unsigned short m_singleCubeVertexNum;
  unsigned short m_singleCubeIndexNum;

  // User interaction
  void HandleKeyDown(bool& quit, SDL_KeyboardEvent* pe);
  void HanleMouseDrag(SDL_MouseMotionEvent* me);

  // Setting up OpenGL
  // It wills load shaders and compile shaders
  // Returns true if successful, false otherwise
  bool initGL();
  // Shader helpers
  // loads shadders files as a string
  std::string LoadShader(const std::string& fname);
  // creates shaders program
  unsigned int CreateShader(const std::string& vertexShaderSource,
                            const std::string& fragmentShaderSource);
  // compiles shaders
  unsigned int CompileShader(unsigned int type, const std::string& source);
  // tests link status
  bool CheckLinkStatus(GLuint programID);

  // Handling Buffer
  // Generate any vertex buffers
  void GenerateBuffers();
  // delete buffer
  void DeleteBuffer();

  // Updates buffers with new cube data
  // numCubes: the number of cubes wanted to be added to buffer
  // cubeLocations: the location of the center of each cube, its layout is
  //   [x,y,z,x,y,z,...] where each x,y,z represents the coordinates of each
  //   cube. Hence, the length of the array is 3*numCubes.
  // cubeColors: the color of each cube, its layout is
  //   [r,g,b,a,r,g,b,a,...] where each r,g,b,a represents the color of each
  //   cube. Hence, the length of the array is 4*numCubes
  void makeCubes(int numCubes, std::vector<float> cubeLocations,
                 std::vector<float> cubeColors);
};

#endif
