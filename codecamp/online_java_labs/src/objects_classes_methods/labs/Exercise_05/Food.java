package objects_classes_methods.labs.Exercise_05;

public class Food {

    protected String color;
    protected String shape;
    protected String name;
    protected static int instances = 0;
    protected int value = 0;

    public Food() {
        updateInstances();
    }

    public Food( String color ) {
        this.color = color;
    }

    public Food( String color, String shape ) {
        this.color = color;
        this.shape = shape;
        updateInstances();
    }

    public Food( String color, String shape, String name ) {
        this.color = color;
        this.shape = shape;
        this.name = name;
        updateInstances();
    }

    public void setColor( String color ) {
        this.color = color;
    }

    public void setShape( String shape ) {
        this.shape = shape;
    }

    public void setName( String name ) {
        this.name = name;
    }

    public String getColor() {
        return color;
    }

    public String getShape() {
        return shape;
    }

    public String getName(){
        return name;
    }

    public static String returnPotato()
    {
        return "potato";
    }

    public static void updateInstances()
    {
        instances++;
    }

    public static int getInstances()
    {
        return instances;
    }
}