/* dg_scr.cpp - Screen output functions.
**
** Maf - 10.09.96 - Created
** Maf - 30.03.97 - ported to Watcom v10.6 under DOS4GW
** Maf - 10.04.97 - improved dir displaying
**
** 2do : Re Write highlight same as drawscreen to speed things up.
*/

/*[ Includes ]***************************************************************/

#include        <stdio.h>
#include        <conio.h>
#include        <string.h>
#include        <dos.h>
#include        <math.h>
#include        <graph.h>
#include        <ctype.h>


#include        "dg_scr.h"
#include        "dg_data.h"
#include        "dg_defs.h"
#include        "dg_misc.h"
#include        "dg_io.h"


/****************************************************************************/

int pressanykey(void)
{
    cprintf("--------------------------------------------------------------------------------");
    cprintf("Process complete, and hopefully sucessfull, Press the 'ANY' key to continue. ");

    getch();

    return 0;
}

/****************************************************************************/

int drawscreen(void)
{
    _setvideomode( _TEXTC80 );

    _settextrows(50);             // Hires
    _setbkcolor(BLACK);
    _settextcolor(WHITE);
    _settextcursor(0x2000);       // No cursor

    _clearscreen( _GCLEARSCREEN );
    selectwin(0);

    return 0;
}

/****************************************************************************/

int filenumber(void)
{
    _settextcolor(RED);
    _setbkcolor(BLACK);
                                                  
    _settextposition(44,1);                       
    cprintf("File %4d of %4d",Item[0],Files[0]);  
    _settextposition(44,41);                      
    cprintf("File %4d of %4d",Item[1],Files[1]);  
                                                  
    return 0;
}

/****************************************************************************/

int buttonbar(char barno)
{
    char    *text;

    _settextposition(45,1);
    _settextcolor(YELLOW);
    _setbkcolor(BLUE);
    _outtext("                                                                                ");
    _outtext("      #1     #2     #3     #4      #5     #6     #7     #8      #9     #0       ");
    _settextcolor(CYAN);
    _setbkcolor(BLUE);
    _outtext("    лллллл лллллл лллллл лллллл  лллллл лллллл лллллл лллллл  лллллл лллллл     ");
    _outtext("    лллллл лллллл лллллл лллллл  лллллл лллллл лллллл лллллл  лллллл лллллл     ");
    _outtext("    лллллл лллллл лллллл лллллл  лллллл лллллл лллллл лллллл  лллллл лллллл     ");
    _outtext("                                                                               ");

    _settextcolor( CYAN );
    _settextposition(45,0);
    sprintf(text,"BAR %2d",barno);
    _outtext(text);

    _settextcolor(BLACK);
    _setbkcolor(CYAN);

    if ( barno == 0 )
    {
        _settextposition(48,06);
        _outtext("COPY");
        _settextposition(48,13);
        _outtext("DEL");
        _settextposition(48,20);
        _outtext("CDIR");
        _settextposition(48,27);
        _outtext("MOVE");
        _settextposition(48,35);
        _outtext("EDIT");
        _settextposition(48,42);
        _outtext("DEV:");
        _settextposition(48,49);
        _outtext("ALL");
        _settextposition(48,56);
        _outtext("NONE");
        _settextposition(48,64);
        _outtext("HELP");
        _settextposition(48,71);
        _outtext("QUIT");
    }
    else
    if ( barno == 1 )
    {
        _settextposition(48,06);
        _outtext("MDIR");
        _settextposition(48,13);
        _outtext("RUN");
        _settextposition(48,20);
        _outtext("REN");
        _settextposition(48,27);
        _outtext("FINF");
        _settextposition(48,35);
        _outtext("HEX");
        _settextposition(48,42);
        _outtext("ALPH");
        _settextposition(48,49);
        _outtext("SIZE");
    }
    else
    if ( barno == 2 )
    {
        _settextposition(48,06);
        _outtext("UNZP");
        _settextposition(48,13);
        _outtext("MKZP");
        _settextposition(48,20);
        _outtext("VWZP");
    }
    else
    if ( barno == 3 )
    {
        _settextposition(48,06);
        _outtext("XLHA");
        _settextposition(48,13);
        _outtext("MLHA");
        _settextposition(48,20);
        _outtext("VLHA");
    }
    else
    if ( barno == 4 )
    {
        _settextposition(48,06);
        _outtext("SEA");
        _settextposition(48,13);
        _outtext("");
        _settextposition(48,20);
        _outtext("");
    }
    else
    if ( barno == 5 )
    {
        _settextposition(48,06);
        _outtext("FT2");
        _settextposition(48,13);
        _outtext("");
        _settextposition(48,20);
        _outtext("");
    }


  return 0;
}


