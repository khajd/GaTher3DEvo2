#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#ifdef LIBXML_TREE_ENABLED

// typedef float ***Matrix;
#define PI 3.14159265
#define elements 100 // CHANGE!!!

typedef struct
{
  int type; // 0-brak, 1-step, 2-linear, 3-sine
  float v;
  float c;
  float param1;
  float param2;
  float param3;
} ResourcesFunctionStruct;

typedef struct
{
  float *fen;
} Fenotypestruct;

typedef struct
{
  int x[26]; // 26 maksymalna liczba sąsiadów
  int y[26];
  int z[26];
  // float Fitness[6];
  // Fenotypestruct Population[6];
  int number; // rzeczywista liczba niepustych sąsiadów
} Neighbourhoodstruct;

void sort(Neighbourhoodstruct *neighbours, Fenotypestruct ***Population, float ***Fitness, int param, unsigned int seed)
{
  int j, i, min;

  for (i = 0; i < param; i++) // zmieniłem żeby sortował tyle ile trzeba
  {
    min = i;
    for (j = i + 1; j < (*neighbours).number; j++)
    {
      // usunąłem lub bo w funkcja FindNeighbourhood nie zwraca pustych komórek
      if ((Fitness[(*neighbours).x[j]][(*neighbours).y[j]][(*neighbours).z[j]] > Fitness[(*neighbours).x[min]][(*neighbours).y[min]][(*neighbours).z[min]] && Population[(*neighbours).x[j]][(*neighbours).y[j]][(*neighbours).z[j]].fen[3] >= 0))
      {
        min = j;
      }
      else if ((Fitness[(*neighbours).x[j]][(*neighbours).y[j]][(*neighbours).z[j]] == Fitness[(*neighbours).x[min]][(*neighbours).y[min]][(*neighbours).z[min]] && Population[(*neighbours).x[j]][(*neighbours).y[j]][(*neighbours).z[j]].fen[3] >= 0))
      {
        if ((rand_r(&seed) / (float)RAND_MAX) > 0.5)
        {
          min = j;
        }
      }
    }
    int temp = (*neighbours).x[min];
    (*neighbours).x[min] = (*neighbours).x[i];
    (*neighbours).x[i] = temp;
    temp = (*neighbours).y[min];
    (*neighbours).y[min] = (*neighbours).y[i];
    (*neighbours).y[i] = temp;
    temp = (*neighbours).z[min];
    (*neighbours).z[min] = (*neighbours).z[i];
    (*neighbours).z[i] = temp;
    // printf("i: %d j: %d\n",i,j);
    // ShowNeighbourhood((*neighbours));
  }
}

Neighbourhoodstruct FindNeighbourhood(int i, int j, int k, int size[3], int neighbourmethod)
{
  int n = 0;
  Neighbourhoodstruct output;

  output.x[n] = i;
  output.y[n] = j;
  output.z[n] = k;
  n++;

  output.x[n] = (i + 1) % size[0];
  output.y[n] = j;
  output.z[n] = k;
  n++;

  output.x[n] = (size[0] + i - 1) % size[0];
  output.y[n] = j;
  output.z[n] = k;
  n++;

  if (size[1] > 1) // 2D lub 3D
  {
    // sąsiedztwo Moore'a - dodać if'a, klamra już jest
    if (neighbourmethod)
    {
      output.x[n] = (size[0] + i - 1) % size[0];
      output.y[n] = (size[1] + j - 1) % size[1];
      output.z[n] = k;
      n++;

      output.x[n] = (size[0] + i - 1) % size[0];
      output.y[n] = (j + 1) % size[1];
      output.z[n] = k;
      n++;

      output.x[n] = (i + 1) % size[0];
      output.y[n] = (size[1] + j - 1) % size[1];
      output.z[n] = k;
      n++;

      output.x[n] = (i + 1) % size[0];
      output.y[n] = (j + 1) % size[1];
      output.z[n] = k;
      n++;

      output.x[n] = i;
      output.y[n] = (size[1] + j - 1) % size[1];
      output.z[n] = k;
      n++;

      output.x[n] = i;
      output.y[n] = (j + 1) % size[1];
      output.z[n] = k;
      n++;
    }
    else
    {
      // sąsiedztwo von Neumanna
      output.x[n] = i;
      output.y[n] = (size[1] + j - 1) % size[1];
      output.z[n] = k;
      n++;

      output.x[n] = i;
      output.y[n] = (j + 1) % size[1];
      output.z[n] = k;
      n++;
    }

    if (size[2] > 1) // 3D
    {

      // Moore'a - dać if'a
      if (neighbourmethod)
      {

        // dla k=size[2]+k-1)%size[2]
        output.x[n] = (size[0] + i - 1) % size[0];
        output.y[n] = j;
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = (i + 1) % size[0];
        output.y[n] = j;
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = i;
        output.y[n] = (size[1] + j - 1) % size[1];
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = i;
        output.y[n] = (j + 1) % size[1];
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = (size[0] + i - 1) % size[0];
        output.y[n] = (size[1] + j - 1) % size[1];
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = (size[0] + i - 1) % size[0];
        output.y[n] = (j + 1) % size[1];
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = (i + 1) % size[0];
        output.y[n] = (size[1] + j - 1) % size[1];
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = (i + 1) % size[0];
        output.y[n] = (j + 1) % size[1];
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        // dla k=(k+1)%size[2]
        output.x[n] = (size[0] + i - 1) % size[0];
        output.y[n] = j;
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = (i + 1) % size[0];
        output.y[n] = j;
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = i;
        output.y[n] = (size[1] + j - 1) % size[1];
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = i;
        output.y[n] = (j + 1) % size[1];
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = (size[0] + i - 1) % size[0];
        output.y[n] = (size[1] + j - 1) % size[1];
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = (size[0] + i - 1) % size[0];
        output.y[n] = (j + 1) % size[1];
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = (i + 1) % size[0];
        output.y[n] = (size[1] + j - 1) % size[1];
        output.z[n] = (k + 1) % size[2];
        n++;

        output.x[n] = (i + 1) % size[0];
        output.y[n] = (j + 1) % size[1];
        output.z[n] = (k + 1) % size[2];
        n++;
        //
      }
      else
      {
        // von Neumanna

        output.x[n] = i;
        output.y[n] = j;
        output.z[n] = (size[2] + k - 1) % size[2];
        n++;

        output.x[n] = i;
        output.y[n] = j;
        output.z[n] = (k + 1) % size[2];
        n++;
      }
    }
  }

  output.number = n;

  return output;
}

float getR(float H, float param1)
{
  float r;
  if (param1 == 1.0)
  {
    r = -H + 1;
  }
  else if (param1 == 2.0)
  {
    r = -(H - 1) * (H + 1);
  }
  else if (param1 == 3.0)
  {
    r = (2 / (H + 1)) - 1;
  }
  else if (param1 == 4.0)
  {
    r = cos(H * PI * 0.5);
  }
  return 1 - r;
}

void ChangePayoffMatrix(float *****tab, ResourcesFunctionStruct resources, int g, Fenotypestruct ***Population, int size[3], int neighbourmethod)
{
  float r;
  int i, j, k, n;
  float suma = 0.0;
  float H;
  Neighbourhoodstruct neighbours;

  if (resources.type == 0)
  {
    // do nothing
  }
  else if (resources.type == 5)
  {
    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        for (k = 0; k < size[2]; k++)
        {
          neighbours = FindNeighbourhood(i, j, k, size, neighbourmethod);
          suma = 0.0;
          for (n = 0; n < neighbours.number; n++)
          {
            suma += Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[3];
          }
          H = suma / neighbours.number;
          r = getR(H, resources.param1);
          r = (r > 1.0) ? 1.0 : (r < 0.0) ? 0.0
                                          : r;

          tab[i][j][k][0][0] = resources.v - resources.c;
          tab[i][j][k][0][1] = 2 * resources.v;
          tab[i][j][k][1][0] = r * 0.5 * resources.v;
          tab[i][j][k][1][1] = (1.0 + r) * resources.v;
        }
      }
    }
  }
  else
  {
    if (resources.type == 1)
    {
      r = (g < ((int)resources.param3)) ? resources.param1 : resources.param2;
      // return tab;
    }
    if (resources.type == 2)
    {
      r = resources.param1 * g + resources.param2;
    }
    if (resources.type == 3)
    {
      r = resources.param1 * sin((2.0 * PI * g) / resources.param3) + resources.param2;
    }
    if (resources.type == 4)
    {
      for (i = 0; i < size[0]; i++)
      {
        for (j = 0; j < size[1]; j++)
        {
          for (k = 0; k < size[2]; k++)
          {
            suma += Population[i][j][k].fen[0];
          }
        }
      }
      H = suma / (size[0] * size[1] * size[2]);
      r = getR(H, resources.param1);
    }

    r = (r > 1.0) ? 1.0 : (r < 0.0) ? 0.0
                                    : r;
    // printf("r=%f\n",r);

    tab[0][0][0][0][0] = resources.v - resources.c;
    tab[0][0][0][0][1] = 2 * resources.v;
    tab[0][0][0][1][0] = r * 0.5 * resources.v;
    tab[0][0][0][1][1] = (1.0 + r) * resources.v;
  }
}

float Payoff(float i, float j)
{
  float b = 2.0, c = 1.0, d = 2.0, e = 0.0;
  float table[2][2];
  table[1][1] = 1 + b + e - c; // C-C
  table[1][0] = 1 + b - c;     // C-D
  table[0][1] = 1 + d;         // D-C
  table[0][0] = 1;             // D-D
  // printf("ok2 i:%d j:%d\n",(int)i,(int)j);
  return table[(int)i][(int)j];
}

