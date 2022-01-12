#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <immintrin.h>
//#include <mpi.h>

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

struct spielfeldStruct
{
    char typ[Y * X];

    int alter[Y * X];

    //Hai
    int verhungerungsZeit[Y * X];

    int bewegt[Y * X];
};

struct entity spielfeld[Y][X];
struct spielfeldStruct spielfeld2;

const int fischVermehrungsDauer = 5;
const int haiVermehrungsDauer = 30;
const int verhungerungsZeit = 10;

int leer = 0;

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
                spielfeld[i][j].verhungerungsZeit = verhungerungsZeit;
                spielfeld[i][j].alter = 0;
                spielfeld[i][j].bewegt = 0;

                spielfeld2.typ[i * Y + j] = 'h';
                spielfeld2.verhungerungsZeit[i * Y + j] = verhungerungsZeit;
                spielfeld2.alter[i * Y + j] = 0;
                spielfeld2.bewegt[i * Y + j] = 0;
            }
            else if (r < 15)
            {
                spielfeld[i][j].typ = 'f';
                spielfeld[i][j].verhungerungsZeit = 0;
                spielfeld[i][j].alter = 0;
                spielfeld[i][j].bewegt = 0;

                spielfeld2.typ[i * Y + j] = 'f';
                spielfeld2.verhungerungsZeit[i * Y + j] = 0;
                spielfeld2.alter[i * Y + j] = 0;
                spielfeld2.bewegt[i * Y + j] = 0;
            }
            else
            {
                spielfeld[i][j].typ = 'l';
                spielfeld[i][j].verhungerungsZeit = 0;
                spielfeld[i][j].alter = 0;
                spielfeld[i][j].bewegt = 0;

                spielfeld2.typ[i * Y + j] = 'l';
                spielfeld2.verhungerungsZeit[i * Y + j] = 0;
                spielfeld2.alter[i * Y + j] = 0;
                spielfeld2.bewegt[i * Y + j] = 0;
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
                    spielfeld[Y - 1][j].verhungerungsZeit = verhungerungsZeit;
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
                    spielfeld[i - 1][j].verhungerungsZeit = verhungerungsZeit;
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
                    spielfeld[i][X - 1].verhungerungsZeit = verhungerungsZeit;
                    spielfeld[i][X - 1].bewegt = 1;

                    spielfeld[i][j].typ = 'l';
                }
            }
            else
            {
                if (spielfeld[i][j - 1].typ == 'f')
                {
                    spielfeld[i][j - 1].typ = 'h';
                    spielfeld[i][j - 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][j - 1].verhungerungsZeit = verhungerungsZeit;
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
                    spielfeld[0][j].verhungerungsZeit = verhungerungsZeit;
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
                    spielfeld[i + 1][j].verhungerungsZeit = verhungerungsZeit;
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
                    spielfeld[i][0].verhungerungsZeit = verhungerungsZeit;
                    spielfeld[i][0].bewegt = 1;

                    spielfeld[i][j].typ = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld[i][j + 1].typ == 'f')
                {
                    spielfeld[i][j + 1].typ = 'h';
                    spielfeld[i][j + 1].alter = spielfeld[i][j].alter;
                    spielfeld[i][j + 1].verhungerungsZeit = verhungerungsZeit;
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
            if (spielfeld[Y - 1][j].typ != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (j == 0)
        {
            if (spielfeld[i][X - 1].typ != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (i == (Y - 1))
        {
            if (spielfeld[0][j].typ != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (j == (X - 1))
        {
            if (spielfeld[i][0].typ != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
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
                if (i != (Y - 1))
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
                if (j != (X - 1))
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
        spielfeld[i][j].verhungerungsZeit = verhungerungsZeit;
    }
}

void FischSchritt(int i, int j)
{
    spielfeld[i][j].alter++;
    int bewegt = 0;
    if (i == 0)
    {
        if (spielfeld[Y - 1][j].typ != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (j == 0)
    {
        if (spielfeld[i][X - 1].typ != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (i == (Y - 1))
    {
        if (spielfeld[0][j].typ != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (j == (X - 1))
    {
        if (spielfeld[i][0].typ != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
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
            if (i != (Y - 1))
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
            if (j != (X - 1))
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

void HaiSchrittSoA(int i, int j, int SIMD)
{
    int gefressen = 0;
    int bewegt = 0;

    if (SIMD == 0)
    {
        spielfeld2.alter[Y * i + j]++;
        spielfeld2.verhungerungsZeit[Y * i + j]--;
    }

    if (spielfeld2.verhungerungsZeit[i * Y + j] <= 0)
    {
        spielfeld2.typ[i * Y + j] = 'l';
        spielfeld2.alter[i * Y + j] = 0;
        spielfeld2.verhungerungsZeit[i * Y + j] = 0;
        return;
    }

    //Jagen
    for (int k = 0; k < 4; k++)
    {
        switch (k)
        {
        case 0: //oben
            if (i == 0)
            {
                if (spielfeld2.typ[Y * (Y - 1) + j] == 'f')
                {
                    spielfeld2.typ[Y * (Y - 1) + j] = 'h';
                    spielfeld2.alter[Y * (Y - 1) + j] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[Y * (Y - 1) + j] = verhungerungsZeit;
                    spielfeld2.bewegt[Y * (Y - 1) + j] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld2.typ[(i - 1) * Y + j] == 'f')
                {
                    spielfeld2.typ[(i - 1) * Y + j] = 'h';
                    spielfeld2.alter[(i - 1) * Y + j] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[(i - 1) * Y + j] = verhungerungsZeit;
                    spielfeld2.bewegt[(i - 1) * Y + j] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';

                    gefressen = 1;
                }
            }
            break;
        case 1: //links
            if (j == 0)
            {
                if (spielfeld2.typ[i * Y + (X - 1)] == 'f')
                {
                    spielfeld2.typ[i * Y + (X - 1)] = 'h';
                    spielfeld2.alter[i * Y + (X - 1)] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[i * Y + (X - 1)] = verhungerungsZeit;
                    spielfeld2.bewegt[i * Y + (X - 1)] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';
                }
            }
            else
            {
                if (spielfeld2.typ[i * Y + (j - 1)] == 'f')
                {
                    spielfeld2.typ[i * Y + (j - 1)] = 'h';
                    spielfeld2.alter[i * Y + (j - 1)] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[i * Y + (j - 1)] = verhungerungsZeit;
                    spielfeld2.bewegt[i * Y + (j - 1)] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';
                }
            }
            break;
        case 2: //unten
            if (i == (Y - 1))
            {
                if (spielfeld2.typ[j] == 'f')
                {
                    spielfeld2.typ[j] = 'h';
                    spielfeld2.alter[j] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[j] = verhungerungsZeit;
                    spielfeld2.bewegt[j] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld2.typ[(i + 1) * Y + j] == 'f')
                {
                    spielfeld2.typ[(i + 1) * Y + j] = 'h';
                    spielfeld2.alter[(i + 1) * Y + j] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[(i + 1) * Y + j] = verhungerungsZeit;
                    spielfeld2.bewegt[(i + 1) * Y + j] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';

                    gefressen = 1;
                }
            }
            break;
        case 3: //rechts
            if (j == (X - 1))
            {
                if (spielfeld2.typ[i * Y] == 'f')
                {
                    spielfeld2.typ[i * Y] = 'h';
                    spielfeld2.alter[i * Y] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[i * Y] = verhungerungsZeit;
                    spielfeld2.bewegt[i * Y] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';

                    gefressen = 1;
                }
            }
            else
            {
                if (spielfeld2.typ[i * Y + j + 1] == 'f')
                {
                    spielfeld2.typ[i * Y + j + 1] = 'h';
                    spielfeld2.alter[i * Y + j + 1] = spielfeld2.alter[i * Y + j];
                    spielfeld2.verhungerungsZeit[i * Y + j + 1] = verhungerungsZeit;
                    spielfeld2.bewegt[i * Y + j + 1] = 1;

                    spielfeld2.typ[i * Y + j] = 'l';

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
            if (spielfeld2.typ[Y * (Y - 1) + j] != 'l')
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (j == 0)
        {
            if (spielfeld2.typ[i * Y + (X - 1)] != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (i == (X - 1))
        {
            if (spielfeld2.typ[j] != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (j == (Y - 1))
        {
            if (spielfeld2.typ[i * Y] != 'l' && bewegt != 2)
            {
                bewegt = 1;
            }
            else
            {
                bewegt = 2;
            }
        }
        if (spielfeld2.typ[Y * (i - 1) + j] != 'l' && spielfeld2.typ[Y * (i + 1) + j] != 'l' &&
            spielfeld2.typ[Y * i + (j - 1)] != 'l' && spielfeld2.typ[Y * i + (j + 1)] != 'l')
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
                    if (spielfeld2.typ[Y * (i - 1) + j] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * (i - 1) + j] = 'h';
                    spielfeld2.alter[Y * (i - 1) + j] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * (i - 1) + j] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * (i - 1) + j] = 1;
                    spielfeld2.typ[Y * (i - 1) + j] = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld2.typ[Y * (Y - 1) + j] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * (Y - 1) + j] = 'h';
                    spielfeld2.alter[Y * (Y - 1) + j] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * (Y - 1) + j] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * (Y - 1) + j] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
                    bewegt = 1;
                }
                break;
            case 1: //links
                if (j != 0)
                {
                    if (spielfeld2.typ[Y * i + (j - 1)] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * i + (j - 1)] = 'h';
                    spielfeld2.alter[Y * i + (j - 1)] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * i + (j - 1)] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * i + (j - 1)] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld2.typ[Y * i + (X - 1)] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * i + (X - 1)] = 'h';
                    spielfeld2.alter[Y * i + (X - 1)] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * i + (X - 1)] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * i + (X - 1)] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
                    bewegt = 1;
                }
                break;
            case 2: //unten
                if (i != (Y - 1))
                {
                    if (spielfeld2.typ[Y * (i + 1) + j] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * (i + 1) + j] = 'h';
                    spielfeld2.alter[Y * (i + 1) + j] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * (i + 1) + j] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * (i + 1) + j] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld2.typ[j] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[j] = 'h';
                    spielfeld2.alter[j] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[j] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[j] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
                    bewegt = 1;
                }
                break;
            case 3: //rechts
                if (j != (X - 1))
                {
                    if (spielfeld2.typ[Y * i + (j + 1)] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * i + (j + 1)] = 'h';
                    spielfeld2.alter[Y * i + (j + 1)] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * i + (j + 1)] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * i + (j + 1)] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
                    bewegt = 1;
                }
                else
                {
                    if (spielfeld2.typ[Y * i] != 'l')
                    {
                        break;
                    }
                    spielfeld2.typ[Y * i] = 'h';
                    spielfeld2.alter[Y * i] = spielfeld2.alter[Y * i + j];
                    spielfeld2.verhungerungsZeit[Y * i] = spielfeld2.verhungerungsZeit[Y * i + j];
                    spielfeld2.bewegt[Y * i] = 1;
                    spielfeld2.typ[Y * i + j] = 'l';
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
    if ((bewegt == 1 || gefressen == 1) && spielfeld2.alter[Y * i + j] > haiVermehrungsDauer)
    {
        spielfeld2.typ[Y * i + j] = 'h';
        spielfeld2.alter[Y * i + j] = 0;
        spielfeld2.verhungerungsZeit[Y * i + j] = verhungerungsZeit;
    }
}

void FischSchrittSoA(int i, int j, int SIMD)
{
    int bewegt = 0;

    if (SIMD == 0)
    {
        spielfeld2.alter[Y * i + j]++;
    }

    if (i == 0)
    {
        if (spielfeld2.typ[Y * (Y - 1) + j] != 'l')
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (j == 0)
    {
        if (spielfeld2.typ[i * Y + (X - 1)] != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (i == (X - 1))
    {
        if (spielfeld2.typ[j] != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (j == (Y - 1))
    {
        if (spielfeld2.typ[i * Y] != 'l' && bewegt != 2)
        {
            bewegt = 1;
        }
        else
        {
            bewegt = 2;
        }
    }
    if (spielfeld2.typ[Y * (i - 1) + j] != 'l' && spielfeld2.typ[Y * (i + 1) + j] != 'l' &&
        spielfeld2.typ[Y * i + (j - 1)] != 'l' && spielfeld2.typ[Y * i + (j + 1)] != 'l')
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
                if (spielfeld2.typ[Y * (i - 1) + j] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * (i - 1) + j] = 'f';
                spielfeld2.alter[Y * (i - 1) + j] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * (i - 1) + j] = 1;
                spielfeld2.typ[Y * (i - 1) + j] = 'l';
                bewegt = 1;
            }
            else
            {
                if (spielfeld2.typ[Y * (Y - 1) + j] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * (Y - 1) + j] = 'f';
                spielfeld2.alter[Y * (Y - 1) + j] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * (Y - 1) + j] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
            }
            break;
        case 1: //links
            if (j != 0)
            {
                if (spielfeld2.typ[Y * i + (j - 1)] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * i + (j - 1)] = 'f';
                spielfeld2.alter[Y * i + (j - 1)] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * i + (j - 1)] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
            }
            else
            {
                if (spielfeld2.typ[Y * i + (X - 1)] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * i + (X - 1)] = 'f';
                spielfeld2.alter[Y * i + (X - 1)] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * i + (X - 1)] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
            }
            break;
        case 2: //unten
            if (i != (Y - 1))
            {
                if (spielfeld2.typ[Y * (i + 1) + j] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * (i + 1) + j] = 'f';
                spielfeld2.alter[Y * (i + 1) + j] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * (i + 1) + j] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
            }
            else
            {
                if (spielfeld2.typ[j] != 'l')
                {
                    break;
                }
                spielfeld2.typ[j] = 'f';
                spielfeld2.alter[j] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[j] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
            }
            break;
        case 3: //rechts
            if (j != (X - 1))
            {
                if (spielfeld2.typ[Y * i + (j + 1)] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * i + (j + 1)] = 'f';
                spielfeld2.alter[Y * i + (j + 1)] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * i + (j + 1)] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
            }
            else
            {
                if (spielfeld2.typ[Y * i] != 'l')
                {
                    break;
                }
                spielfeld2.typ[Y * i] = 'f';
                spielfeld2.alter[Y * i] = spielfeld2.alter[Y * i + j];
                spielfeld2.bewegt[Y * i] = 1;
                spielfeld2.typ[Y * i + j] = 'l';
                bewegt = 1;
                break;
            }
            break;
        default:
            printf("Error Fisch Bewegung %d", r);
            break;
        }
    } while (bewegt != 1);

    //vermehren
    if ((bewegt == 1) && spielfeld2.alter[Y * i + j] > fischVermehrungsDauer)
    {
        spielfeld2.typ[Y * i + j] = 'f';
        spielfeld2.alter[Y * i + j] = 0;
    }
}

void zaehlenSIMD()
{
    //printf("SIMD Start");
    //_mm256_maskload_epi32 (int const* mem_addr, __m256i mask)
    //_mm256_add_epi32 (__m256i a, __m256i b)
    //_mm256_sub_epi32 (__m256i a, __m256i b)
    //_mm256_maskstore_epi32 (int* mem_addr, __m256i mask, __m256i a)

    int *pAlter = &spielfeld2.alter[0];
    int *pHunger = &spielfeld2.verhungerungsZeit[0];

    int W[8] = {1, 1, 1, 1, 1, 1, 1, 1}; //Werte Array zum rechnen

    int *pW = &W[0]; //Pointer zum Werte Array W

    __m256i mask = _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1, -1);
    __m256i Wloaded = _mm256_maskload_epi32(pW, mask); //Werte von W laden

    for (int i = 0; i < (X * Y) / 8; i++)
    {
        _mm256_maskstore_epi32(pAlter, mask, _mm256_add_epi32(_mm256_maskload_epi32(pAlter, mask), Wloaded));
        _mm256_maskstore_epi32(pHunger, mask, _mm256_sub_epi32(_mm256_maskload_epi32(pHunger, mask), Wloaded));
        pAlter += 8;
        pHunger += 8;
    }
    //printf("SIMD Ende");
}

void SchrittTiling() //Funktioniert nicht, Deadlock
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

void SchrittSoASIMD()
{
    zaehlenSIMD();
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld2.typ[Y * i + j] == 'l')
            {
            }
            else if (spielfeld2.typ[Y * i + j] == 'h' && spielfeld2.bewegt[Y * i + j] == 0)
            {
                HaiSchrittSoA(i, j, 1);
            }
            else if (spielfeld2.typ[Y * i + j] == 'f' && spielfeld2.bewegt[Y * i + j] == 0)
            {
                FischSchrittSoA(i, j, 1);
            }
        }
    }
}

void SchrittSoA()
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld2.typ[Y * i + j] == 'l')
            {
            }
            else if (spielfeld2.typ[Y * i + j] == 'h' && spielfeld2.bewegt[Y * i + j] == 0)
            {
                HaiSchrittSoA(i, j, 0);
            }
            else if (spielfeld2.typ[Y * i + j] == 'f' && spielfeld2.bewegt[Y * i + j] == 0)
            {
                FischSchrittSoA(i, j, 0);
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
#pragma omp parallel for collapse(2)
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            spielfeld[i][j].bewegt = 0;
        }
    }
}

