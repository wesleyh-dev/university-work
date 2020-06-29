#include "Sim.h"
#include "basic.h"


void simnet()
{
  Sig(set,1);  Sig(reset,1);        // Special control signals
  Sig(D,1);    Sig(Q,1);            // D = Dff input, Q = Dff output
  Sig(clock,1);                     // Clock signal

  Switch( "aa", D,  '0', Zero );   

  Switch( "fa", set,   'b', Zero );   
  Switch( "ga", reset, 'a', Zero );   
  Switch( "ia", clock, 'z', Zero );   

  /* ============================================================
 *        reset (a) = 1 ==> force Dff output to 0
 *               set (b)   = 1 ==> force Dff output to 1
 *
 *                      reset = 0 AND set = 0 ==> Operate like a normal Dff
 *
 *                                  D = data in
 *                                              Clock = write signal for Dff
 *                                                          Q = output of Dff
 *                                                               ============================================================ */
  Dff ( "ab-hb", (set,D,clock,reset), Q );  // D-flipflop

  Probe ( "ac", Q );              // Probe output
}
