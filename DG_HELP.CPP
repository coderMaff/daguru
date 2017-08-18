/* dg_help.cpp - Online help pages.
**
** Maf - 23.10.96 - Created
**
*/

/*[ Includes ]***************************************************************/

#include    <stdio.h>   // for file opening/closing
#include    <string.h>  // for strcpy.
#include    <conio.h>   // 

#include    "dg_scr.h"  // For various screen draw routines.
#include    "dg_defs.h" // My niceties

/****************************************************************************/

int help()
{
/*    FILE    *FHelp;          // File handle
    void    *pBuffer;        // Mem to put help file in.
    int     Buffer = 32766;  // Set to slightly larger than file size of
                             // dg_help.dat
    char    text[78];        // One line of text
    int     n = 0;
    static  char    page = 0;


    pBuffer = malloc(Buffer);

    if ( !pBuffer )
    {
        message("Not enough free memory to open help file",0,0,0,0,2,20,1);
        return 2;
    }


    FHelp = fopen("dg_help.dat","r");

    if ( !FHelp )
    {
        message("Failed to open help file. DG_HELP.DAT",0,0,0,0,2,20,1);
        return 1;
    }

    n = fread( pBuffer, 1, Buffer, FHelp );

    if ( !n )
    {
        message("Failed to read help file. DG_HELP.DAT",0,0,0,0,2,20,1);
        return 3;
    }
*/
/*   look up andreas` source code on text printing.


    clrscr();

    hline(1,2,80);
    hline(45,2,80);

    vline(45,2,1);
    vline(45,2,80);

    gotoxy(1,1);
    putch('É');
    gotoxy(80,1);
    putch('»');
    gotoxy(1,45);
    putch('È');
    gotoxy(80,45);
    putch('¼');

*/
    memorisescr(COPY);

    message("TEMP HELP - PAGE UNDER CONSTRUCTION",0,0,0,0,0,4,1);

    message("1-0 - Command Keys. a - all, c - copy, d - delete, e - edit",
            "C - copy all, D - delete all, Esc | q - quit, +/- change buttons",
            "L/R arrow keys = change windows, U/D arrow keys = change item",
            "SPACE = Select, RETURN = Interprate. ( OR JUST PRAY! ) Maff ",0,2,10,4);

    memorisescr(PASTE);
/*
    if ( FHelp ) fclose( FHelp );
    free( pBuffer );
*/
    return 0;
}
