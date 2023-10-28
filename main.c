#include "structs.h"
#include <string.h>

int main(int argc, char *argv[])
{
    if (!strcmp(argv[1], "-c1") || !strcmp(argv[1], "-c2")) {
        FILE *f = fopen(argv[3], "rb");
        char type[4], c;
        int cnt = 0;
        int width = 0, height = 0, maxi = 0;
        fread(type, sizeof(char), 3, f);
        fread(&c, sizeof(char), 1, f);
        while (c != ' ') {
            width = width * 10 + c - '0';
            fread(&c, sizeof(char), 1, f);
        }
        fread(&c, sizeof(char), 1, f);
        while (c != '\n') {
            height = height * 10 + c - '0';
            fread(&c, sizeof(char), 1, f);
        }
        fread(&c, sizeof(char), 1, f);
        while (c != '\n') {
            maxi = maxi * 10 + c - '0';
            fread(&c, sizeof(char), 1, f);
        }
        Pixel **mat = (Pixel **)malloc(height * sizeof(Pixel *));
        Triplet **sum = (Triplet **)malloc(height * sizeof(Triplet *));
        for (int i = 0; i < height; ++i) {
            mat[i] = (Pixel *)malloc(width * sizeof(Pixel));
            sum[i] = (Triplet *)malloc(width * sizeof(Triplet));
            for (int j = 0; j < width; ++j) {
                fread(&mat[i][j], sizeof(Pixel), 1, f);
                if (i == 0 && j == 0) {
                    sum[i][j].r = (int)mat[i][j].r;
                    sum[i][j].g = (int)mat[i][j].g;
                    sum[i][j].b = (int)mat[i][j].b;
                    continue;
                }
                if (i == 0) {
                    sum[i][j].r = sum[i][j - 1].r + (int)mat[i][j].r;
                    sum[i][j].g = sum[i][j - 1].g + (int)mat[i][j].g;
                    sum[i][j].b = sum[i][j - 1].b + (int)mat[i][j].b;
                    continue;
                }
                if (j == 0) {
                    sum[i][j].r = sum[i - 1][j].r + (int)mat[i][j].r;
                    sum[i][j].g = sum[i - 1][j].g + (int)mat[i][j].g;
                    sum[i][j].b = sum[i - 1][j].b + (int)mat[i][j].b;
                    continue;
                }
                sum[i][j].r = sum[i - 1][j].r + sum[i][j - 1].r - sum[i - 1][j - 1].r + (int)mat[i][j].r;
                sum[i][j].g = sum[i - 1][j].g + sum[i][j - 1].g - sum[i - 1][j - 1].g + (int)mat[i][j].g;
                sum[i][j].b = sum[i - 1][j].b + sum[i][j - 1].b - sum[i - 1][j - 1].b + (int)mat[i][j].b;
            }
        }
        int nivmax = 1, nivmin = (1 << 30), factor = 0, lg = strlen(argv[2]), size = width;
        for (int i = 0; i < lg; ++i)
            factor = factor * 10 + argv[2][i] - '0';
        Triplet z;
        z.r = z.g = z.b = -1;
        TArb r = AlocNod(z);
        Divide_square(0, 0, height, width, width, r, sum, mat, factor);
        if (!strcmp(argv[1], "-c1")) {
            Parcurgere(r, 1, &nivmax, &cnt, &nivmin);
            for (int i = 1; i < nivmin; ++i)
                size /= 2;
            FILE *g = fopen(argv[4], "w");
            fprintf(g, "%d\n%d\n%d", nivmax, cnt, size);
        } else {
            FILE *g = fopen(argv[4], "wb");
            fwrite(&height, 1, sizeof(int), g);
            Parcurgere_nivel(r, g);
        }
    } else if (!strcmp(argv[1], "-d")) {
        FILE *f = fopen(argv[2], "rb");
        FILE *g = fopen(argv[3], "wb");
        int size;
        fread(&size, 1, sizeof(int), f);
        unsigned char c;
        Pixel x; Triplet z;
        z.r = z.g = z.b = -1;
        TArb r = AlocNod(z);
        TCoada q = Aloccelula(NULL);
        Push_back(q, r);
        fread(&c, 1, sizeof(unsigned char), f);
        if (c == 1) {
            fread(&x, 1, sizeof(Pixel), f);
            z.r = (int)x.r;
            z.g = (int)x.g;
            z.b = (int)x.b;
            r->color = z;
        } else {
            fread(&c, 1, sizeof(unsigned char), f);
            while (!feof(f)) {
                if (c == 1) {
                    fread(&x, 1, sizeof(Pixel), f);
                    z.r = (int)x.r;
                    z.g = (int)x.g;
                    z.b = (int)x.b;
                } else
                    z.r = z.g = z.b = -1;
                TArb fiu = AlocNod(z);
                if (q->urm->cnt == 0)
                    Pop_front(q);
                if (q->urm->cnt == 4)
                    q->urm->rad->fiu1 = fiu;
                else if (q->urm->cnt == 3)
                    q->urm->rad->fiu2 = fiu;
                else if (q->urm->cnt == 2)
                    q->urm->rad->fiu3 = fiu;
                else if (q->urm->cnt == 1)
                    q->urm->rad->fiu4 = fiu;
                --q->urm->cnt;
                if (c == 0)
                    Push_back(q, fiu);
                if (!feof(f))
                    fread(&c, 1, sizeof(unsigned char), f);
            }
        }
        Triplet **mat = (Triplet **)malloc(size * sizeof(Triplet *));
        for (int i = 0; i < size; ++i)
            mat[i] = (Triplet *)malloc(size * sizeof(Triplet));
        Creare_imagine(r, 0, 0, size, size, mat);
        unsigned char type[7];
        strcpy(type, "P6\n");
        fwrite(type, 3, sizeof(unsigned char), g);
        unsigned char nr[10];
        int cnt = 0;
        int lg = size;
        do {
            nr[cnt++] = lg % 10 + '0';
            lg /= 10;
        } while (lg);
        for (int i = 0; i < cnt / 2; ++i) {
            char aux = nr[i];
            nr[i] = nr[cnt - i - 1];
            nr[cnt - i - 1] = aux;
        }
        fwrite(nr, cnt, sizeof(unsigned char), g);
        strcpy(type, " ");
        fwrite(type, 1, sizeof(unsigned char), g);
        fwrite(nr, cnt, sizeof(unsigned char), g);
        strcpy(type, "\n");
        fwrite(type, 1, sizeof(unsigned char), g);
        lg = 255;
        cnt = 0;
        do {
            nr[cnt++] = lg % 10 + '0';
            lg /= 10;
        } while (lg);
        for (int i = 0; i < cnt / 2; ++i) {
            unsigned char aux = nr[i];
            nr[i] = nr[cnt - i - 1];
            nr[cnt - i - 1] = aux;
        }
        fwrite(nr, cnt, sizeof(unsigned char), g);
        strcpy(type, "\n");
        fwrite(type, 1, sizeof(unsigned char), g);
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j) {
                Pixel x;
                x.r = (unsigned char)mat[i][j].r;
                x.g = (unsigned char)mat[i][j].g;
                x.b = (unsigned char)mat[i][j].b;
                fwrite(&x, 1, sizeof(Pixel), g);
            }
    }
}