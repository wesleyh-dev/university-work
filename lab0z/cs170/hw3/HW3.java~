public class HW3{
	public static int closestBusStop(int street){
		int a= street%8;
		int lowStop= street-a;
		int highStop= street+(8-a);
		if (a<=4||a==8){
			return lowStop;
		} else if (a>4){
			return highStop;
		}
		int error= 999;
		return error; 
	}

	public static int numNickels(int price){
		int quarter=25;
		int numQuarters= (price/quarter);
		price= price-numQuarters*quarter;
		int dime=10;
		int numDimes= (price/dime);
		price= price-numDimes*dime;
		int nickel=5;	
		int numNickels=(price/nickel);
		price= price-numNickels*nickel;
		int penny=1;
		return numNickels;
	}

	public static boolean ticket(int tnum){
		int n=6;
		int x= (tnum%(int)Math.pow(10,n)-tnum%(int)Math.pow(10,(n-1)))/(int)Math.pow(10,(n-1));
		int m=5;
		int y= (tnum%(int)Math.pow(10,m)-tnum%(int)Math.pow(10,(m-1)))/(int)Math.pow(10,(n-1));
		
		
		return true;

	}
	
	public static void main(String[] args){
		System.out.println("The closest bus stop to you: "+closestBusStop(1));
		System.out.println("The closest bus stop to you: "+closestBusStop(4));
		System.out.println("The closest bus stop to you: "+closestBusStop(5));
		System.out.println("The number of nickels needed for making change: "+numNickels(40));
		System.out.println("The number of nickels needed for making change: "+numNickels(36));
		System.out.println("The number of nickels needed for making change: "+numNickels(99));
		System.out.println(ticket(923995));
	}
}
