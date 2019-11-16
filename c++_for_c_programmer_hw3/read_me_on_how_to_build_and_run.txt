This is a note on how I was able to build and run the program. I have already attached a windows built .exe for you. Running .exe with Graph.txt in the same directory should work.

Please don't easily give up because of the multiple files. Thank you.

Windows Visual Studio 2017 
Create a solution, throw headers files into header directory, and source files into source directory, and Graph.txt into executable program (debug) directory, click on build and run

Linux
g++ -o Homework3 Homework3.cpp graph.cpp process_graph.cpp pch.cpp
Put Graph.txt in the executable directory before run

