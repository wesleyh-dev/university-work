
#ifndef _basic_h
#define _basic_h

/* ==================================================================
   Decoder2x4():  a 2x4 decoder

   Input:    c:   c[1] c[0]
   Output:   One of s[3], s[2], s[1], s[0] is 1, all other = 0
   =================================================================== */
void Decoder2x4( const SD &coord,
                 const Signal &c, const Signal &s)
{

   Signal not_c0, not_c1;

   Module( coord, "Decoder", (c[1],c[0]), (s[3]-s[0]) );

   Not( SD(coord,"aa"), c[1], not_c1);
   Not( SD(coord,"ba"), c[0], not_c0);

   And( SD(coord,"ba"), (c[1],   c[0]),   s[3]);
   And( SD(coord,"bb"), (c[1],   not_c0), s[2]);
   And( SD(coord,"bc"), (not_c1, c[0]),   s[1]);
   And( SD(coord,"bd"), (not_c1, not_c0), s[0]);
}


void Full_Adder( const SD &coord,
		 const Signal &a, const Signal &b, const Signal &CarryIn, 
		 const Signal &Sum, const Signal &CarryOut)
{
   Module( coord, "FA", (CarryIn,a,b), (CarryOut,Sum) );

   Signal x, y, z;

   Xor( SD(coord,"aa"),    (a,b),        x);
   Xor( SD(coord,"ab"),    (x, CarryIn), Sum);
   And( SD(coord,"bb"),    (a, b),       y);
   And( SD(coord,"cb"),    (CarryIn, x), z);
   Or ( SD(coord,"bc-cc"), (y, z),       CarryOut);
}



void ResetableReg8( const SD &coord,
		     const Signal &in, const Signal &load, 
		     const Signal &reset,
		     const Signal &out)
{
   Module( coord, "R8", (in,load,reset), (out) );

   Dff ( SD(coord,"aa"), (Zero, in[7], load, reset), out[7] ); 
   Dff ( SD(coord,"aa"), (Zero, in[6], load, reset), out[6] ); 
   Dff ( SD(coord,"aa"), (Zero, in[5], load, reset), out[5] ); 
   Dff ( SD(coord,"aa"), (Zero, in[4], load, reset), out[4] ); 
   Dff ( SD(coord,"aa"), (Zero, in[3], load, reset), out[3] ); 
   Dff ( SD(coord,"aa"), (Zero, in[2], load, reset), out[2] ); 
   Dff ( SD(coord,"aa"), (Zero, in[1], load, reset), out[1] ); 
   Dff ( SD(coord,"aa"), (Zero, in[0], load, reset), out[0] ); 
}



void ResetableReg16( const SD &coord,
		     const Signal &in, const Signal &load, const Signal &reset,
		     const Signal &out)
{
   Module( coord, "R16", (in,load,reset), (out) );

   Dff ( SD(coord,"aa"), (Zero, in[15], load, reset), out[15] ); 
   Dff ( SD(coord,"aa"), (Zero, in[14], load, reset), out[14] ); 
   Dff ( SD(coord,"aa"), (Zero, in[13], load, reset), out[13] ); 
   Dff ( SD(coord,"aa"), (Zero, in[12], load, reset), out[12] ); 
   Dff ( SD(coord,"aa"), (Zero, in[11], load, reset), out[11] ); 
   Dff ( SD(coord,"aa"), (Zero, in[10], load, reset), out[10] ); 
   Dff ( SD(coord,"aa"), (Zero, in[9], load, reset), out[9] ); 
   Dff ( SD(coord,"aa"), (Zero, in[8], load, reset), out[8] ); 
   Dff ( SD(coord,"aa"), (Zero, in[7], load, reset), out[7] ); 
   Dff ( SD(coord,"aa"), (Zero, in[6], load, reset), out[6] ); 
   Dff ( SD(coord,"aa"), (Zero, in[5], load, reset), out[5] ); 
   Dff ( SD(coord,"aa"), (Zero, in[4], load, reset), out[4] ); 
   Dff ( SD(coord,"aa"), (Zero, in[3], load, reset), out[3] ); 
   Dff ( SD(coord,"aa"), (Zero, in[2], load, reset), out[2] ); 
   Dff ( SD(coord,"aa"), (Zero, in[1], load, reset), out[1] ); 
   Dff ( SD(coord,"aa"), (Zero, in[0], load, reset), out[0] ); 
}


void Reg16( const SD &coord,
	    const Signal &in, const Signal &load, const Signal &out)
{
   Module( coord, "R16", (in,load), (out) );

   Signal nload;

   Not(SD(coord,"aa"), load, nload );
   Register( SD(coord,"aa"), One, nload, in, out );
}

void Constant16( const SD &coord,
                  const int x, const Signal &out)
{
   Signal help(16, x);

   Or( SD(coord,"aa"), help, out );
}


#endif
