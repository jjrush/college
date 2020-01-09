package datatypes_operators.labs;

/**
 * Data Types and Operators Exercise 6: 2D Array
 *
 *      Creat and populate a 5x5 2D array with multiples of 3, starting with 3. Once populated, print out the results.
 *
 *      The output should look something like this:
 *
 *      3 6 9 12 15
 *      18 21 24 27 30
 *      ...
 *      ...
 *      ...
 *
 */

public class Exercise_06 {

    public static void main(String[] args)
    {

        int[][] twoDArray = new int[5][5];
        int k = 1;
        // loop over each dimension of array adding multiples of 3 into elements
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                twoDArray[i][j] = 3 * k;
                k++;
            }
        }

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                System.out.print(twoDArray[i][j] + " ");
            }
            System.out.print("\n");
        }
    }
}
