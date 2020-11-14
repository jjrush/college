package conditions_loops.labs;

import java.util.Scanner;
/**
 * Conditions and Loops Exercise 5: Calculator
 *
 *      Take two numbers from the user, an upper and lower bound. Using a "for-loop", calculate the sum
 *      of numbers from the lower bound to the upper bound. Also, calculate the average of numbers.
 *      Print the results to the console.
 *
 *      For example, if a user enters 1 and 100, the output should be:
 *
 *      The sum is: 5050
 *      The average is: 50.5
 *
 *
 */

public class Exercise_05 {
    public static void main(String args[])
    {
        System.out.print("Please enter an upper bound: ");
        Scanner scanner = new Scanner(System.in);
        int upperBound = scanner.nextInt();

        System.out.print("Please enter a lower bound: ");
        scanner = new Scanner(System.in);
        int lowerBound = scanner.nextInt();

        if( lowerBound > upperBound )
        {
            System.out.print("Lower bound was greater than upper bound so values were swapped." + "\n");
            int temp = lowerBound;
            lowerBound = upperBound;
            upperBound = temp;
        }

        double average = 0;
        int sum = 0;

        for (int i = lowerBound; i < upperBound + 1; ++i) {
                sum += i;
        }

        average = (double)sum/(upperBound+1-lowerBound);

        System.out.print("Sum: " + sum + "\n");
        System.out.print("Average: " + average);

    }
}
