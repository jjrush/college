package objects_classes_methods.labs.Exercise_03;

public class Person {

    protected String job;
    protected String height;
    protected String name;


    public Person() {
    }

    public Person( String job ) {
        this.job = job;
    }

    public Person( String job, String height ) {
        this.job = job;
        this.height = height;
    }

    public Person( String job, String height, String name ) {
        this.job = job;
        this.height = height;
        this.name = name;
    }

    public void setJob( String job ) {
        this.job = job;
    }

    public void setHeight( String height ) {
        this.height = height;
    }

    public void setName( String name ) {
        this.name = name;
    }

    public String getJob() {
        return job;
    }

    public String getHeight() {
        return height;
    }

    public String getName(){
        return name;
    }
}
