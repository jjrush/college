package conditions_loops.labs;

import java.util.Scanner;

/**
 * Conditions and Loops Exercise 2: Days of the week
 *
 *      Take in a number from the user and print "Monday", "Tuesday", ... "Sunday", or "Other"
 *      if the number from the user is 1, 2,... 7, or other respectively. Use a "nested-if" statement.
 *
 */

public class Exercise_02 {

    public static void main(String[] args) {

        // create scanner
        Scanner scanner = new Scanner(System.in);

        // prompt user
        System.out.print("Enter a number of the week (1 through 7): ");

        // assign input to variable as int
        int input = scanner.nextInt();

        // write completed code here
        switch (input)
        {
            case 1:
                System.out.print("Monday");
                break;
            case 2:
                System.out.print("Tuesday");
                break;
            case 3:
                System.out.print("Wednesday");
                break;
            case 4:
                System.out.print("Thursday");
                break;
            case 5:
                System.out.print("Friday");
                break;
            case 6:
                System.out.print("Saturday");
                break;
            case 7:
                System.out.print("Sunday");
                break;
            default:
                System.out.print("Other");
                break;
        }
    }
}
