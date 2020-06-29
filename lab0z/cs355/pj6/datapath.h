
/* ========================================================================
   datapath.h: skeleton project file for CS355

   Use this file to write the DataPath circuit component of Pj6
   ======================================================================== */
void D_Latch(const SD &coord,
               const Signal &D, const Signal &Clk, const Signal &q0)
{
   Module( coord, "D-latch", (D, Clk), q0);  

   Signal n_D, set, reset, n_q0;

   Not( SD(coord,"bc"), D, n_D);
   And( SD(coord,"ad"), (D, Clk),   set);
   And( SD(coord,"bd"), (n_D, Clk), reset);

   Nor( SD(coord,"ae"), (set,  q0),   n_q0);
   Nor( SD(coord,"be"), (reset,n_q0), q0);
}

void DataPath( const SD &coord,
	       const Signal &mir, const Signal &mbr, 
	       const Signal &Phase, const Signal &Reset, 
	       const Signal &R0, const Signal &R1, const Signal &R2, 
	       const Signal &R3, const Signal &R4, const Signal &R5, 
	       const Signal &R6, const Signal &R7, const Signal &R8, 
	       const Signal &R9, const Signal &R10, const Signal &R11, 
	       const Signal &R12, const Signal &R13, const Signal &R14, 
	       const Signal &R15, const Signal &ABus, const Signal &BBus, 
	       const Signal &ALatch, const Signal &BLatch,
	       const Signal &AMux, const Signal &ALU, const Signal &CBus, 
	       const Signal &N, const Signal &Z 
              )
{
   Module( coord, "DataPath", 
           (mir, mbr, Phase, Reset), 
           (R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, 
	    R10, R11, R12, R13, R14, R15, 
	    ABus, BBus, ALatch, BLatch, AMux, ALU, CBus, N, Z)
         );

   /* ----------------------------------------------------------------------
      Write your DataPath ciruit here
      Make sure you use a component CompName as follows:

            CompName( SD(coord,"aa"),  ...., .... );
                      ^^^^^^^^^^^^^^
                      Use this coordinate for EVERY component (keep "aa")
      ---------------------------------------------------------------------- */

Sig(andC,1);
Sig(decC,16);
And( SD(coord,"aa"), (mir[20], Phase[3]), andC);
Decoder( SD(coord,"aa"), andC, (mir[19]-mir[16]), (decC[15]-decC[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[15], (R15[15]-R15[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[14], (R14[15]-R14[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[13], (R13[15]-R13[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[12], (R12[15]-R12[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[11], (R11[15]-R11[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[10], (R10[15]-R10[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[9], (R9[15]-R9[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[8], (R8[15]-R8[0]) );
Constant16( SD(coord,"aa"), -1, (R7[15]-R7[0]) );
Constant16( SD(coord,"aa"), 1, (R6[15]-R6[0]) );
Constant16( SD(coord,"aa"), 0, (R5[15]-R5[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[4], (R4[15]-R4[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[3], (R3[15]-R3[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[2], (R2[15]-R2[0]) );
Reg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[1], (R1[15]-R1[0]) );
ResetableReg16( SD(coord,"aa"), (CBus[15]-CBus[0]), decC[0], Reset, (R0[15]-R0[0]) );


Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[15],R14[15],R13[15],R12[15],R11[15],R10[15],R9[15],R8[15],R7[15],R6[15],R5[15],R4[15],R3[15],R2[15],R1[15],R0[15]), ABus[15]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[14],R14[14],R13[14],R12[14],R11[14],R10[14],R9[14],R8[14],R7[14],R6[14],R5[14],R4[14],R3[14],R2[14],R1[14],R0[14]), ABus[14]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[13],R14[13],R13[13],R12[13],R11[13],R10[13],R9[13],R8[13],R7[13],R6[13],R5[13],R4[13],R3[13],R2[13],R1[13],R0[13]), ABus[13]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[12],R14[12],R13[12],R12[12],R11[12],R10[12],R9[12],R8[12],R7[12],R6[12],R5[12],R4[12],R3[12],R2[12],R1[12],R0[12]), ABus[12]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[11],R14[11],R13[11],R12[11],R11[11],R10[11],R9[11],R8[11],R7[11],R6[11],R5[11],R4[11],R3[11],R2[11],R1[11],R0[11]), ABus[11]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[10],R14[10],R13[10],R12[10],R11[10],R10[10],R9[10],R8[10],R7[10],R6[10],R5[10],R4[10],R3[10],R2[10],R1[10],R0[10]), ABus[10]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[9],R14[9],R13[9],R12[9],R11[9],R10[9],R9[9],R8[9],R7[9],R6[9],R5[9],R4[9],R3[9],R2[9],R1[9],R0[9]), ABus[9]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[8],R14[8],R13[8],R12[8],R11[8],R10[8],R9[8],R8[8],R7[8],R6[8],R5[8],R4[8],R3[8],R2[8],R1[8],R0[8]), ABus[8]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[7],R14[7],R13[7],R12[7],R11[7],R10[7],R9[7],R8[7],R7[7],R6[7],R5[7],R4[7],R3[7],R2[7],R1[7],R0[7]), ABus[7]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[6],R14[6],R13[6],R12[6],R11[6],R10[6],R9[6],R8[6],R7[6],R6[6],R5[6],R4[6],R3[6],R2[6],R1[6],R0[6]), ABus[6]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[5],R14[5],R13[5],R12[5],R11[5],R10[5],R9[5],R8[5],R7[5],R6[5],R5[5],R4[5],R3[5],R2[5],R1[5],R0[5]), ABus[5]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[4],R14[4],R13[4],R12[4],R11[4],R10[4],R9[4],R8[4],R7[4],R6[4],R5[4],R4[4],R3[4],R2[4],R1[4],R0[4]), ABus[4]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[3],R14[3],R13[3],R12[3],R11[3],R10[3],R9[3],R8[3],R7[3],R6[3],R5[3],R4[3],R3[3],R2[3],R1[3],R0[3]), ABus[3]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[2],R14[2],R13[2],R12[2],R11[2],R10[2],R9[2],R8[2],R7[2],R6[2],R5[2],R4[2],R3[2],R2[2],R1[2],R0[2]), ABus[2]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[1],R14[1],R13[1],R12[1],R11[1],R10[1],R9[1],R8[1],R7[1],R6[1],R5[1],R4[1],R3[1],R2[1],R1[1],R0[1]), ABus[1]);
Mux( SD(coord,"aa"), (mir[11]-mir[8]), (R15[0],R14[0],R13[0],R12[0],R11[0],R10[0],R9[0],R8[0],R7[0],R6[0],R5[0],R4[0],R3[0],R2[0],R1[0],R0[0]), ABus[0]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[15],R14[15],R13[15],R12[15],R11[15],R10[15],R9[15],R8[15],R7[15],R6[15],R5[15],R4[15],R3[15],R2[15],R1[15],R0[15]), BBus[15]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[14],R14[14],R13[14],R12[14],R11[14],R10[14],R9[14],R8[14],R7[14],R6[14],R5[14],R4[14],R3[14],R2[14],R1[14],R0[14]), BBus[14]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[13],R14[13],R13[13],R12[13],R11[13],R10[13],R9[13],R8[13],R7[13],R6[13],R5[13],R4[13],R3[13],R2[13],R1[13],R0[13]), BBus[13]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[12],R14[12],R13[12],R12[12],R11[12],R10[12],R9[12],R8[12],R7[12],R6[12],R5[12],R4[12],R3[12],R2[12],R1[12],R0[12]), BBus[12]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[11],R14[11],R13[11],R12[11],R11[11],R10[11],R9[11],R8[11],R7[11],R6[11],R5[11],R4[11],R3[11],R2[11],R1[11],R0[11]), BBus[11]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[10],R14[10],R13[10],R12[10],R11[10],R10[10],R9[10],R8[10],R7[10],R6[10],R5[10],R4[10],R3[10],R2[10],R1[10],R0[10]), BBus[10]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[9],R14[9],R13[9],R12[9],R11[9],R10[9],R9[9],R8[9],R7[9],R6[9],R5[9],R4[9],R3[9],R2[9],R1[9],R0[9]), BBus[9]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[8],R14[8],R13[8],R12[8],R11[8],R10[8],R9[8],R8[8],R7[8],R6[8],R5[8],R4[8],R3[8],R2[8],R1[8],R0[8]), BBus[8]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[7],R14[7],R13[7],R12[7],R11[7],R10[7],R9[7],R8[7],R7[7],R6[7],R5[7],R4[7],R3[7],R2[7],R1[7],R0[7]), BBus[7]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[6],R14[6],R13[6],R12[6],R11[6],R10[6],R9[6],R8[6],R7[6],R6[6],R5[6],R4[6],R3[6],R2[6],R1[6],R0[6]), BBus[6]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[5],R14[5],R13[5],R12[5],R11[5],R10[5],R9[5],R8[5],R7[5],R6[5],R5[5],R4[5],R3[5],R2[5],R1[5],R0[5]), BBus[5]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[4],R14[4],R13[4],R12[4],R11[4],R10[4],R9[4],R8[4],R7[4],R6[4],R5[4],R4[4],R3[4],R2[4],R1[4],R0[4]), BBus[4]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[3],R14[3],R13[3],R12[3],R11[3],R10[3],R9[3],R8[3],R7[3],R6[3],R5[3],R4[3],R3[3],R2[3],R1[3],R0[3]), BBus[3]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[2],R14[2],R13[2],R12[2],R11[2],R10[2],R9[2],R8[2],R7[2],R6[2],R5[2],R4[2],R3[2],R2[2],R1[2],R0[2]), BBus[2]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[1],R14[1],R13[1],R12[1],R11[1],R10[1],R9[1],R8[1],R7[1],R6[1],R5[1],R4[1],R3[1],R2[1],R1[1],R0[1]), BBus[1]);
Mux( SD(coord,"aa"), (mir[15]-mir[12]), (R15[0],R14[0],R13[0],R12[0],R11[0],R10[0],R9[0],R8[0],R7[0],R6[0],R5[0],R4[0],R3[0],R2[0],R1[0],R0[0]), BBus[0]);
D_Latch( SD(coord,"aa"), ABus[15], Phase[1], ALatch[15] );
D_Latch( SD(coord,"aa"), ABus[14], Phase[1], ALatch[14] );
D_Latch( SD(coord,"aa"), ABus[13], Phase[1], ALatch[13] );
D_Latch( SD(coord,"aa"), ABus[12], Phase[1], ALatch[12] );
D_Latch( SD(coord,"aa"), ABus[11], Phase[1], ALatch[11] );
D_Latch( SD(coord,"aa"), ABus[10], Phase[1], ALatch[10] );
D_Latch( SD(coord,"aa"), ABus[9], Phase[1], ALatch[9] );
D_Latch( SD(coord,"aa"), ABus[8], Phase[1], ALatch[8] );
D_Latch( SD(coord,"aa"), ABus[7], Phase[1], ALatch[7] );
D_Latch( SD(coord,"aa"), ABus[6], Phase[1], ALatch[6] );
D_Latch( SD(coord,"aa"), ABus[5], Phase[1], ALatch[5] );
D_Latch( SD(coord,"aa"), ABus[4], Phase[1], ALatch[4] );
D_Latch( SD(coord,"aa"), ABus[3], Phase[1], ALatch[3] );
D_Latch( SD(coord,"aa"), ABus[2], Phase[1], ALatch[2] );
D_Latch( SD(coord,"aa"), ABus[1], Phase[1], ALatch[1] );
D_Latch( SD(coord,"aa"), ABus[0], Phase[1], ALatch[0] );
D_Latch( SD(coord,"aa"), BBus[15], Phase[1], BLatch[15] );
D_Latch( SD(coord,"aa"), BBus[14], Phase[1], BLatch[14] );
D_Latch( SD(coord,"aa"), BBus[13], Phase[1], BLatch[13] );
D_Latch( SD(coord,"aa"), BBus[12], Phase[1], BLatch[12] );
D_Latch( SD(coord,"aa"), BBus[11], Phase[1], BLatch[11] );
D_Latch( SD(coord,"aa"), BBus[10], Phase[1], BLatch[10] );
D_Latch( SD(coord,"aa"), BBus[9], Phase[1], BLatch[9] );
D_Latch( SD(coord,"aa"), BBus[8], Phase[1], BLatch[8] );
D_Latch( SD(coord,"aa"), BBus[7], Phase[1], BLatch[7] );
D_Latch( SD(coord,"aa"), BBus[6], Phase[1], BLatch[6] );
D_Latch( SD(coord,"aa"), BBus[5], Phase[1], BLatch[5] );
D_Latch( SD(coord,"aa"), BBus[4], Phase[1], BLatch[4] );
D_Latch( SD(coord,"aa"), BBus[3], Phase[1], BLatch[3] );
D_Latch( SD(coord,"aa"), BBus[2], Phase[1], BLatch[2] );
D_Latch( SD(coord,"aa"), BBus[1], Phase[1], BLatch[1] );
D_Latch( SD(coord,"aa"), BBus[0], Phase[1], BLatch[0] );

