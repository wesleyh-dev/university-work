
#include "Sim.h"
#include "basic.h"

#include "clock.h"            // Student must provide this file


void simnet()
{
   Signal Clk, Reset, Branch;
   Sig(Phase, 4);
   Sig(MPC, 8);
   Sig(a, 8);

/* ======================================
 * Reset and Clk tick switches
 * ====================================== */
   Switch("ea", Clk,   '0', Zero);
   Switch("ba", Reset, '1', Zero);
   Switch("ca", Branch, 'a', Zero);

   Switch("cc", a[7], '2', Zero);
   Switch("cd", a[6], '3', Zero);
   Switch("ce", a[5], '4', Zero);
   Switch("cf", a[4], '5', Zero);
   Switch("cg", a[3], '6', One);
   Switch("ch", a[2], '7', Zero);
   Switch("ci", a[1], '8', One);
   Switch("cj", a[0], '9', Zero);

/* -------------------------------
   Place the components
   ------------------------------- */
   MPC_With_Reset("cb-eb", Reset, Clk, Branch, a, MPC);

   Probe("ec", MPC[7]);
   Probe("ed", MPC[6]);
   Probe("ee", MPC[5]);
   Probe("ef", MPC[4]);
   Probe("eg", MPC[3]);
   Probe("eh", MPC[2]);
   Probe("ei", MPC[1]);
   Probe("ej", MPC[0]);
}

