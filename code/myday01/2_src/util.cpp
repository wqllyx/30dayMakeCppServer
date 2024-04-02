#include "util.hpp"
#include<cstdio>
#include<cstdlib>

void errorif(bool condition, const char *errmsg)
{
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
