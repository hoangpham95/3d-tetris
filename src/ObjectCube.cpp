//
//  ObjectCube.cpp
//  3d-tetris
//
//  Created by Shanghao Zhong on 11/28/18.
//  Copyright © 2018 Shanghao Zhong. All rights reserved.
//

#include "ObjectCube.hpp"

// Calls the initalization routine
ObjectCube::ObjectCube() {
}



void ObjectCube::Bind() {
    // create buffer
    // Bind to our vertex array
    glBindVertexArray(m_VAOId);
    // Bind to our vertex information
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    // Bind to the elements we are drawing
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
}


void ObjectCube::init() {
    GLfloat halfLength = c_sideLength / 2.0f;
    
    // create vertices
    // a cube
    // zPos layer   | zNeg layer
    // 1 0          | 5 4
    // 2 3          | 6 7
    GLfloat vdata[] = {
        halfLength, halfLength, halfLength,
        -halfLength, halfLength, halfLength,
        -halfLength, -halfLength, halfLength,
        halfLength, -halfLength, halfLength,
        halfLength, halfLength, -halfLength,
        -halfLength, halfLength, -halfLength,
        -halfLength, -halfLength, -halfLength,
        halfLength, -halfLength, -halfLength
    };
    
    // create index
    GLshort idata[] = {
        // front
        0, 1, 2,
        0, 2, 3,
        // back
        4, 6, 5,
        4, 7, 6,
        // top
        0, 4, 5,
        0, 5, 1,
        // bottom
        2, 6, 7,
        2, 7, 3,
        // left
        2, 1, 5,
        2, 5, 6,
        // right
        0, 3, 7,
        0, 7, 4
    };
    
    // Vertex array object
    glGenVertexArrays(1, &m_VAOId);
    glBindVertexArray(m_VAOId);
    
    // vertex buffer
    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 8 *sizeof(GLfloat), vdata, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                // layout 0
                          3,                // vec3 to represent position
                          GL_FLOAT,         // data type
                          GL_FALSE,         // is normalized
                          3*sizeof(GLfloat),// stride
                          0);               // offset
    
    // index buffer
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 12 * sizeof(GLshort), idata, GL_STATIC_DRAW);
}

void ObjectCube::render() {
    Bind();
    glDrawElements(
                   GL_TRIANGLES,
                   36,              // there are 36 indices
                   GL_SHORT,        // use GLshort to represent indices
                   nullptr);
}