void BewegungAusSoA()
{
#pragma omp parallel for collapse(2)
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            spielfeld2.bewegt[Y * i + j] = 0;
        }
    }
}

void FeldLeer()
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld[i][j].typ != 'l')
            {
                leer = 0;
                return;
            }
            else
            {
                leer = 1;
            }
        }
    }
}

void FeldLeerSoA()
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (spielfeld2.typ[Y * i * j] != 'l')
            {
                leer = 0;
                return;
            }
            else
            {
                leer = 1;
            }
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
    double secs = 0;

    for (size_t i = 0; i < anzahl; i++)
    {
        gettimeofday(&start, 0);
        SchrittOMP();
        gettimeofday(&end, 0);
        sec = end.tv_sec - start.tv_sec;
        usec = end.tv_usec - start.tv_usec;
        secs += (sec + usec / 1000000.0);
        BewegungAus();
        FeldLeer();
        if (leer == 1)
        {
            printf("Feld leer in Runde: %d\n", i);
            leer = 0;
            break;
        }
        //Ausgabe();
        //printf("%d \n", i);
    }

    printf("OMP: %.2f sec\n", secs);

    secs = 0;

    FeldFuellen();

    //for (size_t i = 0; i < anzahl; i++)
    //{
    //    gettimeofday(&start, 0);
    //    SchrittTiling(i);
    //    gettimeofday(&end, 0);
    //    sec = end.tv_sec - start.tv_sec;
    //    usec = end.tv_usec - start.tv_usec;
    //    secs += (sec + usec / 1000000.0);
    //    BewegungAus();
    //    FeldLeer();
    //    if (leer == 1)
    //    {
    //        printf("Feld leer in Runde: %d\n", i);
    //        leer = 0;
    //        break;
    //    }
    //    //Ausgabe();
    //    //printf("%d \n", i);
    //}
    //
    //printf("Tiling: %.2f sec\n", secs);

    secs = 0;

    FeldFuellen();

    for (size_t i = 0; i < anzahl; i++)
    {
        gettimeofday(&start, 0);
        Schritt();
        gettimeofday(&end, 0);
        sec = end.tv_sec - start.tv_sec;
        usec = end.tv_usec - start.tv_usec;
        secs += (sec + usec / 1000000.0);
        BewegungAus();
        FeldLeer();
        if (leer == 1)
        {
            printf("Feld leer in Runde: %d\n", i);
            leer = 0;
            break;
        }
        //Ausgabe();
        //printf("%d \n", i);
    }

    printf("Schritt: %.2f sec\n", secs);

    secs = 0;

    for (size_t i = 0; i < anzahl; i++)
    {
        gettimeofday(&start, 0);
        SchrittSoASIMD();
        gettimeofday(&end, 0);
        sec = end.tv_sec - start.tv_sec;
        usec = end.tv_usec - start.tv_usec;
        secs += (sec + usec / 1000000.0);
        BewegungAusSoA();
        FeldLeerSoA();
        if (leer == 1)
        {
            printf("Feld leer in Runde: %d\n", i);
            leer = 0;
            break;
        }
        //Ausgabe();
        //printf("%d \n", i);
    }

    printf("Simd SoA: %.2f sec\n", secs);

    secs = 0;

    FeldFuellen();

    for (size_t i = 0; i < anzahl; i++)
    {
        gettimeofday(&start, 0);
        Schritt();
        gettimeofday(&end, 0);
        sec = end.tv_sec - start.tv_sec;
        usec = end.tv_usec - start.tv_usec;
        secs += (sec + usec / 1000000.0);
        BewegungAus();
        FeldLeer();
        if (leer == 1)
        {
            printf("Feld leer in Runde: %d\n", i);
            leer = 0;
            break;
        }
        //Ausgabe();
        //printf("%d \n", i);
    }

    printf("Schritt : %.2f sec\n", secs);

    secs = 0;

    for (size_t i = 0; i < anzahl; i++)
    {
        gettimeofday(&start, 0);
        SchrittSoA();
        gettimeofday(&end, 0);
        sec = end.tv_sec - start.tv_sec;
        usec = end.tv_usec - start.tv_usec;
        secs += (sec + usec / 1000000.0);
        BewegungAusSoA();
        FeldLeerSoA();
        if (leer == 1)
        {
            printf("Feld leer in Runde: %d\n", i);
            leer = 0;
            break;
        }
        //Ausgabe();
        //printf("%d \n", i);
    }

    printf("SoA: %.2f sec\n", secs);

    return 0;
}