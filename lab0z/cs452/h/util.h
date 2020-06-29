/* external definition of assembly service routines */

/* the address of each function should be loaded in the 
   corresponding location in the EVT */

/*       function name 			location in EVT        type*/

extern int STLDMM();			/*   0x008            mm trap   */
extern int STLDADDRESS();		/*   0x00c            prog trap */
extern int STLDILLEGAL();		/*   0x010            prog trap */
extern int STLDZERO();			/*   0x014            prog trap */
extern int STLDPRIVILEGE();		/*   0x020            prog trap */
extern int STLDSYS();			/*   0x08c            sys trap  */
extern int STLDSYS9();			/*   0x94             sys trap  */
extern int STLDSYS10();			/*   0x98             sys trap  */
extern int STLDSYS11();			/*   0x9c             sys trap  */
extern int STLDSYS12();			/*   0xa0             sys trap  */
extern int STLDSYS13();			/*   0xa4             sys trap  */
extern int STLDSYS14();			/*   0xa8             sys trap  */
extern int STLDSYS15();			/*   0xac             sys trap  */
extern int STLDSYS16();			/*   0xb0             sys trap  */
extern int STLDSYS17();			/*   0xb4             sys trap  */
extern int STLDTERM0();			/*   0x100            interrupt */
extern int STLDTERM1();			/*   0x104            interrupt */
extern int STLDTERM2();			/*   0x108            interrupt */
extern int STLDTERM3();			/*   0x10c            interrupt */
extern int STLDTERM4();			/*   0x110            interrupt */
extern int STLDPRINT0();		/*   0x114            interrupt */
extern int STLDDISK0();			/*   0x11c            interrupt */
extern int STLDFLOPPY0();		/*   0x12c            interrupt */
extern int STLDCLOCK();			/*   0x140            interrupt */
