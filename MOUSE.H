/* mouse.h - Mouse v1.00 - mouse interface for watcom C/C++ v10.6 ( dos4gw )
**
** maf - 23.03.97 - created
*/ 

extern int mouse_event = 0;
extern int right_button = 0;
extern int mouse_code = 0;
extern int mouse_cx = 0;
extern int mouse_dx = 0;

void _loadds far click_handler(int max, int mcx, int mdx); /* mouse Handler */
extern int _cdecl mouse_install();                                /* install mouse */
extern int _cdecl mouse_remove();                                 /* remove mouse  */
extern int _cdecl mouse_limit(int x1,int y1,int x2,int y2);       /* limit mouse   */
