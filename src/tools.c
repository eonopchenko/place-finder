/*
 * tools.c
 *
 *  Created on: 14/03/2018
 *      Author: jake
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int Trim(char* str, int argn, ...)
{
  char* args = malloc(argn * sizeof(char));
  va_list valist;
  va_start(valist, argn);
  for (int i = 0; i < argn; i++)
  {
    args[i] = va_arg(valist, int);
  }
  va_end(valist);

  int len = strlen(str);
  char *ptr1 = str;
  char *ptr2 = str + 1;

  while (ptr2 < (str + len))
  {
    int f = 0;
    for (int i = 0; i < argn; i++)
	{
      if (*ptr2 == args[i])
      {
        f = 1;
        break;
      }
    }
    if (f != 0)
    {
      ptr2++;
    }
    else
    {
      *(++ptr1) = *ptr2++;
    }
  }

  *(++ptr1) = '\0';

  free(args);

  return (int)(ptr1 - str);
}
