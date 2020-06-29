
#include "Sim.h"
#include "basic.h"

#include "alu-shf.h"            // Student must provide this file


void simnet()
{
   Sig(a, 16);
   Sig(s, 16);
   Sig(c1, 1);
   Sig(c0, 1);

   Switch("aC", a[15], 'a', Zero);
   Switch("aD", a[14], 'b', Zero);
   Switch("aE", a[13], 'c', Zero);
   Switch("aF", a[12], 'd', Zero);
   Switch("aG", a[11], 'e', Zero);
   Switch("aH", a[10], 'f', Zero);
   Switch("aI", a[9], 'g', Zero);
   Switch("aJ", a[8], 'h', Zero);
   Switch("aK", a[7], 'i', Zero);
   Switch("aL", a[6], 'j', Zero);
   Switch("aM", a[5], 'k', Zero);
   Switch("aN", a[4], 'l', Zero);
   Switch("aO", a[3], 'm', Zero);
   Switch("aP", a[2], 'n', Zero);
   Switch("aQ", a[1], 'o', Zero);
   Switch("aR", a[0], 'p', Zero);


   Switch("dA-dB", c1, '8', Zero);
   Switch("dC-dD", c0, '9', Zero);
   
   SHIFTER16("bC-bR", a, (c1, c0), s);
   
   ProbeH("cC-cR", s);
}
