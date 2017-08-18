/* dg_misc.c miscellaneous program functions
**
** Maf - 10.09.96 - Created
** Des - 18.09.96 - Restuctured main loop
**                  Changed variables to 2 position arrays
** Maf - 05.10.96 - Seperated menu loop from functions
**                  Implamented button bars
** Maf - 31.03.97 - ported to watcom v10.6 & DOS4GW
** Maf - 10.04.97 - Improved code to keep different dir types seperate
** Maf - 16.04.97 - Removed all Occurances of direct output to dg_scr.cpp
**
*/

/*[ includes ]**************************************************************/

#include    <string.h>
#include    <stdio.h>    // for getch(); ZTC   Weird eh?
#include    <conio.h>    // for getch(); BTC
#include    <process.h>  // Used to run other progs
#include    <dos.h>      // For keypress thing
#include    <graph.h>    // Watcom gfx routines
#include    <bios.h>     // TEMP FOR DEBUG

#include    "mouse.h"

#include    "dg_scr.h"   // My screendrawing routines
#include    "dg_io.h"    // My IO functions
#include    "dg_misc.h"  // MIsc functionss
#include    "dg_data.h"  // Static variables
#include    "dg_defs.h"  // Defines
#include    "dg_recog.h" // Recognise file types
#include    "dg_help.h"  // Online help manual

/****************************************************************************/

int init(char Window)
{
	int n = 0;

    for (n = 0; n<1024; n++)
	{
        win[Window][n].name[0]=00;
        win[Window][n].size = 0;
        win[Window][n].tagged = 0;
	}

    strcpy(win[Window][0].name,"<..>");
    win[Window][0].attr = _A_SUBDIR;

	return 0;
}

/****************************************************************************/

int isdir(char attr)
{
    if ( attr & 16 )  // BIT 16 set if DIR
        return TRUE;
    else
        return FALSE;
}

/****************************************************************************/

int isfile(char attr)
{
    if ( attr & 16 )  // BIT 16 set if DIR
        return FALSE;
    else
        return TRUE;
}

/****************************************************************************/

int sortlistalpha(char window)
{
    int     n = 0;
    int     result = 0;
    char    attr;           // File or Directory
    char    name[13];       // File / Dir Name
    long    size;           // File / Dir Size
    char    tagged;         // Selected Y / N
    int     change = FALSE; // Change ?


    do
    {
        change = FALSE;

        for(n = 2; n < ( Files[window] + 1 ); n++)
        {
            if ( isdir( win[window][n-1].attr ) AND
                 isfile( win[window][n].attr) )
                result = 0;
            else 
            if  ( ( isfile( win[window][n-1].attr) AND
                  isdir( win[window][n].attr) ) )
                result = 1;
            else
                result = strcmp(win[window][n-1].name,win[window][n].name);

            if ( result > 0 )
            {
                change = TRUE;

                attr = win[window][n-1].attr;   // Move old data out
                size   = win[window][n-1].size;
                tagged = win[window][n-1].tagged;
                strcpy(name,win[window][n-1].name);

                win[window][n-1].attr = win[window][n].attr;     // Bung new data in
                win[window][n-1].size = win[window][n].size;
                win[window][n-1].tagged = win[window][n].tagged;
                strcpy(win[window][n-1].name,win[window][n].name);

                win[window][n].attr = attr;    // Put temp data back below
                win[window][n].size = size;
                win[window][n].tagged = tagged;
                strcpy(win[window][n].name,name);

                // SORTID!
           }

        }
    }while ( change == TRUE );

    displaywin(Offset,Item,window,Path[window],Space[window]);
    highlight(Item,window,Offset);

    return 0;
}

/****************************************************************************/

