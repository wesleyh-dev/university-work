// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon


/* ===================================================================
   Skeleton file for CS355 pj3: 16 bit ALU and shifter
   =================================================================== */


/* --------------------------------------------------------------------
   ALU16( coord, a, b, c, s, N, Z)

     inputs:  a = first  (16 bit) binary number
              b = second (16 bit) binary number
	      c = 2 constrol signals to select the ALU operation

		  c[1] c[0]     Function
                 -----------------------------------
                   0    0       Add (s = a + b)
                   0    1       AND (s = a AND b  bitwise and)
                   1    0       Select a (s = a)
                   1    1       Not a (s = not a)

     outputs: s = 16 bit result of the selected operation
	      N = the negative flag (N=1 is result s is negative)
	      Z = the zero flag (Z=1 if the result s is equal to 0)
   -------------------------------------------------------------------- */



// i: i[3] = ONE, i[2] = left, i[1] = right, i[0] = center
// c: c[0] = c0, c[1] = c1
// o: 1-bit output
void shifter(const SD &coord, const Signal &c, const Signal &i, const Signal &o)
{
         Module( coord, "Shifter", (c,i), o);

         Mux( SD(coord,"aa"), (c[0],c[1]), (i[3],i[2],i[1],i[0]), o);

         
}                         



void alu(const SD &coord, const Signal &a, const Signal &b, 
	 const Signal &f, const Signal &cin, const Signal &cout,
	 const Signal &z)
{

Module(coord, "alu", (cin, a, b, f), (cout, z));

Sig(and1,1);
Sig(not1,1);
Sig(sum,1);

Full_Adder( SD(coord,"aa"), a, b, cin, sum, cout);
And( SD(coord,"aa"), a, b, and1);
Not( SD(coord,"aa"), a, not1);

Mux( SD(coord,"aa"), (f[1],f[0]), (not1, a, and1, sum), z);
Probe( SD(coord,"aa"), cout);

} 


void ALU16(const SD &coord, 
           const Signal &a, const Signal &b, const Signal &c, // c[1] c[0]
           const Signal &s, const Signal &N, const Signal &Z)
{
   Module( coord, "ALU-16", (a,b,c), (s,N,Z) );

   /* ----------------------------------------------------------------------
      Write your ALU16 ciruit here

      Make sure you use a component CompName as follows:

            CompName( SD(coord,"aa"),  ...., .... );
                      ^^^^^^^^^^^^^^
                      Use this coordinate for EVERY component (keep "aa")
      ---------------------------------------------------------------------- */
Sig(cin,16);
alu( SD(coord,"aa"), a[0], b[0], (c[1],c[0]), ZERO, cin[0], s[0]);
alu( SD(coord,"aa"), a[1], b[1], (c[1],c[0]), cin[0], cin[1], s[1]);
alu( SD(coord,"aa"), a[2], b[2], (c[1],c[0]), cin[1], cin[2], s[2]);
alu( SD(coord,"aa"), a[3], b[3], (c[1],c[0]), cin[2], cin[3], s[3]);
alu( SD(coord,"aa"), a[4], b[4], (c[1],c[0]), cin[3], cin[4], s[4]);
alu( SD(coord,"aa"), a[5], b[5], (c[1],c[0]), cin[4], cin[5], s[5]);
alu( SD(coord,"aa"), a[6], b[6], (c[1],c[0]), cin[5], cin[6], s[6]);
alu( SD(coord,"aa"), a[7], b[7], (c[1],c[0]), cin[6], cin[7], s[7]);
alu( SD(coord,"aa"), a[8], b[8], (c[1],c[0]), cin[7], cin[8], s[8]);
alu( SD(coord,"aa"), a[9], b[9], (c[1],c[0]), cin[8], cin[9], s[9]);
alu( SD(coord,"aa"), a[10], b[10], (c[1],c[0]), cin[9], cin[10], s[10]);
alu( SD(coord,"aa"), a[11], b[11], (c[1],c[0]), cin[10], cin[11], s[11]);
alu( SD(coord,"aa"), a[12], b[12], (c[1],c[0]), cin[11], cin[12], s[12]);
alu( SD(coord,"aa"), a[13], b[13], (c[1],c[0]), cin[12], cin[13], s[13]);
alu( SD(coord,"aa"), a[14], b[14], (c[1],c[0]), cin[13], cin[14], s[14]);
alu( SD(coord,"aa"), a[15], b[15], (c[1],c[0]), cin[14], cin[15], s[15]);


And( SD(coord,"aa"), (ONE, cin[15]), Z);
And( SD(coord,"aa"), (ONE, s[15]), N);

Probe( SD(coord,"aa"), s);
Probe( SD(coord,"aa"), Z);
Probe( SD(coord,"aa"), N);


}




/* --------------------------------------------------------------------
   SHIFTER16( coord, a, c, s)

     inputs:  a = the (16 bit) input binary number
	      c = 2 constrol signals to select the ALU operation

		  c[1] c[0]     Function
                 ---------------------------------
                   0    0       no shift (s == a)
                   0    1       shift right 1 bit position
                   1    0       shift left 1 bit position
                   1    1       not used (any output is OK)

     outputs: s = the shifted result
   -------------------------------------------------------------------- */
void SHIFTER16(const SD &coord, 
               const Signal &a, const Signal &c, // c[1] c[0]
               const Signal &s)
{
   Module( coord, "Shifter-16", (a,c), s );

   /* ----------------------------------------------------------------------
      Write your SHIFTER16 ciruit here

      Make sure you use a component CompName as follows:

            CompName( SD(coord,"aa"),  ...., .... );
                      ^^^^^^^^^^^^^^
                      Use this coordinate for EVERY component (keep "aa")
      ---------------------------------------------------------------------- */

shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[1], ZERO, a[0]),  s[0]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[2], a[0], a[1]),  s[1]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[3], a[1], a[2]),  s[2]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[4], a[2], a[3]),  s[3]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[5], a[3], a[4]),  s[4]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[6], a[4], a[5]),  s[5]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[7], a[5], a[6]),  s[6]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[8], a[6], a[7]),  s[7]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[9], a[7], a[8]),  s[8]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[10], a[8], a[9]), s[9]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[11], a[9],a[10]), s[10]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[12],a[10],a[11]), s[11]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[13],a[11],a[12]), s[12]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[14],a[12],a[13]), s[13]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,a[15],a[13],a[14]), s[14]);
shifter( SD(coord,"aa"), (c[1],c[0]), (ONE,ZERO,a[14], a[15]), s[15]);

Probe( SD(coord,"aa"), s);

}


