#include "Sim.h"
#include "basic.h"

void D_Flipflop(const SD &sd,
                const Signal &D, const Signal &Clk,
                const Signal &q, const Signal &n_q)
{
   Signal set, reset, n_D, n_Clk;
   Signal slave_set, slave_reset;
   Sig( q1, 1 );
   Sig( n_q1, 1 );

   Module( sd, "Dff", (D, Clk), (q, n_q) );

   /* ----------------------------------------------------
 *       Master stage
 *             ---------------------------------------------------- */
   Not( SD(sd,"bc"), D, n_D);

   And( SD(sd,"ad"), (D, Clk),   set);
   And( SD(sd,"bd"), (Clk, n_D), reset);


   Nor ( SD(sd,"ae"), (set,  q1),   n_q1);
   Nor ( SD(sd,"be"), (n_q1,reset), q1);

   /* ----------------------------------------------------
 *       Slave stage
 *             ---------------------------------------------------- */
   Not( SD(sd,"dg"), Clk, n_Clk);

   And( SD(sd,"ah"), (q1, n_Clk), slave_set);
   And( SD(sd,"bh"), (n_Clk, n_q1), slave_reset);

   Nor( SD(sd,"ai"), (slave_set, q), n_q);
   Nor( SD(sd,"bi"), (n_q, slave_reset), q);
}


void simnet()
{
   Signal clk, q0, q0_bar, q1, q1_bar;
   Signal and1, or1, and2, or2, reset;

   Switch("da", clk, '0', One);
   Switch("aa", reset, '1', One);
   
   And("bb", (q0, q1_bar), and1);
   Or ("bc", (reset, and1), or1);
   D_Flipflop("bd", or1, clk, q1, q1_bar );
   
   And("bf", (q1_bar, q0_bar), and2);
   Or ("bg", (reset, and2), or2);
   D_Flipflop("bh", or2, clk, q0, q0_bar );
   
   Probe("ad", q1);
   Probe("ah", q0);
}
