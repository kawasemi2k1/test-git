#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Max 100000

typedef int element;

int *createArray(int size)
{
  int *a;
  int i;
  srand((int)time(0));
  a = (int *)malloc(size * sizeof(int));
  for (i = 0; i < size; i++)
    *(a + i) = rand() % 10;
  return a;
}

int *dumpArray(int *a, int size)
{
  int *b;
  int i;
  b = (int *)malloc(size * sizeof(int));
  for (i = 0; i < size; i++)
    *(b + i) = *(a + i);
  return b;
}

void exch(int a[], int i, int j)
{
  int temp;
  temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}

void sort_3Way(int a[], int l, int r)
{
  if (r <= l)
    return;
  int i = l - 1, j = r;
  int p = l - 1, q = r;
  while (1)
  {
    while (a[++i] > a[r])
      ;
    while (a[r] > a[--j])
      if (j == l)
        break;
    if (i >= j)
      break;
    exch(a, i, j);
    if (a[i] == a[r])
      exch(a, ++p, i);
    if (a[j] == a[r])
      exch(a, --q, j);
  }
  exch(a, i, r);
  j = i - 1;
  i = i + 1;
  for (int k = l; k <= p; k++)
    exch(a, k, j--);
  for (int k = r - 1; k >= q; k--)
    exch(a, k, i++);
  sort_3Way(a, l, j);
  sort_3Way(a, i, r);
}

int cmpfunc(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}


void sapXepChon(element a[], int n)
{
  int i, j, min, tmp;
  for (i = 0; i < n - 1; i++)
  {
    min = i;
    for (j = i + 1; j < n; j++)
      if (a[j] > a[min])
        min = j;
    tmp = a[i];
    a[i] = a[min];
    a[min] = tmp;
  }
}

int main()
{
  int *a, *b, *c;
  clock_t tstart, tfinish;
  float tcomp;
  a = createArray(Max);
  b = dumpArray(a, Max);
  c = dumpArray(a, Max);
  //for(int i=0;i<Max;i++)
  // printf("%d\n",a[i]);

  tstart = clock();
  qsort(a, Max, sizeof(int), cmpfunc);
  tfinish = clock();
  tcomp = (float)(tfinish - tstart) / CLOCKS_PER_SEC;
  printf("Thoi gian qsort chay la:%f\n", tcomp);
  // for(int i=0;i<Max;i++)
  //printf("%d\n",a[i]);

  tstart = clock();
  sort_3Way(b, 0, Max - 1);
  tfinish = clock();
  tcomp = (float)(tfinish - tstart) / CLOCKS_PER_SEC;
  printf("Thoi gian 3 Way chay la:%f\n", tcomp);

  //for(int i=0;i<Max;i++)
  //printf("%d\n",b[i]);

  tstart = clock();
  sapXepChon(c, Max);
  tfinish = clock();
  tcomp = (float)(tfinish - tstart) / CLOCKS_PER_SEC;
  printf("Thoi gian sap xep chon chay la:%f\n", tcomp);
  // for(int i=0;i<Max;i++)
  //printf("%d\n",a[i]);
  free(a);
  free(b);
  free(c);
}
