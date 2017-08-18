/*  dg_io.cpp - IO functions.
**
**  Maf - 10.09.96 - Created
**  Maf - 05.12.96 - Added movefile();
**  Maf - 31.03.97 - ported to Watcom v10.6 / DOS4GW
**  Maf - 10.04.97 - Improved filelist code to keep dir types seperate
**
*/

/****************************************************************************/

#include        <dos.h>       // Used for reading Dirs
#include        <string.h>
#include        <stdio.h>     // Used for copyfile();
#include        <conio.h>     // Ditto
#include        <io.h>        // Used for delfile();
#include        <malloc.h>    // Used for malloc & free supprise eh?
#include        <direct.h>    // RMDIR & MKDIR

#include        "dg_io.h"
#include        "dg_scr.h"    
#include        "dg_data.h"   // Static variables
#include        "dg_defs.h"   // Niceties
#include        "dg_misc.h"   // For init

/****************************************************************************/

/*  32 - Archive    ( A )
    16 - Directory
    8  - 
    4  - System     ( S ) 
    2  - Hidden     ( H )
    1  - Read Only  ( R )  
    0  - None Set, Normal File.
*/

int returnattribs(char attr, char *attrstring )
{
    if( attr & 32 ) // Archive
        strcpy(attrstring,"A");
    else
        strcpy(attrstring,"-");

    if( attr & 16 ) // Directory
        strcat(attrstring,"D");
    else
        strcat(attrstring,"-");

    if( attr & 8 ) // ????
        strcat(attrstring,"?");
    else
        strcat(attrstring,"-");

    if( attr & 4 ) // System
        strcat(attrstring,"S");
    else
        strcat(attrstring,"-");

    if( attr & 2 ) // Hidden
        strcat(attrstring,"H");
    else
        strcat(attrstring,"-");

    if( attr & 1 ) // ReadOnly
        strcat(attrstring,"R");
    else
        strcat(attrstring,"-");

    return 0;

}

/****************************************************************************/


int getdrives(char whichwin)
{
    int n = 0;
    int number = 0;
    long dspace[25];

    init( whichwin );

    for( n = 3; n<25; n++ )
    {
        dspace[n] = getdiskspace(n);
        if ( dspace[n] > -1 )
        {
            number++;
            sprintf(win[whichwin][number].name,"%c:",n+64);
            win[whichwin][number].attr = 16;  // DIR
            win[whichwin][number].size = dspace[n];
        }
    }

    Files[ whichwin ] = number;

    return 0;
}
    
/****************************************************************************/


int getfilelist(int whichwin, char Path[30])
{

/*[ notes ]*****************************************************************
 *                                                                         *
 *  _A_NORMAL       0x00    Normal file - read/write permitted             *
 *  _A_RDONLY       0x01    Read-only file                                 *
 *  _A_HIDDEN       0x02    Hidden file                                    *
 *  _A_SYSTEM       0x04    System file                                    *
 *  _A_VOLID        0x08    Volume-ID entry                                *
 *  _A_SUBDIR       0x10    Subdirectory                                   *
 *  _A_ARCH         0x20    Archive file                                   *
 *                                                                         *
 ***************************************************************************/

    char    destination[128];
    int     number = 0;
    int     done;
    struct  find_t find_t;

 // DEBUG    fShowAttrib = TRUE;

    strcpy(destination,Path);
    strcat(destination,"\\*.*");

    done = _dos_findfirst(destination,_A_SUBDIR |
                                      _A_SYSTEM |
                                      _A_HIDDEN, &find_t );  //GRAB ALL
    while (!done) 
    {
       if ( find_t.name[0] != '.' )
       {
            number++;
            if ( number > 1023 )
            {
                memorisescr( COPY );
                message("More than 1024 files in here!",0,0,0,0,2,23,1);
                memorisescr( PASTE );
                
                break;
            }

            strcpy(win[whichwin][number].name,find_t.name);

            if (fShowAttrib)
                win[whichwin][number].size = find_t.attrib;
            else
                win[whichwin][number].size = find_t.size;

            win[whichwin][number].attr = find_t.attrib;
            sprintf(win[whichwin][number].date,"%02d.%02d.%02d",
                    DAY(find_t.wr_date),
                    MONTH(find_t.wr_date),
                    ( (YEAR(find_t.wr_date)) - 1900) );

            sprintf(win[whichwin][number].time,"%02d:%02d",
                    HOUR(find_t.wr_time),
                    MINUTE(find_t.wr_time));
       }
       done = _dos_findnext(&find_t);
    }

	return number;
}

/****************************************************************************/

