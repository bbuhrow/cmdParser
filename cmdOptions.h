// ============================================================================
// a simple command line parser
// ============================================================================


// the number of recognized command line options
#define NUMOPTIONS 4
// maximum length of command line option strings
#define MAXOPTIONLEN 20
// maximum length of help string for each option
#define MAXHELPLEN 128
// maximum length of an argument to an option
#define MAXARGLEN 256


typedef struct
{
    // command line options (names to be preceeded by a -)
    char OptionArray[NUMOPTIONS][MAXOPTIONLEN];
    char OptionHelp[NUMOPTIONS][MAXHELPLEN];
    char LongOptionAliases[NUMOPTIONS][MAXOPTIONLEN];

    // indication of whether or not an option needs a corresponding argument
    // 0 = no argument
    // 1 = argument required
    // 2 = argument optional
    int needsArg[NUMOPTIONS];

    // ========================================================================
    // this is where you define things that can be
    // set via command line options... add items here as necessary
    int optionInt;
    char optionStr[MAXARGLEN];
    double optionDbl;
    int optionNoarg;
    // ========================================================================

} options_t;

extern options_t* initOpt(void);
extern void applyOpt(char* opt, char* arg, options_t* options);
extern int processOpts(int argc, char** argv, options_t* options);


