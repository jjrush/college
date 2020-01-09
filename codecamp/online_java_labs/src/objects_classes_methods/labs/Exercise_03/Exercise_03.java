package objects_classes_methods.labs.Exercise_03;

/**
 * Objects, Classes and Methods Exercise 3:
 *
 *      Create at least three example classes (class1, class2, class3) that demonstrate comfort
 *      in creating multiple classes with multiple methods, and then call between those methods across classes.
 *      These methods can perform any task, such as getter/setter methods or simple addition/subtraction.
 *      1) Class1 should have the main method.
 *      2) The main method then needs to create an object of class1, class2, class3.
 *      3) The main method should then  call one or more non-static methods in class2 and class3.
 *
 */

class Exercise_03
{
    public static void main( String args[] )
    {
        // create an instance of each of the other two classes

        Food apple = new Food("red", "round", "apple" );
        System.out.println(apple.getColor());
        Food orange = new Food( "orange", "round", "orange");
        Person bob = new Person( "mason", "6 feet", "Bob");
        System.out.println(apple.getColor());
    }
}


