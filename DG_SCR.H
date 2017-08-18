/* dg_scr.h - Screen Output Functions
**
** Maf - 10.09.96 - Created
** maf - 31.03.97 - moved to watcom/DOS4GW dropped zortech/borland support
*/

/****************************************************************************/

extern int pressanykey(void);                               /* Press any key messag*/
extern int drawscreen(void);                                /* Draw Screen         */
extern int filenumber(void);                                /* Which file of homany*/
extern int buttonbar(char barno);                           /* Draw Buttonbar      */
extern int memorisescr(char loadsave);                      /* Grab screen mem     */
extern int displayfile( char x, char y, char *text, char fcolor, char bcolor);
extern int displaywin(char a[2],int b[2],int,char c[30], long Space);  /* Display window one  */
extern int clearview(char CurrentWin);                      /* Clears out the view */
extern int selectwin(int Whichwin);                         /* Highlights window   */
extern int highlight(int d[2],char,char e[2]);              /* Highlight file      */
extern int dumpinfo(int f[2],int,int g[2],char h[2],char);  /* Diagnostics Info    */
extern int fileinfo();                                      /* File information    */
extern int report(char text[256], int x, int y);            /* Splat text at x,y   */
extern int message(char *title1,
                   char *title2,
                   char *title3,
                   char *title4,
                   char *title5,
                   char question,
                   char height,
                   char lines);                             /* ask / inform user   */
extern int gettext(char *text, char *output, char height);  /* Get a text string   */
extern int hline(char height, char start, char length);     /* Draw horizontal line*/
extern int vline(char height, char  start, char accross);   /* Draw vertical line  */


/****************************************************************************/
