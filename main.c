#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define Y 1024
#define X 1024

#define P 16   // Anzahl Threads
#define N 1024 //Groesse Matrix

// Wurzel(P)
#define BLOCKS_Y 4
#define BLOCKS_X 4

#define BLOCKSIZEX (N / BLOCKS_X)
#define BLOCKSIZEY (N / BLOCKS_Y)

struct entity
{
    char typ;

    int alter;

    //Hai
    int verhungerungsZeit;

    int bewegt;
};

struct entity spielfeld[Y][X];

const int fischVermehrungsDauer = 5;
const int haiVermehrungsDauer = 30;

void FeldFuellen()
{
    srand(time(NULL));

    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            int r = rand() % 100;
            if (r < 3)
            {
                spielfeld[i][j].typ = 'h';
                spielfeld[i][j].verhungerungsZeit = 10;
                spielfeld[i][j].alter = 0;
                spielfeld[i][j].bewegt = 0;
            }
            else if (r < 15)
            {
                spielfeld[i][j].typ = 'f';
                spielfeld[i][j].alter = 0;
                spielfeld[i][j].bewegt = 0;
            }
            else
            {
                spielfeld[i][j].typ = 'l';
            }
        }
    }
}

void HaiSchritt(int i, int j)
{
    int gefressen = 0;
    int bewegt = 0;
    int tempAlter;

    spielfeld[i][j].alter++;
    spielfeld[i][j].verhungerungsZeit--;

    if (spielfeld[i][j].verhungerungsZeit <= 0)
    {
        spielfeld[i][j].typ = 'l';
        spielfeld[i][j].alter = 0;
        spielfeld[i][j].verhungerungsZeit = 0;
        return;
    }

    tempAlter = spielfeld[i][j].alter;

    //Jagen
    for (int k = 0; k < 4; k++)
    {
        switch (k)
        {
        case 0: //oben
            if (i == 0)
            {
                if (spielfeld[Y - 1][j].typ == 'f')
                {
                    spielfeld[Y - 1][j].typ = 'h';
                    spielfeld[Y - 1][j].alter = spielfeld[i][j].alter;
                    spielfeld[Y - 1][j].verhungerungsZeit = 10;
                    spielfeld[Y - 1][j].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld[i - 1][j].typ == 'f')
                {
                    spielfeld[i - 1][j].typ = 'h';
                    spielfeld[i - 1][j].alter = spielfeld[i][j].alter;
                    spielfeld[i - 1][j].verhungerungsZeit = 10;
                    spielfeld[i - 1][j].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            break;
        case 1: //links
            if (j == 0)
            {
                if (spielfeld[i][X - 1].typ == 'f')
                {
                    spielfeld[i][X - 1].typ = 'h';
                    spielfeld[i][X - 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][X - 1].verhungerungsZeit = 10;
                    spielfeld[i][X - 1].bewegt = 1;

                    spielfeld[i][j].typ = 'l';
                }
            }
            else
            {
                if (spielfeld[i][j].typ == 'f')
                {
                    spielfeld[i][j - 1].typ = 'h';
                    spielfeld[i][j - 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][j - 1].verhungerungsZeit = 10;
                    spielfeld[i][j - 1].bewegt = 1;

                    spielfeld[i][j].typ = 'l';
                }
            }
            break;
        case 2: //unten
            if (i == (Y - 1))
            {
                if (spielfeld[0][j].typ == 'f')
                {
                    spielfeld[0][j].typ = 'h';
                    spielfeld[0][j].alter = spielfeld[i][j].alter;
                    spielfeld[0][j].verhungerungsZeit = 10;
                    spielfeld[0][j].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld[i + 1][j].typ == 'f')
                {
                    spielfeld[i + 1][j].typ = 'h';
                    spielfeld[i + 1][j].alter = spielfeld[i][j].alter;
                    spielfeld[i + 1][j].verhungerungsZeit = 10;
                    spielfeld[i + 1][j].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            break;
        case 3: //rechts
            if (j == (X - 1))
            {
                if (spielfeld[i][0].typ == 'f')
                {
                    spielfeld[i][0].typ = 'h';
                    spielfeld[i][0].alter = spielfeld[i][j].alter;
                    spielfeld[i][0].verhungerungsZeit = 10;
                    spielfeld[i][0].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld[i][j].typ == 'f')
                {
                    spielfeld[i][j + 1].typ = 'h';
                    spielfeld[i][j + 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][j + 1].verhungerungsZeit = 10;
                    spielfeld[i][j + 1].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            break;
        default:
            printf("Error Hai Jagd");
            break;
        }

        if (gefressen == 1)
        {
            break;
        }
    }

    //Bewegen

    if (gefressen == 0)
    {
        if (i == 0)
        {
            if (spielfeld[X - 1][j].typ != 'l')
            {
                bewegt = 1;
            }
        }
        if (j == 0)
        {
            if (spielfeld[i][Y - 1].typ != 'l')
            {
                bewegt = 1;
            }
        }
        if (i == X)
        {
            if (spielfeld[0][j].typ != 'l')
            {
                bewegt = 1;
            }
        }
        if (j == Y)
        {
            if (spielfeld[i][0].typ != 'l')
            {
                bewegt = 1;
            }
        }
        if (spielfeld[i - 1][j].typ != 'l' && spielfeld[i + 1][j].typ != 'l' &&
            spielfeld[i][j - 1].typ != 'l' && spielfeld[i][j + 1].typ != 'l')
        {
            bewegt = 1;
        }

        do
        {
            int r = rand() % 4;
            switch (r)
            {
            case 0: //oben
                if (i != 0)
                {
                    if (spielfeld[i - 1][j].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[i - 1][j].typ = 'h';
                    spielfeld[i - 1][j].alter = spielfeld[i][j].alter;
                    spielfeld[i - 1][j].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[i - 1][j].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld[Y - 1][j].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[Y - 1][j].typ = 'h';
                    spielfeld[Y - 1][j].alter = spielfeld[i][j].alter;
                    spielfeld[Y - 1][j].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[Y - 1][j].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                break;
            case 1: //links
                if (j != 0)
                {
                    if (spielfeld[i][j - 1].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[i][j - 1].typ = 'h';
                    spielfeld[i][j - 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][j - 1].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[i][j - 1].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld[i][X - 1].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[i][X - 1].typ = 'h';
                    spielfeld[i][X - 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][X - 1].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[i][X - 1].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                break;
            case 2: //unten
                if (i != Y)
                {
                    if (spielfeld[i + 1][j].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[i + 1][j].typ = 'h';
                    spielfeld[i + 1][j].alter = spielfeld[i][j].alter;
                    spielfeld[i + 1][j].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[i + 1][j].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld[0][j].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[0][j].typ = 'h';
                    spielfeld[0][j].alter = spielfeld[i][j].alter;
                    spielfeld[0][j].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[0][j].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                break;
            case 3: //rechts
                if (j != X)
                {
                    if (spielfeld[i][j + 1].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[i][j + 1].typ = 'h';
                    spielfeld[i][j + 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][j + 1].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[i][j + 1].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld[i][0].typ != 'l')
                    {
                        break;
                    }
                    spielfeld[i][0].typ = 'h';
                    spielfeld[i][0].alter = spielfeld[i][j].alter;
                    spielfeld[i][0].verhungerungsZeit = spielfeld[i][j].verhungerungsZeit;
                    spielfeld[i][0].bewegt = 1;
                    spielfeld[i][j].typ = 'l';
                    bewegt = 1;
                    break;
                }
                break;
            default:
                printf("Error Hai Bewegung %d", r);
                break;
            }
        } while (bewegt != 1);
    }

    //Vermehren
    if ((bewegt == 1 || gefressen == 1) && spielfeld[i][j].alter > haiVermehrungsDauer)
    {
        spielfeld[i][j].typ = 'h';
        spielfeld[i][j].alter = 0;
        spielfeld[i][j].verhungerungsZeit = 10;
    }
}

void FischSchritt(int i, int j)
{
    spielfeld[i][j].alter++;
    int bewegt = 0;
    if (i == 0)
    {
        if (spielfeld[X - 1][j].typ != 'l')
        {
            bewegt = 1;
        }
    }
    if (j == 0)
    {
        if (spielfeld[i][Y - 1].typ != 'l')
        {
            bewegt = 1;
        }
    }
    if (i == X)
    {
        if (spielfeld[0][j].typ != 'l')
        {
            bewegt = 1;
        }
    }
    if (j == Y)
    {
        if (spielfeld[i][0].typ != 'l')
        {
            bewegt = 1;
        }
    }
    if (spielfeld[i - 1][j].typ != 'l' && spielfeld[i + 1][j].typ != 'l' &&
        spielfeld[i][j - 1].typ != 'l' && spielfeld[i][j + 1].typ != 'l')
    {
        bewegt = 1;
    }

    do
    {
        int r = rand() % 4;
        switch (r)
        {
        case 0: //oben
            if (i != 0)
            {
                if (spielfeld[i - 1][j].typ != 'l')
                {
                    break;
                }
                spielfeld[i - 1][j].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[i - 1][j].alter = spielfeld[i][j].alter;
                spielfeld[i - 1][j].bewegt = 1;
                bewegt = 1;
            }
            else
            {
                if (spielfeld[Y - 1][j].typ != 'l')
                {
                    break;
                }
                spielfeld[Y - 1][j].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[Y - 1][j].alter = spielfeld[i][j].alter;
                spielfeld[Y - 1][j].bewegt = 1;
                bewegt = 1;
            }
            break;
        case 1: //links
            if (j != 0)
            {
                if (spielfeld[i][j - 1].typ != 'l')
                {
                    break;
                }
                spielfeld[i][j - 1].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[i][j - 1].alter = spielfeld[i][j].alter;
                spielfeld[i][j - 1].bewegt = 1;
                bewegt = 1;
            }
            else
            {
                if (spielfeld[i][X - 1].typ != 'l')
                {
                    break;
                }
                spielfeld[i][X - 1].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[i][X - 1].alter = spielfeld[i][j].alter;
                spielfeld[i][X - 1].bewegt = 1;
                bewegt = 1;
            }
            break;
        case 2: //unten
            if (i != Y)
            {
                if (spielfeld[i + 1][j].typ != 'l')
                {
                    break;
                }
                spielfeld[i + 1][j].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[i + 1][j].alter = spielfeld[i][j].alter;
                spielfeld[i + 1][j].bewegt = 1;
                bewegt = 1;
            }
            else
            {
                if (spielfeld[0][j].typ != 'l')
                {
                    break;
                }
                spielfeld[0][j].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[0][j].alter = spielfeld[i][j].alter;
                spielfeld[0][j].bewegt = 1;
                bewegt = 1;
            }
            break;
        case 3: //rechts
            if (j != X)
            {
                if (spielfeld[i][j + 1].typ != 'l')
                {
                    break;
                }
                spielfeld[i][j + 1].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[i][j + 1].alter = spielfeld[i][j].alter;
                spielfeld[i][j + 1].bewegt = 1;
                bewegt = 1;
            }
            else
            {
                if (spielfeld[i][0].typ != 'l')
                {
                    break;
                }
                spielfeld[i][0].typ = 'f';
                spielfeld[i][j].typ = 'l';
                spielfeld[i][0].alter = spielfeld[i][j].alter;
                spielfeld[i][0].bewegt = 1;
                bewegt = 1;
                break;
            }
            break;
        default:
            printf("Error Fisch %d", r);
            break;
        }
    } while (bewegt != 1);

    //vermehren
    if ((bewegt == 1) && spielfeld[i][j].alter > fischVermehrungsDauer)
    {
        spielfeld[i][j].typ = 'f';
        spielfeld[i][j].alter = 0;
    }
}

void SchrittTiling() //Nicht schneller
{
#pragma omp parallel for
    for (int block = 0; block < P; block++)
    {
        int blocky = block / BLOCKS_Y;
        int blockx = block % BLOCKS_X;
        int ystart = blocky * BLOCKSIZEY;
        int yend = ystart + BLOCKSIZEY;
        if (yend > N)
        {
            yend = N;
        }
        int xstart = blockx * BLOCKSIZEX;
        int xend = xstart + BLOCKSIZEX;
        if (xend > N)
        {
            xend = N;
        }
        for (int y = ystart; y < yend; y++)
        {
            for (int x = xstart; x < xend; x++)
            {
                if (spielfeld[y][x].typ == 'l')
                {
                }
                else if (spielfeld[y][x].typ == 'h' && spielfeld[y][x].bewegt == 0)
                {
                    HaiSchritt(y, x);
                }
                else if (spielfeld[y][x].typ == 'f' && spielfeld[y][x].bewegt == 0)
                {
                    FischSchritt(y, x);
                }
            }
        }
    }
}

void SchrittOMP()
{
#pragma omp parallel for collapse(2)
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld[i][j].typ == 'l')
            {
            }
            else if (spielfeld[i][j].typ == 'h' && spielfeld[i][j].bewegt == 0)
            {
                HaiSchritt(i, j);
            }
            else if (spielfeld[i][j].typ == 'f' && spielfeld[i][j].bewegt == 0)
            {
                FischSchritt(i, j);
            }
        }
    }
}

void Schritt()
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld[i][j].typ == 'l')
            {
            }
            else if (spielfeld[i][j].typ == 'h' && spielfeld[i][j].bewegt == 0)
            {
                HaiSchritt(i, j);
            }
            else if (spielfeld[i][j].typ == 'f' && spielfeld[i][j].bewegt == 0)
            {
                FischSchritt(i, j);
            }
        }
    }
}

void BewegungAus()
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            spielfeld[i][j].bewegt = 0;
        }
    }
}

void Ausgabe()
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld[i][j].typ == 'h')
            {
                printf("H ");
            }
            else if (spielfeld[i][j].typ == 'f')
            {
                printf("F ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}

int main()
{
    int anzahl = 1000;
    FeldFuellen();

    struct timeval start, end;

    long sec;
    long usec;
    double secs;

    //gettimeofday(&start, 0);

    //for (size_t i = 0; i < anzahl; i++)
    //{
    //    SchrittOMP();
    //    BewegungAus();
    //    //Ausgabe();
    //    //printf("%d \n", i);
    //}

    //gettimeofday(&end, 0);

    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    secs = (sec + usec / 1000000.0);

    printf("%.2f sec\n", secs);

    FeldFuellen();
    gettimeofday(&start, 0);

    for (size_t i = 0; i < anzahl; i++)
    {
        SchrittTiling();
        BewegungAus();
        //Ausgabe();
        //printf("%d \n", i);
    }

    gettimeofday(&end, 0);

    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    secs = (sec + usec / 1000000.0);

    printf("%.2f sec\n", secs);

    FeldFuellen();
    //gettimeofday(&start, 0);
    //
    //for (size_t i = 0; i < anzahl; i++)
    //{
    //    Schritt();
    //    BewegungAus();
    //    //Ausgabe();
    //    //printf("%d \n", i);
    //}
    //
    //gettimeofday(&end, 0);

    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    secs = (sec + usec / 1000000.0);

    printf("%.2f sec\n", secs);

    return 0;
}