int sortlistsize(char window)
{
    int     n = 0;
    char    changes = 0;
    char    attr;         // File or Directory
    char    name[13];       // File / Dir Name
    long    size;           // File / Dir Size
    char    tagged;         // Selected Y / N


    do
    {
        changes = 0;

        for(n = 2; n < ( Files[window] + 1); n++)
        {

            if ( win[window][n].size > win[window][n-1].size )
            {
                changes = n;

                attr = win[window][n-1].attr;   // Move old data out
                size   = win[window][n-1].size;
                tagged = win[window][n-1].tagged;
                strcpy(name,win[window][n-1].name);

                win[window][n-1].attr = win[window][n].attr;     // Bung new data in
                win[window][n-1].size = win[window][n].size;
                win[window][n-1].tagged = win[window][n].tagged;
                strcpy(win[window][n-1].name,win[window][n].name);

                win[window][n].attr = attr;    // Put temp data back below
                win[window][n].size = size;
                win[window][n].tagged = tagged;
                strcpy(win[window][n].name,name);

                // SORTID!
           }
        }

    }while ( changes != 0 );

    displaywin(Offset,Item,window,Path[window],Space[window]);
    highlight(Item,window,Offset);

    return 0;
}

/****************************************************************************/

int getkey()
{
    union   REGS    regsin, regsout;

    regsin.h.ah = 7;
    intdos(&regsin, &regsout);

    return (int) regsout.h.al;
}

/****************************************************************************/

int menu(void)
{

int     i;            // Last keypressed
int     n;
char    quit = 0;     // Quit out?

    bbar = 0;

    memset( Path, 0, sizeof( Path ) );
    strcpy(Path[0],"C:");
    strcpy(Path[1],"D:");

    Files[0] = getfilelist(0,Path[0]);
    Files[1] = getfilelist(1,Path[1]);

    Space[0] = getdiskspace(Path[0][0] - 64);
    Space[1] = getdiskspace(Path[1][0] - 64);

	Offset[0] = 0;
	Offset[1] = 0;

	Item[0] = 0;
	Item[1] = 0;

    sortlistalpha(1);

    CurrentWin = 0;     // Must be zero, because this is also set
                        // in dg_scr.cpp/highlight();
    sortlistalpha(0);

    buttonbar(bbar);

    filenumber();

    do
	{
        i = getkey();

        if ( i == 0 )
        {
            i = getkey();
            i = i + 256;
        }

        switch ( i )
        {
            case SPACE:
                do_Space();
                break;

            case UP:
                do_Up();
                break;

            case RIGHT:
                do_Right();
                break;

            case LEFT:
                do_Left();
                break;

            case DOWN:
                do_Down();
                break;

            case RETURN:
                n = do_Return();      // If dir change if not do recog
                break;

            case DEL:
                do_FKey(2);
                break;

            case ESC:
                quit = do_FKey(0);
                break;

            case KBD_a:
                do_FKey(7);        // All shortcut
                break;

            case KBD_c:
                do_FKey(1);        // Copy shortcut
                break;

            case KBD_d:
                do_FKey(2);        // Delete shortcut
                break;

            case KBD_e:
                do_FKey(5);        // Edit shortcut
                break;

            case KBD_f:             // file info shortcut
                do_FKey(14);
                break;

            case KBD_h:
                do_FKey(9);        // Help shortcut
                break;

            case KBD_m:
                do_FKey(4);        // Move shortcut
                break;

            case KBD_n:
                do_FKey(8);        // None shortcut
                break;

            case KBD_r:
                do_FKey(13);       // Rename shortcut
                break;

            case KBD_q:
                quit = do_FKey(0);        // Quit shortcut
                break;

            case KBD_v:             // Path (Volume) shortcut
                do_FKey(3);
                break;

            case KBD_C:
                do_FKey(7);        // Copy All shortcut
                do_FKey(1);
                break;

            case KBD_D:
                do_FKey(7);        // Delete All shortcut
                do_FKey(2);
                break;

            case KBD_M:
                do_FKey(7);        // Move All shortcut
                do_FKey(4);
                break;

            case MINUS:
                if ( bbar > 0 )
                {
                    bbar--;
                    buttonbar( bbar );
                }
                break;

            case PLUS:
            case EQUAL:
                if ( bbar < 5 )    // Next button bar
                {
                    bbar++;
                    buttonbar( bbar );
                }
                break;

            case PGUP:
                do_PGUP();
                break;

            case PGDN:
                do_PGDN();
                break;

            case F1:
            case F2:
            case F3:
            case F4:
            case F5:
            case F6:
            case F7:
            case F8:
            case F9:
            case F10:
            case KBD_0:
            case KBD_1:
            case KBD_2:
            case KBD_3:
            case KBD_4:
            case KBD_5:
            case KBD_6:
            case KBD_7:
            case KBD_8:
            case KBD_9:
                quit = do_FKey((i-48) + ( bbar*10 ));
                break;

        }

//        sprintf(text,"Keycode  DEC:%d CHAR:%c HEX:%x",i,i,i);
//        message(text,0,0,0,0,0,1,1);


    } while ( quit == FALSE );

    message("        DaGuru (Beta Test Version) (c)1997 Matthew Bushell",
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
            " Bug reports to,  mjw17@leicester.ac.uk  please mark the subject as",
            " 'DAGURU' Also send suggestions, comments, etc.... ",0,2,10,4);

    _settextposition(50,1);
    _settextcursor( 0x0607 );

    return 0;
}

