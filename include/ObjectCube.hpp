//
//  ObjectCube.hpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/28/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#ifndef ObjectCube_hpp
#define ObjectCube_hpp


#include "Object.h"
#include <glad/glad.h>


// The class is responsible for render a cube in openGL. Since this is a relatively
// simple task, it is not using Buffer class or Texture class for now since it
// doesn't need texture. In fact, it just need the a vertex buffer to record the
// positions of vertices and an index buffer.
class ObjectCube : public Object {
public:
    ObjectCube();
    void init() override;
    void render() override;
    
private:
    // Vertex Array Object
    GLuint m_VAOId;
    // Vertex Buffer
    GLuint m_vertexPositionBuffer;
    // Index Buffer Object
    GLuint m_indexBufferObject;
    const GLfloat c_sideLength{0.8};
    void Bind() override;
};



#endif /* ObjectCube_hpp */