/****************************************************************************/

int     memorisescr(char loadsave)  // 0 = COPY = Read Screen Mem
{                                   // 1 = PASTE = Write Screen Mem

    static  short   screen[8000];
    short    *ptr;
    int     i;


 /*[ Notes ]***************************************************
 **                                                          **
 ** Under DOS/4GW, the first megabyte of physical memory     **
 ** (real-mode memory) is mapped as a shared linear address  **
 ** space. This allows your application to access video RAM  **
 ** using its linear address.  The DOS segment:offset of     **
 ** B800:0000 corresponds to a linear address of B8000.      **
 **                                                          **
 *************************************************************/

    /* Set the pointer to the screen's linear address */

    ptr = (short *)SCREEN_LIN_ADDR;

    for( i = 0; i < SCREEN_SIZE - 1; i++ )
    {
        if (loadsave == COPY )
        {
            screen[i] = *ptr;
        }
        else
        {
            *ptr = screen[i];
        }

        ptr++;
    }

    return 0;

}

/****************************************************************************/

int displayfile( char x, char y, char *text, char fcolor, char bcolor)
{
    _settextposition(x,y);
    _settextcolor(fcolor);
    _setbkcolor(bcolor);
    _outtext(text);

    return 0;
}

/****************************************************************************/

int displaywin(char Offset[2],
               int  Item[2],
               int  CurrentWin,
               char Path[30],
               long Space)
{
	int     n = 0;
    char    text1[40];
    char    text2[40];
    char    fcolor;
    char    bcolor;

   while( (n<39) AND (win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].name[0] != 00) )
   {
        sprintf(text1,"%-12s",win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].name);

        if ( isdir(win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].attr) AND
             ( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].attr & 2 ) )
        {
            sprintf(text2,"           %s %s",
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].date,
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].time);

            strcat(text1,text2);

            if ( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].tagged == 1)
            {
                fcolor = RED;
                bcolor = LIGHTGRAY;
            }
            else
            {
                fcolor = LBLUE;
                bcolor = LIGHTGRAY;
            }              
        }
        else
        if ( isdir(win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].attr))
        {
            sprintf(text2,"           %s %s",
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].date,
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].time);

            strcat(text1,text2);

            if ( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].tagged == 1)
            {
                fcolor = RED;
                bcolor = LIGHTGRAY;
            }
            else
            {
                fcolor = WHITE;
                bcolor = LIGHTGRAY;
            }              
        }
        else
        if (( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].attr & 2 ) OR
            ( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].attr & 4 ))
        {
            sprintf(text2,"%10ld %s %s",
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].size,
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].date,
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].time);
            strcat(text1,text2);

            if ( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].tagged == 1)
            {
                fcolor = CYAN;
                bcolor = LIGHTGRAY;
            }
            else
            {
                fcolor = DARKGRAY;
                bcolor = LIGHTGRAY;
            }
        }
        else
        {
            sprintf(text2,"%10ld %s %s",
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].size,
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].date,
                    win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].time);
            strcat(text1,text2);

            if ( win[CurrentWin][n+( Item[CurrentWin]-Offset[CurrentWin] )].tagged == 1)
            {
                fcolor = CYAN;
                bcolor = LIGHTGRAY;
            }
            else
            {
                fcolor = BLACK;
                bcolor = LIGHTGRAY;
            }
        }

        displayfile(n+2,
                    2 + (CurrentWin * 40),
                    text1,
                    fcolor,
                    bcolor);

       n++;
	}

    _setbkcolor(RED);
    _settextcolor(WHITE);

    _settextposition(42,(CurrentWin * 41));
    sprintf(text1,"%-40s",Path);
    _outtext(text1);

    _setbkcolor(BLACK);
    _settextcolor(YELLOW);

    _settextposition(43,(CurrentWin * 41));
    sprintf(text1,"Free : %ld       ",Space);
    _outtext(text1);

	return 0;
}

