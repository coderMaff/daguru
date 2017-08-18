/* dg_data.h - All global variables
**
** Maf - 10.09.96 - Created
**
*/

typedef struct 
{
    char    attr;         // File or Directory
    char    name[13];       // File / Dir Name
    long    size;           // File / Dir Size
    char    tagged;         // Selected Y / N
    char    date[11];        // File date
    char    time[9];        // File time
} file_structure_win;

typedef struct
{
    char    name[10][4];    // Button Bar name
} buttonbar_structure;

extern  file_structure_win      win[2][1024];
extern  buttonbar_structure     ButtonBar[2];


//icky global vars

extern  long    Space[2];       // Space in window
extern  int     Item[2];        // item highlighted in window 1 / 2
extern  char    CurrentWin;     // active window
extern  int     Files[2];       // Total files, window 1 / 2
extern  char    Offset[2];      // position of highlight in window 1 / 2
extern  char    Path[2][512];   // Path`s for win 1 & 2
extern  char    bbar;           // Button Bar being used.
extern  char    fDebug;         // Debug mode? Starts FALSE
extern  char    fMouse;        // Mouse on/off
extern  char    fShowAttrib;    // Show file attributes instead of size



