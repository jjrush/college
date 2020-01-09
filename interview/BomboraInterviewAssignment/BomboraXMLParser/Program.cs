using System;
using System.Xml;
using System.IO;
using System.Collections.Generic;

namespace BomboraXMLParser
{
    class Program
    {
        class Individual
        {
            public string ID;
            public string fName;
            public string lName;
            public string gradeID;

            // constructor
            public Individual()
            {
                ID = fName = lName = gradeID = "";
            }
        }

        class Classroom
        {
            public string classID;
            public string className;
            public string classGrade;
            public int numTeachers;
            public List<Individual> Students;
            public List<Individual> Teachers;

            // constructor
            public Classroom()
            {
                classID = className = classGrade = "";
                numTeachers = 0;
                Students = new List<Individual>();
                Teachers = new List<Individual>();
            }

            // public functions
            public void PrintDataToScreen()
            {
                // no students so print the teachers only
                if (Students.Count == 0)
                {
                    // no students in classroom so print teachers only
                    foreach (Individual teacher in Teachers)
                    {
                        Console.Write(teacher.ID + "," + teacher.lName + "," + teacher.fName + ",");
                    }
                    if (Teachers.Count < 1)
                    {
                        for (int i = Teachers.Count; i < 1; i++)
                        {
                            // write three commas for id, first name, and last name
                            Console.Write(" , , ,");
                        }
                    }
                    // write out empty spaces for students and grade and then append new line
                    Console.Write(" , , , " + Environment.NewLine);
                }
                else
                {
                    // students are present
                    foreach (Individual student in Students)
                    {
                        // write info for class
                        Console.Write(classID + "," + className + ",");
                        // write info for teachers
                        foreach (Individual teacher in Teachers)
                        {
                            Console.Write(teacher.ID + "," + teacher.lName + "," + teacher.fName + ",");
                        }
                        // write rest of students with their grade and a newline at the end of each student
                        Console.Write(student.ID + "," + student.lName + "," + student.fName + "," + classGrade + Environment.NewLine);
                    }
                }
            }
        }

        class Grade
        {
            public List<Classroom> Classrooms;
            public string classGrade;

            // constructor
            public Grade()
            {
                Classrooms = new List<Classroom>();
            }
        }

