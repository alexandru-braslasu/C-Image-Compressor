#include <stdio.h>
#include <stdlib.h>

typedef struct pixel {
    unsigned char r, g, b;
} Pixel;

typedef struct triplet {
    int r, g, b;
} Triplet;

typedef struct nod {
    Triplet color;
    struct nod *fiu1, *fiu2, *fiu3, *fiu4;
} TNod, *TArb;

typedef struct coada {
    TArb rad;
    int cnt;
    struct coada *urm, *pre;
} TCelula, *TCoada;

TCoada Aloccelula(TArb x);
void Push_back(TCoada s, TArb x);
void Pop_front(TCoada s);
Triplet Calculate(int xi, int yi, int xf, int yf, Triplet **sum);
void Divide_square(int xi, int yi, int xf, int yf, int size, TArb r, Triplet **sum, Pixel **mat, int factor);
TArb AlocNod(Triplet x);
void Parcurgere(TArb r, int niv, int *nivmax, int *cnt, int *nivmin);
void Parcurgere_nivel(TArb r, FILE *g);
void Creare_imagine(TArb r, int xi, int yi, int xf, int yf, Triplet **mat);