void Show(Fenotypestruct ***population, int size[3], int fenotypes)
{
  int i, j, k, l;
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {
        for (l = 0; l < fenotypes; l++)
        {
          printf("%f/", population[i][j][k].fen[l]);
        }
        printf("\t");
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}

void ShowFitness(float ***Fitness, int size[3])
{
  int i, j, k, l;
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {

        printf("%f", Fitness[i][j][k]);

        printf("\t");
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}

void ShowChosenCells(int ***Fitness, int size[3])
{
  int i, j, k, l;
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {

        printf("%d", Fitness[i][j][k]);

        printf("\t");
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}

void FillArray(int ***array, int size[3], int value)
{
  int i, j, k;
#pragma omp parallel for private(i, j, k) shared(size, value, array)
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {
        array[i][j][k] = value;
      }
    }
  }
}

void InitialisePopulationCSV(char *strFilePathFen0, char *strFilePathFen1, char *strFilePathFen2, char *strFilePathFen3, Fenotypestruct ***Population, int size[3], int fenotypes, int MSEG)
{
  // int length = 100; // TODO: zmien jak zmieniasz rozmiar!!!
  //  int ***array[length][length][length];
  // int array[length][length][length];

  int length = size[0];
  int(*array)[length][length] = malloc(sizeof(int[length][length][length]));
  float(*trans)[length][length] = malloc(sizeof(float[length][length][length]));

  int idx = 0;
  int i = 0;
  int j = 0;
  int k = 0;

  int temp = 0;
  int depth = 0;

  char *buffer = NULL;
  size_t len = 0;
  size_t read;
  char *ptr = NULL;

  char line[length * length * 2 * 10]; // te 10 to do testw...
  int rowIndex = 0;
  char *token = NULL;

  FILE *fp;
  // fen0
  fp = fopen(strFilePathFen0, "r"); // open file , read only
  if (!fp)
  {
    fprintf(stderr, "failed to open file for reading\n");
  }
  i = 0;
  while (fgets(line, sizeof(line), fp) != NULL && rowIndex < length)
  {
    k = 0;
    int colIndex = 0;
    j = 0;
    // printf("\nrowIndex: %d\ncolIndex: %d\n", rowIndex, colIndex);
    char *token = strtok(line, ",");
    while (token)
    {
      // printf("\nk: %d, colIndex: %d, temp: ", k, colIndex);
      // temp = atof(token);
      // printf(" %d ", temp);
      // fflush(stdout);
      // array[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      array[i][j][k] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[0] = array[k][i][j];
      Population[i][j][k].fen[0] = array[i][j][k];

      // array[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[0] = array[k][i][j];

      // Population[i][k][j].fen[0] = array[k][i][j];
      //  printf("Population[%d][%d][%d].fen[0]: %d\n", k, i, j, array[k][i][j]);
      depth++;
      colIndex++;
      j++;
      if (j == length)
      {
        k++;
        j = 0;
        depth = 0;
        // printf("\nk: %d\n", k);
      }
      token = strtok(NULL, ",");
    }
    rowIndex++;
    i++;
  }

  /*   for (i = 0; i < length; i++)
    {
      printf("\nk: %d: ", i);
      for (j = 0; j < length; j++)
      {
        printf("\n||i: %d: ", j);

        for (k = 0;k < length; k++)
        {
          // printf("j: %d: ", j);
          printf("%0.1f ", Population[i][j][k].fen[0]);
          fflush(stdout);
          if (j == length)
          {
            printf("\n");
          }
        }
      }
      printf("\n");
    } */

  // cleanup
  fclose(fp);
  // realloc(*array);
  rowIndex = 0;

  // fen1
  fp = fopen(strFilePathFen1, "r"); // open file , read only
  if (!fp)
  {
    fprintf(stderr, "failed to open file for reading\n");
  }
  i = 0;
  while (fgets(line, sizeof(line), fp) != NULL && rowIndex < length)
  {
    k = 0;
    int colIndex = 0;
    j = 0;
    // printf("\nrowIndex: %d\ncolIndex: %d\n", rowIndex, colIndex);
    char *token = strtok(line, ",");
    while (token)
    {
      // printf("\nk: %d, colIndex: %d, temp: ", k, colIndex);
      // temp = atof(token);
      // printf(" %d ", temp);
      // fflush(stdout);
      // array[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      array[i][j][k] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[1] = array[k][i][j];
      Population[i][j][k].fen[1] = array[i][j][k];
      // printf("Population[%d][%d][%d].fen[1]: %d\n", k, i, j, array[k][i][j]);
      // array[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[1] = array[k][i][j];
      depth++;
      colIndex++;
      j++;
      if (j == length)
      {
        k++;
        j = 0;
        depth = 0;
        // printf("\nk: %d\n", k);
      }
      token = strtok(NULL, ",");
    }
    rowIndex++;
    i++;
  }
  /*
  printf("\nFen1\n");
  for (k = 0; k < length; k++)
  {
    printf("\nk: %d: ", k);
    for (i = 0; i < length; i++)
    {
      printf("\n||i: %d: ", i);

      for (j = 0; j < length; j++)
      {
        // printf("j: %d: ", j);
        printf("%0.1f ", Population[k][i][j].fen[1]);
        fflush(stdout);
        if (j == length)
        {
          printf("\n");
        }
      }
    }
    printf("\n");
  }
  */
  // cleanup
  fclose(fp);
  // realloc(array);
  rowIndex = 0;

  // fen2
  fp = fopen(strFilePathFen2, "r"); // open file , read only
  if (!fp)
  {
    fprintf(stderr, "failed to open file for reading\n");
  }
  i = 0;
  while (fgets(line, sizeof(line), fp) != NULL && rowIndex < length)
  {
    k = 0;
    int colIndex = 0;
    j = 0;
    // printf("\nrowIndex: %d\ncolIndex: %d\n", rowIndex, colIndex);
    char *token = strtok(line, ",");
    while (token)
    {
      // printf("\nk: %d, colIndex: %d, temp: ", k, colIndex);
      // temp = atof(token);
      // printf(" %d ", temp);
      // fflush(stdout);
      // array[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      array[i][j][k] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[2] = array[k][i][j];
      Population[i][j][k].fen[2] = array[i][j][k];
      /// array[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[2] = array[k][i][j];
      //  printf("Population[%d][%d][%d].fen[2]: %d\n", k, i, j, array[k][i][j]);
      depth++;
      colIndex++;
      j++;
      if (j == length)
      {
        k++;
        j = 0;
        depth = 0;
        // printf("\nk: %d\n", k);
      }
      token = strtok(NULL, ",");
    }
    rowIndex++;
    i++;
  }

  // cleanup
  fclose(fp);
  // realloc(array);
  rowIndex = 0;

  // fen3
  fp = fopen(strFilePathFen3, "r"); // open file , read only
  if (!fp)
  {
    fprintf(stderr, "failed to open file for reading\n");
  }
  i = 0;
  while (fgets(line, sizeof(line), fp) != NULL && rowIndex < length)
  {
    k = 0;
    int colIndex = 0;
    j = 0;
    // printf("\nrowIndex: %d\ncolIndex: %d\n", rowIndex, colIndex);
    char *token = strtok(line, ",");
    while (token)
    {
      // printf("\nk: %d, colIndex: %d, temp: ", k, colIndex);
      // temp = atof(token);
      // printf(" %d ", temp);
      // fflush(stdout);
      // array[i][j][k] = atof(token); // temp; // atof(token); // temp; //
      array[i][j][k] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[3] = array[k][i][j];
      Population[i][j][k].fen[3] = array[i][j][k];
      //[k][i][j] = atof(token); // temp; // atof(token); // temp; //
      // Population[k][i][j].fen[3] = array[k][i][j];

      // printf("Population[%d][%d][%d].fen[3]: %f\n", k, i, j, Population[i][j][k].fen[3]);
      depth++;
      colIndex++;
      j++;
      if (j == length)
      {
        k++;
        j = 0;
        depth = 0;
        // printf("\nk: %d\n", k);
      }
      token = strtok(NULL, ",");
    }
    rowIndex++;
    i++;
  }

  // cleanup
  fclose(fp);
  // free(array);
  rowIndex = 0;

  // printf("\nFen3\n");
  // for (i = 0; i < length; i++)
  // {
  //   printf("\ni: %d: ", i);
  //   for (j = 0; j < length; j++)
  //   {
  //     printf("\n||j: %d: ", j);

  //     for (k = 0; k < length; k++)
  //     {
  //        //printf("j: %d: ", j);
  //        printf("%0.1f ", Population[i][j][k].fen[3]); // printf("%0.1f ", Population[i][j][k].fen[3]);
  //        fflush(stdout);
  //       if (k == length)
  //       {
  //         printf("\n");
  //       }
  //     }
  //   }
  //   printf("\n");
  // }
  //     printf("\n");
}
// liczy fitness komórki o danej współrzędnej
void CalculatePayoff(Fenotypestruct ***Population, float ***Fitness, int x, int y, int z, int size[3], int fenotypes, float **tab, int neighbourmethod, int arrIntZone[elements][elements][elements], int intZoneCheck[elements][elements][elements], float **tab1, float **tab2, float **tab3, float **tab4, float **tab5)
{
  int n, l, m;
  Neighbourhoodstruct neighbours;
  Fitness[x][y][z] = 0;

  // printf("thread = %d i=%d j%d k=%d\n", tid,x,y,z);
  neighbours = FindNeighbourhood(x, y, z, size, neighbourmethod);
  //       for (int i = 0; i < size[0]; i++)
  // {
  //   printf("\nk: %d: ", i);
  //   for (int j = 0; j < size[0]; j++)
  //   {
  //     printf("\n||i: %d: ", j);

  //     for (int k = 0;k < size[0]; k++)
  //     {
  //       // printf("j: %d: ", j);
  //       printf("%d ", intZoneCheck[i][j][k]);
  //       fflush(stdout);
  //       if (j == size[0])
  //       {
  //         printf("\n");
  //       }
  //     }
  //   }
  //   printf("\n");
  // }
  for (n = 0; n < neighbours.number; n++)
  {
    for (l = 0; l < fenotypes; l++)
    {
      for (m = 0; m < fenotypes; m++)
      { // n to numer sąsiada, a neighbours[n].x y z to jego współrzędne
        // przelatujemy przez wszystkie fenotypy komórki, mnożymy je przez wszystkie fenotypy sąsiadów i przez tablicę z tabeli korzyści
        // printf("n: %d, x: %d, y: %d, z: %d, arrIntZone: %d, intZoneCheck: %d\n", n, neighbours.x[n], neighbours.y[n], neighbours.z[n], arrIntZone[x][y], intZoneCheck[x][y]);
        // printf("n: %d, x: %d, y: %d, z: %d, arrIntZone: %d, intZoneCheck: %d\n", n, x + 1, y + 1, z + 1, arrIntZone[x][y], intZoneCheck[x][y]);
        // for (int g = 0; g < neighbours.number; g++)
        //{
        //   arrIntZone[neighbours.x[n]][neighbours.y[n]];
        //   printf("arrIntZone[%d][%d]: %d\n", neighbours.x[g] + 1, neighbours.y[g] + 1, arrIntZone[neighbours.x[g]][neighbours.y[g]]);
        // }

        if (intZoneCheck[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] == 1)
        {
          Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab1[l][m];
        }
        else if (intZoneCheck[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] == 2)
        {
          Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab2[l][m];
        }
        else if (intZoneCheck[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] == 3)
        {
          Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab3[l][m];
        }
        else if (intZoneCheck[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] == 4)
        {
          Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab4[l][m];
        }
        else if (intZoneCheck[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] == 5)
        {
          Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab5[l][m];
        }

        //  //  Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab54[l][m];
        //}
        else
        {
          Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab[l][m];
        }

        // org
        // Fitness[x][y][z] = Fitness[x][y][z] + Population[x][y][z].fen[l] * Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[m] * tab[l][m];
      }
    }
  }
  // printf("koniec pętli\n");
}

int PayoffUpdate(Fenotypestruct ***Population, float ***Fitness, int size[3], float *****tab, int fenotypes, int neighbourmethod, float **tab1, float **tab2, float **tab3, float **tab4, float **tab5, int arrIntZone[elements][elements][elements], int intZoneCheck[elements][elements][elements], int intCellZone1, int **intCellZone1Fen0, int *intCellZone2, int **intCellZone2Fen0, int *intCellZone3, int **intCellZone3Fen0, int *intCellZone4, int **intCellZone4Fen0)
{
  int tempint = 4;
  int i, j, k, l;
  int phen_nr;
  int counter0_1 = 0;
  int counter0_2 = 0;
  int counter0_3 = 0;
  int counter0_4 = 0;
  int counter1_1 = 0;
  int counter1_2 = 0;
  int counter1_3 = 0;
  int counter1_4 = 0;
  int counter2_1 = 0;
  int counter2_2 = 0;
  int counter2_3 = 0;
  int counter2_4 = 0;
  int counter3_1 = 0;
  int counter3_2 = 0;
  int counter3_3 = 0;
  int counter3_4 = 0;

//   pomyśleć nad przerobieniem tego fora na to żeby zaczynał od 1 i osobno robić krańcowe pozycje
//   dodać osobno dla metody neuman/moore bo zmienia metodologia szukania somsiadów w powiązaniu z intZoneCheck
#pragma omp parallel for shared(Population, Fitness, size, fenotypes, tab1, tab2, tab3, tab4, tab5, arrIntZone, intZoneCheck, intCellZone1Fen0, intCellZone2Fen0, intCellZone3Fen0, intCellZone4Fen0) private(i, j, k) // FIXME: komentarz tylko na czas debug
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {
        // printf("Nowy payoff\n");
        CalculatePayoff(Population, Fitness, i, j, k, size, fenotypes, tab[i][j][k], neighbourmethod, arrIntZone, intZoneCheck, tab1, tab2, tab3, tab4, tab5);

        for (l = 0; l < fenotypes; l++)
        {
          // printf("\narrIntZone[i][j]: %d", arrIntZone[i][j]);
          if (Population[i][j][k].fen[l] != 0)
          {
            phen_nr = l; // to jest male L
          }
          if (arrIntZone[i][j][k] == 1)
          {
            if (phen_nr == 0)
            {
              // printf("BEFORE phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              //  intCellZone1Fen0[0] += 1;
              counter0_1++;
              // printf("counter fen0: %d \n", counter0_1);
              //*intCellZone1Fen0 = *intCellZone1Fen0 / 4;
              // printf("AFTER phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              // intCellZone1Fen0 = intCellZone1Fen0 / 4;
              // printf("mule\n");
            }
            else if (phen_nr == 1)
            {
              // intCellZone1Fen0[1]++;
              counter0_2++;
              // printf("counter fen1: %d \n", counter0_2);
              // printf("mule\n");
            }
            else if (phen_nr == 2)
            {
              // intCellZone1Fen0[2]++;
              counter0_3++;
              // printf("counter fen2: %d \n", counter0_3);
              // printf("mule\n");
            }
            else if (phen_nr == 3)
            {
              // intCellZone1Fen0[3]++;
              counter0_4++;
              // printf("counter fen3: %d \n", counter0_4);
              // printf("mule\n");
            }
            // intCellZone1[phen_nr]++;
            //  printf("phenotype %d counter: %d\n", phen_nr, intCellZone1[phen_nr]);
          }
          else if (arrIntZone[i][j][k] == 2)
          {
            // phen_nr = Population[i][j][k].fen[l];
            // if (Population[i][j][k].fen[l] != 0)
            //{
            //  phen_nr = l; // to jest male L
            //}
            if (phen_nr == 0)
            {
              // printf("BEFORE phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              //  intCellZone1Fen0[0] += 1;
              counter1_1++;
              //*intCellZone1Fen0 = *intCellZone1Fen0 / 4;
              // printf("AFTER phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              // intCellZone1Fen0 = intCellZone1Fen0 / 4;
              // printf("mule\n");
            }
            else if (phen_nr == 1)
            {
              // intCellZone1Fen0[1]++;
              counter1_2++;
            }
            else if (phen_nr == 2)
            {
              // intCellZone1Fen0[2]++;
              counter1_3++;
            }
            else if (phen_nr == 3)
            {
              // intCellZone1Fen0[3]++;
              counter1_4++;
            }
            // intCellZone1[phen_nr]++;
            //  printf("phenotype %d counter: %d\n", phen_nr, intCellZone1[phen_nr]);
          }
          else if (arrIntZone[i][j][k] == 3)
          {
            // phen_nr = Population[i][j][k].fen[l];
            // if (Population[i][j][k].fen[l] != 0)
            //{
            //  phen_nr = l; // to jest male L
            //}
            if (phen_nr == 0)
            {
              // printf("BEFORE phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              //  intCellZone1Fen0[0] += 1;
              counter2_1++;
              //*intCellZone1Fen0 = *intCellZone1Fen0 / 4;
              // printf("AFTER phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              // intCellZone1Fen0 = intCellZone1Fen0 / 4;
              // printf("mule\n");
            }
            else if (phen_nr == 1)
            {
              // intCellZone1Fen0[1]++;
              counter2_2++;
            }
            else if (phen_nr == 2)
            {
              // intCellZone1Fen0[2]++;
              counter2_3++;
            }
            else if (phen_nr == 3)
            {
              // intCellZone1Fen0[3]++;
              counter2_4++;
            }
            // intCellZone1[phen_nr]++;
            //  printf("phenotype %d counter: %d\n", phen_nr, intCellZone1[phen_nr]);

            // intCellZone3[phen_nr]++;
            //  printf("phenotype %d counter: %d\n", phen_nr, intCellZone3[phen_nr]);
          }
          else if (arrIntZone[i][j][k] == 4)
          {
            // phen_nr = Population[i][j][k].fen[l];
            // if (Population[i][j][k].fen[l] != 0)
            //{
            //  phen_nr = l; // to jest male L
            //}
            if (phen_nr == 0)
            {
              // printf("BEFORE phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              //  intCellZone1Fen0[0] += 1;
              counter3_1++;
              //*intCellZone1Fen0 = *intCellZone1Fen0 / 4;
              // printf("AFTER phenotype %d counter: %d, %d \n", phen_nr, intCellZone1Fen0[0], counter);
              // intCellZone1Fen0 = intCellZone1Fen0 / 4;
              // printf("mule\n");
            }
            else if (phen_nr == 1)
            {
              // intCellZone1Fen0[1]++;
              counter3_2++;
            }
            else if (phen_nr == 2)
            {
              // intCellZone1Fen0[2]++;
              counter3_3++;
            }
            else if (phen_nr == 3)
            {
              // intCellZone1Fen0[3]++;
              counter3_4++;
            }
            // intCellZone1[phen_nr]++;
            //  printf("phenotype %d counter: %d\n", phen_nr, intCellZone1[phen_nr]);

            // intCellZone4[phen_nr]++;
            //  printf("phenotype %d counter: %d\n", phen_nr, intCellZone4[phen_nr]);
          }
        }
      }
    }
  }
  intCellZone1Fen0[0] = counter0_1;
  intCellZone1Fen0[1] = counter0_2;
  intCellZone1Fen0[2] = counter0_3;
  intCellZone1Fen0[3] = counter0_4;
  // printf("%d, %d, %d, %d \n", intCellZone1Fen0[0], intCellZone1Fen0[1], intCellZone1Fen0[2], intCellZone1Fen0[3]);
  intCellZone2Fen0[0] = counter1_1;
  intCellZone2Fen0[1] = counter1_2;
  intCellZone2Fen0[2] = counter1_3;
  intCellZone2Fen0[3] = counter1_4;
  intCellZone3Fen0[0] = counter2_1;
  intCellZone3Fen0[1] = counter2_2;
  intCellZone3Fen0[2] = counter2_3;
  intCellZone3Fen0[3] = counter2_4;
  intCellZone4Fen0[0] = counter3_1;
  intCellZone4Fen0[1] = counter3_2;
  intCellZone4Fen0[2] = counter3_3;
  intCellZone4Fen0[3] = counter3_4;

  // intCellZone1 = intCellZone1Fen0[0];
  // printf("%d \n", intCellZone1Fen0[0]);
  // printf("%d \n", intCellZone1);

  // printf("test");

  // SaveZone1(buffer, intCellZone1Fen0, 0);
  return intCellZone1;
}

