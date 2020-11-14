package datatypes_operators.labs;

import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * Data Types and Operators Exercise 5: More arrays
 *
 *      Using the array below, take in a number from 1-10 from the user and print out the index of that
 *      element.
 *
 */

public class Exercise_05{

    public static void main(String[] args)
            throws java.io.IOException {

        // create object to read the string in from user
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        // variables
        int[] array = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int num = 0;

        // prompt user
        System.out.print("Please a number: ");

        num = Integer.parseInt(br.readLine());

        System.out.print( array[num] );

    }
}