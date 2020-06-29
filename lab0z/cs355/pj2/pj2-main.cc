

/* =============================================================
   Project 2: the circuit testing main program

   Do NOT make any changes to this file.
   You must provide a circuit in the file "pj2.h" to complete
   this project
   ============================================================= */


#include "Sim.h"
#include "basic.h"      // Include it so you can access my provided components

#include "pj2.h"   	// YOU must write this program


void simnet()
{
   Sig(x, 4);          // defines signals x[3] x[2] x[1] x[0]
   Sig(y, 4);          // defines signals y[3] y[2] y[1] y[0]
   Sig(m, 8);          // defines signals m[7] m[6] ... m[1] m[0]

   /* Put up first input number */
   Switch("aa", x[3], '0', Zero);
   Switch("ab", x[2], '1', Zero);
   Switch("ac", x[1], '2', Zero);
   Switch("ad", x[0], '3', Zero);

   /* Put up second input number */
   Switch("af", y[3], '4', Zero);
   Switch("ag", y[2], '5', Zero);
   Switch("ah", y[1], '6', Zero);
   Switch("ai", y[0], '7', Zero);

   Multiply4("cc-cf", (x[3],x[2],x[1],x[0]),
		 (y[3],y[2],y[1],y[0]), 
	(m[7],m[6],m[5],m[4],m[3],m[2],m[1],m[0]));

   ProbeH( "eb-ei", m);
}