/****************************************************************************/

int do_Space(void)
{

    if (( win[CurrentWin][Item[CurrentWin]].tagged == FALSE) AND
        ( win[CurrentWin][Item[CurrentWin]].attr != FA_PARENT )) // Dont tag parent dir
        win[CurrentWin][Item[CurrentWin]].tagged = TRUE;
    else
        win[CurrentWin][Item[CurrentWin]].tagged = FALSE;

    highlight(Item,CurrentWin,Offset);

    return 0;
}

/****************************************************************************/

int do_Return(void)
{

int     n;
char    text[79];

    if ( Item[CurrentWin] == 0 )                      // Parent Dir Selected
    {
        for (n = strlen(Path[CurrentWin]); n > 0; n--)
        {
            if (Path[CurrentWin][n] == '\\')
            {
                Path[CurrentWin][n] = 00;
                break;
            }
        }

        init(CurrentWin);
        Files[CurrentWin] = getfilelist(CurrentWin,Path[CurrentWin]);
        Offset[CurrentWin] = 0;
        Item[CurrentWin] = 0;
        clearview(CurrentWin);
        sortlistalpha(CurrentWin);
    }
    else
    if (isdir(win[CurrentWin][Item[CurrentWin]].attr))
    {
        strcat(Path[CurrentWin],"\\");
        strcat(Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        init(CurrentWin);
        Files[CurrentWin] = getfilelist(CurrentWin,Path[CurrentWin]);
        Offset[CurrentWin] = 0;
        Item[CurrentWin] = 0;
        clearview(CurrentWin);
        sortlistalpha(CurrentWin);
    }
    else
    {
        n = recognise_file(Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);

        if (n == -1)
        {
            sprintf(text,"Error opening file %s",n,win[CurrentWin][Item[CurrentWin]].name);
            memorisescr(COPY);
            message(text,0,0,0,0,1,20,1);
            memorisescr(PASTE);
        }
        else
        if (n == -2)
        {
            sprintf(text,"Unknown filetype for file %s",win[CurrentWin][Item[CurrentWin]].name);
            memorisescr(COPY);
            message(text,0,0,0,0,1,20,1);
            memorisescr(PASTE);
        }

        return n;  // Return info back to button bar
    }

    filenumber();

    return  -1;
}

/****************************************************************************/

int do_Up(void)
{
    if ( Item[CurrentWin] > 0 )
    {
        if ( Offset[CurrentWin] > 0 )
        {
            Item[CurrentWin]--;
            Offset[CurrentWin]--;
            highlight(Item,CurrentWin,Offset);
        }
        else
        {
            Item[CurrentWin]--;
            displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
            highlight(Item,CurrentWin,Offset);
        }

        filenumber();
    }

    return 0;

}

/****************************************************************************/

int do_Down(void)
{
    if ( Item[CurrentWin] < Files[CurrentWin] )
    {
        if ( Offset[CurrentWin] < 38 )
        {
            Item[CurrentWin]++;
            Offset[CurrentWin]++;
            highlight(Item,CurrentWin,Offset);
        }
        else
        {
            Item[CurrentWin]++;
            displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
            highlight(Item,CurrentWin,Offset);
        }

        filenumber();
    }

    return 0;
}

/****************************************************************************/

int do_PGUP(void)
{
    if ( Item[CurrentWin] > 38 )
    {
        Item[CurrentWin] = Item[CurrentWin] - 38;
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
        filenumber();
    }
    else
    {
        Item[CurrentWin] = 0;
        Offset[CurrentWin] = 0;
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
        filenumber();
    }

    return 0;
}

/****************************************************************************/

int do_PGDN(void)
{
    if ( ( Item[CurrentWin] + 38 ) < Files[CurrentWin] )
    {
        Item[CurrentWin] = Item[CurrentWin] + 38;
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
        filenumber();
    }
    else
    {
        Item[CurrentWin] = Files[CurrentWin];
        if ( Item[CurrentWin] > 38 )
            Offset[CurrentWin] = 38;
        else
            Offset[CurrentWin] = Item[CurrentWin];
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
        filenumber();
    }

    return 0;
}


/****************************************************************************/

int do_Right(void)
{
    CurrentWin = 1;
    selectwin(CurrentWin);
    highlight(Item,CurrentWin,Offset);

    return 0;
}

/****************************************************************************/

int do_Left(void)
{
    CurrentWin = 0;
    selectwin(CurrentWin);
    highlight(Item,CurrentWin,Offset);

    return 0;
}

/****************************************************************************/

int do_FKey(int Button)
{

char    result;
char    *text;
long    n;
long    time1,time2;

    if ( Button == 0 )  // BB0,F0 = Quit
    {
        memorisescr(COPY);
        result = message("Are you sure you wanna quit?",0,0,0,0,1,10,1);
        memorisescr(PASTE);
        return result;
    }
    else
    if ( Button == 1 )  // BB0,F1 = Copy
    {
        copyfile(CurrentWin,Path[0],Path[1]);

        if ( CurrentWin == 0 )
        {
            Files[1] = getfilelist(1,Path[1]);
            Offset[1] = 0;
            Item[1] = 0;
            Space[0] = getdiskspace(Path[0][0] - 64);
            sortlistalpha(1);
            sortlistalpha(0);
        }
        else
        {
            Files[0] = getfilelist(0,Path[0]);
            Offset[0] = 0;
            Item[0] = 0;
            Space[1] = getdiskspace(Path[1][0] - 64);
            sortlistalpha(0);
            sortlistalpha(1);
        }
    }
    else
    if ( Button == 2 )  // BB0,F2 = Delete
    {
        delfile(CurrentWin,Path[CurrentWin],FALSE);
        init(CurrentWin);

        Space[CurrentWin] = getdiskspace(Path[CurrentWin][0] - 64);
        Files[CurrentWin] = getfilelist(CurrentWin,Path[CurrentWin]);

        Offset[CurrentWin] = 0;
        Item[CurrentWin] = 0;

        clearview(CurrentWin);

        sortlistalpha(CurrentWin);
    }
    else
    if ( Button == 3 )  // BB0,F3 = Path
    {
        gettext("Path:",Path[CurrentWin],20);

        init(CurrentWin);

        Files[CurrentWin] = getfilelist(CurrentWin,Path[CurrentWin]);
        Space[CurrentWin] = getdiskspace(Path[CurrentWin][0] - 64);            //

        Offset[CurrentWin] = 0;
        Item[CurrentWin] = 0;

        clearview(CurrentWin);

        sortlistalpha(CurrentWin);

        filenumber();
    }
    else
    if ( Button == 4 )  // BB0,F4 = Move
    {
        movefile(CurrentWin,Path[0],Path[1]);

        init(0);
        init(1);

        Files[0] = getfilelist(0,Path[0]);    // Re-read file lists
        Files[1] = getfilelist(1,Path[1]);
        Space[0] = getdiskspace(Path[0][0] - 64);            // Renab file space
        Space[1] = getdiskspace(Path[1][0] - 64);

        Item[CurrentWin] = 0;
        Offset[CurrentWin] = 0;

        clearview(CurrentWin);                // Clear both windows
        sortlistalpha(0);
        sortlistalpha(1);

    }
    else
    if ( Button == 5 )  // BB0,F5 = Edit
    {
        sprintf(text,"Edit %s\\%s",Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        system(text);
    }
    else
    if ( Button == 6 )  // BB0,F6 = Devices
    {
        getdrives( CurrentWin );
        Offset[CurrentWin] = 0;
        Item[CurrentWin] = 0;
        clearview(CurrentWin);
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
    }
    else
    if ( Button == 7 )  // BB0,F7 = All
    {
        for (n = 1; n < Files[CurrentWin]+1; n++ )
        {
            win[CurrentWin][n].tagged = 1;
        }
        clearview(CurrentWin);
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
    }
    else
    if ( Button == 8 )  // BB0,F8 = None
    {
        for (n = 1; n < Files[CurrentWin]+1; n++ )
        {
            win[CurrentWin][n].tagged = 0;
        }
        clearview(CurrentWin);
        displaywin(Offset,Item,CurrentWin,Path[CurrentWin],Space[CurrentWin]);
        highlight(Item,CurrentWin,Offset);
    }
    else
    if ( Button == 9 )  // BB0,F9 = Help
    {
        help();
    }
    else
    if ( Button == 11 ) // BB1,F1 = Makedir
    {
        char    DirName[12];
        char    FullName[1024];
        char    result = 0;

        gettext("Enter new dircory name:",DirName,30);
        init(CurrentWin);
        result = makeadir(DirName,Path[CurrentWin]);

        if ( result != 0  )
        {
            memorisescr(COPY);
            message("Create Directory failed",0,0,0,0,2,23,1);
            memorisescr(PASTE);
        }

        Files[CurrentWin] = getfilelist(CurrentWin,Path[CurrentWin]);
        Offset[CurrentWin] = 0;
        Item[CurrentWin] = 0;
        clearview(CurrentWin);
        sortlistalpha(CurrentWin);
    }
    else
    if ( Button == 12 ) // BB1,F2 = RUN
    {
        sprintf(text,"%s\\%s",Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        _clearscreen(BLACK);
        system(text);

        drawscreen();
        clearview(6);

        displaywin(Offset,Item,0,Path[0],Space[0]);
        displaywin(Offset,Item,1,Path[1],Space[1]);

        highlight(Item,CurrentWin,Offset);
        buttonbar(bbar);
    }
    else
    if ( Button == 13 ) // BB1,F3 = Rename
    {
        renamefile(CurrentWin,Path[0],Path[1]);
        Files[CurrentWin] = getfilelist(CurrentWin,Path[CurrentWin]);
        Offset[CurrentWin] = 0;
    	Item[CurrentWin] = 0;
	    clearview(CurrentWin);
        sortlistalpha(CurrentWin);
    }
    else
    if ( Button == 14 )  // BB1,F4 = Fileinfo
    {
        fileinfo();
    }
    else
    if ( Button == 15 )  // BB1,F5 = Hex Edit
    {
        sprintf(text,"Hex %s\\%s",Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        system(text);
    }
    else
    if ( Button == 16 )  // BB1,F6 = Sort list alphabetically
    {
        sortlistalpha(CurrentWin);
    }
    else
    if ( Button == 17 )
    {
        sortlistsize(CurrentWin);  // BB1,F7 = Sort list by size
    }
    else
    if ( Button == 19 )  // Do speed test by doodling the screen
    {
        _bios_timeofday( _TIME_GETCLOCK, &time1 );
        memorisescr( COPY );

        for (n=0; n<500000; n++)
            printf("#");

        memorisescr( PASTE );
        _bios_timeofday( _TIME_GETCLOCK, &time2 );

        memorisescr( COPY );
        sprintf(text,"Time to display = Start %lu - Stop %lu = %lu ",time1,time2,time2-time1);
        message(text,0,0,0,0,2,10,1);
        memorisescr( PASTE );
    }
    else
    if ( Button == 21 ) // BB2,F1 = Pkunzip highlighted file
    {
        if ( CurrentWin == 0 )
        {
            sprintf(text,"pkunzip -d %s\\%s %s",Path[0],win[CurrentWin][Item[CurrentWin]].name,Path[1]);
        }
        else
        {
            sprintf(text,"pkunzip -d %s\\%s %s",Path[1],win[CurrentWin][Item[CurrentWin]].name,Path[0]);
        }

        _clearscreen(0);
        system(text);

        pressanykey();

        drawscreen();
        clearview(66);

        Files[0] = getfilelist(0,Path[0]);
        Files[1] = getfilelist(1,Path[1]);

        Offset[0] = 0;
        Offset[1] = 0;

        Item[0] = 0;
        Item[1] = 0;

        Space[1] = getdiskspace(Path[1][0] - 64);
        Space[0] = getdiskspace(Path[0][0] - 64);

        sortlistalpha(0);
        sortlistalpha(1);

        buttonbar(bbar);
    }
    else
    if ( Button == 22 ) // BB2,F2 = Pkzip all currentwin
    {
        char    arcname[8];
        char    setrp = 0;

        gettext("Archive Name:",arcname,30);

        for (n = 1; n < Files[CurrentWin]; n++)
        {
                if (isdir(win[1][CurrentWin].attr))
                {
                    setrp = message("Directory Encountered, use -rP ?",0,0,0,0,1,20,1);
                    break;
                }
        }

        if ( CurrentWin == 0 )
        {
            if (setrp == 0)
                sprintf(text,"pkzip %s\\%s %s\\%s",Path[1],arcname,Path[0],"*.*");
            else
                sprintf(text,"pkzip -rP %s\\%s %s\\%s",Path[1],arcname,Path[0],"*.*");
        }
        else
        {
            if (setrp == 0)
                sprintf(text,"pkzip %s\\%s %s\\%s",Path[0],arcname,Path[1],"*.*");
            else
                sprintf(text,"pkzip -rP %s\\%s %s\\%s",Path[0],arcname,Path[1],"*.*");
        }

        _clearscreen(BLACK);
        system(text);

        pressanykey();

        drawscreen();
        clearview(66);

        Files[0] = getfilelist(0,Path[0]);
        Files[1] = getfilelist(1,Path[1]);

        Offset[0] = 0;
        Offset[1] = 0;

        Item[0] = 0;
        Item[1] = 0;

        Space[1] = getdiskspace(Path[1][0] - 64);
        Space[0] = getdiskspace(Path[0][0] - 64);

        sortlistalpha(0);
        sortlistalpha(1);

        buttonbar(bbar);
    }
    else
    if ( Button == 23 ) // BB2,F3 = View PKZIP file
    {
        sprintf(text,"pkunzip -v %s\\%s|more",Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        _clearscreen(BLACK);
        system(text);

        pressanykey();

        drawscreen();
        clearview(66);

        displaywin(Offset,Item,0,Path[0],Space[0]);
        displaywin(Offset,Item,1,Path[1],Space[1]);

        highlight(Item,0,Offset);
        highlight(Item,1,Offset);

        buttonbar(bbar);
    }
    else
    if ( Button == 41 ) // BB4,F1 = View picture
    {
        sprintf(text,"c:\\utils\\sea\\sea.exe %s\\%s",Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        _clearscreen(BLACK);
        system(text);

        pressanykey();

        drawscreen();
        clearview(66);

        displaywin(Offset,Item,0,Path[0],Space[0]);
        displaywin(Offset,Item,1,Path[1],Space[1]);

        highlight(Item,0,Offset);
        highlight(Item,1,Offset);

        buttonbar(bbar);
    }
    else
    if ( Button == 51 ) // BB5,F1 = Play MOD
    {
        sprintf(text,"c:\\utils\\Track\\ft2.exe %s\\%s",Path[CurrentWin],win[CurrentWin][Item[CurrentWin]].name);
        _clearscreen(BLACK);
        system(text);

        pressanykey();

        drawscreen();
        clearview(66);

        displaywin(Offset,Item,0,Path[0],Space[0]);
        displaywin(Offset,Item,1,Path[1],Space[1]);

        highlight(Item,0,Offset);
        highlight(Item,1,Offset);

        buttonbar(bbar);
    }



    return 0;
}

/****************************************************************************/