void Mortality(int ***ChosenCells, float MortalityProbability, int size[3], int updating) // ok
{
  int i, j, k;
  int x, y, z, sum = 0;
  int max = size[0] * size[1] * size[2];
  int random;
  int n;
  unsigned int seed;

  ////////Usunięcie jednego elementu - asynchroniczna metoda/////////////////
  if (updating == 0)
  {
    ChosenCells[(rand() % size[0])][(rand() % size[1])][(rand() % size[2])] = 1;
  }
  if (updating == 1)
  {

    ////////Usunięcie MortalityProbability wszystkich komórek - półsynchroniczna metoda//
    // n=(int)(MortalityProbability*max);
    // printf("n:%d max:%d\n",n,max);

    random = rand() + 1; // różny początek dla kolejnych iteracji

// można porównać jakby na czas wpływa gdyby zrobić fora po wszystkich komórkach i losować liczbę od 0 do 1 i sprawdzać czy liczba>MortalityProbability

/*
#pragma omp parallel private(seed,i,x,y,z) shared(ChosenCells,n)
{
  seed = random + omp_get_thread_num();
#pragma omp for
for (i=0;i<=n;i++)
{
   //trzeba losować tak długo aż wylosuje komórkę, która nie została jeszcze usunięta
  do
  {
  x=rand_r(&seed)%size[0];
  y=rand_r(&seed)%size[1];
  z=rand_r(&seed)%size[2];
  }while(ChosenCells[x][y][z]==1);
  ///////////TU MÓGŁ INNY RDZEŃ WYLOSOWAĆ TO SAMO ZANIM NASTĄPIŁA ZMIANA
  ChosenCells[x][y][z]=1;
}
}
//printf("\n");
*/
#pragma omp parallel private(seed, i, j, k) shared(ChosenCells, MortalityProbability)
    {
      seed = random + omp_get_thread_num();
#pragma omp for
      for (i = 0; i < size[0]; i++)
      {
        for (j = 0; j < size[1]; j++)
        {
          for (k = 0; k < size[2]; k++)
          {
            if ((rand_r(&seed) / (float)RAND_MAX) > MortalityProbability)
            {
              ChosenCells[i][j][k] = 0;
            }
            else
            {
              ChosenCells[i][j][k] = 1;
            }
          }
        }
      }
    }
  }

  //////////usunięcie wszystkich komórek - metoda synchroniczna ///////////////
  if (updating == 2)
  {
    FillArray(ChosenCells, size, 1);
  }
}

