package objects_classes_methods.labs.Exercise_05;

/**
 * Objects, Classes and Methods Exercise 5:
 *
 *      Demonstrate the use of static instance variables. Create a class with at least one static variable
 *      and one non-static variable. Create a few objects of that class and set the values of the variables.
 *      Use System.out.println to demonstrate when the variables are changed.
 *
 */


class Exercise_05
{
    public static void main( String args[] )
    {
        // create an instance of each of the other two classes

        Food apple = new Food("red", "round", "apple" );
        System.out.println(apple.getInstances());
        System.out.println();

        Food orange = new Food( "orange", "round", "orange");
        System.out.println(apple.getInstances());
        System.out.println();
    }
}