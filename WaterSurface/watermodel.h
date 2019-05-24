#ifndef GLITTERMODEL_H
#define GLITTERMODEL_H

int waterVertexCount = 6;
int waterVerticesSizeInBytes = waterVertexCount * 3 * sizeof(float);
int waterUvsSizeInBytes = waterVertexCount * 2 * sizeof(float);

float waterVertices[] = {
    -1.0f, 0.0f, -1.0f,
    1.0f, 0.0f, -1.0f,
    1.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 1.0f,
    -1.0f, 0.0f, 1.0f,
    -1.0f, 0.0f, -1.0f
};

float waterUvs[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};

#endif

