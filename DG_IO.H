/* dg_io.h - IO functions
**
**  Maf - 10.09.96 - Created
**  Maf - 05.12.96 - Added movefile();
**
**
*/

/*[ Prototypes ]*************************************************************/

extern int returnattribs(char attr, char *attrstring );                   /* Get file atributes        */
extern int getdrives( char whichwin );                                    /* Gets Available Drives     */
extern int getfilelist(int, char c[30]);                                  /* Gets the dirlisting       */
extern int delfile( char CurrentWin, char *FPath, char KillAll );         /* Deletes a file            */
extern int renamefile( char CurrentWin, char *FPath0, char *FPath1 );     /* Renames a file            */
extern int movefile( char CurrentWin, char *FPath0, char *FPath1 );       /* Copies a file             */
extern int copyfile( char CurrentWin, char *FPath0, char *FPath1 );       /* Copies a file             */
extern int duplicate( char  *From, char *To, char fOverwrite, long Size );/* IO - Copies a file        */
extern int makeadir( char *DirName, char *Path);                          /* Make a directory called x */
extern long getdiskspace(char drive);                                     /* Get drive space           */

/****************************************************************************/
