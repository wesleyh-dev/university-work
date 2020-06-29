// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon


/* ===================================================================
   Skeleton file for CS355 pj4: 4 Phase clock and MPC
   =================================================================== */

/* --------------------------------------------------------------------
   Four_Phase_Clock( coord, Reset, Clk, Phase )

     inputs:  Reset = pressing reset will force Four_Phase_Clock()
		      to go to phase 1
              Clk = toggling Clk twice will make Four_Phase_Clock()
		      go to the next phase

              Progression:   Ph1 --> Ph2 --> Ph3 --> Ph4 --+
                              ^                            |
                              |                            |
                              +----------------------------+

     output: Phase = 4 bit array representing the 4 phases

                     Phase[0]   Phase[1]   Phase[2]   Phase[3]
              ==================================================
                Ph1:     1         0          0         0
                Ph2:     0         1          0         0
                Ph3:     0         0          1         0
                Ph4:     0         0          0         1
   -------------------------------------------------------------------- */
void Four_Phase_Clock(const SD &coord,
                      const Signal &Reset, const Signal &Clk,
                      const Signal &Phase)
{
   Module( coord, "4-Ph-Clock", (Clk, Reset), Phase);

   /* ----------------------------------------------------------------------
      Write your clock Four_Phase_Clock ciruit here

      Make sure you use a component CompName as follows:

            CompName( SD(coord,"aa"),  ...., .... );
                      ^^^^^^^^^^^^^^
                      Use this coordinate for EVERY component (keep "aa")
      ---------------------------------------------------------------------- */

Sig(q,2);
Sig(notq,2);
Sig(orr,2);
Sig(aand,4);

Not( SD(coord,"aa"), q[0], notq[0]);
Not( SD(coord,"aa"), q[1], notq[1]);

And( SD(coord,"aa"), (q[0],notq[1]), aand[0]);
And( SD(coord,"aa"), (notq[0],q[1]), aand[1]);
Or ( SD(coord,"aa"), (aand[0],aand[1]), orr[0]);
Dff( SD(coord,"aa"), (ZERO,orr[0],Clk,Reset), q[1]);

And( SD(coord,"aa"), (notq[0],notq[1]), aand[2]);
And( SD(coord,"aa"), (notq[0],q[1]), aand[3]);
Or ( SD(coord,"aa"), (aand[2],aand[3]), orr[1]);
Dff( SD(coord,"aa"), (ZERO,orr[1],Clk,Reset), q[0]);

And( SD(coord,"aa"), (notq[0],notq[1]), Phase[0]);
And( SD(coord,"aa"), (q[0],notq[1]), Phase[1]);
And( SD(coord,"aa"), (notq[0],q[1]), Phase[2]);
And( SD(coord,"aa"), (q[0],q[1]), Phase[3]);

}




/* --------------------------------------------------------------------
   MPC_With_Reset( coord, Reset, Clk, Branch, Addr, MPC )

        The MPC is a 8-bit register that is updated with a new value
        when he MPC receives a "Clk" signal

     inputs: Reset = pressing Reset will store 00000000 into the MPC
	     Clk = toggling Clk twice will make the MPC_With_Reset
	           circuit update itself with the next MPC value

             Branch: if Branch = 0, MPC is updated with MPC + 1
                     if Branch = 1, MPC is updated with Addr

             Addr = the branch address - used only when Branch = 1

     output: MPC = 8 bit array representing the address in the MPC
   -------------------------------------------------------------------- */



void MPC_With_Reset(const SD &coord,
              const Signal &Reset, const Signal &Clk,
              const Signal &Branch, const Signal &Addr,  // Addr[7]..Addr[0]
              const Signal &MPC  // MPC[7].. MPC[0] 
             )
{
   Module( coord, "MPC-W-Reset", (Reset, Clk, Branch, Addr), MPC);

   /* ----------------------------------------------------------------------
      Write your MPC_With_Reset ciruit here

      Make sure you use a component CompName as follows:

            CompName( SD(coord,"aa"),  ...., .... );
                      ^^^^^^^^^^^^^^
                      Use this coordinate for EVERY component (keep "aa")
      ---------------------------------------------------------------------- */

Sig(MPC1,8);

Sig(fa,8);
Sig(c, 8);
Sig(sum,8);

Sig(m,8);

Full_Adder( SD(coord,"aa"), MPC[7], ZERO, c[6], sum[7], c[7]);
Full_Adder( SD(coord,"aa"), MPC[6], ZERO, c[5], sum[6], c[6]);
Full_Adder( SD(coord,"aa"), MPC[5], ZERO, c[4], sum[5], c[5]);
Full_Adder( SD(coord,"aa"), MPC[4], ZERO, c[3], sum[4], c[4]);
Full_Adder( SD(coord,"aa"), MPC[3], ZERO, c[2], sum[3], c[3]);
Full_Adder( SD(coord,"aa"), MPC[2], ZERO, c[1], sum[2], c[2]);
Full_Adder( SD(coord,"aa"), MPC[1], ZERO, c[0], sum[1], c[1]);
Full_Adder( SD(coord,"aa"), MPC[0], ONE , ZERO, sum[0], c[0]);

Mux( SD(coord,"aa"), Branch, (Addr[7],sum[7]), m[7]);
Mux( SD(coord,"aa"), Branch, (Addr[6],sum[6]), m[6]);
Mux( SD(coord,"aa"), Branch, (Addr[5],sum[5]), m[5]);
Mux( SD(coord,"aa"), Branch, (Addr[4],sum[4]), m[4]);
Mux( SD(coord,"aa"), Branch, (Addr[3],sum[3]), m[3]);
Mux( SD(coord,"aa"), Branch, (Addr[2],sum[2]), m[2]);
Mux( SD(coord,"aa"), Branch, (Addr[1],sum[1]), m[1]);
Mux( SD(coord,"aa"), Branch, (Addr[0],sum[0]), m[0]);

Dff( SD(coord,"aa"), (ZERO,m[0],Clk,Reset), MPC[0]);
Dff( SD(coord,"aa"), (ZERO,m[1],Clk,Reset), MPC[1]);
Dff( SD(coord,"aa"), (ZERO,m[2],Clk,Reset), MPC[2]);
Dff( SD(coord,"aa"), (ZERO,m[3],Clk,Reset), MPC[3]);
Dff( SD(coord,"aa"), (ZERO,m[4],Clk,Reset), MPC[4]);
Dff( SD(coord,"aa"), (ZERO,m[5],Clk,Reset), MPC[5]);
Dff( SD(coord,"aa"), (ZERO,m[6],Clk,Reset), MPC[6]);
Dff( SD(coord,"aa"), (ZERO,m[7],Clk,Reset), MPC[7]);

}
