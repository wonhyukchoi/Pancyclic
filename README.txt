This program is based off of a program that my professor gave me, based off of code found at http://stackoverflow.com/questions/12367801/finding-all-cycles-in-undirected-graphs, and modified by Dr. David Leach. The code I was given(has, findAllCycles, findNewCycles, visited, rotate_to_smallest, invert, isNew) was intended to find all cycles in a graph
I then wrote additional code (biPanCheck, biECheck, biFCheck, biCheck, degree, degree2, chordCheck) to test possible combinations of edge lengths to search for n-pancyclic, n-bipancyclic, and n-oddly bipancyclic graphs. 
I also re-wrote most of the code (change, proper, main, run, write, writeChords) to accept input and give output.

These programs find look for graphs that are members of families of graphs such as uniquely pancyclic graphs. A uniquely pancyclic graph is a graph of order k that includes exactly one cycle of length i for each i from 3 to k.
These also look for n-pancyclic graphs (graphs of order k that include exactly n cycles of length i for each i from 3 to k.)
	n-bipancyclic graphs (graphs of order k that include exactly n cycles of length i for each even i from 4 to k.)
	and n-oddly bipancyclic graphs (graphs of order k that include exactly n cycles of length i for each even i from 4 to k-1.)

Because these graphs are defined entirely by their cycle lengths, it is possible to describe groups of graphs by replacing every path with a single edge with a variable length. These programs accept as input a list of this type of groups of graphs, and outputs which groups contain a graph in the relevant category.
Because n-pancylic graphs and n-bipancyclic graphs must have a hamiltonian cycle, an n-oddly bipancyclic graphs must have a near-hamiltonian cycle, the programs consider all possible combinations of lengths of the edges that make up the hamiltonian cycle, and assume all the chords are of length 1.

These programs require C++ 11. In gcc, they should be compiled with the command
g++ -std=c++11 filename.cpp -o name.exe
Each of these programs should be given two text files as initial arguments, an input file and an output file.
These files are parsed as a sequence of words, separated by whitespace. Any collection of characters not separated by whitespace are seen as a single word, and whitespace is otherwise ignored.
Each sees a graph as a sequence of pairs of numbers. Each reads a graph starting with a 0, and ending with a word that is not a number.
No graph should have any identical edges. To avoid this, add additional vertices to break up the redundant arc.
All code may behave oddly when it comes to arcs of length 0. It will consider an arc of length 0, but will not recognize when that changes what cycle patterns there are. Therefore, it is at times necessary to manually input a chord pattern for when arcs are length 0.
Additionally, in OddlyBipancyclic, 2OddlyBipancyclic, and nOddlyBipancyclic, it will consider two cycles made identical because an arc is of length 0 to be two distinct cycles.
Therefore, some graphs will be found which the program thinks are valid, but which are not valid. Check over any solutions it does find to ensure they are correct.

Pancyclic.cpp/2pancyclic.cpp
Finds uniquely pancyclic graphs/Finds 2-pancyclic graphs
Graphs should be written with the hamiltonian cycle in sequential order, with all arcs in increasing order except for the final arc

Bipancyclic.cpp/2Bipancyclic.cpp
Finds uniquely bipancyclic graphs/Finds 2bipancyclic graphs
Graphs should be written with the hamiltonian cycle in sequential order, with all arcs in increasing order except for the final arc

OddlyBipancyclic.cpp/2OddlyBipancyclic.cpp
Finds uniquely oddly bipancyclic graphs/Finds 2-oddly bipancyclic graphs
Graphs should be written with the near-hamiltonian cycle in sequential order, with all arcs in increasing order except for the final arc. The external point should have a value one higher then the highest hamiltonian point.

nPancyclic.cpp
Finds n-pancyclic graphs
The very first word should be a number, which will be n
Graphs should be written with the hamiltonian cycle in sequential order, with all arcs in increasing order except for the final arc

nBipancyclic.cpp
Finds n-bipancyclic graphs
The very first word should be a number, which will be n
Graphs should be written with the hamiltonian cycle in sequential order, with all arcs in increasing order except for the final arc

nOddlyBipancyclic.cpp
Finds n-oddly bipancyclic graphs
The very first word should be a number, which will be n
Graphs should be written with the near-hamiltonian cycle in sequential order, with all arcs in increasing order except for the final arc. The external point should have a value one higher then the highest hamiltonian point.