void CompetitiveReproduction(Fenotypestruct ***Population, Fenotypestruct ***NewPopulation, float ***Fitness, int ***ChosenCells, int size[3], int fenotypes, int neighbourmethod, int nrmethod, int param, int arrIntZone[elements][elements][elements], int *intCellCounter)
{
  Neighbourhoodstruct neighbours;
  int i, j, k, l, n, ok, max, random1, abc;
  float temp, temp2, random;
  // Fenotypestruct newPopulation;
  unsigned int seed;

  // int nrmethod=2;
  // int param=1;//ile do średniej - przy method=1 param=1
  random1 = rand() + 1;

  // #pragma omp parallel shared(Population, NewPopulation, Fitness, ChosenCells, size, fenotypes, nrmethod, param, random1, neighbourmethod) private(seed, i, j, k, l, n, max, neighbours, temp, temp2, ok, random)
  //   {
  //     seed = random1 + omp_get_thread_num();
  // #pragma omp for
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {
        if (i == 15)
        {
          // printf("\n[%d][%d][%d] - arrIntZone[i][j][k]: %d, ChosenCells[i][j][k]: %d", i, j, k, arrIntZone[i][j][k], ChosenCells[i][j][k]);
          fflush(stdout);
        }

        // jesli nr zone komorki do zastapienia to 1, to NewPopulation [funkcja CompetitiveReproduction] dla tej komorki to dalej bedzie stary fenotyp, to co bylo do tej pory w tym polu
        if (arrIntZone[i][j][k] == 1 || arrIntZone[i][j][k] == 12)
        {
          for (l = 0; l < fenotypes; l++)
          {
            // Przepisanie wartości do nowej populacji
            intCellCounter[0] += NewPopulation[i][j][k].fen[0];
            intCellCounter[1] += NewPopulation[i][j][k].fen[1];
            intCellCounter[2] += NewPopulation[i][j][k].fen[2];
            intCellCounter[3] += NewPopulation[i][j][k].fen[3];
            // printf("x: %d, y: %d, z: %d, 1: %f, 2: %f, 3: %f, 4: %f", i, j, k, l, NewPopulation[i][j][k].fen[0], NewPopulation[i][j][k].fen[1], NewPopulation[i][j][k].fen[2], NewPopulation[i][j][k].fen[3]);
            NewPopulation[i][j][k].fen[l] = Population[i][j][k].fen[l];
            // NewPopulation[i][j][k].fen[0] = 0;
            // NewPopulation[i][j][k].fen[1] = 0;
            // NewPopulation[i][j][k].fen[2] = 0;
            // NewPopulation[i][j][k].fen[3] = 1;
            //  NewPopulation[i][j][k].fen[0] = 1;
            //  NewPopulation[i][j][k].fen[1] = 0;
            //  NewPopulation[i][j][k].fen[2] = 0;
            //  NewPopulation[i][j][k].fen[3] = 0;

            //  printf("ir[0]:%d,ir[1]:%d,ir[2]:%d,ir[3]:%d\n", intCellCounter[0], intCellCounter[1], intCellCounter[2], intCellCounter[3]);
          }
        }
        else if (ChosenCells[i][j][k]) // jeśli wybrana komórka ma być zastąpiona
        {
          // jesli nr zone komorki [sprawdzany w macierzy 'Kula_5phen_2D'], ktora zostala wybrana do zastapienia [czyli wymiera] to 5,
          // to program moze olac wyliczanie odpowiednich macierzy z CompetitiveReproduction i zastapic ja FENOTYPEM PIERWSZYM [czyli chyba zerowym wg nomenklatury C - tym najslabszym, ktory atakuje z moca 0.01 0.01 0.01 0.1]
          // TERAZ CZWARTY FENOTYP PO ZMIANIE
          if (arrIntZone[i][j][k] == 5 || arrIntZone[i][j][k] == 54)
          {
            // printf("  PRE FEN =5 Population[%d][%d][%d].fen[0]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[0], arrIntZone[i][j]);
            NewPopulation[i][j][k].fen[0] = 0;
            // printf("AFTER FEN =5 Population[%d][%d][%d].fen[0]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[0], arrIntZone[i][j]);
            // printf("  PRE FEN =5 Population[%d][%d][%d].fen[1]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[1], arrIntZone[i][j]);
            NewPopulation[i][j][k].fen[1] = 0;
            // printf("AFTER FEN =5 Population[%d][%d][%d].fen[1]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[1], arrIntZone[i][j]);
            // printf("  PRE FEN =5 Population[%d][%d][%d].fen[2]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[2], arrIntZone[i][j]);
            NewPopulation[i][j][k].fen[2] = 0;
            // printf("AFTER FEN =5 Population[%d][%d][%d].fen[2]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[2], arrIntZone[i][j]);
            // printf("  PRE FEN =5 Population[%d][%d][%d].fen[3]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[3], arrIntZone[i][j]);
            NewPopulation[i][j][k].fen[3] = 1;
            // printf("AFTER FEN =5 Population[%d][%d][%d].fen[3]= %f, zone= %d\n", i, j, k, Population[i][j][k].fen[3], arrIntZone[i][j]);

            // ZMIANA Z Population[i][j][k].fen[...] na NewPopulation[i][j][k].fen[...] -> zeby aktualizowalo NOWA populacje
          }
          // pozostałe przypadki
          else
          {
            // for (l = 0; l < fenotypes; l++)
            // {
            //    printf("Population[%d][%d][%d].fen[%d]= %f, zone= %d\n", i, j, k, l, Population[i][j][k].fen[l], arrIntZone[i][j][k]);
            // }
            ChosenCells[i][j][k] = 0;
            temp = 0.0;
            neighbours = FindNeighbourhood(i, j, k, size, neighbourmethod);

            ////////////metoda 1, 2//////////////////
            // param - ile komórek wziąć do średniej
            //
            if (nrmethod)
            {
              sort(&neighbours, Population, Fitness, param, seed);
              // wyzerowanie NewPopulation
              for (l = 0; l < fenotypes; l++)
              {
                // printf("%d\n",l);
                NewPopulation[i][j][k].fen[l] = 0;
              }

              temp = 0.0;
              temp2 = 0.0; // równoważenie wag - tak żeby nie były ujemne
              if (Fitness[neighbours.x[param - 1]][neighbours.y[param - 1]][neighbours.z[param - 1]] <= 0)
              {
                temp2 = -Fitness[neighbours.x[param - 1]][neighbours.y[param - 1]][neighbours.z[param - 1]];
                if (Fitness[neighbours.x[0]][neighbours.y[0]][neighbours.z[0]] + temp2 <= 0)
                {
                  temp2 += 1.0; // wszystkie wagi są zerami więc dajemy wszystkie na 1 (bo są tak samo istotne)
                }
              }
              for (n = 0; n < param; n++)
              {
                // printf("OK1\n");
                // ok=1;

                for (l = 0; l < fenotypes; l++)
                {
                  // sumowanie z odpowiednią wagą
                  NewPopulation[i][j][k].fen[l] = NewPopulation[i][j][k].fen[l] + (Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[l] * (Fitness[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] + temp2));
                }
                // suma wag
                temp = temp + Fitness[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]] + temp2;
              }

              for (l = 0; l < fenotypes; l++)
              {
                // Obliczenie średniej ważonej
                NewPopulation[i][j][k].fen[l] = NewPopulation[i][j][k].fen[l] / temp;
              }
            }
            else // probabilistyczna
            {
              for (n = 0; n < neighbours.number; n++)
              {
                temp = temp + Fitness[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]];
                // printf("i=%d,j=%d,k=%d,n=%d,temp=%f\n",i,j,k,n,temp);
              }
              random = rand_r(&seed) / (float)RAND_MAX;
              temp2 = 0.0; // do sumowania fitnessów już sprawdzonych sąsiadów
              float pop_temp;
              float starepop_temp;
              for (n = 0; n < neighbours.number; n++)
              {
                temp2 = temp2 + Fitness[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]];
                if (random < (temp2 / temp))
                {
                  for (l = 0; l < fenotypes; l++)
                  {
                    //  starepop_temp = NewPopulation[i][j][k].fen[l];
                    NewPopulation[i][j][k].fen[l] = Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[l];
                    // pop_temp = Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[l];
                  }
                  break;
                }
              }
            }
          }
        }
        else
        {
          for (l = 0; l < fenotypes; l++)
          {
            // Przepisanie wartości do nowej populacji
            NewPopulation[i][j][k].fen[l] = Population[i][j][k].fen[l];
          }
        }
      }
    }
    //  }
  }

  // printf("\nNewPopulation\n");
  // for (int ii = 0; ii < size[0]; ii++)
  // {
  //   printf("\ni: %d: ", ii);
  //   for (int jj = 0; jj < size[0]; jj++)
  //   {
  //     printf("\n||j: %d: ", jj);

  //     for (int kk = 0; kk < size[0]; kk++)
  //     {
  //        //printf("j: %d: ", jj);
  //       //  float temp1 = NewPopulation[ii][jj][kk].fen[0];
  //       //  int temp2 = NewPopulation[ii][jj][kk].fen[1];
  //       //  int temp3 = NewPopulation[ii][jj][kk].fen[2];
  //       //  int temp4 = NewPopulation[ii][jj][kk].fen[3];
  //       //  printf("%d ", Population[ii][jj][kk].fen[1]); // printf("%0.1f ", Population[i][j][k].fen[3]);
  //       //  fflush(stdout);
  //         for (l = 0; l < fenotypes; l++)
  //         {
  //         //    printf("NewPopulation[%d][%d][%d] 0= %f, 1= %f, 2= %f, 3= %f, Population 0= %f, 1= %f, 2= %f, 3= %f,, zone= %d\n", ii, jj, kk, NewPopulation[ii][jj][kk].fen[0], NewPopulation[ii][jj][kk].fen[1],NewPopulation[ii][jj][kk].fen[2], NewPopulation[ii][jj][kk].fen[3],Population[ii][jj][kk].fen[0], Population[ii][jj][kk].fen[1], Population[ii][jj][kk].fen[2], Population[ii][jj][kk].fen[3], arrIntZone[ii][jj][kk]);
  //         //fflush(stdout);
  //         }
  //       if (kk == size[0])
  //       {
  //         printf("\n");
  //       }
  //     }
  //   }
  //   printf("\n");
  // }
  printf("\n");
}

