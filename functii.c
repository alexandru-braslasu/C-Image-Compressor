#include "structs.h"

TArb AlocNod(Triplet x) {
    TArb arb = (TArb)malloc(sizeof(TNod));
    if (arb) {
        arb->color = x;
        arb->fiu1 = arb->fiu2 = arb->fiu3 = arb->fiu4 = NULL;
        return arb;
    }
    return NULL;
}

TCoada Aloccelula(TArb x) {
    TCoada aux = (TCoada)malloc(sizeof(TCelula));
    if (aux) {
        aux->rad = x;
        aux->cnt = 4;
        aux->urm = aux->pre = NULL;
    }
    return aux;
}

void Push_back(TCoada s, TArb x) {
    TCoada aux = Aloccelula(x);
    if (!aux)
        return;
    if (s->urm == NULL) {
        s->pre = s->urm = aux;
        aux->pre = aux->urm = s;
        return;
    }
    aux->pre = s->pre;
    s->pre->urm = aux;
    aux->urm = s;
    s->pre = aux;
}

void Pop_front(TCoada s) {
    TCoada aux = s->urm;
    if (s->urm == s->pre) {
        free(aux);
        s->urm = s->pre = NULL;
        return;
    }
    s->urm = s->urm->urm;
    s->urm->pre = s;
    free(aux);
}

Triplet Calculate(int xi, int yi, int xf, int yf, Triplet **sum) {
    Triplet x;
    if ((xi - 1 < 0) && (yi - 1 < 0))
        x = sum[xf][yf];
    else if ((xi - 1 < 0) && (yi - 1 >= 0)) {
        x.r = sum[xf][yf].r - sum[xf][yi - 1].r;
        x.g = sum[xf][yf].g - sum[xf][yi - 1].g;
        x.b = sum[xf][yf].b - sum[xf][yi - 1].b;
    } else if ((xi - 1 >= 0) && (yi - 1 < 0)) {
        x.r = sum[xf][yf].r - sum[xi - 1][yf].r;
        x.g = sum[xf][yf].g - sum[xi - 1][yf].g;
        x.b = sum[xf][yf].b - sum[xi - 1][yf].b;
    } else if ((xi - 1 >= 0) && (yi - 1 >= 0)) {
        x.r = sum[xf][yf].r - sum[xi - 1][yf].r - sum[xf][yi - 1].r + sum[xi - 1][yi - 1].r;
        x.g = sum[xf][yf].g - sum[xi - 1][yf].g - sum[xf][yi - 1].g + sum[xi - 1][yi - 1].g;
        x.b = sum[xf][yf].b - sum[xi - 1][yf].b - sum[xf][yi - 1].b + sum[xi - 1][yi - 1].b;
    }
    return x;
}

void Divide_square(int xi, int yi, int xf, int yf, int size, TArb r, Triplet **sum, Pixel **mat, int factor) {
    if (size >= 1) {
        Triplet x = Calculate(xi, yi, xf - 1, yf - 1, sum);
        int red = x.r / (size * size);
        int green = x.g / (size * size);
        int blue = x.b / (size * size);
        long long suma = 0;
        for (int i = xi; i < xf; ++i)
            for (int j = yi; j < yf; ++j) {
                int rosu = red - (int)mat[i][j].r;
                int verde = green - (int)mat[i][j].g;
                int albastru = blue - (int)mat[i][j].b;
                suma = suma + 1LL * (rosu * rosu + verde * verde + albastru * albastru);
            }
        suma = suma / (3 * size *size);
        if (suma > factor) {
            Triplet z;
            z.r = z.g = z.b = -1;
            TArb child1 = AlocNod(z), child2 = AlocNod(z), child3 = AlocNod(z), child4 = AlocNod(z);
            r->fiu1 = child1; r->fiu2 = child2; r->fiu3 = child3; r->fiu4 = child4;
            r->color = z;
            Divide_square(xi, yi, (xi + xf) / 2, (yi + yf) / 2, size / 2, r->fiu1, sum, mat, factor);
            Divide_square(xi, (yi + yf) / 2, (xi + xf) / 2, yf, size / 2, r->fiu2, sum, mat, factor);
            Divide_square((xi + xf) / 2, (yi + yf) / 2, xf, yf, size / 2, r->fiu3, sum, mat, factor);
            Divide_square((xi + xf) / 2, yi, xf, (yi + yf) / 2, size / 2, r->fiu4, sum, mat, factor);
            return;
        }
        r->color.r = red;
        r->color.g = green;
        r->color.b = blue;
    }
}

void Parcurgere(TArb r, int niv, int *nivmax, int *cnt, int *nivmin) {
    if (r == NULL)
        return;
    if (niv > *nivmax)
        *nivmax = niv;
    if (r->fiu1 == NULL && r->fiu2 == NULL && r->fiu3 == NULL && r->fiu4 == NULL) {
        ++(*cnt);
        if (niv < *nivmin)
           *nivmin = niv;
    }
    Parcurgere(r->fiu1, niv + 1, nivmax, cnt, nivmin);
    Parcurgere(r->fiu2, niv + 1, nivmax, cnt, nivmin);
    Parcurgere(r->fiu3, niv + 1, nivmax, cnt, nivmin);
    Parcurgere(r->fiu4, niv + 1, nivmax, cnt, nivmin);
}

void Parcurgere_nivel(TArb r, FILE *g) {
    TCoada q = Aloccelula(NULL);
    unsigned char c;
    Push_back(q, r);
    while (q->urm != NULL) {
        if (q->urm->rad == NULL) {
            Pop_front(q);
            continue;
        }
        Push_back(q, q->urm->rad->fiu1);
        Push_back(q, q->urm->rad->fiu2);
        Push_back(q, q->urm->rad->fiu3);
        Push_back(q, q->urm->rad->fiu4);
        TArb x = q->urm->rad;
        Pop_front(q);
        if (x->fiu1 == NULL && x->fiu2 == NULL && x->fiu3 == NULL && x->fiu4 == NULL) {
            c = 1;
            fwrite(&c, 1, sizeof(unsigned char), g);
            Triplet y = x->color;
            c = (unsigned char)y.r;
            fwrite(&c, 1, sizeof(unsigned char), g);
            c = (unsigned char)y.g;
            fwrite(&c, 1, sizeof(unsigned char), g);
            c = (unsigned char)y.b;
            fwrite(&c, 1, sizeof(unsigned char), g);
            //printf("frunza red: %d, green: %d, blue: %d\n", y.r, y.g, y.b);
        } else {
            c = 0;
            fwrite(&c, 1, sizeof(unsigned char), g);
            //printf("interior 0\n");
        }
    }
}

void Creare_imagine(TArb r, int xi, int yi, int xf, int yf, Triplet **mat) {
    if (r == NULL)
        return;
    if (r->color.r == -1) {
        Creare_imagine(r->fiu1, xi, yi, (xi + xf) / 2, (yi + yf) / 2, mat);
        Creare_imagine(r->fiu2, xi, (yi + yf) / 2, (xi + xf) / 2, yf, mat);
        Creare_imagine(r->fiu3, (xi + xf) / 2, (yi + yf) / 2, xf, yf, mat);
        Creare_imagine(r->fiu4, (xi + xf) / 2, yi, xf, (yi + yf) / 2, mat);
        return;
    }
    for (int i = xi; i < xf; ++i)
        for (int j = yi; j < yf; ++j)
            mat[i][j] = r->color;
}