package datatypes_operators.labs;

/**
 * Data Types and Operators Exercise 7: Irregular Arrays
 *
 *      Create and populate an irregular array of size and contents of your choosing. Using a nested
 *      "for-each" loop, iterate and print out each element of the array.
 *
 */

public class Exercise_07 {
    public static void main(String[] args)
    {
        int[][] twoDArray = new int[2][2];
        twoDArray[0][0] = 0;
        twoDArray[0][1] = 1;
        twoDArray[1][0] = 2;
        twoDArray[1][1] = 3;

        for( int[] dimension: twoDArray)
        {
            for (int element: dimension )
            {
                System.out.print(element + " ");
            }
            System.out.print("\n");
        }
    }
}