/****************************************************************************/

int clearview(char CurrentWin)
{
	int     n = 0;

    _setbkcolor(LIGHTGRAY);
    _settextcolor(WHITE);

    if ( CurrentWin == 3 )        // Clear Screen - for Zortech
    {
        for ( n = 0; n <51; n++ )
        {
            _settextposition(n,0);
            _outtext("                                                                                 ");
        }
    }
    else
    {
        for( n=1; n<40; n++ )
        {
            if ( CurrentWin == 0 )   // Clear left window
            {
                _settextposition(1+n,2);
                _outtext("                                      ");
            }
            else
            if ( CurrentWin == 1 )   // Clear right window
            {
                _settextposition(1+n,42);
                _outtext("                                      ");
            }
            else                     // Clear both windows
            {
                _settextposition(1+n,2);
                _outtext("                                      ");
                _settextposition(1+n,42);
                _outtext("                                      ");
            }
        }
    }

	return 0;

}

/****************************************************************************/

int selectwin(int Whichone)
{

    int n,x,y;

    _setbkcolor(LIGHTGRAY);

    for (n = 0; n < 2; n++)
    {
        if ( Whichone == 0 )
        {
            _settextcolor(WHITE);
            Whichone = 1;
        }
        else
        {
            _settextcolor(BLACK);
            Whichone = 0;
        }

        _settextposition(1,1+(n*40));
        _outtext("Щ"); //_outtext("Щ");
        _settextposition(1,40+(n*40));
        _outtext("Л");
        _settextposition(41,1+(n*40));
        _outtext("Ш");
        _settextposition(41,40+(n*40));
        _outtext("М");

        _settextposition(1,2+(n*40));

        for(x=2; x<40; x++)
        {
            _outtext("Э");
        }

        _settextposition(41,2+(n*40));

        for(x=2; x<40; x++)
        {
            _outtext("Э");
        }

        for(y=2; y<41; y++)
        {
            _settextposition(y,1+(n*40));
            _outtext("К");
            _settextposition(y,40+(n*40));
            _outtext("К");
        }
    }

    _settextcolor(RED);
    _settextposition(1,5);
    _outtext("[dIRECTORY gURU bY MAF]");
    _settextposition(1,60);
    _outtext(VERSION);

    return 0;
}

/****************************************************************************/

