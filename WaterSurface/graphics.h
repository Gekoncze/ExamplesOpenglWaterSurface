#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "myopengl.h"
#include "watermodel.h"
#include "wallmodel.h"
#include "opensimplexnoise.h"
#include "Matrix4f.h"

const float FIELD_OF_VIEW = 45.0f * 3.14f / 180.0f;
const float Z_NEAR = 0.1f;
const float Z_FAR = 20.0f;

Matrix4f projectionViewMatrix;
float currentTime = 0.0f;
float cameraDistance = 3.0f;
float cameraYaw = 0;//60.0f * 3.14f / 180.0f;
float cameraPitch = 0;//40.0f * 3.14f / 180.0f;

void updateMatrix(int w, int h)
{
    float aspect = (float)w / h;
    projectionViewMatrix = Matrix4f::perspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
    projectionViewMatrix = projectionViewMatrix * Matrix4f::translation(0.0f, 0.0f, -cameraDistance);
    projectionViewMatrix = projectionViewMatrix * Matrix4f::yawPitchRoll(cameraYaw, cameraPitch, 0.0f);
    projectionViewMatrix = projectionViewMatrix * Matrix4f::rotationX(90*3.1415/180);
}

void initOpengl()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 1, 1);
    checkForErrors("Could not init OpenGL");
}


/*******************************************************************************************************/


GLuint wallArray;
GLuint wallVertexBuffer;
GLuint wallUvBuffer;
GLuint wallProgram;

GLint wallVertexHandle;
GLint wallUvHandle;
GLint wallMatrixHandle;
GLint wallPermHandle;
GLint wallPermGradIndex3DHandle;
GLint wallGradients3DHandle;
GLint wallTimeHandle;

void initWall()
{
    wallProgram = loadProgram("Resources\\shaders\\wall.vert", "Resources\\shaders\\wall.frag");
	glUseProgram(wallProgram);

    wallVertexHandle = glGetAttribLocation(wallProgram, "vertex");
    wallUvHandle = glGetAttribLocation(wallProgram, "uv");
    wallMatrixHandle = glGetUniformLocation(wallProgram, "matrix");
	wallPermHandle = glGetUniformLocation(wallProgram, "perm");
	wallPermGradIndex3DHandle = glGetUniformLocation(wallProgram, "permGradIndex3D");
	wallGradients3DHandle = glGetUniformLocation(wallProgram, "gradients3D");
	wallTimeHandle = glGetUniformLocation(wallProgram, "time");

	glUniform1iv(wallPermHandle, permSize, perm);
	glUniform1iv(wallPermGradIndex3DHandle, permGradIndex3DSize, permGradIndex3D);
	glUniform1iv(wallGradients3DHandle, gradients3DSize, gradients3D);

    glGenBuffers(1, &wallVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wallVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, wallVerticesSizeInBytes, wallVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &wallUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wallUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, wallUvsSizeInBytes, wallUvs, GL_STATIC_DRAW);

    glGenVertexArrays(1, &wallArray);
    glBindVertexArray(wallArray);

    glEnableVertexAttribArray(wallVertexHandle);
    glBindBuffer(GL_ARRAY_BUFFER, wallVertexBuffer);
    glVertexAttribPointer(wallVertexHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(wallUvHandle);
    glBindBuffer(GL_ARRAY_BUFFER, wallUvBuffer);
    glVertexAttribPointer(wallUvHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);

    checkForErrors("Could not init wall");
}

void drawWall()
{
    glUseProgram(wallProgram);
    glBindVertexArray(wallArray);
	glUniform1f(wallTimeHandle, currentTime);
	glUniformMatrix4fv(wallMatrixHandle, 1, GL_FALSE, projectionViewMatrix.getData());
    glDrawArrays(GL_TRIANGLES, 0, wallVertexCount);
    checkForErrors("Could not draw wall");
}


/*******************************************************************************************************/


GLuint waterArray;
GLuint waterVertexBuffer;
GLuint waterUvBuffer;
GLuint waterProgram;

GLint waterVertexHandle;
GLint waterUvHandle;
GLint waterMatrixHandle;
GLint waterPermHandle;
GLint waterPermGradIndex3DHandle;
GLint waterGradients3DHandle;
GLint waterTimeHandle;

void initWater(){
    waterProgram = loadProgram("Resources\\shaders\\water.vert", "Resources\\shaders\\water.frag");
    glUseProgram(waterProgram);

    waterVertexHandle = glGetAttribLocation(waterProgram, "vertex");
    waterUvHandle = glGetAttribLocation(waterProgram, "uv");
    waterMatrixHandle = glGetUniformLocation(waterProgram, "matrix");
    waterPermHandle = glGetUniformLocation(waterProgram, "perm");
    waterPermGradIndex3DHandle = glGetUniformLocation(waterProgram, "permGradIndex3D");
    waterGradients3DHandle = glGetUniformLocation(waterProgram, "gradients3D");
    waterTimeHandle = glGetUniformLocation(waterProgram, "time");

    glUniform1iv(waterPermHandle, permSize, perm);
    glUniform1iv(waterPermGradIndex3DHandle, permGradIndex3DSize, permGradIndex3D);
    glUniform1iv(waterGradients3DHandle, gradients3DSize, gradients3D);

    glGenBuffers(1, &waterVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, waterVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, waterVerticesSizeInBytes, waterVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &waterUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, waterUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, waterUvsSizeInBytes, waterUvs, GL_STATIC_DRAW);

    glGenVertexArrays(1, &waterArray);
    glBindVertexArray(waterArray);

    glEnableVertexAttribArray(waterVertexHandle);
    glBindBuffer(GL_ARRAY_BUFFER, waterVertexBuffer);
    glVertexAttribPointer(waterVertexHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(waterUvHandle);
    glBindBuffer(GL_ARRAY_BUFFER, waterUvBuffer);
    glVertexAttribPointer(waterUvHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);

    checkForErrors("Could not init water");
}

void drawWater()
{
    glUseProgram(waterProgram);
    glBindVertexArray(waterArray);
    glUniformMatrix4fv(waterMatrixHandle, 1, GL_FALSE, projectionViewMatrix.getData());
    glUniform1f(waterTimeHandle, currentTime);
    glDrawArrays(GL_TRIANGLES, 0, waterVertexCount);
    checkForErrors("Could not draw water");
}


/*******************************************************************************************************/


void initGraphics()
{
    initOpengl();
    updateMatrix(1, 1);
    initWall();
    initWater();
}

void drawGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawWall();
	drawWater();
}

#endif
