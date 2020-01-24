I worked on this compiler with two other students, Jakob Patino and Hunter Blakely

This is a compiler that:
  1. Takes a .txt file in called "final.txt"
  2. Translates it into a file that the compiler I made can read (Takes out comments, empty space, blank lines and adds space when needed).
     Makes a new file called "pre-compiled_code.txt" and stores correct code in there.
  3. Once it is done it runs the compiler, which checks for:
        -Specific keywords on specific lines such as PROGRAM, BEGIN, END, VAR, etc
        -Makes sure lines have semicolons when needed.
        -If not it will say "PROGRAM expected at line ....."
  
  4. It then translates the language into a .cpp file, which the program creates. It does this by:
        -Turning BEGIN into int main()
        -END into reutrn 0;
        -INTEGER into int
        -PRINT into cout<<
        -Adds #include <iostream> and using namespace std;
        -Many more
      
