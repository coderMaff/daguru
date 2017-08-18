/* dg_misc.h - Misc functions
**
** Maf - 10.09.96 - Created
**
**
*/

/*[ Prototypes ]*************************************************************/

extern int init(char Window);         /* Initialize Structutes */
int isdir(char attr);                 /* Is this a directory?  */
int isfile(char attr);                /* Is this a file?       */
int sortlistalpha(char window);       /* Sort file list alpha  */
int sortlistsize(char window);        /* Sort file list size   */
int getkey(void);                     /* Get keypress          */
extern int menu(void);                /* Menu                  */
                                      
int do_Space(void);                   /* Space pressed         */
int do_Return(void);                  /* Return pressed        */
int do_Up(void);                      /* Up Arrow Key          */
int do_Down(void);                    /* Down Arrow Key        */
int do_PGUP(void);                    /* Page Up Key           */
int do_PGDN(void);                    /* Page Down Key         */
int do_Left(void);                    /* Left Arrow Key        */
int do_Right(void);                   /* Right Arrow Key       */
int do_FKey(int Button);              /* Fkey Pressed          */

/****************************************************************************/
