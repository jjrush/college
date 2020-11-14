package conditions_loops.labs;

/**
 * Conditions and Loops Exercise 9: break
 *
 *      Demonstrate the use of the "break" statement to exit a loop.
 *
 */

public class Exercise_09 {
    public static void main(String args[])
    {
        System.out.print("This loop prints until 100." + "\n");
        for (int i = 0; i < 100; i++) {
            System.out.print(i);
            if( i == 5 )
                break;
        }
        System.out.print("\n" + "I broke the loop at 5 so now it is over.");
    }
}
