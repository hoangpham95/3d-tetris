#include "SDLGraphicsProgram.h"
#include <fstream>
#include <sstream>

#define PI 3.1415926535f

SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, TetrisWorld tetris)
    : screenWidth(w), screenHeight(h), tetris(tetris) {
  // Initialization flag
  bool success = true;
  // String to hold any errors that occur.
  std::stringstream errorStream;
  // The window we'll be rendering to
  gWindow = NULL;
  // Render flag

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError()
                << "\n";
    success = false;
  } else {
    // Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    gWindow = SDL_CreateWindow(
        "Assignment 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Check if Window did not create.
    if (gWindow == NULL) {
      errorStream << "Window could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
      success = false;
    }

    // Create an OpenGL Graphics Context
    gContext = SDL_GL_CreateContext(gWindow);
    if (gContext == NULL) {
      errorStream << "OpenGL context could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
      success = false;
    }

    // Initialize GLAD Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
      errorStream << "Failed to iniitalize GLAD\n";
      success = false;
    }

    // Initialize OpenGL
    if (!initGL()) {
      errorStream << "Unable to initialize OpenGL!\n";
      success = false;
    }
  }

  // If initialization did not work, then print out a list of errors in the
  // constructor.
  if (!success) {
    errorStream << "Failed to initialize!\n";
    std::string errors = errorStream.str();
    std::cout << errors << "\n";
  } else {
    std::cout << "No SDL, GLAD, or OpenGL, errors Detected\n\n";
  }

  // initialize data for drawing cubes
  GLfloat halfWidth = m_cubeWidth / 2;
  m_singleCubeVertice = new GLfloat[24]{
      -halfWidth, -halfWidth, -halfWidth, -halfWidth, -halfWidth, halfWidth,
      -halfWidth, halfWidth,  -halfWidth, -halfWidth, halfWidth,  halfWidth,
      halfWidth,  -halfWidth, -halfWidth, halfWidth,  -halfWidth, halfWidth,
      halfWidth,  halfWidth,  -halfWidth, halfWidth,  halfWidth,  halfWidth,
  };
  m_singleCubeVertexNum = 8;
  m_singleCubeIndices = new GLshort[36]{
      0, 4, 1, 1, 2, 0, 1, 4, 5, 1, 7, 3, 2, 4, 0, 2, 7, 6,
      3, 2, 1, 3, 7, 2, 4, 7, 5, 5, 7, 1, 6, 4, 2, 6, 7, 4,
  };
  m_singleCubeIndexNum = 36;
}

SDLGraphicsProgram::~SDLGraphicsProgram() {
  // Destroy window
  SDL_DestroyWindow(gWindow);
  // Point gWindow to NULL to ensure it points to nothing.
  gWindow = NULL;
  // delete buffer
  DeleteBuffer();
  // Deallocate Program
  glDeleteProgram(shader);
  // Deallocate cube data
  delete[] m_singleCubeVertice;
  delete[] m_singleCubeIndices;
  // Quit SDL subsystems
  SDL_Quit();
}

bool SDLGraphicsProgram::initGL() {
  // Success flag
  bool success = true;

  // Setup shaders
  std::string vertexShader = LoadShader("./shaders/cube_vert.glsl");
  std::string fragmentShader = LoadShader("./shaders/cube_frag.glsl");
  shader = CreateShader(vertexShader, fragmentShader);
  // Use our shader
  glUseProgram(shader);

  // Setup geometry
  GenerateBuffers();

  return success;
}

void SDLGraphicsProgram::GenerateBuffers() {
  m_stride = 10;
  // VertexArrays
  glGenVertexArrays(1, &VAOId);
  glBindVertexArray(VAOId);
  // Vertex position buffer
  glGenBuffers(1, &vertexPositionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
  // vertex attribute
  glEnableVertexAttribArray(0);  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(GLfloat),
                        0);
  glEnableVertexAttribArray(1);  // color
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, m_stride * sizeof(GLfloat),
                        (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);  // position
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(GLfloat),
                        (void*)(7 * sizeof(GLfloat)));
  // index buffer
  glGenBuffers(1, &indexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
}

// delete vertex array object and buffer objects
void SDLGraphicsProgram::DeleteBuffer() {
  glDeleteVertexArrays(1, &VAOId);
  glDeleteBuffers(1, &vertexPositionBuffer);
  glDeleteBuffers(1, &indexBufferObject);
}

