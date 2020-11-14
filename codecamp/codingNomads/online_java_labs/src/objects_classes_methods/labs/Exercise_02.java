package objects_classes_methods.labs;

/**
 * Objects, Classes and Methods Exercise 2:
 *
 *      Create a class that demonstrates the following: (all methods below can be in the same class)
 *
 *      1) A static method that calls another static method.
 *      2) A static method that calls a non-static method.
 *      3) A non-static method1 that calls another non-static method2, but this time method2 should return a value to method1.
 *      4) A non-static method1 that calls another non-static method2 and passes a value to method2.
 */

class Exercise_02
{
    public static void main(String args[])
    {
        System.out.println(metaMethod());
    }

    public static int metaMethod()
    {
        return subMetaMethod();
    }

    public static int subMetaMethod()
    {
        Exercise_02 foo = new Exercise_02();
        return foo.nonStaticMethod1();
    }

    public int nonStaticMethod1()
    {
        Exercise_02 foo = new Exercise_02();
        return foo.nonStaticMethod2(-1);
    }

    public int nonStaticMethod2(int value)
    {
        return value;
    }
}