void GetInformationFromXml(xmlNode *a_node, int *dimension, int *size, int *cores, int *maxgeneration, float *mortality, float ***tab, int *rows, int *columns, int *ok, int *neighbourmethod, int *reproductionmethod, int *reproductionparam, int *updating, int *MSEG, int *save, char *savepath, int *initPopulation, char *initPopulationPath, ResourcesFunctionStruct *resources)
{
  xmlNode *cur_node = NULL;
  xmlChar *content = NULL;
  xmlChar *prop = NULL;
  int temp, i = 0;
  char korektor[] = " ,";
  xmlChar *schowek;
  float tempfloat = 0.0;
  // void *tmp;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next)
  {
    // wejście poziom niżej
    if (xmlStrEqual(cur_node->name, (const xmlChar *)"data") || xmlStrEqual(cur_node->name, (const xmlChar *)"inputs") || xmlStrEqual(cur_node->name, (const xmlChar *)"outputs") || xmlStrEqual(cur_node->name, (const xmlChar *)"payoff"))
    {
      GetInformationFromXml(cur_node->children, dimension, size, cores, maxgeneration, mortality, tab, rows, columns, ok, neighbourmethod, reproductionmethod, reproductionparam, updating, MSEG, save, savepath, initPopulation, initPopulationPath, resources);
    }
    else
    {
      if (cur_node->type == XML_ELEMENT_NODE)
      {

        content = xmlNodeGetContent(cur_node);
        // printf("%s\n",cur_node->name);
        if (xmlStrEqual(cur_node->name, "dimension"))
        {
          *dimension = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "size"))
        {
          *size = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "cores"))
        {
          *cores = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "maxgeneration"))
        {
          *maxgeneration = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "mortality"))
        {
          *mortality = atof(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "neighbourhood"))
        {
          *neighbourmethod = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "reproduction"))
        {
          *reproductionmethod = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "param"))
        {
          *reproductionparam = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "updating"))
        {
          *updating = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "MSEG"))
        {
          *MSEG = atoi(content);
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "init-population"))
        {
          strcpy(initPopulationPath, content);
          *initPopulation = 1;
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "save-path"))
        {
          // savepath=(char*)content;
          strcpy(savepath, content);
          *save = 1;
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "resources"))
        {
          if (xmlStrEqual(content, "step"))
          {
            resources->type = 1;
          }
          else if (xmlStrEqual(content, "linear"))
          {
            resources->type = 2;
          }
          else if (xmlStrEqual(content, "sine"))
          {
            resources->type = 3;
          }
          else if (xmlStrEqual(content, "globalH"))
          {
            resources->type = 4;
          }
          else if (xmlStrEqual(content, "localH"))
          {
            resources->type = 5;
          }
          else
          {
            resources->type = 0;
            printf("Błędna nazwa funkcji - tablica wzięta z payoffMatrix \n");
            xmlFree(content);
            continue;
          }

          prop = xmlGetProp(cur_node, "param1");
          tempfloat = atof(prop);
          resources->param1 = tempfloat;
          xmlFree(prop);

          if (resources->type == 1 || resources->type == 2 || resources->type == 3)
          {
            prop = xmlGetProp(cur_node, "param2");
            tempfloat = atof(prop);
            resources->param2 = tempfloat;
            xmlFree(prop);
          }

          if (resources->type == 1 || resources->type == 3)
          {
            prop = xmlGetProp(cur_node, "param3");
            tempfloat = atof(prop);
            resources->param3 = tempfloat;
            xmlFree(prop);
          }

          prop = xmlGetProp(cur_node, "v");
          tempfloat = atof(prop);
          resources->v = tempfloat;
          xmlFree(prop);

          prop = xmlGetProp(cur_node, "c");
          tempfloat = atof(prop);
          resources->c = tempfloat;
          xmlFree(prop);

          *tab = realloc(*tab, 2 * sizeof(**tab));
          (*tab)[0] = malloc(2 * sizeof(***tab));
          (*tab)[1] = malloc(2 * sizeof(***tab));
          *rows = 2;
          *columns = 2;
          xmlFree(content);
          continue;
        }
        if (xmlStrEqual(cur_node->name, "row"))
        {
          if (resources->type == 0)
          {
            prop = xmlGetProp(cur_node, "r");
            temp = atoi(prop);
            if (temp + 1 > *rows)
            {
              *tab = realloc(*tab, (temp + 1) * sizeof(**tab));
              // tab=tmp;
              *rows = temp + 1;
              /*
              if( (tmp = realloc(tab,(temp+1)*sizeof(*tab))) == NULL )
              {
          //Jeśli brakuje pamięci to coś tu trzeba zrobić!!!
              }
              else
              {
              tab=tmp;
              *rows=temp+1;
              }*/
            }

            (*tab)[temp] = malloc(sizeof(***tab));
            schowek = strtok(content, korektor);
            // printf("0 %s\n",schowek);
            for (i = 0; schowek != NULL; i++)
            {
              // printf("1 %s\n",schowek);
              (*tab)[temp] = realloc((*tab)[temp], (i + 1) * sizeof(***tab));
              // printf("2 %s\n",schowek);
              (*tab)[temp][i] = atof(schowek);
              // printf("3 %s\n",schowek);
              schowek = strtok(NULL, korektor);
              /*
              if( (tmp = realloc(tab[temp],(i+1)*sizeof(**tab))) == NULL )
              {
          //Jeśli brakuje pamięci to coś tu trzeba zrobić!!!
              }
              else
              {
              tab[temp]=tmp;
              tab[temp][i]=atof(schowek);
              schowek=strtok(NULL,korektor); neighbourhood
              }*/
            }
            xmlFree(prop);
            if (*columns == 0 || *columns == i)
            {
              *columns = i;
            }
            else
            {
              printf("Nierowna liczba elementow w wierszach\n");
              *ok = 0;
              // BŁĄD!!!
            }
          }
          xmlFree(content);
        }
        else
        {
          xmlFree(content);
        }
      }
    }
  }
}

void Save(char *filename, Fenotypestruct ***Population, int fenotypes, int size[3], int createNewFile)
{
  FILE *file;

  int i, j, k, l;

  float suma;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("Kula_HD.bin") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "Kula_HD.bin");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    file = fopen(newfilename, "wb");
  else
    file = fopen(newfilename, "ab");

  if (file == NULL)
  {
    printf("Unable to open file!\n");
    exit(0);
  }

  // fprintf(fp,"%d",g);

  // for(l=0;l<fenotypes;l++)
  //{
  suma = 0.0;
  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {
        // printf("%p\n%p\n", &Population[i][j][k].fen[0],Population[i][j][k].fen);
        // printf("%p\n%p\n%p\n%p\n", &Population[i][j][k].fen, &sizeof(float), &fenotypes, &file);;
        fwrite(Population[i][j][k].fen, sizeof(float), fenotypes, file);
        // suma=suma+Population[i][j][k].fen[l];
      }
    }
  }

  // fprintf(fp,";%f",suma);
  //}
  // fprintf(fp,"\n");
  fclose(file);
}

void Save2(char *filename, Fenotypestruct ***Population, int fenotypes, int g, int size[3], int createNewFile)
{
  FILE *fp;

  int i, j, k, l;

  float suma;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("Kula_HD.csv") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "Kula_HD.csv");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    fp = fopen(newfilename, "w");
  else
    fp = fopen(newfilename, "a");

  fprintf(fp, "%d", g);

  for (l = 0; l < fenotypes; l++)
  {
    suma = 0.0;
    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        for (k = 0; k < size[2]; k++)
        {
          suma = suma + Population[i][j][k].fen[l];
        }
      }
    }
    // liczba komorek danego fenotypu / ilosc komorek w calej macierzy
    fprintf(fp, ",%f", suma / (size[0] * size[1] * size[2]));
    // fprintf(fp, ",%f", suma);
    // fprintf("cholerstwo %f", (size[0] * size[1] * size[2]))
  }
  fprintf(fp, "\n");
  fclose(fp);
}