Mux( SD(coord,"aa"), mir[31], (mbr[15],ALatch[15]), AMux[15]);
Mux( SD(coord,"aa"), mir[31], (mbr[14],ALatch[14]), AMux[14]);
Mux( SD(coord,"aa"), mir[31], (mbr[13],ALatch[13]), AMux[13]);
Mux( SD(coord,"aa"), mir[31], (mbr[12],ALatch[12]), AMux[12]);
Mux( SD(coord,"aa"), mir[31], (mbr[11],ALatch[11]), AMux[11]);
Mux( SD(coord,"aa"), mir[31], (mbr[10],ALatch[10]), AMux[10]);
Mux( SD(coord,"aa"), mir[31], (mbr[9],ALatch[9]), AMux[9]);
Mux( SD(coord,"aa"), mir[31], (mbr[8],ALatch[8]), AMux[8]);
Mux( SD(coord,"aa"), mir[31], (mbr[7],ALatch[7]), AMux[7]);
Mux( SD(coord,"aa"), mir[31], (mbr[6],ALatch[6]), AMux[6]);
Mux( SD(coord,"aa"), mir[31], (mbr[5],ALatch[5]), AMux[5]);
Mux( SD(coord,"aa"), mir[31], (mbr[4],ALatch[4]), AMux[4]);
Mux( SD(coord,"aa"), mir[31], (mbr[3],ALatch[3]), AMux[3]);
Mux( SD(coord,"aa"), mir[31], (mbr[2],ALatch[2]), AMux[2]);
Mux( SD(coord,"aa"), mir[31], (mbr[1],ALatch[1]), AMux[1]);
Mux( SD(coord,"aa"), mir[31], (mbr[0],ALatch[0]), AMux[0]);

ALU16( SD(coord,"aa"), (AMux[15]-AMux[0]), (BLatch[15]-BLatch[0]), (mir[28],mir[27]), (ALU[15]-ALU[0]), N, Z);
SHIFTER16( SD(coord,"aa"), (ALU[15]-ALU[0]), (mir[26],mir[25]), (CBus[15]-CBus[0]));


}
