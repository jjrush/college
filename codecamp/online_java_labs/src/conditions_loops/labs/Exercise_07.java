package conditions_loops.labs;

import java.util.Scanner;
/**
 * Conditions and Loops Exercise 7: First vowel
 *
 *      Take in a word from the user and using a "while" loop, find the first vowel in the word.
 *      Once you find the vowel, print out the word and the first vowel.
 *
 */

public class Exercise_07 {
    public static void main(String args[])
    {
        System.out.print("Enter a word: ");
        Scanner scanner = new Scanner(System.in);
        String word = scanner.nextLine();
        int vowel = -1;
        int i = 0;
        while( i < word.length() )
        {
            if( word.charAt(i) == 'a' ||
                word.charAt(i) == 'e' ||
                word.charAt(i) == 'i' ||
                word.charAt(i) == 'o' ||
                word.charAt(i) == 'u' )
            {
                vowel = i;
                i = word.length()+1;
            }
            i++;
        }
        if( vowel == -1 )
        {
            System.out.print("Error: no vowels found in word \"" + word + ".\"");
        }
        else
        {
            System.out.print("First vowel in word \"" + word + "\": " + word.charAt(vowel));
        }
    }
}
