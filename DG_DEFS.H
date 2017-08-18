/*  dg_defs.h - Usefull definitions.
**
**  Maf - 20.09.96 - Created + Nicked some from Des`s GEN.h
**  Maf - 10.10.96 - Moved Zortech Compiler Def`s in here
**  Maf - 06.12.96 - Finished off all keyboard defs.
**  Maf - 31.03.97 - Moved to Watcom v10.6 - DOS4GW
**                   removed all zortech & borland support 
**  
*/

#define VERSION "[1.01 25.06.97]"

// Video memory stuff for DOS4GW

#define SCREEN_AREA 0xb800
#define SCREEN_LIN_ADDR ((SCREEN_AREA) << 4)
#define SCREEN_SIZE 80*50

// Arrow Keys

#define LEFT    331
#define RIGHT   333
#define UP      328
#define DOWN    336

// Keyboard

#define KBD_a    97
#define KBD_c    99
#define KBD_d   100
#define KBD_e   101
#define KBD_f   102
#define KBD_g   103
#define KBD_h   104
#define KBD_i   105
#define KBD_j   106
#define KBD_k   107
#define KBD_l   108
#define KBD_m   109
#define KBD_n   110
#define KBD_o   111
#define KBD_p   112
#define KBD_q   113
#define KBD_r   114
#define KBD_s   115
#define KBD_t   116
#define KBD_u   117
#define KBD_v   118
#define KBD_w   119
#define KBD_x   120
#define KBD_y   121
#define KBD_z   122

#define KBD_A    65
#define KBD_B    66
#define KBD_C    67
#define KBD_D    68
#define KBD_E    69
#define KBD_F    70
#define KBD_G    71
#define KBD_H    72
#define KBD_I    73
#define KBD_J    74
#define KBD_K    75
#define KBD_L    76
#define KBD_M    77
#define KBD_N    78
#define KBD_O    79
#define KBD_P    80
#define KBD_Q    81
#define KBD_R    82
#define KBD_S    83
#define KBD_T    84
#define KBD_U    85
#define KBD_V    86
#define KBD_W    87
#define KBD_X    88
#define KBD_Y    89
#define KBD_Z    90

#define KBD_0    48
#define KBD_1    49
#define KBD_2    50
#define KBD_3    51
#define KBD_4    52
#define KBD_5    53 
#define KBD_6    54
#define KBD_7    55
#define KBD_8    56
#define KBD_9    57

#define SPACE   32
#define RETURN  13
#define DEL     339
#define ESC     27
#define PGUP    329
#define PGDN    337
#define MINUS   45
#define PLUS    43
#define EQUAL   61

#define F1              315
#define F2              316
#define F3              317
#define F4              318
#define F5              319
#define F6              320
#define F7              321
#define F8              322
#define F9              323
#define F10             324

// Loops

#define FOREVER         for(;;)
#define repeat          do
#define until(x)        while(!(x))

// Booleans

#define FALSE   0
#define TRUE    1

#define COPY    0       // For memorisescr();
#define PASTE   1

#define AND     &&
#define OR      ||

/*[ dirs ]*******************************************************************/

#define BACKDIR    ".."
#define SAMEDIR    "."

/*[ attribs ]****************************************************************/

#define FA_PARENT   -1

/*[ Colors ]*****************************************************************/

// Default 16 Color Palette Colors.

#define BLACK       0x00
#define BLUE        0x01
#define GREEN       0x02
#define CYAN        0x03
#define RED         0x04
#define MOVE        0x05
#define TOPE        0x06
#define LIGHTGRAY   0x07
#define DARKGRAY    0x08
#define VOILET      0x09
#define LGREEN      0x0a
#define LBLUE       0x0b
#define OCRE        0x0c
#define PUCE        0x0d
#define YELLOW      0x0e
#define WHITE       0x0f

#define DESKTOP 0x07
#define BRIGHT  0x0f
#define DARK    0x00

/*[ time macros ]************************************************************/

#define YEAR(t)   (((t & 0xFE00) >> 9) + 1980)
#define MONTH(t)  ((t & 0x01E0) >> 5)
#define DAY(t)    (t & 0x001F)
#define HOUR(t)   ((t & 0xF800) >> 11)
#define MINUTE(t) ((t & 0x07E0) >> 5)
#define SECOND(t) ((t & 0x001F) << 1)

/*[ End ]********************************************************************/