void SDLGraphicsProgram::update() {
  tetris.Update(m_Direction, m_Rotation);
  std::vector<Cube*> cubes = tetris.GetAllCubes();
  //   std::cout << "Cube size: " << cubes.size() << std::endl;
  std::vector<float> cubeLocs;
  for (int i = 0; i < cubes.size(); i++) {
    // std::cout << cubes[i] << std::endl;
    cubeLocs.push_back(cubes[i]->m_x);
    cubeLocs.push_back(cubes[i]->m_y);
    cubeLocs.push_back(cubes[i]->m_z);
  }
  std::vector<float> cubeCols;
  for (int i = 0; i < cubes.size(); i++) {
    float* cubeColor = cubes[i]->GetColor();
    for (int j = 0; j < 4; j++) {
      cubeCols.push_back(cubeColor[j]);
    }
  }
  makeCubes(cubes.size(), cubeLocs, cubeCols);

  // finishing rendering, reset back to old move
  m_Direction = D_NONE;
  m_Rotation = R_NONE;
  // if the timer reaches the time speed, move down 1 unit
  if (m_timer >= m_gameSpeed) {
    tetris.Update(D_DOWN, R_NONE);
    m_timer = 0;
  }
}

void SDLGraphicsProgram::makeCubes(int numCubes,
                                   std::vector<float> cubeLocations,
                                   std::vector<float> cubeColors) {
  GLfloat vertexBufferData[m_stride * m_singleCubeVertexNum * numCubes];
  for (int i = 0; i < numCubes; ++i) {
    for (int j = 0; j < m_singleCubeVertexNum; ++j) {
      // location x
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j)] =
          m_singleCubeVertice[3 * j] + m_cubeDistance * cubeLocations[3 * i];
      // location y
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 1] =
          m_singleCubeVertice[3 * j + 1] +
          m_cubeDistance * cubeLocations[3 * i + 1];
      // location z
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 2] =
          m_singleCubeVertice[3 * j + 2] +
          m_cubeDistance * cubeLocations[3 * i + 2];
      // color r
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 3] =
          cubeColors[4 * i];
      // color g
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 4] =
          cubeColors[4 * i + 1];
      // color b
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 5] =
          cubeColors[4 * i + 2];
      // color a
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 6] =
          cubeColors[4 * i + 3];
      // origin position x
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 7] =
          m_singleCubeVertice[3 * j];
      // origin position y
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 8] =
          m_singleCubeVertice[3 * j + 1];
      // origin position z
      vertexBufferData[m_stride * (m_singleCubeVertexNum * i + j) + 9] =
          m_singleCubeVertice[3 * j + 2];
    }
  }
  GLuint indexBufferData[m_singleCubeIndexNum * numCubes];
  for (int i = 0; i < numCubes; ++i) {
    for (int j = 0; j < m_singleCubeIndexNum; ++j) {
      indexBufferData[m_singleCubeIndexNum * i + j] =
          m_singleCubeIndices[j] + m_singleCubeVertexNum * i;
    }
  }

  GLint cubeHalfsizeUniformLocation =
      glGetUniformLocation(shader, "cubeHalfsize");
  glUniform1f(cubeHalfsizeUniformLocation, m_cubeWidth / 2.0f);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData,
               GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferData),
               indexBufferData, GL_STATIC_DRAW);

  m_numIdx = m_singleCubeIndexNum * numCubes;
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::render() {
  // Initialize clear color
  // This is the background of the screen.
  glViewport(0, 0, screenWidth, screenHeight);
  glClearColor(0.8f, 0.8f, 0.8f, 1.f);
  glEnable(GL_DEPTH_TEST);

  // Clear color buffer and Depth Buffer
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  // Render data
  // TODO:
  glDrawElements(GL_TRIANGLES,
                 m_numIdx,         // The number of indicies, not triangles.
                 GL_UNSIGNED_INT,  // Make sure the data type matches
                 nullptr);  // Offset pointer to the data. nullptr because we
                            // are currently bound
}

