package objects_classes_methods.labs.Exercise1;

public class Food {

    protected String color;
    protected String shape;
    protected String name;


    public Food() {
    }

    public Food( String color ) {
        this.color = color;
    }

    public Food( String color, String shape ) {
        this.color = color;
        this.shape = shape;
    }

    public Food( String color, String shape, String name ) {
        this.color = color;
        this.shape = shape;
        this.name = name;
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
}
