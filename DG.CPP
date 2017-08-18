/* DGuru v1.o1
**
** 29.08.96 - maf - created
** 01.04.97 - maf - moved to watcom c/c++ v10.6 ( DOS4GW )
**
**        Sort by SIZE dosnt work if there are no DIRS in the list.
**        Critical error handling.
**        Change zip, to only do selected files
**        add mouse routines
**        display todays date
**        bring up device list
**        Do help screen
**        do config file, buttons
**        Writting to video memory is very slooooow
**        in dg_misc.cpp add a parameter whichwin to sortlist alpha 1,2,3
*/

/*[ Includes ]***************************************************************/

#include        "dg_scr.h"  // All my screen drawing functions
#include        "dg_misc.h" // Miscallanious (Bad spealing functions! ;)

/****************************************************************************/

int main()        
{
    int n;

    init(0);              // In dg_misc.h
    init(1);

	drawscreen();
    clearview(66);
	menu();

	return 0;
}

