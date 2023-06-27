#include "APIG23.h"
//constante 2^32 - 1
#define ERR_NUM 4294967295

void swap(u32* px, u32* py)
{
    u32 temp = *px;
    *px = *py;
    *py = temp;
}

void quicksort(u32* peers0, u32* peers1, u32 izq, u32 der)
{
    u32 i = izq, j = der;
    u32 pivot = peers0[(izq + der) / 2];

    /* partición */
    while (i <= j) {
        while (peers0[i] < pivot) i++;
        while (peers0[j] > pivot) j--;
        if (i <= j) {
            swap(&peers0[i], &peers0[j]);
            swap(&peers1[i], &peers1[j]);
            i++;
            j--;
        }
    }

    /* recursión */
    if (izq < j)
        quicksort(peers0, peers1, izq, j);
    if (i < der)
        quicksort(peers0, peers1, i, der);
}


void ConstruirVertex(Grafo G, u32 name, u32 i){
    G->vertexs[i].name = name;
    G->vertexs[i].grade = 1;
}

u32 Comandvalido(char *line){
    u32 res = 2;
    if (strncmp(line, "p",1)!=0){
        res = 0;
    } else {
        if (strncmp(line, "p edge ",7)==0){
            u32 vertex;
            u32 sides;
            sscanf(line,
                "%*[^0123456789]%u%*[^0123456789]%u",
                &vertex, 
                &sides
            );
            if (vertex>0 && sides>0){
                res = 1;
            }
        }
    }
    return res;
}


u32 Binary_Search(Grafo G, u32 lower_bound, u32 upper_bound, u32 name){
    u32 beg, end, pos, mid;
    beg = lower_bound;
    end = upper_bound;
    pos = -1;
    mid = (beg+end)/2;
    while (beg<=end){
        if (G->vertexs[mid].name == name) {
            pos = mid;
            return pos;
        } else {
            if (G->vertexs[mid].name > name) {
                end = mid - 1;
            } else {
                beg = mid + 1;  
            }
            mid = (beg+end)/2;
        }
    }
    return pos;
}


Grafo ConstruirGrafo(){
    Grafo grafo = malloc(sizeof(GrafoSt));
    char *line = malloc(1024*sizeof(char));
    char *caracter = fgets(line, 1024, stdin);
    if(caracter==NULL) {
        return NULL;                                                                // invalid FILE
    }
    u32 is_valid = Comandvalido(caracter);
    while(is_valid==0) {                                                            // omite las primeras lineas
        caracter = fgets(line,1024,stdin);
        is_valid = Comandvalido(caracter);
    }
    if (is_valid==1){                                                               // encontro p edge
        sscanf(line,
            "%*[^0123456789]%u%*[^0123456789]%u",
            &grafo->num_vertexs,
            &grafo->num_sides
        );
        u32* peers0 = (u32*) malloc(grafo->num_sides * 2 * sizeof(u32));
        u32* peers1 = (u32*) malloc(grafo->num_sides * 2 * sizeof(u32));
        for (u32 i = 0; i < grafo->num_sides; i++){
            caracter = fgets(line,1024,stdin);
            if (caracter == NULL){
                return NULL;
            } else {
                sscanf(line,
                    "%*[^0123456789]%u%*[^0123456789]%u",
                    &peers0[i],
                    &peers1[i]
                );
                peers0[grafo->num_sides+i] = peers1[i];
                peers1[grafo->num_sides+i] = peers0[i];                         // duplica de aristas espejadas
            }
        }
        free(line);

        quicksort(peers0, peers1, 0, (grafo->num_sides * 2)-1);
        
        // carga de los vertices
        u32 j=0;                                                                    // indice de los vertices
        grafo->vertexs = malloc(grafo->num_vertexs * sizeof(Vertex));               // reserva de memoria para la cantidad de vertices
        grafo->delta = 0;
        ConstruirVertex(grafo, peers0[0], 0);                                     // carga del vertice inicial

        for (u32 i = 1; i < (grafo->num_sides*2); i++) {
            if (peers0[i] != peers0[i-1]) {                                     // carga de los vertices no repetidas
                j++;
                ConstruirVertex(grafo, peers0[i], j);
            }else{
                grafo->vertexs[j].grade++;
            }
        }

        // carga de las aristas
        j = 0;
        u32 i_neighbor  = 0;
        grafo->vertexs[j].neighbors = malloc(sizeof(u32) * grafo->vertexs[j].grade);
        grafo->delta = grafo->vertexs[j].grade;
        
        for (u32 i = 0; i < (grafo->num_sides*2); i++) {
            grafo->vertexs[j].neighbors[i_neighbor] = Binary_Search(grafo, 0, grafo->num_vertexs - 1, peers1[i]);
            i_neighbor++;
            if (grafo->vertexs[j].grade == i_neighbor){
                j++;
                i_neighbor=0;
                if (grafo->delta < grafo->vertexs[j].grade)
                    grafo->delta = grafo->vertexs[j].grade;
                if (j < grafo->num_vertexs)
                    grafo->vertexs[j].neighbors = malloc(sizeof(u32) * grafo->vertexs[j].grade);
            }        
        }

        // Liberacion de la memoria utilizada por peers
        free(peers0);
        free(peers1);
        peers0 = NULL;
        peers1 = NULL;

    } else {
        return NULL;                                                                    // Invalid Command
    }
    //fclose(archivo);
    return grafo;
}


//Devuelve el numero de vertices
u32 NumeroDeVertices(Grafo G){
    return G->num_vertexs;
}


//Devuelve el numero de lados
u32 NumeroDeLados(Grafo G){
    return G->num_sides;
}


//Devuelve delta de un grafo que ya se calcula cuando se construye el grafo
u32 Delta(Grafo G){
    return G->delta;
}


//Devuelve el nombre del vertice cuyo indice en el arreglo de vertices es i
//Hay que tener cuidado al usarla porque la consigna pone que no hay forma de notificar un error
u32 Nombre(u32 i,Grafo G){
    return G->vertexs[i].name;
}


//Devuelve el grado del vertice cuyo indice en el arreglo de vertices es i
u32 Grado(u32 i,Grafo G){
    return i<G->num_vertexs?G->vertexs[i].grade:ERR_NUM;
}


//Devuelve el indice del vecino del vertice cuyo indice en el arreglo de vertices es i
//y el indice j es la posicion el el arreglo de vecinos
u32 IndiceVecino(u32 j,u32 i,Grafo G){
    return i<G->num_vertexs&&j<G->vertexs[i].grade?G->vertexs[i].neighbors[j]:ERR_NUM;
}


// Libera toda la memoria utilizada por la estructura de tipo Grafo 
void DestruirGrafo(Grafo G){
    for (u32 i = 0; i < NumeroDeVertices(G); i++){
        free(G->vertexs[i].neighbors);
        G->vertexs[i].neighbors = NULL;
    }
    free(G->vertexs);
    G->vertexs = NULL;
    free(G);
    G = NULL;
}