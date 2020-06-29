public class Scope {
   static double a = 123.4;                     /* location (1) */

   public static void method( double a ) {      /* location (2) */
      System.out.println("overloaded method( double a )");

      System.out.print("a at (2) = ");
      // TODO: Write a S.o.pln() statement that prints a from location (2)
	System.out.println(a);
      System.out.print("a at (1) = ");
	// TODO: Write a S.o.pln() statement that prints a from location (1)
	System.out.println(Scope.a);
   }

   public static void method( String a ) {       /* location (3) */
      System.out.println("overloaded method( String a)");

      System.out.print("a at (3) = ");
      // TODO: Write a S.o.pln() statement that prints a from location (3)
	System.out.println(a);

      System.out.print("a at (1) = ");
      // TODO: Write a S.o.pln() statement that prints a on location (1)
	System.out.println(Scope.a);
   }

   public static void main( String[] args ) {
      double x = -444.4;                    /* location (4) */
	
      // TODO: Write a call to method() that passes x from location (4)
		
	method(x);
      if (x < 0.0) {
         double a = 9999.9;                 /* location (5) */
	
         // TODO: Write a call to method() that passes a from location (5)     
	  method(a);
	// TODO: Write a call to method() that passes a from location (1)
	  method(Scope.a); 
      }							
	
													    /* location (6) */
      // TODO: Write a call to method() that passes x from location (4)
	method(x);
      
	System.out.println("Impossible: Write a call to method() that passes a from location (5)");

      if (x < -100) {
         String a = "Hello";               /* location (7) */

         // TODO: Write a call to method() that passes x from location (4)
	  method(x);
         // TODO: Write a call to method() that passes a from location (7)
	  method(a);
         // TODO: Write a call to method() that passes a from location (1)
	  method(Scope.a);
      }
   }
}
