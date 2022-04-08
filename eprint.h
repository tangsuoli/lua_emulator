#ifndef EPRINT_H_INCLUDED
#define EPRINT_H_INCLUDED

#include <stdio.h>

extern FILE * fp_err;

#define err_print(fmt, args...)  do{\
        fprintf(fp_err, fmt, ##args);\
}while(0)

void err_print_init(char* path);

#endif // EPRINT_H_INCLUDED
