package datatypes_operators.labs;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Data Types and Operators Exercise 3: Pig latin
 *
 *      Take in the user's name and print out their name translated to pig latin.
 *      For the purpose of this program, we will say that any word or name can be
 *      translated to pig latin by moving the first letter to the end, followed by "ay".
 *      For example: ryan -> yanray, caden -> adencay
 *
 *      HINT: We will make use of the substring() method.
 */

public class Exercise_03 {

    public static void main(String[] args)
            throws java.io.IOException {

        // create object to read the string in from user
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        // prompt user
        System.out.print("Please provide your name: ");

        // get name of user
        String pigLatinName = br.readLine();

        // create a StringBuilder object
        StringBuilder sb = new StringBuilder(pigLatinName);

        // make alterations
        sb.append(pigLatinName.charAt(0));
        sb.append("ay");
        sb.deleteCharAt(0);

        // write to console
        System.out.print(sb);
    }
}


