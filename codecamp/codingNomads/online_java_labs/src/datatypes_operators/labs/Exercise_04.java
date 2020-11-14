package datatypes_operators.labs;

import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * Data Types and Operators Exercise 4: Arrays calculator
 *
 *      Take in 10 numbers from the user. Place the numbers in an array. Using the loop of your choice,
 *      calculate the sum of all of the numbers in the array as well as the average.
 *
 *      Print the results to the console.
 *
 */

public class Exercise_04{
	public static void main(String[] args)
			throws java.io.IOException {

	    // variables
		int[] arr = new int[10];
        int sum = 0;
		// create object to read the string in from user
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        // loop to get ten different numbers from user
        for (int i = 0; i < 10; i++) {
			System.out.print("Please enter a number (" + (i+1) + " of 10 nums): ");
			arr[i] = Integer.parseInt(br.readLine());
			System.out.print("\n");
		}

        for (int i = 0; i < 10 ; i++) {
            sum += arr[i];
        }
        System.out.print(sum);

	}
}