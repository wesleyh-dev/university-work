
#include "Sim.h"

#include "basic.h"

void Four_Phase_Clock(const SD &coord,
                      const Signal &Reset, const Signal &Clk,
                      const Signal &Phase);
void ALU16(const SD &coord,
           const Signal &a, const Signal &b, const Signal &c, // c[1] c[0]
           const Signal &s, const Signal &N, const Signal &Z);
void SHIFTER16(const SD &coord,
               const Signal &a, const Signal &c, // c[1] c[0]
               const Signal &s);
void MIR( const SD &coord,
          const Signal & Addr /* 6 bits */,
          const Signal & mir  /* 32 bits */
        );

#include "datapath.h"

void simnet()
{

   Sig(addr,6);            // Address signals

   Sig(AMux, 1);           // Micro instruction signals
   Sig(CC, 2);
   Sig(ALU, 2);
   Sig(Shf, 2);
   Sig(MBR, 1);
   Sig(MAR, 1);
   Sig(RD, 1);
   Sig(WR, 1);
   Sig(EnC, 1);
   Sig(C, 4);
   Sig(B, 4);
   Sig(A, 4);
   Sig(Addr, 8);


   Sig( Reset, 1 );
   Sig( Clk, 1 );

   SigV(mbr, 16, 0b1100110011001100);  // SigV() = Sig() with initialization
   Sig( Phase, 4 );


   /* =====================================================================
      4 phase clock
      ===================================================================== */
   Switch( "jz", Reset, 'b', One);              // Controls for 4 phase clock
   Switch( "kz", Clk,   'a', Zero );   

   Four_Phase_Clock("kw-ky", Reset, Clk, Phase );

   Probe("ju", Phase[0]);
   Probe("jv", Phase[1]);
   Probe("jw", Phase[2]);
   Probe("jx", Phase[3]);


   /* =====================================================================
      MIR
      ===================================================================== */
   Switch( "ht", addr[5], '5', Zero );     // Controls to select instr in MIR
   Switch( "ht", addr[4], '4', Zero );   
   Switch( "ht", addr[3], '3', Zero );   
   Switch( "it", addr[2], '2', Zero );   
   Switch( "it", addr[1], '1', Zero );   
   Switch( "it", addr[0], '0', Zero );   

   MIR( "hu-iy", addr,
         (AMux, CC, ALU, Shf, MBR, MAR, RD, WR, EnC, C, B, A, Addr) );

   ProbeH("as", AMux);
   ProbeH("at-au", CC);
   ProbeH("av-aw", ALU);
   ProbeH("ax-ay", Shf);
   ProbeH("bv", MBR);
   ProbeH("bw", MAR);
   ProbeH("bx", RD);
   ProbeH("by", WR);
   ProbeH("ct", EnC);
   ProbeH("cv-cy", C);

   ProbeH("dv-dy", B);
   ProbeH("ev-ey", A);
   ProbeH("fs-fz", Addr);



   Sig( R0, 16 ); Sig( R1, 16 ); Sig( R2, 16 ); Sig( R3, 16 );
   Sig( R4, 16 ); Sig( R5, 16 ); Sig( R6, 16 ); Sig( R7, 16 );
   Sig( R8, 16 ); Sig( R9, 16 ); Sig( R10, 16 ); Sig( R11, 16 );
   Sig( R12, 16 ); Sig( R13, 16 ); Sig( R14, 16 ); Sig( R15, 16 );
   Sig( ABus, 16 ); Sig( BBus, 16 ); 
   Sig( A_Latch, 16 ); Sig( B_Latch, 16 ); 
   Sig( Amux, 16 );  // AMux is taken
   Sig( Alu, 16 );   // ALU  is taken
   Sig( C_bus, 16 ); // C    is taken
   Sig(N,1);
   Sig(Z,1);

   DataPath("ih-kk", 
      (AMux, CC, ALU, Shf, MBR, MAR, RD, WR, EnC, C, B, A, Addr),
      mbr, Phase, Reset,
      R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
      ABus, BBus, A_Latch, B_Latch, Amux, Alu, C_bus, N, Z);


   ProbeH("aa-ah", R0);
   ProbeH("ba-bh", R1);
   ProbeH("ca-ch", R2);
   ProbeH("da-dh", R3);
   ProbeH("ea-eh", R4);
   ProbeH("fa-fh", R5);
   ProbeH("ga-gh", R6);
   ProbeH("ha-hh", R7);

   ProbeH("aj-aq", R8);
   ProbeH("bj-bq", R9);
   ProbeH("cj-cq", R10);
   ProbeH("dj-dq", R11);
   ProbeH("ej-eq", R12);
   ProbeH("fj-fq", R13);
   ProbeH("gj-gq", R14);
   ProbeH("hj-hq", R15);


   ProbeH("lk-lr", ABus);
   ProbeH("lt-l{", BBus);

   ProbeH("ok-or", A_Latch);
   ProbeH("ot-o{", B_Latch);

   ProbeH("qa-qh", mbr);
   ProbeH("qk-qr", Amux);

   ProbeH("so-sv", Alu);
   ProbeH("sm", N);
   ProbeH("sm", Z);

   ProbeH("uo-uv", C_bus);
}





