/* dg_data.cpp - All stratic variables
**
** Maf - 10.09.96 - Created
**
*/

#include "dg_data.h"

file_structure_win      win[2][1024];
buttonbar_structure     ButtonBar[2];


// Icky global vars

long    Space[2];     // Space in window
int     Item[2];      // item highlighted in window 1 / 2
char    CurrentWin;   // active window
int     Files[2];     // Total files, window 1 / 2
char    Offset[2];    // position of highlight in window 1 / 2
char    Path[2][512]; // Path`s for win 1 & 2
char    bbar;         // Button Bar being used.
char    fDebug;       // Debug mode? Starts FALSE
char    fMouse;       // Mouse on/off
char    fShowAttrib;  // Show file attributes instead of size

