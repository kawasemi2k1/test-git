#include <stdio.h>
#include <stdio_ext.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "cgraphv1.h"

int main()
{

  int menu;
  char c;
  do
  {

    printf("\n=======MENU======\n");
    printf("1. \n");
    printf("2. \n");
    printf("3. \n");
    printf("4. \n");
    printf("5. Thoat\n");
    printf("-----------------------------------------\n");
    do
    {
      __fpurge(stdin);
      menu = 0;
      printf("Nhap lua chon cua ban: ");
      scanf("%d%c", &menu, &c);
      if (menu < 1 || menu > 5 || c != '\n')
        printf("Khong dung dinh dang! Vui long nhap lai!!\n");
    } while (menu < 1 || menu > 5 || c != '\n');

    switch (menu)
    {
    case 1:
      //ham1
      break;
    case 2:
      //ham2
      break;
    case 3:
      //ham3
      break;
    case 4:
      //ham3
      break;
    case 5:
      break;
    }
  } while (menu != 5);
}
