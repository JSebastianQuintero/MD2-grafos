#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t u32;

typedef struct{
    u32 original;
    u32 hash;
} hashNode;

typedef struct{
    u32 numVertices;
    u32 numEdges;
    hashNode** hashes;
} hashInfo;

hashInfo* createHashInfo(u32 numVertices, u32 numEdges){
    hashInfo* hashHeader = malloc(sizeof(hashInfo));
    hashHeader->numVertices = numVertices;
    hashHeader->numEdges = numEdges;
    hashHeader->hashes = malloc(numVertices * sizeof(hashNode));

    return hashHeader;
}

u32 hashFun(hashInfo* hashHeader, u32 vertex){ 
    return vertex % hashHeader->numVertices;
}

u32 addVertex(hashInfo* hashHeader, u32 vertex){
    u32 hash = hashFun(hashHeader, vertex);
    if (hashHeader->hashes[hash] == NULL){
        hashHeader->hashes[hash] = malloc(sizeof(hashNode));
        hashHeader->hashes[hash]->original = vertex;
        hashHeader->hashes[hash]->hash = hash; 
    }
    return !(hashHeader->hashes[hash]->original == vertex); 
}

void printHashes(hashInfo* hashHeader){
    for(u32 i=0; i <  hashHeader->numVertices; i++){
        if (hashHeader->hashes[i] != NULL)
            printf("%u. hash [original:%u, hash:%u]\n", i, hashHeader->hashes[i]->original, hashHeader->hashes[i]->hash);
    }
}

void raiseError(hashInfo* hashHeader, u32 vertex){
    int hash = hashFun(hashHeader, vertex);
    printf("collision detected\n vertex:%u, hash,%u\n vertex:%u, hash,%u\n", hashHeader->hashes[hash]->original, hashHeader->hashes[hash]->hash, vertex, hash);
    exit(1);
}

int main(int argc, char *argv[]){
    u32 numVertices = 0, numEdges = 0;
    int inputCheck = scanf("p edge %u %u\n", &numVertices, &numEdges);

    if (inputCheck != 2) {
        printf("Error: No file input provided or invalid format.\n");
        printf("Usage: ./program < <input_file>\n");
        return 1; 
    }

    hashInfo* hashHeader = createHashInfo(numVertices, numEdges);
    
    for (u32 i = 0; i < numEdges; i++) {
        char type;
        u32 src, dest, err;
        scanf("%c %u %u\n", &type, &src, &dest);
        if (type == 'e') {
            err = addVertex(hashHeader, src);
            if (err) raiseError(hashHeader, src);
            addVertex(hashHeader, dest);
            if (err) raiseError(hashHeader, dest);
        }
    }

    //printHashes(hashHeader);
    for (u32 i = 0; i < hashHeader->numVertices; i++){
        free(hashHeader->hashes[i]);
    }
    free(hashHeader->hashes);
    free(hashHeader);
    return 0; 
}
                                                                                                            