void SaveRemovedCells(char *filename, int *intCellCounter, int createNewFile)
{
  FILE *f;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("RemovedCells.csv") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "RemovedCells.csv");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    f = fopen(newfilename, "w");
  else
    f = fopen(newfilename, "a");

  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%d,%d,%d,%d\n", intCellCounter[0], intCellCounter[1], intCellCounter[2], intCellCounter[3]);
  fclose(f);
}

void SaveZone1(char *filename, int **intCellZone1Fen0, int createNewFile)
{
  FILE *f;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("CellsZone1.csv") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "CellsZone1.csv");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    f = fopen(newfilename, "w");
  else
    f = fopen(newfilename, "a");

  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%d,%d,%d,%d\n", intCellZone1Fen0[0], intCellZone1Fen0[1], intCellZone1Fen0[2], intCellZone1Fen0[3]);
  fclose(f);
}

void SaveZone2(char *filename, int **intCellZone2Fen0, int createNewFile)
{
  FILE *f;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("CellsZone2.csv") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "CellsZone2.csv");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    f = fopen(newfilename, "w");
  else
    f = fopen(newfilename, "a");

  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%d,%d,%d,%d\n", intCellZone2Fen0[0], intCellZone2Fen0[1], intCellZone2Fen0[2], intCellZone2Fen0[3]);
  fclose(f);
}

void SaveZone3(char *filename, int **intCellZone3Fen0, int createNewFile)
{
  FILE *f;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("CellsZone3.csv") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "CellsZone3.csv");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    f = fopen(newfilename, "w");
  else
    f = fopen(newfilename, "a");

  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%d,%d,%d,%d\n", intCellZone3Fen0[0], intCellZone3Fen0[1], intCellZone3Fen0[2], intCellZone3Fen0[3]);
  fclose(f);
}

void SaveZone4(char *filename, int **intCellZone4Fen0, int createNewFile)
{
  FILE *f;
  char *newfilename;
  newfilename = malloc(strlen(filename) + strlen("CellsZone4.csv") + 1);
  strcpy(newfilename, filename);
  strcat(newfilename, "CellsZone4.csv");
  // newfilename = strcat(filename, "Kula_HD.bin");
  if (createNewFile)
    f = fopen(newfilename, "w");
  else
    f = fopen(newfilename, "a");

  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%d,%d,%d,%d\n", intCellZone4Fen0[0], intCellZone4Fen0[1], intCellZone4Fen0[2], intCellZone4Fen0[3]);
  fclose(f);
}

void Save3(char *filename, Fenotypestruct ***Population, int fenotypes, int g, int size[3])
{
  FILE *fp;

  int i, j, k, l;

  float suma;
  fp = fopen(filename, "a");

  fprintf(fp, "%d", g);

  for (l = 0; l < fenotypes; l++)
  {
    suma = 0.0;
    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        for (k = 0; k < size[2]; k++)
        {
          // suma = suma + Population[i][j][k].fen[l];
          fprintf(fp, ",%f", Population[i][j][k].fen[1]);
        }
      }
    }
    // liczba komorek danego fenotypu / ilosc komorek w calej macierzy
    // fprintf(fp, ",%f", suma / (size[0] * size[1] * size[2]));
    // fprintf(fp, ",%f", suma);
  }
  fprintf(fp, "\n");
  fclose(fp);
}

const char *getfield(char *line, int num)
{
  const char *tok;
  for (tok = strtok(line, ";");
       tok && *tok;
       tok = strtok(NULL, ";\n"))
  {
    if (!--num)
      return tok;
  }
  return NULL;
}

int readCSV(int intArrayWidth, char *strFilePath, int array[elements][elements][elements])
{
  // int(*array)[intArrayWidth][intArrayWidth] = malloc(sizeof(int[intArrayWidth][intArrayWidth][intArrayWidth]));

  int idx = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int temp = 0;

  int depth = 0;
  int rowIndex = 0;

  char line[intArrayWidth * intArrayWidth * 2 * 10]; // te 10 to do testw...
  char *buffer = NULL;
  size_t len = 0;
  size_t read;
  char *ptr = NULL;

  FILE *fp;
  fp = fopen(strFilePath, "r"); // open file , read only
  if (!fp)
  {
    fprintf(stderr, "failed to open file for reading\n");
  }

  while (fgets(line, sizeof(line), fp) != NULL && rowIndex < intArrayWidth)
  {
    k = 0;
    int colIndex = 0;
    j = 0;
    // printf("\nrowIndex: %d\ncolIndex: %d\n", rowIndex, colIndex);
    char *token = strtok(line, ",");
    while (token)
    {
      array[i][j][k] = atof(token); // temp; // atof(token); // temp; //

      // printf("\nk: %d, colIndex: %d, temp: ", k, colIndex);
      // printf(" %d ", array[i][k][j]);
      // fflush(stdout);
      // printf("Population[%d][%d][%d].fen[0]: %d\n", k, i, j, array[k][i][j]);
      depth++;
      colIndex++;
      j++;
      if (j == intArrayWidth)
      {
        k++;
        j = 0;
        depth = 0;
        //  printf("\nk: %d\n", k);
      }
      token = strtok(NULL, ",");
    }
    rowIndex++;
    i++;
  }

  fclose(fp);

  // int **arr = (int **)malloc(sizeof(int *) * intArrayWidth);

  // for (int i = 0; i < intArrayWidth; i++) // checking purpose

  // {

  //   arr[i] = (int *)malloc(sizeof(int) * intArrayWidth);

  //   for (int j = 0; j < intArrayWidth; j++)

  //   {

  //     arr[i][j] = array[i][j];
  //   }
  // }
  // for (int i = 0; i < idx; i++)
  // {
  //   // printf("\narray[%d][] =", i);    for (j = 0; j < intArrayWidth; j++)
  //   {
  //     //*intZoneCheck[i][j] = *array[i][j];
  //     printf(" %f", arr[i][j]);
  //   }
  // }
  return array;
}

float **readCSVfloat(int intArrayWidth, char *strFilePath)
{
  float *array[intArrayWidth];
  char *buffer = NULL;
  size_t len = 0;
  char read = NULL;
  char *pch = NULL;
  char *pch2 = NULL;
  FILE *fp;
  fp = fopen(strFilePath, "r"); // open file , read only
  if (!fp)
  {
    fprintf(stderr, "failed to open file for reading\n");
  }
  int i = 0;
  while ((read = getline(&buffer, &len, fp)) != -1)
  {
    pch2 = buffer;
    pch = strtok(pch2, ",\r\n");
    array[i] = malloc(sizeof(array));
    // printf("buffer: %s\n", buffer);
    while (pch != NULL)
    {
      for (int j = 0; j < intArrayWidth; j++)
      {

        if (pch != NULL)
        {
          array[i][j] = strtof(pch, NULL);
          // printf("db2: %f i: %d j: %d\n", array[i][j], i, j);
        }

        pch = strtok(NULL, ",\r\n");
      }
    }
    i++;
  }
  fclose(fp);

  float **arr = (float **)malloc(sizeof(float *) * intArrayWidth);
  for (int i = 0; i < intArrayWidth; i++) // checking purpose
  {
    arr[i] = (float *)malloc(sizeof(float) * intArrayWidth);
    for (int j = 0; j < intArrayWidth; j++)
    {
      // printf("array1: %f\n", array[i][j]);
      arr[i][j] = array[i][j];
      // printf("arr2: %f\n", arr[i][j]);
    }
  }

  // for (int i = 0; i < intArrayWidth; i++)
  //{
  //   printf("\narray[%d][] =", i);
  //
  //  for (int j = 0; j < intArrayWidth; j++)
  //  {
  //    printf(" %f", array[i][j]);
  //  }
  //}
  return arr;
}

