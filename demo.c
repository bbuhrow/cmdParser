#include <stdio.h>
#include <stdlib.h>
#include "cmdOptions.h"


int main(int argc, char** argv)
{
    options_t* options;

    options = initOpt();
    processOpts(argc, argv, options);

    printf("options received:\n");
    printf("\t%d\n", options->optionInt);
    printf("\t%s\n", options->optionStr);
    printf("\t%1.2f\n", options->optionDbl);
    if (options->optionNoarg > 0)
    {
        printf("\toption 'v' was included %d times\n", options->optionNoarg);
    }

    free(options);
    return 0;
}
