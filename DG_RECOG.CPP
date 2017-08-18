/* dg_recog.cpp Recognise file types
**
** Maf - 09.10.96 - Created
** Des - 10.10.96 - Added monsta extractnum() routine.
** Maf - 30.03.97 - ported to Watcom v10.6
*/

#include    <string.h>      // For strcpy / cat etc..
#include    <dos.h>         // 
#include    <stdio.h>       //
#include    <conio.h>
#include    <stdlib.h>      // for atol() in extractnum()

#include    "dg_defs.h"     // My niceties
#include    "dg_recog.h"    // Recog header file.
#include    "dg_scr.h"      // to output debug info.

/****************************************************************************/


/* This bugger willl extract an int from a string contain an ASCII number in
**   any base from binary to base 36
**
*/


int extractnum(char *S, char Len, char radix)


{
    int     unit = 1, t = 0, i=0;
    char    c;

    for (i = Len-1; i >= 0; i--)
    {
        c = S[i];

        if (c >= 'a') c = (c - 'a') + 10; else
        if (c >= 'A') c = (c - 'A') + 10; else
                     c = c - '0';

        t = t + (c * unit);


        unit = unit * radix;
    }

    return t;

}

/****************************************************************************/

int recognise_file( char *path, char *filename )
{

    typedef struct
    {
        char        title[22];
        char        offset[6];
        char        gap1;
        char        datasize[1];
        char        gap2;
        char        data[8];
        char        gap3;
        char        button[2]; 
        char        crap[2];
    } recognise_type;

    recognise_type  recog;
    FILE            *recogf;
    FILE            *targetf;
    long            offset = 0;
    char            datasize = 4;
    char            targetname[256];
    unsigned char   recogdata[4], targetdata[4];
    char            button = -2;
    char            text[79];
    char            result = 1;
    int             n = 0;

    strcpy( targetname,path);
    strcat( targetname,"\\");
    strcat( targetname,filename);

//    message(targetname,1);

    recogf = fopen("dg_recog.dat","r");
    targetf = fopen(targetname,"r");

    if ( recogf AND targetf )
    {
//        message("Successfully opened both files",1);

        while ( fgets( (char*)&recog, sizeof( recog ), recogf ))
        {

//            sprintf(text,"recog : %s ",recog.title );
//            result = message(text,1);

            if ( result == 0 )
                break;

            offset = extractnum( recog.offset, 6, 16);

            datasize = extractnum( recog.datasize, 1, 10);

//            sprintf(text,"Offset = %ld - Datasize = %d",offset,datasize);
//            message(text,1);

            for (n = 0; n < datasize; n++ )
            {
                recogdata[n] = extractnum( &recog.data[n*2], 2, 16);
            }

//            sprintf(text,"recog data %d %d %d %d",recogdata[0],recogdata[1],recogdata[2],recogdata[3]);
//            message(text,1);
 
            fseek( targetf, offset, SEEK_SET );
            fread( targetdata, datasize, 1, targetf );

//            sprintf(text,"target data %d %d %d %d",targetdata[0],targetdata[1],targetdata[2],targetdata[3]);
//            message(text,1);


            if ( memcmp( recogdata, targetdata, datasize ) == 0)
            {
                button = extractnum( recog.button, 2, 10 );
//                sprintf(text,"Match - Launching Command %d", button );
//                message(text,1);
                break;
            }
//            message( "no match", 1 );
            return 9;   // Display help if cant id file
        }
    }

    if ( targetf ) fclose( targetf );
    if ( recogf ) fclose( recogf );

    return button;
}

/****************************************************************************/


        