int delfile( char CurrentWin, char FPath[128], char KillAll)
{
    int     x = 0;
    char    output[256];
    char    display[78];
    char    result = FALSE;
    int     amount = 0;

    if ( KillAll )             // Bullshit to stop compiler moaning.
        KillAll = KillAll;

    memorisescr(COPY);
    result = message("Sure to Maim & Destroy selected files?",0,0,0,0,1,20,1);
    memorisescr(PASTE);

    memorisescr(COPY);

    if ( result )
    {
        while ( ( win[CurrentWin][x].name[0] != 00 ) AND (result == TRUE) )
        {
            x++;
            if ( win[CurrentWin][x].tagged == 1 )
            {
                strcpy(output,FPath);
                strcat(output,"\\");
                strcat(output,win[CurrentWin][x].name);

                if (( win[CurrentWin][x].attr == _A_NORMAL ) OR
                    ( win[CurrentWin][x].attr == _A_ARCH ))
                {
                    amount++;
                    sprintf(display," Killed %d Files ",amount);
                    message(display,0,0,0,0,0,23,1);
                    unlink(output);
                }
                else
                if ( win[CurrentWin][x].attr == _A_RDONLY )
                {
                    //FIGURE OUT HOW TO CHANGE FILE ATTRIBS
                    strcpy(display,output);
                    strcat(display," <- File is read only!");
                    message(display,0,0,0,0,2,26,1);
                }
                else  
                if ( win[CurrentWin][x].attr == _A_SUBDIR )
                {
                    amount++;
                    sprintf(display," Killed %d Files ",amount);
                    message(display,0,0,0,0,0,23,1);
                    rmdir(output);
                }
            }
                                        
        }
    }

    memorisescr(PASTE);

    return 0;
}

/****************************************************************************/

int renamefile( char CurrentWin, char *FPath0, char *FPath1 )
{
	char	text[80];
	char	text2[80];
	char	name[12];
	int	x = 0;

    memorisescr(COPY);

	while ( win[CurrentWin][x].name[0] != 00 )
	{
		x++;
		if ( win[CurrentWin][x].tagged == 1 )
		{
			sprintf(text,"New name for %s is (12 Chars Max):",win[CurrentWin][x].name);
            gettext(text,name,20);

			if ( CurrentWin == 0 )
			{
				strcpy(text,FPath0);
				strcpy(text2,FPath0);
            }
			else
			{
				strcpy(text,FPath1);
				strcpy(text2,FPath1);
			}

            strcat(text,"\\");
            strcat(text2,"\\");
			strcat(text,win[CurrentWin][x].name);
			strcat(text2,name);

			if ( (rename(text,text2)) != 0)
                message("Rename Failed Misserably!",0,0,0,0,2,23,1);
		}
	}

    memorisescr(PASTE);

	return 0;
}

/****************************************************************************/

int movefile( char CurrentWin, char *FPath0, char *FPath1 )
{

    int     x = 0;
    int     Ret = 0;
    char    outputf[256];   // Output from.
    char    outputt[256];   // Output to.
    char    text[79];
    char    foverwrite = 0; // Overwrite flag starts FALSE.
    char    result;


    memorisescr(COPY);
    result = message("Sure to Move selected files?",0,0,0,0,1,20,1);
    memorisescr(PASTE);

    memorisescr(COPY);

    if ( result )
    {
        while ( win[CurrentWin][x].name[0] != 00 )
        {
            x++;
            if ( win[CurrentWin][x].tagged == 1 )
            {
                if (CurrentWin == 0)
                {
                    strcpy(outputf,FPath0);
                    strcat(outputf,"\\");
                    strcat(outputf,win[0][x].name);
                    strcpy(outputt,FPath1);
                    strcat(outputt,"\\");
                    strcat(outputt,win[0][x].name);
                }
                else
                {
                    strcpy(outputf,FPath1);
                    strcat(outputf,"\\");
                    strcat(outputf,win[1][x].name);
                    strcpy(outputt,FPath0);
                    strcat(outputt,"\\");
                    strcat(outputt,win[1][x].name);
                }

                sprintf(text,"Moving...");
                memorisescr(COPY);
                message(text,outputf,outputt,0,0,0,20,3);

                Ret = duplicate(outputf,outputt,foverwrite,win[CurrentWin][x].size);

                if ( Ret == 0 ) // Sucsess
                    unlink(outputf);
                else
                if ( Ret == 3 ) // Check if file exists.
                {
                    sprintf(text,"File %s already exists, overwrite?",outputt);
                    Ret = message(text,0,0,0,0,3,23,1);
                    if ( Ret == 1 )
                    {
                        Ret = duplicate(outputf,outputt,1,win[CurrentWin][x].size);
                        if ( Ret == 0 )
                            unlink(outputf);
                    }
                    else
                    if ( Ret == 2 )
                    {
                        foverwrite = TRUE;
                        Ret = duplicate(outputf,outputt,foverwrite,win[CurrentWin][x].size);
                        if ( Ret == 0 )
                            unlink(outputf);
                    }
                    else
                    if ( Ret == 3 )
                    {
                        message("Opperation Cancelled",0,0,0,0,2,20,1);
                        return 1;
                    }
                }
                else
                if ( Ret == 4 ) //  No memory for copy buffer
                {
                    message("WARNING : No memory for copy buffer",0,0,0,0,2,23,1);
                }

                memorisescr(PASTE);

            }
        }
    }

    return 0;
}

/****************************************************************************/

