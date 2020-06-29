
class List
{
   int  value;
   List next;
}

public class help_list
{
   /* =====================================================
      InsertList(h,e): write teh algorithm in Java first !

      Compile with:  javac HelpList.java
      Run with:      java HelpList

      Correct output:
			List = 30
			List = 10 30
			List = 10 30 50
			List = 10 20 30 50
			List = 10 20 30 40 50
      ===================================================== */
   public static List InsertList(List h, List e, List d)
   {
	List lst = new List();  // holder list

       if (h == null) {
               System.out.println("check1");
               h = e;
               return h; }      
       else if (e.next != null) {
               System.out.println("check2");
               return h;}
       else if (h.next == null) {
               System.out.println("check3");
               h.next = e;
               return d;
       }
       else if (h.next.value >= e.value) {
               if (h.value >= e.value) { // then e must be the smallest value
                       lst = h;                        
                       e.next = lst;
                       System.out.println("check4");
                       h = e;
                       return h;
               }
               else {
                       lst = h.next;
                       System.out.println("check5");
                       h.next = e;
                       e.next = lst;
                       return d;
               }
       }

       else {
               System.out.println("check6");
               return InsertList(h.next, e, d);
       }


   }

   public static void main(String[] args)
   {
      int[] v = {30, 50, 10, 40, 20, -10, 3, 490, 39, 25, -39};
      List head = null;
      List elem;

      for ( int i = 0; i < v.length; i++ )
      {
         elem = new List(); 
         elem.value = v[i];
         head = InsertList(head, elem, head);
         System.out.print("List = ");
         PrintList( head );
         System.out.println();
      }
   }

   public static void PrintList(List h)
   {
      if ( h == null )
         return;
      else
      {
	 System.out.print(h.value + " ");
	 PrintList(h.next);
      }
   }
}

