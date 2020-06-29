
#include "Sim.h"
#include "basic.h"

#include "alu-shf.h"            // Student must provide this file


void simnet()
{
   Sig(a, 16);
   Sig(b, 16);
   Sig(s, 16);
   Sig(c1, 1);
   Sig(c0, 1);
   Sig(N, 1);
   Sig(Z, 1);

   Switch("aA", a[15], 'a', Zero);
   Switch("aB", a[14], 'b', Zero);
   Switch("aC", a[13], 'c', Zero);
   Switch("aD", a[12], 'd', Zero);
   Switch("aE", a[11], 'e', Zero);
   Switch("aF", a[10], 'f', Zero);
   Switch("aG", a[9], 'g', Zero);
   Switch("aH", a[8], 'h', Zero);
   Switch("aI", a[7], 'i', Zero);
   Switch("aJ", a[6], 'j', Zero);
   Switch("aK", a[5], 'k', Zero);
   Switch("aL", a[4], 'l', Zero);
   Switch("aM", a[3], 'm', Zero);
   Switch("aN", a[2], 'n', Zero);
   Switch("aO", a[1], 'o', Zero);
   Switch("aP", a[0], 'p', Zero);


   Switch("aT", b[15], 'q', Zero);
   Switch("aU", b[14], 'r', Zero);
   Switch("aV", b[13], 's', Zero);
   Switch("aW", b[12], 't', Zero);
   Switch("aX", b[11], 'u', Zero);
   Switch("aY", b[10], 'v', Zero);
   Switch("aZ", b[9], 'w', Zero);
   Switch("a[", b[8], 'x', Zero);
   Switch("a\\", b[7], 'y', Zero);
   Switch("a]", b[6], 'z', Zero);
   Switch("a^", b[5], '1', Zero);
   Switch("a_", b[4], '2', Zero);
   Switch("a`", b[3], '3', Zero);
   Switch("aa", b[2], '4', Zero);
   Switch("ab", b[1], '5', Zero);
   Switch("ac", b[0], '6', Zero);


   Switch("dA-dB", c1, '8', Zero);
   Switch("dC-dD", c0, '9', Zero);
   
   ALU16("bC-bc", a, b, (c1, c0), s, N, Z);
   
   ProbeH("cC-cc", s);
   
   ProbeH("bC-bD", N);
   ProbeH("bC-bD", Z);
}
