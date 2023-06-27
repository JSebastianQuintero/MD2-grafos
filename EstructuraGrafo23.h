#ifndef PARTE1_ESTRUCTURAGRAFO23_H
#define PARTE1_ESTRUCTURAGRAFO23_H

#include <stdint.h>

typedef uint32_t u32;

typedef struct adjacencyNode {
    u32 vertex;
    struct adjacencyNode* next;  
} adjacencyNode;

typedef struct vertexNode {
    u32 vertex;
    u32 grade;
    struct vertexNode* collisionCase;
    struct adjacencyNode* adjacencyList;
} vertexNode;

typedef struct Graph {
    u32 numVertices;
    u32 numEdges;
    u32 delta;
    vertexNode** vertexList; // using hash for index
} Graph;

#endif                      //PARTE1_ESTRUCTURAGRAFO23_H