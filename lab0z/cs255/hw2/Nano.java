/*** THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR   
  OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon ***/

import java.util.Arrays;

public class Nano
{
   public static char[] digit 
            = {'#', '!', '%', '@', '(', ')', '[', ']', '$'};

   /* ==========================================================
      Return the 2's complement binary representation for the
      Nano number given in String s
      ========================================================== */
   public static int parseNano(String s)
   {
      /* ------------------------------------------------------------------
         This loop checks if the input contains an illegal (non-Nano) digit
         ------------------------------------------------------------------ */
      for (int i = 0 ; i < s.length(); i++)
      {
         int j = 0;
         while ( j < digit.length )
         {
            if ( s.charAt(i) == digit[j] || s.charAt(i) == '-' )
            {
               break;
            }

            j++;
         }

         if ( j >= digit.length )
         {
            System.out.println("Illegal nano digit found in input: " 
					+ s.charAt(i) );
            System.out.println("A Nano digit must be one of these: " 
				+ Arrays.toString (digit) );
            System.exit(1);
         }
      }

      int nano = 0;
      // Check if number should be negative
      boolean negative;
      if (s.charAt(0) == '-')
      {
         negative = true;
         s = s.substring(1, s.length());
      } else negative = false;

      // If number is bigger than 8, use getInt to get value using base 9
      // Otherwise the value is simply the index of the appropriate symbol
      if (s.length() > 1) nano = getInt(s);
      else 
         for (int i = 0; i < digit.length; i++)
         {
            if (s.charAt(0) == digit[i]) nano = i;
         }

      // Make the value negative or keep positive appropriately
      if (negative) return -nano;
      return nano;
   }

   // Generate a value using base 9, based on the Neptunian value
   public static int getInt(String s)
   {
      int n = 0;
      int pow = s.length()-1;
      for (int i = 0; i < s.length(); i++)
      {
         for (int j = 0; j < digit.length; j++)
         {
            if (s.charAt(i) == digit[j]) n += (int)Math.pow(9, pow)*j;
         }
         pow--;
      }
      return n;
   }
   

   /* ==========================================================
      Return the String of Nano digit that represent the value
      of the 2's complement binary number given in 
      the input parameter 'value'
      ========================================================== */
   public static String toString(int value)
   {
      boolean negative;
      String s = "";

      // Check if value should be negative
      if (value < 0) 
      {
         negative = true;
         value = -value;
      }
      else negative = false;
   

      // If value has more than one digit, get String using base 9 
      if (value > 8) s = getString(value);
      else s += digit[value];
     
      // Return a negative Neptunian value if input was negative
      if (negative) s = "-" + s;
      return s;
   }

   // Get Neptunian representation of given int
   public static String getString(int x)
   {
      int remainder;
      String fin = "";
      while (x > 0)
      {
         remainder = x%9;
         fin = digit[remainder] + fin;
         x/=9;
      }
      return fin;
   }

}

