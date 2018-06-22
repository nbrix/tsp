CS 325 Sp 2018
Group 54 TSP Project
Nikolas Brix
Adam Brulotte
Andy Doan

Note:
	Our program was coded, compiled, and ran on OSU Flip 2 servers.


How to compile and run our program:

1. Have the files (input *.txt's, tsp.cpp, makefile) in the same directory

2. From the shell (command line) type "make" and an executable file (tsp) will be produced

3. Run the program by typing "tsp <input file name>" in the command line.
	-Example: tsp tsp_example_1.txt

4. The tour length will be displed to the user and an output file will be created as well

5. The output file is named the same as the input file used only with a .tour attached
	-Example:
		Input File Name: tsp_example_1.txt
		Output File Name: tsp_example_1.txt.tour
  
   It will contain the tour length (1st line) followed by the city numbers in the order
   they were visited


Verification:

1. After the program has been run, and with the "tsp-verifier.py" file in the same directory

2. Type "python tsp-verifier.py <input file name> <output file name>" in the command prompt
	Example: python tsp-verifier.py tsp_example_1.txt tsp_example_1.txt.tour

3. The results will be output to the screen
	Example: "Each item appears to exist in both the input file and the output file.
			('solution found of length', 125592)"

Clean-up:

1. Type "make clean" in the command prompt to remove the executable file "tsp"