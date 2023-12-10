#include "library.hpp"

int test1()
{
   return 101;
}

int test2()
{
   return 202;
}

int test3()
{
   return 303;
}

int (*functptr[])() = { test1, test2, test3 } ;
