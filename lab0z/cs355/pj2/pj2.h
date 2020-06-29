// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon

/* =============================================================
   Project 2: skeleton file
   ============================================================= */

// s - 4bit sum
// c - 4 element array of carrys
// a,b - two 4-bit inputs
void Four_Adder( const SD &coord,
		 const Signal &a, const Signal &b, 
		 const Signal &s, const Signal &c)
{
	Module(coord, "4bitFA", (a,b,c[0]-c[2]), (s,c[3]));

	Full_Adder( SD(coord,"aa"), a[0], b[0], Zero, s[0], c[0]);
	Full_Adder( SD(coord,"aa"), a[1], b[1], c[0], s[1], c[1]);
	Full_Adder( SD(coord,"aa"), a[2], b[2], c[1], s[2], c[2]);
	Full_Adder( SD(coord,"aa"), a[3], b[3], c[2], s[3], c[3]);
       		
	Probe( SD(coord,"aa"), c[3]);
    
	Probe( SD(coord,"aa"), s[3]);
	Probe( SD(coord,"aa"), s[2]);
	Probe( SD(coord,"aa"), s[1]);
	Probe( SD(coord,"aa"), s[0]);  
}



void Multiply4( const SD &coord, 
                const Signal &x, const Signal &y, const Signal &m)
{
   Module( coord, "Multiple", (x,y), m);

   /* ======================================================
      Write the multiply circuit here....

      Make sure you use a component CompName as follows:

            CompName( SD(coord,"aa"),  ...., .... );
                      ^^^^^^^^^^^^^^
                      Use this coordinate for EVERY component (keep "aa")

      When you're done, compile with:

              	cs355sim  pj2-main

      And run it with:

		simex
      ====================================================== */

	// x - multiplicant
	// y - multiplier
	Sig(and1,3);
	And(SD(coord,"aa"), (x[0],y[0]), m[0]);
	And(SD(coord,"aa"), (x[1],y[0]), and1[0]);
	And(SD(coord,"aa"), (x[2],y[0]), and1[1]);
	And(SD(coord,"aa"), (x[3],y[0]), and1[2]);

	Sig(and2,4);
	And(SD(coord,"aa"), (x[0],y[1]), and2[0]);
	And(SD(coord,"aa"), (x[1],y[1]), and2[1]);
	And(SD(coord,"aa"), (x[2],y[1]), and2[2]);
	And(SD(coord,"aa"), (x[3],y[1]), and2[3]);

	Sig(sum1, 4);
	Sig(c1  , 4);
	Four_Adder( SD(coord,"aa"), (ZERO, and1[2], and1[1], and1[0]), 
		  (and2[3]-and2[0]), (sum1[3]-sum1[0]), (c1[3]-c1[0]) );
	// sum1[0] -> m[1] 
	// c1[3] -> MSB of x in sum2
	And(SD(coord,"aa"), (sum1[0], ONE), m[1]);

	Sig(and3,4);
	And(SD(coord,"aa"), (x[0],y[2]), and3[0]);
	And(SD(coord,"aa"), (x[1],y[2]), and3[1]);
	And(SD(coord,"aa"), (x[2],y[2]), and3[2]);
	And(SD(coord,"aa"), (x[3],y[2]), and3[3]);
	
	Sig(sum2, 4);
	Sig(c2  , 4);
	Four_Adder( SD(coord,"aa"), (c1[3],sum1[3],sum1[2],sum1[1]), 
		  (and3[3]-and3[0]), (sum2[3]-sum2[0]), (c2[3]-c2[0]) );
	// sum2[0] -> m[2]
	// c2[3] -> MSB of x in sum3
	And(SD(coord,"aa"), (sum2[0], ONE), m[2]);
		
	Sig(and4, 4);
	And(SD(coord,"aa"), (x[0],y[3]), and4[0]);
	And(SD(coord,"aa"), (x[1],y[3]), and4[1]);
	And(SD(coord,"aa"), (x[2],y[3]), and4[2]);
	And(SD(coord,"aa"), (x[3],y[3]), and4[3]);
	
	Sig(sum3, 4);
	Sig(c3  , 4);
	Four_Adder( SD(coord,"aa"), (c2[3],sum2[3],sum2[2],sum2[1]), 
		  (and4[3]-and4[0]), (sum3[3]-sum3[0]), (c3[3]-c3[0]) );
	// sum3 -> m[3-6]
	// c3[3] -> m[7]
	And(SD(coord,"aa"), (sum3[0],ONE), m[3]);
	And(SD(coord,"aa"), (sum3[1],ONE), m[4]);
	And(SD(coord,"aa"), (sum3[2],ONE), m[5]);
	And(SD(coord,"aa"), (sum3[3],ONE), m[6]);
	
	And(SD(coord,"aa"), (c3[3],ONE), m[7]);
		
	Probe(SD(coord,"aa"), m );

}
