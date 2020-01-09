package conditions_loops.labs;

/**
 * Conditions and Loops Exercise 10: continue
 *
 *      Demonstrate the use of the "continue" statement to skip to the next iteration of a loop.
 *
 */

public class Exercise_10 {
    public static void main(String args[])
    {
        String word = "potato";
        System.out.print("The word is: " + word + "\n");
        for (int i = 0; i < word.length(); ++i) {
            if( word.charAt(i) == 'o' || word.charAt(i) == 'a' )
                continue;
            else
                System.out.print(word.charAt(i));
        }

            
    }
}
