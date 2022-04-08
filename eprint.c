#include "eprint.h"
#include <stdlib.h>
#include <stdio.h>

FILE * fp_err = NULL;

void err_print_init(char* path) {
    if (strcmp(path, "")) {
        fp_err = fopen (path, "w+");
    }

    if (fp_err == NULL) {
        fp_err = stderr;
    }
}
