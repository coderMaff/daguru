/*****************************************************************************
**                                                                          **
**                            Mouse v1.00                                   **
**                          Mouse interface                                 **
**                     Watcom C/C++ v10.6 - DOS4GW                          **
**                                                                          **
*****************************************************************************/

/*[ history ]*****************************************************************
**                                                                          **
** maf - 23.03.97 - Created from Watcom C/C++ Example                       **
**                                                                          **
*****************************************************************************/

/*[ includes ]***************************************************************/

#include <stdio.h>
#include <dos.h>
#include <i86.h>

/*[ bitmap ]*****************************************************************/

unsigned short cursor[] =
{
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,

    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000,
    0x0001, 0x0200, 0x0000, 0x0000
};

/*[ globals ]****************************************************************/

extern int mouse_event = 0;
extern int right_button = 0;
extern int mouse_code = 0;
extern int mouse_cx = 0;
extern int mouse_dx = 0;

struct SREGS sregs;
union REGS inregs, outregs;

int installed = 0;
int orig_mode = 0;
unsigned short far *ptr;

#pragma off (check_stack)
void ( _loadds far *function_ptr)(int max, int mcx, int mdx);
#pragma on (check_stack)

/*[ void _loadds far click_handler (int max, int mcx, int mdx) ]*************/

#pragma off (check_stack)

void _loadds far click_handler (int max, int mcx, int mdx)
{

#pragma aux click_handler parm [EAX] [ECX] [EDX]

    mouse_event = 1;
    mouse_code = max;
    mouse_cx = mcx;
    mouse_dx = mdx;
    if( mouse_code & 8 ) right_button = 1;
}

#pragma on (check_stack)


/*[ int mouse_install() ]*****************************************************
**                                                                          **
**   Descr  :   initializes mouse                                           **
**                                                                          **
**   In     :   none                                                        **
**                                                                          **
**   Out    :   0 = sucessfull                                              **
**              1 = failed                                                  **
**                                                                          **
*****************************************************************************/

int mouse_install()
{

    /* check for mouse driver */

    inregs.w.ax = 0;
    int386 (0x33, &inregs, &outregs);

    installed = (outregs.w.ax == -1); /* Dont seem to work proper like */

    if( 1 )                           /* So carry on regardless!       */
    {

        /* show mouse cursor */

        inregs.w.ax = 0x1;
        int386( 0x33, &inregs, &outregs );

        /* set mouse cursor form */

        inregs.w.ax = 0x9;
        inregs.w.bx = 0x0;
        inregs.w.cx = 0x0;
        ptr = cursor;
        inregs.x.edx = FP_OFF( ptr );

        sregs.es     = FP_SEG( ptr );
        int386x( 0x33, &inregs, &outregs, &sregs );

        /* install click watcher */

        inregs.w.ax = 0xC;
        inregs.w.cx = 0x0002 + 0x0008;
//        function_ptr = click_handler;
        inregs.x.edx = FP_OFF( function_ptr );
        sregs.es     = FP_SEG( function_ptr );
        int386x( 0x33, &inregs, &outregs, &sregs );
    }

    return 0;
}

/*[ int mouse_remove() ]******************************************************
**                                                                          **
**   Descr  :   removes mouse                                               **
**                                                                          **
**   In     :   none                                                        **
**                                                                          **
**   Out    :   0 = sucessfull                                              **
**              1 = failed                                                  **
**                                                                          **
*****************************************************************************/

int mouse_remove()
{
    /* check installation again (to clear watcher) */

    inregs.w.ax = 0;

    int386( 0x33, &inregs, &outregs );


    if( outregs.w.ax == -1 )
        printf( "DONE : Mouse still installed...\n" );
    else
        printf( "DONE : Mouse NOT installed...\n" );

    inregs.h.ah = 0x00;
    inregs.h.al = orig_mode;
    int386( 0x10, &inregs, &outregs );

    return 0;
}

/*[ int mouse_limit() ]*******************************************************
**                                                                          **
**   Descr  :   removes mouse                                               **
**                                                                          **
**   In     :   x1,y1 = topleft   x2,y2 = bottom right                      **
**                                                                          **
**   Out    :   0 = sucessfull                                              **
**              1 = failed                                                  **
**                                                                          **
*****************************************************************************/

int mouse_limit(int x1, int y1, int x2, int y2)
{

    /* Set mouse boundry */

    inregs.w.ax = 0x7;
    inregs.w.cx = x1;
    inregs.w.dx = x2;
    int386( 0x33, &inregs, &outregs );

    inregs.w.ax = 0x8;
    inregs.w.cx = y1;
    inregs.w.dx = y2;
    int386( 0x33, &inregs, &outregs );

    return 0;
}