int main(int argc, char **argv)
{
  int maxgeneration = 0;
  int size[3] = {1, 1, 1};
  int g, i, j, k, l, cores = 1, dimension = 1, rows = 0, columns = 0, xmlok = 1, flag;
  float ***fitness;
  Fenotypestruct ***population, ***Newpopulation;
  Fenotypestruct ****PopulationTab;
  int ***ChosenCells;
  float MortalityProbability = 0, suma = 0;
  int ile = 0;
  float **tab;
  float *****tabMatrix;
  int neighbourmethod = 0, reproductionmethod = 0, reproductionparam = 1, updating = 0, MSEG = 0, save = 0, initPopulation = 0;
  char savepath[100], savepath2[100], initPopulationPath[100];
  FILE *file;
  ResourcesFunctionStruct resources;
  resources.type = 0;

  int debug = 0;
  int *intCellRemovedCounter;
  int *intCellZone1 = 0;
  int *intCellZone2 = 0;
  int **intCellZone3;
  int **intCellZone4;
  int **intCellZone1Fen0;
  int **intCellZone2Fen0;
  int **intCellZone3Fen0;
  int **intCellZone4Fen0;
  char *absPath;

  srand(time(NULL));

  // printf("Start\n");

  ////////////////Odczyt XML///////////////////
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;
  if (argc != 2)
  {
    printf("Nieprawidlowa liczba argumentow wejsciowych\n");
    return (1);
  }
  absPath = argv[1];
  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION
  /*parse the file and get the DOM */
  doc = xmlReadFile(absPath, NULL, 0);

  if (doc == NULL)
  {
    printf("error: could not parse file %s\n", argv[1]);
  }
  root_element = xmlDocGetRootElement(doc);

  printf("path: %s\n", absPath);
  int len = strlen(absPath);
  absPath[len - 9] = '\0';
  printf("path2: %s\n", absPath);

  // alokacja jednego miejsca pamięci - żeby używać później funkcji realloc
  tab = malloc(sizeof(*tab));

  GetInformationFromXml(root_element, &dimension, &size[0], &cores, &maxgeneration, &MortalityProbability, &tab, &rows, &columns, &xmlok, &neighbourmethod, &reproductionmethod, &reproductionparam, &updating, &MSEG, &save, savepath, &initPopulation, initPopulationPath, &resources);
  /*free the document */

  xmlFreeDoc(doc);

  // wczytanie macierzy kosztow dla KAZDEGO zone
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Data1.csv");
  float **tab1 = readCSVfloat(4, buffer);
  printf("tab1Buff: %s\n", buffer);
  snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Data2.csv");
  float **tab2 = readCSVfloat(4, buffer);
  printf("tab2Buff: %s\n", buffer);

  snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Data3.csv");
  float **tab3 = readCSVfloat(4, buffer);
  printf("tab3Buff: %s\n", buffer);

  snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Data4.csv");
  float **tab4 = readCSVfloat(4, buffer);
  printf("tab4Buff: %s\n", buffer);

  snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Data5.csv");
  float **tab5 = readCSVfloat(4, buffer);
  printf("tab5Buff: %s\n", buffer);

  if (debug) // wyświetlanie plików Data*.csv (debug)
  {
    int tempint = 4;
    for (int i = 0; i < tempint; i++)
    {
      printf("\ntab1[%d][] =", i);

      for (int j = 0; j < tempint; j++)
      {
        printf("i: %d, j: %d", i, j);
        printf("= %f", tab1[i][j]);
      }
    }
    printf("\n");
    for (int i = 0; i < tempint; i++)
    {
      printf("\ntab2[%d][] =", i);

      for (int j = 0; j < tempint; j++)
      {
        printf("i: %d, j: %d", i, j);
        printf("= %f", tab2[i][j]);
      }
    }
    printf("\n");
    for (int i = 0; i < tempint; i++)
    {
      printf("\ntab3[%d][] =", i);

      for (int j = 0; j < tempint; j++)
      {
        printf("i: %d, j: %d", i, j);
        printf("= %f", tab3[i][j]);
      }
    }
    printf("\n");
    for (int i = 0; i < tempint; i++)
    {
      printf("\ntab4[%d][] =", i);

      for (int j = 0; j < tempint; j++)
      {
        printf("i: %d, j: %d", i, j);
        printf("= %f", tab4[i][j]);
      }
    }
    printf("\n");
    for (int i = 0; i < tempint; i++)
    {
      printf("\ntab5[%d][] =", i);

      for (int j = 0; j < tempint; j++)
      {
        printf("i: %d, j: %d", i, j);
        printf("= %f", tab5[i][j]);
      }
    }
    printf("\n");
  }

  // 0 - von neumann; 1 - moore
  // wczytuje odpowiednia tabele w .csv zaznaczonych 'iffy' sasiedztw
  snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Kula_5phen_2D.csv");
  printf("Kula_5phen_2D: %s\n", buffer);
  int intZoneCheck[size[0]][size[0]][size[0]];
  int arrIntZone[size[0]][size[0]][size[0]];
  readCSV(size[0], buffer, intZoneCheck);

  if (neighbourmethod)
  {
    snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Kula_5phen_2D_phenotype_check_moore.csv");
    printf("Kula_5phen_2D_phenotype_check_moore: %s\n", buffer);
    readCSV(size[0], buffer, arrIntZone);
    for (i = 0; i < size[0]; i++)
    {
      printf("\nk: %d: ", i);
      for (j = 0; j < size[0]; j++)
      {
        printf("\n||i: %d: ", j);

        for (k = 0; k < size[0]; k++)
        {
          // printf("j: %d: ", j);
          // printf("%d ", intZoneCheck[i][j][k]);
          fflush(stdout);
          if (j == size[0])
          {
            printf("\n");
          }
        }
      }
      printf("\n");
    }
  }

  else
  {
    snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, "Kula_5phen_2D.csv");
    // printf("arrIntZoneBuffer: %s\n", buffer);
    readCSV(size[0], buffer, intZoneCheck);
    // for (i = 0; i < size[0]; i++)
    //{
    //   printf("\narray[%d][] =", i);
    //
    //  for (j = 0; j < size[0]; j++)
    //    printf(" %d", intZoneCheck[i][j]);
    //}
  }

  /*
   *Free the global variables that may
   *have been allocated by the parser.
   */
  xmlCleanupParser();

  printf("col: %d, rows: %d\n", columns, rows);
  // macierz payoff musi być kwaratowa
  if (columns != rows)
  {
    printf("Macierz nie jest kwadratowa\n");
    xmlok = 0;
  }

  if (xmlok)
  {
    // Odkomentować, jeśli używamy cores w xml a nie ustawiając zmienną OMP_NUM_THREADS
    // omp_set_num_threads(cores);

    cores = omp_get_max_threads();
    // printf("Cores=%d",cores);

    // przepisanie rozmiaru na każdy wymiar (max 3D)
    for (i = 1; i < dimension && i < 3; i++)
    {
      size[i] = size[i - 1];
    }

    if (1) // dynamiczna alokacja pamięci
    {
      // chyba rows powinno tu być - countery do zliczania porawnych komórek, uzależnione od liczby fenotypów
      intCellRemovedCounter = malloc(rows * sizeof(*intCellRemovedCounter));
      // intCellZone1 = malloc(rows * sizeof(*intCellZone1));
      for (int i = 0; i < rows; i++)
      {
        intCellRemovedCounter[i] = 0;
        // intCellZone1[i] = 0;
      }

      intCellZone1Fen0 = malloc(rows * sizeof(*intCellZone1Fen0));
      intCellZone2Fen0 = malloc(rows * sizeof(*intCellZone2Fen0));
      intCellZone3Fen0 = malloc(rows * sizeof(*intCellZone3Fen0));
      intCellZone4Fen0 = malloc(rows * sizeof(*intCellZone4Fen0));

      for (i = 0; i < rows; i++)
      {
        intCellZone1Fen0[i] = 0; //= malloc(rows * sizeof(*intCellZone1Fen0));
        intCellZone2Fen0[i] = 0; //= malloc(rows * sizeof(*intCellZone2Fen0));
        intCellZone3Fen0[i] = 0; //= malloc(rows * sizeof(*intCellZone3Fen0));
        intCellZone4Fen0[i] = 0; // = malloc(rows * sizeof(*intCellZone4Fen0));
      }

      // dwa miejsca które na zmianę będą uzupełniane
      PopulationTab = malloc(2 * sizeof(*PopulationTab));

      population = malloc(size[0] * sizeof(*population));
      for (i = 0; i < size[0]; i++)
      {
        population[i] = malloc(size[1] * sizeof(**population));
        for (j = 0; j < size[1]; j++)
        {
          population[i][j] = malloc(size[2] * sizeof(***population));
          for (k = 0; k < size[2]; k++)
          {
            population[i][j][k].fen = malloc(rows * sizeof(float));
          }
        }
      }

      Newpopulation = malloc(size[0] * sizeof(*Newpopulation));
      for (i = 0; i < size[0]; i++)
      {
        Newpopulation[i] = malloc(size[1] * sizeof(**Newpopulation));
        for (j = 0; j < size[1]; j++)
        {
          Newpopulation[i][j] = malloc(size[2] * sizeof(***Newpopulation));
          for (k = 0; k < size[2]; k++)
          {
            Newpopulation[i][j][k].fen = malloc(rows * sizeof(float));
          }
        }
      }
      PopulationTab[0] = population;
      PopulationTab[1] = Newpopulation;

      fitness = malloc(size[0] * sizeof(*fitness));
      for (i = 0; i < size[0]; i++)
      {
        fitness[i] = malloc(size[1] * sizeof(**fitness));
        for (j = 0; j < size[1]; j++)
        {
          fitness[i][j] = malloc(size[2] * sizeof(***fitness));
        }
      }

      ChosenCells = malloc(size[0] * sizeof(*ChosenCells));
      for (i = 0; i < size[0]; i++)
      {
        ChosenCells[i] = malloc(size[1] * sizeof(**ChosenCells));
        for (j = 0; j < size[1]; j++)
        {
          ChosenCells[i][j] = malloc(size[2] * sizeof(***ChosenCells));
        }
      }

      tabMatrix = malloc(size[0] * sizeof(*tabMatrix));
      for (i = 0; i < size[0]; i++)
      {
        tabMatrix[i] = malloc(size[1] * sizeof(**tabMatrix));
        for (j = 0; j < size[1]; j++)
        {
          tabMatrix[i][j] = malloc(size[2] * sizeof(***tabMatrix));
          for (k = 0; k < size[2]; k++)
          {

            if (resources.type == 5)
            {
              // 2 bo H-D ma 2 wiersze i 2 kolumny
              tabMatrix[i][j][k] = malloc(2 * sizeof(****tabMatrix));
              tabMatrix[i][j][k][0] = malloc(2 * sizeof(*****tabMatrix));
              tabMatrix[i][j][k][1] = malloc(2 * sizeof(*****tabMatrix));
            }
            else
            {
              tabMatrix[i][j][k] = tab;
            }
          }
        }
      }
    }

    FillArray(ChosenCells, size, 0); // wypełnienie tablicy chosencells zerami

    // losowanie populacji - dodać odczyt z pliku binarnego
    // InitialisePopulation(PopulationTab[0], size, rows, MSEG, initPopulation, initPopulationPath);
    char bufferFen0[256];
    char bufferFen1[256];
    char bufferFen2[256];
    char bufferFen3[256];
    snprintf(bufferFen0, sizeof(buffer), "%s%s\0", absPath, "Kula1.csv");
    printf("InitialisePopulationCSVbufferFen0: %s\n", bufferFen0);
    snprintf(bufferFen1, sizeof(buffer), "%s%s\0", absPath, "Kula2.csv");
    printf("InitialisePopulationCSVbufferFen1: %s\n", bufferFen1);
    snprintf(bufferFen2, sizeof(buffer), "%s%s\0", absPath, "Kula3.csv");
    printf("InitialisePopulationCSVbufferFen2: %s\n", bufferFen2);
    snprintf(bufferFen3, sizeof(buffer), "%s%s\0", absPath, "Kula4.csv");
    printf("InitialisePopulationCSVbufferFen3: %s\n", bufferFen3);
    InitialisePopulationCSV(bufferFen0, bufferFen1, bufferFen2, bufferFen3, PopulationTab[0], size, rows, MSEG);
    if (debug) // debug wyświetla population
    {
      Fenotypestruct ***PopulationTest;
      PopulationTest = PopulationTab[0];
      for (int i = 0; i < size[0]; i++)
      {
        printf("\n warstwa [%d][] =\n", i);

        for (int j = 0; j < size[1]; j++)
        {
          for (int k = 0; k < size[2]; k++)
          {
            printf("TestPopulation[%d][%d][%d].fen[0]: %f\n", i, j, k, PopulationTest[i][j][k].fen[0]);
          }
        }
      }
    }

    // jeśli metoda jest deterministyczna to komórka ma być zastępowana przez najsilniejszą
    if (reproductionmethod == 1)
    {
      reproductionparam = 1;
    }

    for (int i = 0; i < 4; i++)
    {

      intCellZone1Fen0[i] = 0;
      intCellZone2Fen0[i] = 0;
      intCellZone3Fen0[i] = 0;
      intCellZone4Fen0[i] = 0;
    }

    if (save)
    {
      snprintf(buffer, sizeof(buffer), "%s%s\0", absPath, savepath);
      printf("savepath: %s\n", buffer);
      int result = mkdir(buffer, 0777);
      SaveZone1(buffer, intCellZone1Fen0, 1);
      SaveZone2(buffer, intCellZone2Fen0, 1);
      SaveZone3(buffer, intCellZone3Fen0, 1);
      SaveZone4(buffer, intCellZone4Fen0, 1);
      Save(buffer, PopulationTab[0], rows, size, 1);
      Save2(buffer, PopulationTab[0], rows, 0, size, 1);
      SaveRemovedCells(buffer, intCellRemovedCounter, 1);
    }

    for (g = 0; g < maxgeneration; g++)
    {
      printf("%d\n", g);

      // ChangePayoffMatrix(tabMatrix, resources, g + 1, PopulationTab[g % 2], size, neighbourmethod);
      //
      // PayoffUpdate(PopulationTab[g % 2], fitness, size, tabMatrix, rows, neighbourmethod);
      //
      // Mortality(ChosenCells, MortalityProbability, size, updating);
      //
      // CompetitiveReproduction(PopulationTab[g % 2], PopulationTab[(g + 1) % 2], fitness, ChosenCells, size, rows, neighbourmethod, reproductionmethod, reproductionparam);

      // ChangePayoffMatrix(tabMatrix, resources, g + 1, PopulationTab[g % 2], size, neighbourmethod);

      // TODO: w PayoffUpdate idziemy po kazdym elemencie macierzy. podczas chodzenia po niej sprawdzaj co krok NUMER ZONE zawarty w macierzach Kula_5phen_2D_phenotype_check_neumann/moore
      // jesli nr w polu to 4, to zmienna **tab [nie jestem pewna ilosci *] z funkcji CalculatePayoff powinna pochodzic z pliku Data4.xml [to sa dane wczytywane nastepujaca funkcja kawalek wyzej:
      // readCSVFloat(4,Data*.csv) (DONE)
      // jesli nr w polu to 5, to Data5.xml, jesli 1 to Data1.xml, itp. ale jesli nr pola to np 45, to wtedy wiadomo, ze przynajmniej jeden z sasiadow [z 4 w przypadku von neumanna i 8 w przypadku moore] bedzie pochodzil z zone 5, a nie 4 jak 'srodek'.
      // mozna sprawdzic, ktory jest niefajny, zagladajac do macierzy 'Kula_5phen_2D'
      // jesli sytuacja wyglada tak jak na obrazku w paincie, to numerek odpowiada maicerzy tab, ktora dane pole ma uzyc, by obliczyc Fitness [czyli przeliczyc CalculatePayoff]
      //
      // nastepnie, progrma kontynuuje, wybierajac odpowiednie komorki do umarcia i zastapenia w funkcji Mortality, a potem je zastepuje odpowiednio wybranymi przy pomocy funkcji CompetitiveReproduction
      // jesli nr zone komorki [sprawdzany w macierzy 'Kula_5phen_2D'], ktora zostala wybrana do zastapienia [czyli wymiera] to 5,
      // to program moze olac wyliczanie odpowiednich macierzy z CompetitiveReproduction i zastapic ja FENOTYPEM PIERWSZYM [czyli chyba zerowym wg nomenklatury C - tym najslabszym, ktory atakuje z moca 0.01 0.01 0.01 0.1]
      // poniewaz tam komorki rakowe maja za malo papu by sie rozmnazac, jak tam wyjda to ok, ale tylko moga umrzec co najwyzej
      // z kolei jesli nr zone komorki do zastapienia to 1, to NewPopulation [funkcja CompetitiveReproduction] dla tej komorki to dalej bedzie stary fenotyp, to co bylo do tej pory w tym polu, czyli chyba mozna to zrobic jak tu:
      //               // Przepisanie wartości do nowej populacji
      //        NewPopulation[i][j][k].fen[l] = Population[i][j][k].fen[l];
      // a fenotyp, ktory ja zastapi [czyli chyba ten, co zostanie wpisayny w tej linijce [probabilistyczna]:
      //                     NewPopulation[i][j][k].fen[l] = Population[neighbours.x[n]][neighbours.y[n]][neighbours.z[n]].fen[l];
      // albo chyba tej [deterministyczna]:
      //                     NewPopulation[i][j][k].fen[l] = NewPopulation[i][j][k].fen[l] / temp;
      // powinien zostac zliczany counterem [ale on przepada bo porywa go krew z zyly]
      // to jest, jesli zostanie porwana 1 komorka fenotypu drugiego i 2 komorki fenotypu trzeciego, to countery powinny wygladac tak: counter2 = 1; counter3 = 2;

      // printf("%d \n", intCellZone1);
      if (1) //((g % 50) == 0)
      {
        printf("generation number: %d \n", g + 1);
      }

      PayoffUpdate(PopulationTab[g % 2], fitness, size, tabMatrix, rows, neighbourmethod, tab1, tab2, tab3, tab4, tab5, arrIntZone, intZoneCheck, intCellZone1, intCellZone1Fen0, intCellZone2, intCellZone2Fen0, intCellZone3, intCellZone3Fen0, intCellZone4, intCellZone4Fen0);

      // Save3(savepath2, PopulationTab[(g + 1) % 2], rows, g + 1, size);

      Mortality(ChosenCells, MortalityProbability, size, updating);

      CompetitiveReproduction(PopulationTab[g % 2], PopulationTab[(g + 1) % 2], fitness, ChosenCells, size, rows, neighbourmethod, reproductionmethod, reproductionparam, arrIntZone, intCellRemovedCounter);

      // Save3(savepath2, PopulationTab[(g + 1)], rows, g + 1, size);

      // intCellCounter;
      // printf("ir[0]:%d,ir[1]:%d,ir[2]:%d,ir[3]:%d\n", intCellCounter[0], intCellCounter[1], intCellCounter[2], intCellCounter[3]);

      // printf("%d \n", intCellZone1Fen0);

      // printf("FEN0 %d \n", intCellZone1Fen0[0]);
      // printf("%d \n", intCellZone1);

      float poptemp;

      if (save)
      {
        // printf("du: %d", (g + 1) % 2);
        //  Save(savepath, PopulationTab[(g + 1) % 2], rows, size);
        //  Save2(savepath2, PopulationTab[(g + 1) % 2], rows, g + 1, size);
        SaveZone1(buffer, intCellZone1Fen0, 0);
        SaveZone2(buffer, intCellZone2Fen0, 0);
        SaveZone3(buffer, intCellZone3Fen0, 0);
        SaveZone4(buffer, intCellZone4Fen0, 0);
        SaveRemovedCells(buffer, intCellRemovedCounter, 0);
        Save(buffer, PopulationTab[(g + 1) % 2], rows, size, 0);
        Save2(buffer, PopulationTab[(g + 1) % 2], rows, g + 1, size, 0);
      }
      for (int i = 0; i < 4; i++)
      {

        intCellZone1Fen0[i] = 0;
        intCellZone2Fen0[i] = 0;
        intCellZone3Fen0[i] = 0;
        intCellZone4Fen0[i] = 0;
      }
    }
    // Show(population, size,rows);
    /*
    suma=0;
    ile=0;

  printf("\nKoniec:\n");

  for(l=0;l<rows;l++)
  {
    suma=0.0;
    for(i=0;i<size[0];i++)
    {
      for(j=0;j<size[1];j++)
      {
        for(k=0;k<size[2];k++)
        {
    suma=suma+PopulationTab[maxgeneration%2][i][j][k].fen[l];
        }
      }
    }
    printf("suma %d: %f\n",l,suma);
  }

    */

    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        for (k = 0; k < size[2]; k++)
        {
          free(population[i][j][k].fen);
        }
        free(population[i][j]);
      }
      free(population[i]);
    }
    free(population);

    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        for (k = 0; k < size[2]; k++)
        {
          free(Newpopulation[i][j][k].fen);
        }
        free(Newpopulation[i][j]);
      }
      free(Newpopulation[i]);
    }
    free(Newpopulation);

    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        free(fitness[i][j]);
      }
      free(fitness[i]);
    }
    free(fitness);

    for (i = 0; i < size[0]; i++)
    {
      for (j = 0; j < size[1]; j++)
      {
        free(ChosenCells[i][j]);
      }
      free(ChosenCells[i]);
    }
    free(ChosenCells);

    free(PopulationTab);
  }

  for (i = 0; i < size[0]; i++)
  {
    for (j = 0; j < size[1]; j++)
    {
      for (k = 0; k < size[2]; k++)
      {

        if (resources.type == 5)
        {
          free(tabMatrix[i][j][k][0]);
          free(tabMatrix[i][j][k][1]);
          free(tabMatrix[i][j][k]);
        }
        else if (i == 0 && j == 0 && k == 0)
        {
          tabMatrix[i][j][k] = tab;
          for (l = 0; l < rows; l++)
          {
            free(tabMatrix[i][j][k][l]);
          }
          free(tabMatrix[i][j][k]);
        }
      }
      free(tabMatrix[i][j]);
    }
    free(tabMatrix[i]);
  }
  free(tabMatrix);

  printf("DONE!\n");

  return 0;
}

#else
int main(void)
{
  fprintf(stderr, "Tree support not compiled in\n");
  exit(1);
}
#endif