#include <stdio.h>
#include <stdlib.h>
#include "APIG23.h"

//constante 2^32 - 1
#define ERR_NUM 4294967295

u32 hashFun(Grafo graph, u32 vertex){ 
    return vertex % graph->numVertices;
}

// Function to create Neighbor
adjacencyNode* createNeighborNode(u32 vertex) {
    adjacencyNode* newNeighbor = malloc(sizeof(adjacencyNode));
    newNeighbor->vertex = vertex;
    newNeighbor->next = NULL;  
    return newNeighbor;
}

// Function to create Vertex
vertexNode* createVertexNode(u32 vertex) {
    vertexNode* newVertex = malloc(sizeof(vertexNode));
    newVertex->vertex = vertex;
    newVertex->grade = 1;
    newVertex->collisionCase = NULL;
    newVertex->adjacencyList = NULL;  
    return newVertex;
}

// Function to create a new Graph
Grafo createGraph(u32 numVertices, u32 numEdges) {
    Grafo graph = malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->delta=0;
    graph->vertexList = malloc(numVertices * sizeof(vertexNode));
    for (u32 i = 0; i < numVertices; i++){
        graph->vertexList[i] = NULL;
    }
    
    return graph;
}

// Function to add an edge to an undirected Grafo
void addEdge(Grafo graph, u32 fst, u32 snd) {
    u32 hash;
    // Add an edge from fst to snd
    hash = hashFun(graph, fst);
    adjacencyNode* newNeighbor = createNeighborNode(snd);
    vertexNode* vertex = graph->vertexList[hash];
    
    // to find if is already in the vertexList
    while (vertex != NULL && vertex->vertex != fst){
        vertex = vertex->collisionCase;
    }
    
    // case not created yet
    if (vertex == NULL){        
        vertex = createVertexNode(fst);
        vertex->adjacencyList = newNeighbor;
        vertex->collisionCase = graph->vertexList[hash];
        graph->vertexList[hash]= vertex;
    
    // case is already created
    } else {
        newNeighbor->next = vertex->adjacencyList;
        vertex->adjacencyList = newNeighbor;
        vertex->grade++;
    }

    hash = hashFun(graph, snd);
    newNeighbor = createNeighborNode(fst);
    vertex = graph->vertexList[hash];

    while (vertex != NULL && vertex->vertex != snd){
        vertex = vertex->collisionCase;
    }

    if (vertex == NULL){
        vertex = createVertexNode(snd);
        vertex->adjacencyList = newNeighbor;
        vertex->collisionCase = graph->vertexList[hash];
        graph->vertexList[hash] = vertex;
    } else {
        newNeighbor->next = vertex->adjacencyList;
        vertex->adjacencyList = newNeighbor;
        vertex->grade++;
    }
}

// Function to create a new Graph from stdin
Grafo readGraphFromStdin() {
     u32 numVertices, numEdges;
    scanf("p edge %u %u\n", &numVertices, &numEdges);
    Grafo graph = createGraph(numVertices, numEdges);

    for (u32 i = 0; i < numEdges; i++) {
        char type;
        u32 src, dest;
        scanf("%c %u %u\n", &type, &src, &dest);
        if (type == 'e') {
            addEdge(graph, src, dest);
        }
    }

    for (u32 i = 0; i < graph->numVertices; i++) {
        vertexNode* current = graph->vertexList[i];
        while (current != NULL) {
            if (graph->delta < current->grade) graph->delta = current->grade; 
            current = current->collisionCase;
        }
    }

    return graph; 
}

// Function to print the Grafo
void printGraph(Grafo graph) {
     int i;
    printf("Graph\n Vertices:%u\n Edges:%u\n Delta:%u\n", NumeroDeVertices(graph), NumeroDeLados(graph), Delta(graph));
    for (i = 0; i < graph->numVertices; i++) {
        vertexNode* vertex = graph->vertexList[i];
        if (vertex != NULL){
            printf("\nAdjacency list of vertex %u[%u]: ", vertex->vertex, vertex->grade);
            adjacencyNode* neighbor = vertex->adjacencyList;
            while (neighbor != NULL) {
                printf("%u -> ", neighbor->vertex);
                neighbor = neighbor->next;
            }
            printf("NULL\n");
        }
    }
}

Grafo ConstruirGrafo(){
    return readGraphFromStdin();
}

void DestruirGrafo(Graph* graph) {
    if (graph == NULL) {
        return;
    }

    // Free adjacency lists of each vertex
    for (u32 i = 0; i < graph->numVertices; i++) {
        if (graph->vertexList[i] != NULL){
            adjacencyNode* adjList = graph->vertexList[i]->adjacencyList;
            while (adjList != NULL) {
                adjacencyNode* temp = adjList;
                adjList = adjList->next;
                free(temp);
            }
            vertexNode* collisionCase = graph->vertexList[i]->collisionCase;
            while (collisionCase != NULL){
                vertexNode* collisionCaseAux = collisionCase->collisionCase;
                adjList = collisionCase->adjacencyList;
                while (adjList != NULL) {
                    adjacencyNode* temp = adjList;
                    adjList = adjList->next;
                    free(temp);
                }
                free(collisionCase);
                collisionCase = collisionCaseAux;
            }
            
        }
    }

    // Free vertex nodes
    for (u32 i = 0; i < graph->numVertices; i++) {
        vertexNode* vertex = graph->vertexList[i];
        free(vertex);
    }

    // Free vertex list
    free(graph->vertexList);

    // Free the graph itself
    free(graph);
}

u32 NumeroDeVertices(Grafo G){
    return G->numVertices;
}

u32 NumeroDeLados(Grafo G){
    return G->numEdges;
}

u32 Delta(Grafo G){
    return G->delta;
}

u32 Nombre(u32 i,Grafo G){
    return G->vertexList[i]->vertex; // some vertex will be ignored 
}

u32 Grado(u32 i,Grafo G){
    return G->vertexList[i]->grade;
}
u32 IndiceVecino(u32 j,u32 i,Grafo G){
    return G->vertexList[i]->adjacencyList[j].vertex;
}


 // Driver code
int main() {
    Grafo grafo = ConstruirGrafo();
    printGraph(grafo);
    DestruirGrafo(grafo);
    return 0;
}