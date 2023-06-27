#include "APIG23.h"
#include "APIParte3.h"

// da un vector con orden natural
u32* ordenNatural(Grafo G){
    u32 n = NumeroDeVertices(G);
    u32* orden = malloc(n * sizeof(u32));
    for (u32 i = 0; i < n; i++){
        orden[i] = i;
    }
    printf("orden natural creado (n=%u)\n", n);
    return orden;
}
// muestra el orden
void showOrder(Grafo G, u32* order){
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i < n; i++){
        printf("order[%u]=%u\n", i, order[i]);
    }
}
// muestra los vertices en orden
void showGraph(Grafo G, u32* order){
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i < n; i++){
        printf("name[%u] = %u\n", order[i], Nombre(order[i], G));
    }
}

#include <stdio.h>

int main(int argc, char *argv[]){
    char *file = argv[1];
    Grafo G;
    //G = ConstruirGrafo(file);
    
    printf("Vertices:%u ", NumeroDeVertices(G));
    printf("Lados:%u ", NumeroDeLados(G));
    printf("Delta:%u \n", Delta(G));

    u32 *order = ordenNatural(G);
    //showOrder(G, order);
    //showGraph(G, order);
    
    free(order);
    ///DestruirGrafo(G);

    return 1; 

}
                                                                                                            