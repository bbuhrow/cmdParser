#include "cmdOptions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================================================================
// In this section, define the options, any long form aliases for them,
// whether they have an argument or not, and help strings to display
// if the user tries -h



// command line options, specified by '-'
char OptionArray[NUMOPTIONS][MAXOPTIONLEN] = { 
    "i", "s", "d", "v"};

// command line option aliases, specified by '--'
// need the same number of strings here, even if
// some of them are blank (i.e., have no long form alias).
char LongOptionAliases[NUMOPTIONS][MAXOPTIONLEN] = {
    "int", "str", "dbl", ""};

// indication of whether or not an option needs a corresponding argument.
// needs to be the same length as the above two arrays.
// 0 = no argument
// 1 = argument required
// 2 = argument optional
int needsArg[NUMOPTIONS] = {
    1,2,1,0};

// help strings displayed with -h
// needs to be the same length as the above arrays, even if 
// some are left blank
char OptionHelp[NUMOPTIONS][MAXHELPLEN] = {
    "This option will be parsed as an integer",
    "This (optional) option will be parsed as an string",
    "This option will be parsed as a double",
    "This option should not have an argument"};
// ========================================================================



// ========================================================================
// modify this function to handle each option case, assigning values
// or arguments to the members of the options_t structure.
// ========================================================================
void applyOpt(char* opt, char* arg, options_t* options)
{
    if (strcmp(opt, options->OptionArray[0]) == 0)
    {
        options->optionInt = atoi(arg);
    }
    else if (strcmp(opt, options->OptionArray[1]) == 0)
    {
        if (arg == NULL)
        {
            strcpy(options->optionStr, "This is the secondary default option string");
        }
        else
        {
            strcpy(options->optionStr, arg);
        }
    }
    else if (strcmp(opt, options->OptionArray[2]) == 0)
    {
        sscanf(arg, "%lf", &options->optionDbl);
    }
    else if (strcmp(opt, options->OptionArray[3]) == 0)
    {
        options->optionNoarg++;
    }
    else
    {
        int i;

        printf("invalid option %s\n", opt);
        printf("supported options are: ");
        for (i = 0; i < NUMOPTIONS; i++)
        {
            if (i % 5 == 0) printf("\n");
            printf("%s ", options->OptionArray[i]);
        }
        exit(0);
    }

    return;
}

// ========================================================================
// modify this function to handle assign a default value to each 
// member of the options_t structure, if desired.
// ========================================================================
options_t* initOpt(void)
{
    options_t* options = (options_t*)malloc(sizeof(options_t));
    int i;

    for (i = 0; i < NUMOPTIONS; i++)
    {
        strcpy(options->OptionArray[i], OptionArray[i]);
        strcpy(options->OptionHelp[i], OptionHelp[i]);
        strcpy(options->LongOptionAliases[i], LongOptionAliases[i]);
        options->needsArg[i] = needsArg[i];
    }


    // ========================================================================
    // default values assigned here:

    options->optionNoarg = 0;
    strcpy(options->optionStr, "This is the default option string");
    options->optionInt = 42;
    options->optionDbl = 299792458.0;

    // ========================================================================

    return options;
}




// ========================================================================
// this function should not need to be changed
// ========================================================================
int processOpts(int argc, char** argv, options_t* options)
{
    // this function takes the command line arguments and returns
    // an options_t, which will have any valid options specified
    // on the command line filled in.  Return the number of options
    // processed.
    int i, j, k = 0, valid, longswitch = 0;
    char optbuf[MAXOPTIONLEN];
    char argbuf[MAXARGLEN];

    //argument loop
    i = 1;
    while (i < argc)
    {
        longswitch = 0;

        // read in the option
        if (argv[i][0] != '-')
        {
            printf("no switch detected\n");
            exit(24);
        }
        if ((strlen(argv[i]) > 1) && (argv[i][1] == '-'))
        {
            longswitch = 1;
        }

        // check if the options is valid
        valid = 0;
        if (longswitch)
        {
            for (j = 0; j < NUMOPTIONS; j++)
            {
                if (strncmp(options->LongOptionAliases[j], &argv[i][2], MAXOPTIONLEN) == 0)
                {
                    valid = 1;
                    strncpy(optbuf, options->OptionArray[j], MAXOPTIONLEN);
                    break;
                }
            }
        }
        else
        {
            for (j = 0; j < NUMOPTIONS; j++)
            {
                if (strncmp(options->OptionArray[j], &argv[i][1], MAXOPTIONLEN) == 0)
                {
                    valid = 1;
                    strncpy(optbuf, &argv[i][1], MAXOPTIONLEN);
                    break;
                }
            }
        }

        if (valid == 0)
        {
            printf("supported options are: \n");
            for (j = 0; j < NUMOPTIONS; j++)
            {
                if (strlen(options->LongOptionAliases[j]) > 0)
                {
                    if (options->needsArg[j])
                    {
                        printf("%s <value>: %s (alias --%s)\n", 
                            options->OptionArray[j], options->OptionHelp[j],
                            options->LongOptionAliases[j]);
                    }
                    else
                    {
                        printf("%s        : %s (alias --%s)\n", 
                            options->OptionArray[j], options->OptionHelp[j],
                            options->LongOptionAliases[j]);
                    }
                }
                else
                {
                    if (options->needsArg[j])
                    {
                        printf("%s <value>: %s\n", options->OptionArray[j], options->OptionHelp[j]);
                    }
                    else
                    {
                        printf("%s        : %s\n", options->OptionArray[j], options->OptionHelp[j]);
                    }
                }
            }
            exit(21);
        }

        //check to see if this option requires an argument
        if (options->needsArg[j] == 1)
        {
            i++;
            if ((i == argc) || argv[i][0] == '-')
            {
                printf("argument expected for %s%s\n", longswitch ? "--" : "-", optbuf);
                exit(22);
            }
            strncpy(argbuf, argv[i], MAXARGLEN);

            //now apply -option argument
            applyOpt(optbuf, argbuf, options);
            k++;
        }
        else if (options->needsArg[j] == 2)
        {
            // check to see if an argument was supplied
            if (((i + 1) == argc) || argv[i + 1][0] == '-')
            {
                // no option supplied.  use default option
                applyOpt(optbuf, NULL, options);
                k++;
            }
            else
            {
                i++;
                // an option was supplied, pass it on
                strncpy(argbuf, argv[i], MAXARGLEN);

                //now apply -option argument
                applyOpt(optbuf, argbuf, options);
                k++;
            }
        }
        else
        {
            //apply -option
            //now apply -option argument
            applyOpt(optbuf, NULL, options);
            k++;
        }
        i++;
    }

    return k;
}

