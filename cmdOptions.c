/*
MIT License

Copyright (c) 2021 Ben Buhrow

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// ============================================================================
// a simple command line parser.
// supports defining required and options arguments to a program.
// supports defining command line switches with optional arguments.
//
// gcc -O2 cmdOptions.h cmdOptions.c demo.c -o demo
//
// example:
// ./demo "hello" -v -v --str "world"
// ============================================================================

#include "cmdOptions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this function prints the help information specified by usageHelp
// and OptionHelp.
void printUsage(options_t* options);

// ========================================================================
// In this section, define the options, any long form aliases for them,
// whether they have an argument or not, and help strings to display
// if the user tries -h

// define help text for usage of the program.  
char usageHelp[MAXHELPLEN] = "msg_to_print [options]";

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
// modify this function to handle input arguments
// ========================================================================
void applyArg(char* arg, int argNum, options_t* options)
{
    if (argNum == 0)
    {
        strcpy(options->inputArg, arg);
    }

    return;
}

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
    // define how many required and optional arguments there are
    options->numArguments = 1;
    options->numRequired = 1;
    // ========================================================================


    // ========================================================================
    // default values assigned to optional arguments here

    // ========================================================================


    // ========================================================================
    // default values assigned to switches here:
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
    int numOpt = 0;

    //argument loop
    i = 1;
    while (i < argc)
    {
        longswitch = 0;

        // read in the option
        if ((strlen(argv[i]) > 1) && (argv[i][0] == '-') && (argv[i][1] == '-'))
        {
            longswitch = 1;
        }
        else if (argv[i][0] == '-')
        {
            longswitch = 0;
        }
        else
        {
            if (numOpt >= (options->numArguments))
            {
                printf("Too many inputs\n\n");
                printUsage(options);
                exit(0);
            }
            else
            {
                // process argument
                applyArg(argv[i], numOpt, options);
                numOpt++;
                i++;
                continue;
            }
        }

        if (numOpt < options->numRequired)
        {
            if (options->numRequired == 1)
            {
                printf("Missing %d required argument \n\n", options->numRequired);
            }
            else
            {
                printf("Missing %d required arguments \n\n", options->numRequired);
            }
            printUsage(options);
            exit(0);
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
            printUsage(options);
            exit(0);
        }

        //check to see if this option requires an argument
        if (options->needsArg[j] == 1)
        {
            i++;
            if ((i == argc) || argv[i][0] == '-')
            {
                printf("argument expected for %s%s\n\n", longswitch ? "--" : "-", optbuf);
                printUsage(options);
                exit(0);
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

    if (numOpt < options->numRequired)
    {
        if (options->numRequired == 1)
        {
            printf("Missing %d required argument \n\n", options->numRequired);
        }
        else
        {
            printf("Missing %d required arguments \n\n", options->numRequired);
        }
        printUsage(options);
        exit(0);
    }

    return k;
}

// ========================================================================
// this function should not need to be changed:
// edit OptionHelp and usageHelp strings.
// ========================================================================
void printUsage(options_t* options)
{
    int j;
    printf("Usage: %s\n", usageHelp);
    printf("supported options are: \n");
    for (j = 0; j < NUMOPTIONS; j++)
    {
        int n;
        if (strlen(options->LongOptionAliases[j]) > 0)
        {
            if (options->needsArg[j])
            {
                n = printf("%s <value>",
                    options->OptionArray[j]);
                for (; n < MAXOPTIONLEN + 8; n++)
                    printf(" ");
                printf(": %s (alias --%s)\n",
                    options->OptionHelp[j],
                    options->LongOptionAliases[j]);
            }
            else
            {
                n = printf("%s        ",
                    options->OptionArray[j]);
                for (; n < MAXOPTIONLEN + 8; n++)
                    printf(" ");
                printf(": %s (alias --%s)\n",
                    options->OptionHelp[j],
                    options->LongOptionAliases[j]);
            }
        }
        else
        {
            if (options->needsArg[j])
            {
                n = printf("%s <value>",
                    options->OptionArray[j]);
                for (; n < MAXOPTIONLEN + 8; n++)
                    printf(" ");
                printf(": %s\n", options->OptionHelp[j]);
            }
            else
            {
                n = printf("%s        ",
                    options->OptionArray[j]);
                for (; n < MAXOPTIONLEN + 8; n++)
                    printf(" ");
                printf(": %s\n", options->OptionHelp[j]);
            }
        }
    }
    return;
}

// ========================================================================
// this function should not need to be changed:
// parse options from a .ini (or other) file in a option=value syntax
// ========================================================================
void readINI(const char* filename, options_t* options)
{
    FILE* doc;
    char* str;
    char* key;
    char* value;
    int len;

    doc = fopen(filename, "r");

    if (doc == NULL)
    {
        printf("warning: could not open %s, no options parsed\n", filename);
        return;
    }

    str = (char*)malloc(1024 * sizeof(char));
    while (fgets(str, 1024, doc) != NULL)
    {
        // if first character is a % sign, skip this line.
        if (str[0] == '%')
            continue;

        // if first character is a blank, skip this line.
        if (str[0] == ' ')
            continue;

        // if last character of line is newline, remove it
        do
        {
            len = strlen(str);
            if (str[len - 1] == 10)
                str[len - 1] = '\0';
            else if (str[len - 1] == 13)
                str[len - 1] = '\0';
            else
                break;
        } while (len > 0);

        // if line is now blank, skip it.
        if (strlen(str) == 0)
            continue;

        // read keyword by looking for an equal sign
        key = strtok(str, "=");

        if (key == NULL)
        {
            // no longer insist on having an argument
            key = str;
            value = NULL;
        }
        else
        {
            // read value
            value = strtok((char*)0, "=");
        }

        // apply the option... same routine command line options use
        applyOpt(key, value, options);
    }

    fclose(doc);
    free(str);

    return;
}