// Loops forever!
// Render images and accept key event
void SDLGraphicsProgram::loop() {
  // Main loop flag
  // If this is quit = 'true' then the program terminates.
  bool quit = false;
  // Event handler that handles various events in SDL
  // that are related to input and output
  SDL_Event e;
  // Enable text input
  SDL_StartTextInput();
  // While application is running
  while (!quit) {
    if (tetris.IsEndGame()) {
      quit = true;
      break;
    }
    static float rotate = 0.0f;
    // MVP matrices
    glm::mat4 projection = glm::perspective(
        45.0f, ((float)screenWidth) / ((float)screenHeight), 0.1f, 30.0f);

    glm::mat4 model = glm::mat4(1.0f);
    // the first translate is for pushing the entire tetris world back
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, -20.0f));
    // the rotate is for rotating the tetris itself
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
    // the second translate is to offset the center of the tetris world from
    model = glm::translate(model, glm::vec3(-(float)tetris.GetX() / 2.0f, 0.0f,
                                            -(float)tetris.GetZ() / 2.0f));

    glm::mat4 view =
        glm::lookAt(glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, -5.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

    GLint modelUniformLocation = glGetUniformLocation(shader, "model");
    GLint viewUniformLocation = glGetUniformLocation(shader, "view");
    GLint projectionUniformLocation =
        glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE,
                       &projection[0][0]);

    static bool isMouseButtonDown(false);
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      // User posts an event to quit
      // An example is hitting the "x" in the corner of the window.
      switch (e.type) {
        case SDL_QUIT:
          std::cout << "==================================================="
                    << std::endl;
          std::cout << "YOU GAINED: " << tetris.GetPoints() << " points!"
                    << std::endl;
          std::cout << "==================================================="
                    << std::endl;
          quit = true;
          break;
        case SDL_KEYDOWN:
          switch (e.key.keysym.sym) {
            case SDLK_w:  // pressed 2
              m_isFilled = !m_isFilled;
              m_isFilled ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
                         : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
              break;
            case SDLK_q:  // pressed q
              quit = true;
              break;
            case SDLK_1:
                  m_Rotation = R3_XCCW;
                  break;
              case SDLK_2:
                  m_Rotation = R3_XCW;
                  break;
              case SDLK_3:
                  m_Rotation = R3_YCCW;
                  break;
              case SDLK_4:
                  m_Rotation = R3_YCW;
                  break;
              case SDLK_5:
                  m_Rotation = R3_ZCCW;
                  break;
              case SDLK_6:
                  m_Rotation = R3_ZCW;
                  break;
            case SDLK_LEFT:
                  if(rotate < PI/4.0f && rotate > -PI/4.0f) {
                      m_Direction = D_XNEG;
                  } else if (rotate > PI/4.0f && rotate < 3.0f*PI/4.0f) {
                      m_Direction = D_ZNEG;
                  } else if (rotate > 3.0f*PI/4.0f || rotate < -3.0f*PI/4.0f) {
                      m_Direction = D_XPOS;
                  } else {
                      m_Direction = D_ZPOS;
                  }
              break;
            case SDLK_RIGHT:
                  if(rotate < PI/4.0f && rotate > -PI/4.0f) {
                      m_Direction = D_XPOS;
                  } else if (rotate > PI/4.0f && rotate < 3*PI/4.0f) {
                      m_Direction = D_ZPOS;
                  } else if (rotate > 3*PI/4.0f || rotate < -3*PI/4.0f) {
                      m_Direction = D_XNEG;
                  } else {
                      m_Direction = D_ZNEG;
                  }
              break;
            case SDLK_UP:
                  if(rotate < PI/4.0f && rotate > -PI/4.0f) {
                      m_Direction = D_ZNEG;
                  } else if (rotate > PI/4.0f && rotate < 3*PI/4.0f) {
                      m_Direction = D_XPOS;
                  } else if (rotate > 3*PI/4.0f || rotate < -3*PI/4.0f) {
                      m_Direction = D_ZPOS;
                  } else {
                      m_Direction = D_XNEG;
                  }
                  break;
              case SDLK_DOWN:
                  if(rotate < PI/4.0f && rotate > -PI/4.0f) {
                      m_Direction = D_ZPOS;
                  } else if (rotate > PI/4.0f && rotate < 3*PI/4.0f) {
                      m_Direction = D_XNEG;
                  } else if (rotate > 3*PI/4.0f || rotate < -3*PI/4.0f) {
                      m_Direction = D_ZNEG;
                  } else {
                      m_Direction = D_XPOS;
                  }
                  break;
            default:
              m_Direction = D_DOWN;
              m_Rotation = R_NONE;
              break;
          }
          break;
        case SDL_MOUSEBUTTONUP:
          isMouseButtonDown = false;
          break;
        case SDL_MOUSEBUTTONDOWN:
          isMouseButtonDown = true;
        case SDL_MOUSEMOTION:
          if (isMouseButtonDown) {
            rotate = (float)e.motion.x / screenWidth * 10.0f;
            if (rotate > PI) {
              rotate -= 2*PI;
            } else if (rotate < -PI) {
              rotate += 2*PI;
            }
          }
          break;
        default:
          break;
      }
    }
    // Update our scene
    update();
    // Render using OpenGL
    render();
    // Update screen of our specified window
    SDL_GL_SwapWindow(getSDLWindow());
    SDL_Delay(m_delay);
    m_timer += m_delay;
  }
  // Update our scene
  update();
  // Render using OpenGL
  render();
  // Update screen of our specified window
  SDL_GL_SwapWindow(getSDLWindow());
  SDL_Delay(750);

  SDL_StopTextInput();
}