int highlight(int Item[2], char CurrentWin, char Offset[2])
{
static  int     olditem[2];   // Used to store previous item win 1 & 2
static  int     oldoffset[2]; // Used to store previous offset win 1 & 2
static  int     oldwin = - 1; // Used to store previous window
char    text[80];             // Text output variable


	if ( oldwin != -1 )
	{
        if (isdir(win[oldwin][olditem[oldwin]].attr) AND
            ( win[oldwin][olditem[oldwin]].attr & 2 ) )
        {
            if ( win[oldwin][olditem[oldwin]].tagged == 1)
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(RED);
            }
            else
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(LBLUE);
            }

            _settextposition(oldoffset[CurrentWin]+2,14 + (CurrentWin * 40));
            _outtext("          ");
        }
        else
        if (isdir(win[oldwin][olditem[oldwin]].attr))
        {
            if ( win[oldwin][olditem[oldwin]].tagged == 1)
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(RED);
            }
            else
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(WHITE);
            }

            _settextposition(oldoffset[CurrentWin]+2,14 + (CurrentWin * 40));
            _outtext("          ");
        }
        else
        if (( win[oldwin][olditem[oldwin]].attr & 2 ) OR // BIT 2 is Hidden
            ( win[oldwin][olditem[oldwin]].attr & 4 ) )  // BIT 4 is System
        {
            if ( win[oldwin][olditem[oldwin]].tagged == 1)
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(CYAN);
            }
            else
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(DARKGRAY);
            }

            _settextposition(oldoffset[CurrentWin]+2,14 + (CurrentWin * 40));
            sprintf(text,"%10ld",win[CurrentWin][olditem[CurrentWin]].size);
            _outtext(text);
        }
        else
        {
            if ( win[oldwin][olditem[oldwin]].tagged == 1)
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(CYAN);
            }
            else
            {
                _setbkcolor(LIGHTGRAY);
                _settextcolor(BLACK);
            }

            _settextposition(oldoffset[CurrentWin]+2,14 + (CurrentWin * 40));
            sprintf(text,"%10ld",win[CurrentWin][olditem[CurrentWin]].size);
            _outtext(text);
        }

        _settextposition(oldoffset[CurrentWin]+2,02 + (CurrentWin * 40));
        sprintf(text,"%-12s",win[CurrentWin][olditem[CurrentWin]].name);
        _outtext(text);
	}

    if (isdir(win[CurrentWin][Item[CurrentWin]].attr) AND
        ( win[CurrentWin][Item[CurrentWin]].attr > 17 ) )
    {
        if ( win[CurrentWin][Item[CurrentWin]].tagged == 1)
        {
            _setbkcolor(CYAN);
            _settextcolor(RED);
        }
        else
        {
            _settextcolor(LBLUE);
            _setbkcolor(CYAN);
        }
        _settextposition(Offset[CurrentWin]+2,02 + (CurrentWin * 40));
        sprintf(text,"%-12s",win[CurrentWin][Item[CurrentWin]].name);
        _outtext(text);
        _settextposition(Offset[CurrentWin]+2,14 + (CurrentWin * 40));
        _outtext("          ");
    }
    else
    if (isdir(win[CurrentWin][Item[CurrentWin]].attr))
    {
        if ( win[CurrentWin][Item[CurrentWin]].tagged == 1)
        {
            _setbkcolor(CYAN);
            _settextcolor(RED);
        }
        else
        {
            _settextcolor(WHITE);
            _setbkcolor(CYAN);
        }
        _settextposition(Offset[CurrentWin]+2,02 + (CurrentWin * 40));
        sprintf(text,"%-12s",win[CurrentWin][Item[CurrentWin]].name);
        _outtext(text);
        _settextposition(Offset[CurrentWin]+2,14 + (CurrentWin * 40));
        _outtext("          ");
    }
    else
    if ((win[CurrentWin][Item[CurrentWin]].attr & 2 ) OR
        (win[CurrentWin][Item[CurrentWin]].attr & 4 ))
    {
        if ( win[CurrentWin][Item[CurrentWin]].tagged == 1)
        {
            _setbkcolor(CYAN);
            _settextcolor(BLUE);
        }
        else
        {
            _settextcolor(DARKGRAY);
            _setbkcolor(CYAN);
        }
        _settextposition(Offset[CurrentWin]+2,02 + (CurrentWin * 40));
        sprintf(text,"%-12s",win[CurrentWin][Item[CurrentWin]].name);
        _outtext(text);
        _settextposition(Offset[CurrentWin]+2,14 + (CurrentWin * 40));
        sprintf(text,"%10ld",win[CurrentWin][Item[CurrentWin]].size);
        _outtext(text);
    }

    else
    {
        if ( win[CurrentWin][Item[CurrentWin]].tagged == 1)
        {
            _setbkcolor(CYAN);
            _settextcolor(BLUE);
        }
        else
        {
            _settextcolor(BLACK);
            _setbkcolor(CYAN);
        }

        _settextposition(Offset[CurrentWin]+2,14 + (CurrentWin * 40));
        sprintf(text,"%10ld",win[CurrentWin][Item[CurrentWin]].size);
        _outtext(text);
        _settextposition(Offset[CurrentWin]+2,02 + (CurrentWin * 40));
        sprintf(text,"%-12s",win[CurrentWin][Item[CurrentWin]].name);
        _outtext(text);

    }

	olditem[CurrentWin] = Item[CurrentWin];  // Set up Static variables for
                                             // the next visit.
	oldwin = CurrentWin;

	oldoffset[CurrentWin] = Offset[CurrentWin];

	return 0;
}


