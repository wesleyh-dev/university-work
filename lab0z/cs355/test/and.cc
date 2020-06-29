/* =====================================================================
 * Always include this header file when writing digital simulation !!!
 * ===================================================================== */
#include "Sim.h"

void simnet()
{
  Sig(sw0,1);           // Define sw0 as a signal
  Sig(sw1,1);           // Define sw1 as a signal
  Sig(out,1);           // Define out as a signal

  Switch ( "aa", sw0, 'a', Zero );  // Location = "aa", name = sw0, key = 'a'
  Switch ( "ca", sw1, 'b', One );   // Initial value: Zero or One

  And  ( "bb", (sw0,sw1), out );    // And: inputs = (sw0,sw1), output = out

  // Note:  ( .., .. )   group signals into 1 signal
  // Note:  sw0 is equivalent to sw0[0]

  Probe ( "bc", out );              // Probe out
}