// Loads a shader and returns a string
std::string SDLGraphicsProgram::LoadShader(const std::string& fname) {
  std::string result;
  // 1.) Get every line of data
  std::string line;
  std::ifstream myFile(fname.c_str());

  if (myFile.is_open()) {
    while (getline(myFile, line)) {
      result += line + '\n';
    }
  }
  // Close file
  myFile.close();
  return result;
}

unsigned int SDLGraphicsProgram::CreateShader(
    const std::string& vertexShaderSource,
    const std::string& fragmentShaderSource) {
  // Create a new program
  unsigned int program = glCreateProgram();
  // Compile our shaders
  unsigned int myVertexShader =
      CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  unsigned int myFragmentShader =
      CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  // Link our program
  // These have been compiled already.
  glAttachShader(program, myVertexShader);
  glAttachShader(program, myFragmentShader);
  // Link our programs that have been 'attached'
  glLinkProgram(program);
  glValidateProgram(program);

  // Once the shaders have been linked in, we can delete them.
  glDetachShader(program, myVertexShader);
  glDetachShader(program, myFragmentShader);

  glDeleteShader(myVertexShader);
  glDeleteShader(myFragmentShader);

  if (!CheckLinkStatus(program)) {
    std::cout << "ERROR, shader did not link!\n";
  }

  return program;
}

unsigned int SDLGraphicsProgram::CompileShader(unsigned int type,
                                               const std::string& source) {
  // Compile our shaders
  // id is the type of shader (Vertex, fragment, etc.)
  unsigned int id;

  if (type == GL_VERTEX_SHADER) {
    id = glCreateShader(GL_VERTEX_SHADER);
  } else if (type == GL_FRAGMENT_SHADER) {
    id = glCreateShader(GL_FRAGMENT_SHADER);
  }
  const char* src = source.c_str();
  // The source of our shader
  glShaderSource(id, 1, &src, nullptr);
  // Now compile our shader
  glCompileShader(id);

  // Retrieve the result of our compilation
  int result;
  // This code is returning any compilation errors that may have occurred!
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* errorMessages = new char[length];  // Could also use alloca here.
    glGetShaderInfoLog(id, length, &length, errorMessages);
    if (type == GL_VERTEX_SHADER) {
      std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n"
                << errorMessages << "\n";
    } else if (type == GL_FRAGMENT_SHADER) {
      std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n"
                << errorMessages << "\n";
    }
    // Reclaim our memory
    delete[] errorMessages;
    // Delete our broken shader
    glDeleteShader(id);
    return 0;
  }

  return id;
}

// Check to see if linking was successful
bool SDLGraphicsProgram::CheckLinkStatus(GLuint programID) {
  // Link our shaders
  // id is the type of shader (Vertex, fragment, etc.)
  unsigned int id;

  // Retrieve the result of our compilation
  int result;
  // This code is returning any Linker errors that may have occurred!
  glGetProgramiv(id, GL_LINK_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    char* errorMessages = new char[length];  // Could also use alloca here.
    glGetProgramInfoLog(id, length, &length, errorMessages);
    // Reclaim our memory
    std::cout << "ERROR in linking process " << errorMessages << "\n";
    delete[] errorMessages;
    return false;
  }
  return true;
}

// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::getSDLWindow() { return gWindow; }

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::getOpenGLVersionInfo() {
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << "\n";
}