/****************************************************************************/

int dumpinfo(int a[2], int b, int c[2], char d[2], char e)
{
    char    *text1;
    char    *text2;
    char    *text3;
    char    *text4;

    sprintf(text1,"Number files Win0.:%3d  :  Number files win1.:%3d",a[0],a[1]);
    sprintf(text2,"Current Item Win0.:%3d  :  Current Item Win1.:%3d",c[0],c[1]);
    sprintf(text3,"Offset Win 0......:%3d  :  Offset Win 1......:%3d",d[0],d[1]);
    sprintf(text4,"Active Window.....:%3d  :  Last keypress.....:%3d - %c ",b,e,e);

    message("Diagnostics Page",
            text1,
            text2,
            text3,
            text4,
            2,
            40,
            5);

	return 0;
}

/****************************************************************************/

int fileinfo()
{
    char    text0[45];
    char    text1[45];
    char    text2[45];
    char    text3[45];
    char    text4[45];
    char    attrib[7];

    memset( &text0, 0, sizeof( text0 ) );
    memset( &text1, 0, sizeof( text1 ) );
    memset( &text2, 0, sizeof( text2 ) );
    memset( &text3, 0, sizeof( text3 ) );
    memset( &text4, 0, sizeof( text4 ) );
    memset( &attrib, 0, sizeof( attrib ) );

    returnattribs(win[CurrentWin][Item[CurrentWin]].attr,attrib);

    sprintf(text0," %s ",win[CurrentWin][Item[CurrentWin]].name);
    sprintf(text1," Size         : %d ",win[CurrentWin][Item[CurrentWin]].size);
    sprintf(text2," Attributes   : %s ",attrib);
    sprintf(text3," Time & Date  : %s %s ",win[CurrentWin][Item[CurrentWin]].time,win[CurrentWin][Item[CurrentWin]].date);
    sprintf(text4," Path         : %s ",Path[CurrentWin]);

    memorisescr( COPY );

    message(text0,
            text1,
            text2,
            text3,
            text4,
            2,
            20,
            5);

    memorisescr( PASTE );

    return 0;
}

/****************************************************************************/


int report(char *text, int x, int y)
{

    _settextposition(y,x);
    _outtext(text);

    return 0;
}

/****************************************************************************/

int message(char *title1,      // First line of text
            char *title2,      // second
            char *title3,      // etc...
            char *title4,
            char *title5,
            char question,     // message type
            char height,       // vertcal line to appear on
            char lines)        // number of lines of text

/* Message types 0 - Display text, dont pause for anything.
**               1 - Display text, Pause Y/N return TRUE / FALSE
**               2 - Display text wait for any keypress.
**               3 - Display text, Pause Y/N/All/Cancel.
**
** Returns : 0 - No or Any Keypress.
**           1 - Yes
**           2 - All
**           3 - Cancel
*/