int copyfile( char CurrentWin, char *FPath0, char *FPath1 )
{

    int     x = 0;
    int     Ret = 0;
    char    outputf[256];   // Output from.
    char    outputt[256];   // Output to.
    char    *text;
    char    foverwrite = 0; // Overwrite flag starts FALSE.
    int     number = 0;
    int     current = 0;

    while ( win[CurrentWin][x].name[0] != 00 )
    {
        x++;
        if ( win[CurrentWin][x].tagged == 1 )
            number++;
    }
     
    x = 0;

    while ( win[CurrentWin][x].name[0] != 00 )
    {
        x++;
        if ( win[CurrentWin][x].tagged == 1 )
        {
            current++;

            if (CurrentWin == 0)
            {
                strcpy(outputf,FPath0);
                strcat(outputf,"\\");
                strcat(outputf,win[0][x].name);
                strcpy(outputt,FPath1);
                strcat(outputt,"\\");
                strcat(outputt,win[0][x].name);
            }
            else
            {
                strcpy(outputf,FPath1);
                strcat(outputf,"\\");
                strcat(outputf,win[1][x].name);
                strcpy(outputt,FPath0);
                strcat(outputt,"\\");
                strcat(outputt,win[1][x].name);
            }

            sprintf(text,"File %d of %d",current,number);
            memorisescr(COPY);
            message("Copying Files, Please wait...",text,outputf,outputt,0,0,20,3);

            Ret = duplicate(outputf,outputt,foverwrite,win[CurrentWin][x].size);

            if ( Ret == 3 ) // Check if file exists.
            {
                sprintf(text,"File %s already exists, overwrite?",outputt);
                Ret = message(text,0,0,0,0,3,23,1);
                if ( Ret == 1 )
                {
                    Ret = duplicate(outputf,outputt,1,win[CurrentWin][x].size);
                }
                else
                if ( Ret == 2 )
                {
                    foverwrite = TRUE;
                    Ret = duplicate(outputf,outputt,foverwrite,win[CurrentWin][x].size);

                }
                else
                if ( Ret == 3 )
                {
                    message("Opperation Cancelled",0,0,0,0,2,20,1);
                    return 1;
                }
            }
            else
            if ( Ret == 4 ) //  No memory for copy buffer
            {
                message("WARNING : No memory for copy buffer",0,0,0,0,2,23,1);
            }

            memorisescr(PASTE);
        }
    }

    return 0;
}

/****************************************************************************/

int duplicate( char  *From, char *To, char fOverwrite, long Size )

{
    FILE    *FFrom;
    FILE    *FTo;
    int     n;
    int     cbBuffer = 32766;
    void    *pBuffer;
    char    text[256];
    long    much = 0;

    FOREVER  // Check Heap mem for Copy Buffer
    {

        pBuffer = malloc(cbBuffer);

        if ( pBuffer OR (cbBuffer < 1024) )
        {
            break;
        }
        else
        {
            cbBuffer = cbBuffer / 2;
        }
    }

    if (!pBuffer)
        return 4;                    // No memory for buffer.

//    sprintf(text,"Allocated %d size copy buffer",cbBuffer);
//    message(text,2,10);

	if (!fOverwrite)
	{
		FTo = fopen( To, "rb" );

		if (FTo)
		{
			fclose( FTo );
            free( pBuffer );
            return 3;            // File exists.
		}
	}
	
	FFrom = fopen( From, "rb" );

    if (!FFrom)
    {
        free( pBuffer );
		  return 1;                // Can't open source
    }

    FTo = fopen( To, "wb" );

    if (!FTo)
    {
        fclose( FFrom );
        free( pBuffer );
        return 2;                    // Cant open target
    }

//    sprintf(text,"performing %s %s",To,From);
//    message(text,0,0,0,0,2,10,1);


    FOREVER
	{
        n = fread( pBuffer, 1, cbBuffer, FFrom);
        if (!n) break;

        much = much + n;
        sprintf(text,"%ld of %ld",much,Size);
        message(text,0,0,0,0,0,28,1);

        if (!fwrite( pBuffer, 1, n, FTo )) break;

	}
	
	fclose( FFrom );
	fclose( FTo );

    free ( pBuffer );

    return 0;
}

/****************************************************************************/

int makeadir(char *DirName, char *Path)
{
    char    text[1024];
    int     output = 0;

    strcpy(text,Path);
    strcat(text,"\\");
    strcat(text,DirName);

    output = mkdir(text);

    return output;
}

/****************************************************************************/

long getdiskspace(char drive)
{

    struct  diskfree_t free;
    long    avail;
    char    text[79];

    if (_dos_getdiskfree(drive, &free) != 0)
    {
        memorisescr( COPY );
        message("Error in _dos_getdiskfree() call",0,0,0,0,2,20,1);
        memorisescr( PASTE );
        return -1;
    }

    avail = (long) free.avail_clusters
        * (long) free.bytes_per_sector
        * (long) free.sectors_per_cluster;

    return avail;

}

/****************************************************************************/
