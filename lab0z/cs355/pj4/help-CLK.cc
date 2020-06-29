#include "Sim.h"
#include "basic.h"

#include "clock.h"            // Student must provide this file


void simnet()
{
   Signal Clk, Reset;
   Sig(Phase, 4);

/* ======================================
 * Reset and Clk tick switches
 * ====================================== */
   Switch("aa", Clk,   '0', Zero);
   Switch("aa", Reset, '1', Zero);

/* -------------------------------
   Place the components
   ------------------------------- */
   Four_Phase_Clock("ab", Reset, Clk, Phase);

   Probe("ac", Phase[0]);
   Probe("ad", Phase[1]);
   Probe("ae", Phase[2]);
   Probe("af", Phase[3]);
}