{

    int     size = 0; // Length of title
    int     size2 = 0;
    char    a = 1;
    char    width = 00; // width of message box
    char    left = 00;  // Left of message box
    char    right= 00;  // Right of message box
    char    reply = 00; // Reply to question.


    if ( lines > 0 )                // Should put the width of the widest string
        a = strlen( title1 );       // into a

    if ( lines > 1 )
        if ( strlen( title2 ) > a )
            a = strlen( title2 );

    if ( lines > 2 )
        if( strlen( title3 ) > a )
            a = strlen( title3 );

    if( lines > 3 )
        if( strlen( title4 ) > a )
            a = strlen( title4 );

    if( lines > 4 )
        if( strlen( title5 ) > a )
            a = strlen( title5 );

    a++;

    left  = ( 40 - (a / 2) );
    right = ( 40 + (a / 2) );
    width = right - left;

    width = width + 3;
    right++;

    _settextwindow(height,left,height+(1+lines),right);

    _setbkcolor(RED);
    _settextcolor(WHITE);

    _clearscreen( _GWINDOW );

    hline(1,2,width);
    hline(2+lines,2,width);

    vline(lines+2,2,1);
    vline(lines+2,2,width);


    _settextposition(1,1);
    _outtext("Щ");
    _settextposition(1,width);
    _outtext("Л");
    _settextposition(2+lines,1);
    _outtext("Ш");
    _settextposition(height+lines+2,left+width);
//    putc("М",stdout);

    report(title1,2,2);                    // Only print the lines that
    if ( lines >= 2 ) report(title2,2,3);  // exist.
    if ( lines >= 3 ) report(title3,2,4);
    if ( lines >= 4 ) report(title4,2,5);
    if ( lines >= 5 ) report(title5,2,6);

    if ( question == 1)
    {
        FOREVER
        {
            _settextposition( 3,(width  / 2)-4);
            _outtext("[ Y/N? ]");
            reply = getch();

            if ( (reply == 'y') OR (reply == 'Y') )
            {
                question = 1;
                break;
            }
            else
            if ( (reply == 'n') OR (reply == 'N') )
            {
                question = 0;
                break;
            }
        }
    }
    else
    if ( question == 2 )
    {
        _settextposition( 2+lines, (width / 2)-5);
        _outtext("[ ANY KEY ]");
        reply = getch();
        question = 0;
    }
    else
    if ( question == 3 )
    {
        FOREVER
        {
            _settextposition(2+lines, (width / 2) - 6);
            _outtext("[ Y/N/A/C? ]");
            reply = getch();

            if ( (reply == 'y') OR (reply == 'Y') )
            {
                question = 1;
                break;
            }
            else
            if ( (reply == 'n') OR (reply == 'N') )
            {
                question = 0;
                break;
            }
            if ( (reply == 'a') OR (reply == 'A') )
            {
                question = 2;
                break;
            }
            else
            if ( (reply == 'c') OR (reply == 'C') )
            {
                question = 3;
                break;
            }
        }
    }

    _settextwindow(1,1,50,80);

    return question;
}

/****************************************************************************/

int gettext(char *text, char *output, char height)
{
    int n;

    memorisescr(COPY);

    _settextwindow(height,5,height+2,70);
    _setbkcolor(GREEN);
    _settextcolor(WHITE);

    _clearscreen( _GWINDOW );

    _settextposition(1,1);
    _outtext("Щ");
    _settextposition(1,66);
    _outtext("Л");
    _settextposition(3,1);
    _outtext("Ш");
    _settextposition(3,66);
//    cputs('М');

    _settextposition(2,1);
    _outtext("К");

    _settextposition(2,66);
    _outtext("К");

    hline(1,2,67);
    hline(3,2,67);

    report(text,2,2);
    gets(output);

    for(n = 0; n<strlen( output ); n++)
        output[n] = toupper( output[n] );

    _settextwindow(1,1,50,80);

    memorisescr(PASTE);

    return 0;
}

/****************************************************************************/

int hline(char height, char start, char length)
{
    int n;

    length--;

    _settextposition(height,start);

    for ( n = start; n < length; n++)
    {
        _outtext("Э");
    }

    return 0;
}

/****************************************************************************/

int vline(char height, char  start, char accross)
{
    int n;

    for ( n = start; n < height; n++)
    {
        _settextposition(n,accross);
        _outtext("К");
    }

    return 0;
}
