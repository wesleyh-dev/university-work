//Wesley Harmon
//wharmon
//005

import java.util.Scanner;

public class TipCalc {
	//TODO: Write the method bill
	public static double bill(int w, int h){
		double wafPrice= (double)w*3.35;
		double hashPrice= (double)h*1.45;
		double b= wafPrice+hashPrice;
		return b;
	}
		//Calculates total cost based on price of waffles and 			hashbrowns.

	//TODO: Write the method tip
	public static double tip(double b){
		double t;
		if (b<20.00){
			t= 0.15*b;
		} else if (b<30.00){
			t= 0.17*b;
		} else {
			t= 0.20*b;
		};
		return t;
	}
		//Calculates appropriate tip based on total cost of food.

	//TODO: Write the method info
	public static double info(int w, int h, double b, double t){
		System.out.println("You bought "+w+" waffles and "+h
		+" hashbrowns.");
		System.out.println("Calculated bill before tip: $"+b);
		System.out.println("Calculated tip: $"+t);
		double c=t+b;
		System.out.println("Total bill is $"+c+", rounded up to $"+Math.ceil(c));
		return 0.0;
	}
		//Tells you the information about your bill including the 			total tip as well as the cost of the bill and tip 			together, then tells you the final cost rounded up to the 			nearest whole bill.

	
	public static void main(String[] args) {
		//DO NOT change any code in the main method

		Scanner in = new Scanner(System.in);
		
		//get the number of waffles
		System.out.print("How many waffles? ");
		int w = in.nextInt();

		//get the number of hashbrowns from the user
		System.out.print("How many hashbrowns? " );
		int h = in.nextInt();

		//Calculate the total bill for waffles and hashbrowns
		double b = bill(w, h);
		double t = tip(b);
		info(w, h, b, t);
			//Invokes the info method that gives you all the 				required information of your receipt from your 				meal at Waffle House.	

	}

	
	
	 

}