        static void Main(string[] args)
        {
            // create stream writer for output file and save location in documents of user
            System.IO.StreamWriter outputFile = new System.IO.StreamWriter(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\" + "xmlParserOutput.csv");

            // parse command line arguments to obtain xml file
            if (args.Length == 1)
            {
                string file = args[0];
                string fileType = Path.GetExtension(file);

                if (fileType == ".xml")                                     
                {
                    // variables
                    int highestNumberOfTeachers = 0;
                    int teacherCounter = 0;
                    int counter = 1;    // intialized to value 1 for the foreach loops when printing to file
                    Grade grade = new Grade();
                    Classroom tempClassroom = new Classroom();

                    // lists
                    List<Grade> Grades = new List<Grade>();

                    // file is valid xml file so start reading data
                    XmlTextReader xmlReader = new XmlTextReader(file);  // create instance of xml text reader

                    #region ReadingData
                    while (xmlReader.Read())
                    {
                        switch (xmlReader.NodeType)
                        {
                            case XmlNodeType.Element:
                                // begin name checks and parsing here
                                if (xmlReader.Name == "grade")
                                {
                                    grade.classGrade = xmlReader.GetAttribute("id");
                                    tempClassroom.classGrade = grade.classGrade;
                                }
                                else if (xmlReader.Name == "classroom")
                                {
                                    tempClassroom.classID = xmlReader.GetAttribute("id");
                                    tempClassroom.className = xmlReader.GetAttribute("name");
                                }
                                else if (xmlReader.Name == "teacher")
                                {
                                    Individual tempTeacher = new Individual();
                                    teacherCounter++;
                                    tempTeacher.ID = xmlReader.GetAttribute("id");
                                    tempTeacher.fName = xmlReader.GetAttribute("first_name");
                                    tempTeacher.lName = xmlReader.GetAttribute("last_name");
                                    tempClassroom.Teachers.Add(tempTeacher);
                                }
                                else if (xmlReader.Name == "student")
                                {
                                    Individual tempStudent = new Individual();
                                    tempStudent.ID = xmlReader.GetAttribute("id");
                                    tempStudent.fName = xmlReader.GetAttribute("first_name");
                                    tempStudent.lName = xmlReader.GetAttribute("last_name");
                                    tempStudent.gradeID = grade.classGrade;
                                    tempClassroom.Students.Add(tempStudent);
                                }
                                break;
                            case XmlNodeType.Whitespace:
                                // nothing
                                break;
                            case XmlNodeType.EndElement:
                                // end of element
                                if (xmlReader.Name == "classroom")
                                {
                                    // at end of classroom element so add classroom to list
                                    tempClassroom.numTeachers = teacherCounter;
                                    grade.Classrooms.Add(tempClassroom);
                                    if (teacherCounter > highestNumberOfTeachers)   // collect the highest number of recorded teachers in a classroom
                                        highestNumberOfTeachers = teacherCounter;
                                    teacherCounter = 0;
                                    tempClassroom = new Classroom();
                                }
                                if (xmlReader.Name == "grade")
                                {
                                    // at end of grade element so prepare for next grade level
                                    Grades.Add(grade);
                                    grade = new Grade();
                                }
                                break;
                            default:
                                break;
                        }
                    } // end of xml reading
                    #endregion

                    // data is collected to print to csv file

                    #region WritingFirstLineToFile
                    // print first line of classroom id, class name, teachers, students, grade
                    outputFile.Write("classroom_id,classroom_name,");
                    for (int i = 0; i < highestNumberOfTeachers; i++)
                    {
                        outputFile.Write("teacher_" + counter.ToString() + "_id,");
                        outputFile.Write("teacher_" + counter.ToString() + "_last_name,");
                        outputFile.Write("teacher_" + counter.ToString() + "_first_name,");
                        counter++;
                    }
                    counter = 1;
                    outputFile.Write("student_id,student_last_name,student_first_name,student_grade" + Environment.NewLine);
                    #endregion

                    #region WritingAllDataToFile
                    // write to file 
                    foreach (Grade g in Grades)
                    {
                        foreach (Classroom c in g.Classrooms)
                        {
                            // no students so print the teachers only
                            if (c.Students.Count == 0)
                            {
                                outputFile.Write(c.classID + "," + c.className + ",");
                                // no students in classroom so print teachers only
                                foreach (Individual teacher in c.Teachers)
                                {
                                    outputFile.Write(teacher.ID + "," + teacher.lName + "," + teacher.fName + ",");
                                }
                                if (c.Teachers.Count < highestNumberOfTeachers)
                                {
                                    for (int i = c.Teachers.Count; i < highestNumberOfTeachers; i++)
                                    {
                                        // write three commas for id, first name, and last name
                                        outputFile.Write(" , , ,");
                                    }
                                }
                                // write out empty spaces for students and grade and then append new line
                                outputFile.Write(" , , , " + Environment.NewLine);
                            }
                            else
                            {
                                // students are present
                                foreach (Individual student in c.Students)
                                {
                                    // write info for class
                                    outputFile.Write(c.classID + "," + c.className + ",");
                                    // write info for teachers
                                    foreach (Individual teacher in c.Teachers)
                                    {
                                        outputFile.Write(teacher.ID + "," + teacher.lName + "," + teacher.fName + ",");
                                    }
                                    // if there are fewer teachers in this class than the maximum teachers present in a single class then write blanks
                                    if (c.Teachers.Count < highestNumberOfTeachers)
                                    {
                                        for (int i = c.Teachers.Count; i < highestNumberOfTeachers; i++)
                                        {
                                            // write three commas for id, first name, and last name
                                            outputFile.Write(" , , ,");
                                           
                                        }
                                    }
                                    // write rest of students with their grade and a newline at the end of each student
                                    outputFile.Write(student.ID + "," + student.lName + "," + student.fName + "," + student.gradeID + "," + Environment.NewLine);
                                }
                            }
                        }
                    }
                    #endregion
                }
                else if( fileType != ".xml" )
                {
                    Console.WriteLine("Error: Provided file is not in \".xml\" format.");
                }
            }
            else if (args.Length > 1)
            {
                Console.WriteLine("Error: Too many arguments on the command line. Please send only a single \".xml\" to be parsed.");
            }
            else
            {
                Console.WriteLine("Error: No file provided via command line.");
            }
            // write data to file and close it
            outputFile.Close();
            Console.WriteLine("File is named \"xmlParserOutput.csv\" and was written to your Documents folder.");
        }
    }
}
