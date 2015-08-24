#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

std::string change(int n) {
	std::stringstream convert;
	convert<<n;
	std::string s = convert.str();
	return s;
}

typedef int vertex_t;

// Declaration of helper functions
bool visited (vertex_t v, const std::vector<vertex_t> & path);
std::vector<vertex_t> rotate_to_smallest( std::vector<vertex_t> path);
std::vector<vertex_t> invert(std::vector<vertex_t> path);
bool isNew (std::vector< std::vector<vertex_t> > cycles, const std::vector<vertex_t> & path);


// Graph data structures
class edge {
public:
    vertex_t v1;
    vertex_t v2;
	bool chord;
	void chordCheck(vertex_t n);
    int has(vertex_t k);
    void write(std::string &out, std::fstream &file2);
};

void edge::chordCheck(vertex_t n) {
		if (v1 == v2+1 || v1+1 == v2) {
			chord = false;
		} else if (v1 == n && v2 == 0) {
			chord = false;
		} else if (v2 == n && v1 == 0) {
			chord = false;
		} else {chord = true;}
//		chord = false;
	}

int edge::has(vertex_t k){
    if ( (k==v1) || (k==v2) ) return 1;
    return 0;
}

void edge::write(std::string &out, std::fstream &file2){
    out+="[";
	out+=change(v1);
	out+= "-";
	out+=change(v2);
	out+="]";
}


class Graph {
public:
    int order;
    std::vector<int> cycSizes;
    
    int oddDupeFlag =0;

    int isUBPC;
    int hasOddCycle;
    int hasDupeCycleSize;
    
    std::vector< edge > edges;  // Graph should have a vector of vectors called edges
    void init(void);
    std::vector< std::vector<vertex_t> > findAllCycles();
    std::vector< int > findCycleSizes();
    void findNewCycles (std::vector< std::vector<vertex_t> > & cycles, std::vector<vertex_t> sub_path);
	std::vector<bool> degree(int last);
	std::vector<bool> degree2(int last);
    void write(std::string &out, std::fstream &file2);
    void writeChords(std::string &out, std::fstream &file2);
    
};

std::vector<bool> Graph::degree(int last) {
	int a = 0;
	std::vector<bool> vertices;
	while (a <= last) {
		int x = 0;
		int b = 0;
		while (b < edges.size()) {
			if (edges[b].has(a)) {
				x++;
			} b++;
		} if (x == 2) {
			vertices.push_back(true);
		} else {
			vertices.push_back(false);
		}
		a++;
	} a = 0;
	std::vector<bool> arcs;
	while (a < edges.size()) {
		if (vertices[edges[a].v1]) {
			arcs.push_back(false);
		} else {arcs.push_back(true);}
		a++;
	}
	return arcs;
}

std::vector<bool> Graph::degree2(int last) {
	int a = 0;
	std::vector<bool> vertices;
	while (a <= last) {
		int x = 0;
		int b = 0;
		while (b < edges.size()) {
			if (edges[b].has(a)) {
				x++;
			} b++;
		} if (x == 2) {
			vertices.push_back(true);
		} else {
			vertices.push_back(false);
		}
		a++;
	} a = 0;
	std::vector<bool> arcs;
	while (a < edges.size()) {
		if (vertices[edges[a].v2]) {
			arcs.push_back(false);
		} else {arcs.push_back(true);}
		a++;
	}
	return arcs;
}

void Graph::write(std::string &out, std::fstream &file2){
    for (auto e : edges){
        e.write(out, file2);
        out+= " ";
    }
    out+="\n";
}

void Graph::writeChords(std::string &out, std::fstream &file2){
    for (auto e : edges){
        if (e.chord){
            e.write(out, file2);
            out+=" ";
        }
    }
    out+="\n";
}



// The function below checks to see if a graph is uniquely bipancyclic.
//    If it is UBPC, it returns the set of cycles and sets isUBPC=1
//    If not, the set of edges is cleared and it sets hasOddCycle or hasDupeCycleSize equal to 1.
//       Note: once one of these is 1, it doesn't check the other.

std::vector< std::vector<vertex_t> > Graph::findAllCycles() // declares a method for the class Graph called findAllCycles.  It returns a vector of vectors
{
    cycSizes.clear();
  //  cycSizes.reserve(20);
    
    std::vector< std::vector<vertex_t> > cycles; // creates cycles: a vector of (vector of edges)

    
    for(auto edge : edges)
    {
        findNewCycles(cycles, std::vector<vertex_t>(1,edge.v1) );
        findNewCycles(cycles, std::vector<vertex_t>(1,edge.v2) );
    }
    
    
    
    int sum=0;
    for (int x:cycSizes){
        sum+=x;
    }

    //    if (sum == order/2 *(order/2+1) - 2 ) isUBPC = 1;
    
    return cycles;
}
// END OF findAllCycles()
//--------------------------


void Graph::findNewCycles (std::vector< std::vector<vertex_t> > &cycles, std::vector<vertex_t> sub_path )
{
  //    if (oddDupeFlag) return ;
        
    vertex_t start_node = sub_path[0];
    vertex_t next_node;
        
    // visit each edge and each node of each edge
    for(auto edge : edges)
    {
            if( edge.has(start_node) )
            {
                vertex_t node1 = edge.v1, node2 = edge.v2;
                
                if(node1 == start_node)
                    next_node = node2;
                else
                    next_node = node1;
                
                if( !visited(next_node, sub_path) )
                {
                    // neighbor node not on path yet
                    std::vector<vertex_t> sub;
                    sub.push_back(next_node);
                    sub.insert(sub.end(), sub_path.begin(), sub_path.end());
                    findNewCycles(cycles, sub);
                }
                else if( sub_path.size() > 2 && next_node == sub_path.back() )
                {
                    // cycle found
                    auto p = rotate_to_smallest(sub_path);
                    auto inv = invert(p);
                    
                    if( isNew(cycles, p) && isNew(cycles, inv) ){
                        // std::cout << "SIZE " << p.size() << std::endl; ;
                        int ps = (int) p.size();
                        if (ps % 2) {
                            oddDupeFlag=1;
                            cycSizes.clear();
                            cycSizes.push_back(-2);
                            hasOddCycle=1;
                        }

                        // check if ps is contained in cycSIzes
                        std::vector<int>::iterator first = cycSizes.begin();
                        std::vector<int>::iterator last = cycSizes.end();
                        bool psInCycSizes  = false;
                        while (first!=last) {  
    			    if (*first==ps) {
                                psInCycSizes = true;
                                break;
                            }                              
                            ++first;
                        }


                        if (psInCycSizes) {
                            // std::cout << "DUPE FOUND size " << p.size() << std::endl;;
                            oddDupeFlag=1;
                            cycSizes.clear();
                            cycSizes.push_back(-1);
                            hasDupeCycleSize=1;
                        }
                        cycles.push_back( p );
                        cycSizes.push_back((int) p.size());
                }
                                 
            }
        }
    }
}

//--------------------------------------------
//  Helper Functions
//  Rewritten to replace the std functions
//--------------------------------------------


// Returns true if v is contained in path
bool visited(vertex_t v, const std::vector<vertex_t> & path) {
    std::vector<vertex_t>::const_iterator first = path.begin();
    std::vector<vertex_t>::const_iterator last = path.end();
    while (first!=last) {  
    if (*first==v) return true;
        ++first;
    }
    return false;
}


// Rotates the elements in path until the smallest element is  
// the first element in the path. 
// Returns the rotated vector.
// The passed in vector remains unchanged. 

std::vector<vertex_t> rotate_to_smallest(std::vector<vertex_t> path) {
    // find the smallest vertex in path
    int first = 0;
    int last = path.size();
    int smallest = first;
    if (first != last) {
        while (++first != last) {
            if (path[first] < path[smallest]) {
                smallest = first;
            }
        }
    }

    // rotate such that the minimum is at the beginning
    first = 0;
    int next = smallest;
    while (first != next)
    {
        vertex_t tmp = path[first];
        path[first] = path[next];
        path[next] = tmp;
        ++first;
        ++next;

        if (next == last) next = smallest;
        else if (first == smallest) smallest = next;
    }

    return path;
}


// Reverses the elements in path and then rotates the elements until  
// the smallest element is the first element in the path. 
// Returns the rotated vector.
// The passed in vector remains unchanged. 

std::vector<vertex_t> invert(std::vector<vertex_t> path) {
    int first = 0;
    int last = path.size();
    while (first != last && first != --last) {
        vertex_t tmp = path[first];
        path[first] = path[last];
        path[last] = tmp;
        first++;
    }
    return rotate_to_smallest(path);
}
    

// Returns false if cycles contains a vector that has the same 
// content as vector path    

bool isNew (std::vector< std::vector<vertex_t> > cycles, const std::vector<vertex_t> & path){
    std::vector<std::vector<vertex_t>>::const_iterator first = cycles.begin();
    std::vector<std::vector<vertex_t>>::const_iterator last = cycles.end();
    while (first!=last) {  
        if (*first == path) {
            return false;
        }
        ++first;
    }
    return true;
}

std::vector<std::vector<int> > biPanCheck(std::vector<std::vector<int> > cycles, int num_arcs, std::vector<int> len, int order, int place, int sum, std::vector<bool> doub) {
	std::vector<std::vector<int> > solutions;
	if (place < len.size()-1) {
		int a = 0;
		if (doub[place]) {
			while (a+sum <= order) {
				std::vector<int> temp = len;
				temp[place]+=a;
				std::vector<std::vector<int> > out = biPanCheck(cycles, num_arcs, temp, order, place+1, sum+a, doub);
				int b = 0;
				while (b < out.size()) {
					solutions.push_back(out[b]);
					b++;
				} a+=8;
			}
		} else {
			std::vector<std::vector<int> > out = biPanCheck(cycles, num_arcs, len, order, place+1, sum, doub);
			int b = 0;
			while (b < out.size()) {
				solutions.push_back(out[b]);
				b++;
			}
		}
		return solutions;
	} else {
		if ((order-sum)%8 == 0) {
			std::vector<int> temp = len;
			temp[place]+=order-sum;
			int a = 0;
			std::vector<int> totals;
			while (a < cycles.size()) {
				int total = cycles[a][0];
				int b = 1;
				while (b < cycles[a].size()) {
					total+=temp[cycles[a][b]];
					b++;
					}
				if (total < 4) {
					return solutions;
				} if (total > order) {	
					return solutions;
				} b = 0;
				bool first = false;
				while (b < totals.size()) {
					if (total == totals[b]) {
						if (first) {
							return solutions;
						} else {
							first = true;
						}
					}b++;
				}
				totals.push_back(total);
				a++;
			}
			solutions.push_back(temp);
			return solutions;
		} else {
			return solutions;
		}
	}
}


std::vector<std::vector<int> > biECheck(std::vector<std::vector<int> > cycles, int num_arcs, std::vector<int> len, int order, int place, int &x, std::vector<bool> doub) {
	std::vector<std::vector<int> > solutions;
	if (place < len.size()) {
		int a = 0;
		int b = 4;
		if (doub[place]) {b = 8;} 
		while (a < b) {
			std::vector<int> temp = len;
			temp[place]+= a;
			x++;
			std::vector<std::vector<int> > out = biECheck(cycles, num_arcs, temp, order, place+1, x, doub);
			int b = 0;
			while (b < out.size()) {
				solutions.push_back(out[b]);
				b++;
			}a+=4;
		}
	} else {
		int a = 0;
		int zero = 0;
		int two = 0;
		int four = 0;
		int six = 0;
		int low = 0;
		int low2 = 0;
		int low3 = 0;
		while (a < cycles.size()) {
			int sum = cycles[a][0];
			int b = 1;
			while (b < cycles[a].size()) {
				sum+=len[cycles[a][b]];
				b++;
			}
			if (sum == 4) {
				low++;
			}
			if (sum == 6) {
				low2++;
			}
			if (sum == 8 || sum==0) {
				low3++;
			}
			if ((sum%8)==0) {zero++;}
			else if ((sum%8)==2) {two++;}
			else if ((sum%8)==4) {four++;}
			else if ((sum%8)==6) {six++;}
			a++;
		} if (((four == six && four == zero && (four == two || four == two+2)) || 
		(four == six && four == zero+2 && four == two+2) || 
		(four == six && four == zero+2 && four == two+2) ||
		(four == six+2 && four == zero+2 && four == two+2))
		&& low>=2 && low2>=2 && low3>=2) {
			int sum = 0;
			a = 0;
			while (a < len.size()) {
				sum+=len[a];
				a++;
			}
			if ((order-sum)%8==0) {
				std::vector<std::vector<int> > out = biPanCheck(cycles, num_arcs, len, order, 0, sum, doub);
				a = 0;
				while (a < out.size()) {
					solutions.push_back(out[a]);
					a++;
				}
			}
		}
	} return solutions;
}


std::vector<std::vector<int> > biFCheck(std::vector<std::vector<int> > cycles, int num_arcs, std::vector<int> len, int order, int place, int &x, std::vector<bool> doub) {
	std::vector<std::vector<int> > solutions;
	if (place < len.size()) {
		int a = 0;
		int b = 2;
		if (doub[place]) {b = 4;} 
		while (a < b) {
			std::vector<int> temp = len;
			temp[place]+= a;
			std::vector<std::vector<int> > out = biFCheck(cycles, num_arcs, temp, order, place+1, x, doub);
			int b = 0;
			while (b < out.size()) {
				solutions.push_back(out[b]);
				b++;
			}a+=2;
		}
	} else {
		int a = 0;
		int zero = 0;
		int two = 0;
		int four = 0;
		while (a < cycles.size()) {
			int sum = cycles[a][0];
			int b = 1;
			while (b < cycles[a].size()) {
				sum+=len[cycles[a][b]];
				b++;
			}
			if (sum == 4 || sum == 0) {
				four++;
			}
			if ((sum%4)==0) {zero++;}
			else if ((sum%4)==2) {two++;}
			a++;
		} if ((zero == two || zero == two+2) && four>=2) {
			int sum = 0;
			a = 0;
			while (a < len.size()) {
				sum+=len[a];
				a++;
			}
			int y = 0;
			if ((order-sum)%4==0) {
				x++;
				std::vector<std::vector<int> > out = biECheck(cycles, num_arcs, len, order, 0, y, doub);
				std::cout << "   sub-part " << x << " complete" << std::endl;
				a = 0;
				while (a < out.size()) {
					solutions.push_back(out[a]);
					a++;
				}
			}
		}
	} return solutions;
}

std::vector<std::vector<int> > biCheck(std::vector<std::vector<int> > cycles, int num_arcs, std::vector<int> len, int order, int &x, std::vector<bool> doub, std::vector<bool> undoub) {
	std::vector<std::vector<int> > solutions;
	if (len.size() < num_arcs) {
		std::vector<int> temp = len;
		if (undoub[len.size()]) {
			temp.push_back(0);
		} else {
			temp.push_back(4);
		}
		std::vector<std::vector<int> > out = biCheck(cycles, num_arcs, temp, order, x, doub, undoub);
		int a = 0;
		while (a < out.size()) {
			solutions.push_back(out[a]);
			a++;
		} temp = len;
		if (doub[len.size()]) {
			if (undoub[len.size()]) {
				temp.push_back(1);
			} else {
				temp.push_back(3);
			}
			out = biCheck(cycles, num_arcs, temp, order, x, doub, undoub);
			a = 0;
			while (a < out.size()) {
				solutions.push_back(out[a]);
				a++;
			}
		}
		return solutions;
	} else {
		int a = 0;
		bool valid = true;
		while (a < cycles.size()) {	
			int sum = cycles[a][0];
			int b = 1;
			while (b < cycles[a].size()) {
				sum+=len[cycles[a][b]];
				b++;
			}
			if (sum%2 == 1) {
				valid = false;
			}a++;
		}
		if (valid) {
			int sum = 0;
			a = 0;
			while (a < len.size()) {
				sum+=len[a];
				a++;
			}
			if (sum%2 == 0) {
				int y = 0;
				std::vector<std::vector<int> > out = biFCheck(cycles, num_arcs, len, order, 0, y, doub);
				x++;
				std::cout << "part " << x << " completed" << std::endl;
				a = 0;
				while (a < out.size()) {
					solutions.push_back(out[a]);
					a++;
				}
			}
		}
		return solutions;
	}
}
	
	
std::string run (std::vector<int> edges, std::string end, std::fstream &file2) {
	std::cout << "\n" << "begin graph" << std::endl;
	std::string out;
	Graph g;
	int a = 0;
	edge e2;
	vertex_t last = -1;
	while (a < edges.size()) {
		e2.v1 = edges[a];
		e2.v2 = edges[a+1];
		g.edges.push_back(e2);
		if (edges[a] > last) {
			last = edges[a];
		}
		if (edges[a+1] > last) {
			last = edges[a+1];
		}
		a++;
		a++;
	}
		int move = 0;
	while (move < g.edges.size()) {
		g.edges[move].chordCheck(last);
		move++;
	}

   
	out+="EDGES IN THE GRAPH\n\n";
	g.write(out, file2);  // List all the edges in the graph
	out+="\n";
    
    
    g.writeChords(out, file2);
    
    
    std::vector< std::vector<vertex_t> > listOfCycles;  // Create a vector that contains the list of all cycles in G.
    listOfCycles = g.findAllCycles(); // find all the cycles
        
	std::vector< int > lengthlist;
	for (auto i: listOfCycles){
	  lengthlist.push_back(i.size());
	}

	std::sort(lengthlist.begin(), lengthlist.end());
	std::vector<bool> doub = g.degree(last);
	std::vector<bool> undoub = g.degree2(last);

	std::string alpha = "abcdefghijklmnopqrstuvwxyz";
	a = 0;
	int num_red = 0;
	std::vector<std::vector<int> > formula;
	out+="Cycle Equations:\n";
	while (a < listOfCycles.size()) {
		std::vector<int> cycle;
		cycle.push_back(0);
		out+="cycle ";
		out+=change(a);
		out+=": ";
		int b = 0;
		int ChordCount = 0;
		int ArcCount = 0;
		while (b < listOfCycles[a].size()-1) {
			int c = 0;
			while (c < g.edges.size()) {
				if (g.edges[c].has(listOfCycles[a][b]) && g.edges[c].has(listOfCycles[a][b+1])) {
					if (g.edges[c].chord) {
						ChordCount++;
						cycle[0]++;
					} else {
						if (doub[c]) {
							out+=alpha[c];
							out+=" + ";
						} else {
							out+=alpha[c];
							out+="' + ";
						}
						cycle.push_back(c);
						ArcCount++;
					}
				}
				c++;
			} b++;
		}
		int c = 0;
		while (c < g.edges.size()) {
			if (g.edges[c].has(listOfCycles[a][b]) && g.edges[c].has(listOfCycles[a][0])) {
				if (g.edges[c].chord) {
					cycle[0]++;
					ChordCount++;
				} else {
					out+=alpha[c];
					out+=" + ";
					cycle.push_back(c);
					ArcCount++;
				}
			}
			c++;
		}
		formula.push_back(cycle);
		out+=change(ChordCount);
		out+="\n";
		a++;
	}
	int num_arcs = 0;
	a = 0;
	while (a < g.edges.size()) {
		if (!g.edges[a].chord) {
			num_arcs++;
		} a++;
	}
	a = 0;
	while (a < num_arcs) {
		if (!doub[a]) {
			num_red++;
		} a++;
	}
	std::vector<int> blank;
	int keep = 0;
	if (listOfCycles.size()%2 ==1) {
		out+="There is an odd number of cycles, so it cannot be 2-bipancyclic\n";
	} else {
		int order = listOfCycles.size();
		order = order+2;
		out+="The order is ";
		out+=change(order);
		out+= "\n";
//	std::cout << "num cycles " << listOfCycles.size() << " num arcs " << num_arcs << " num red " << num_red << std::endl;
		std::vector<std::vector<int> > bipans = biCheck(formula, num_arcs, blank, order, keep, doub, undoub);
		a = 0;
		if (bipans.size() > 0) {	
			out+= "This is a 2-bipancyclic graph\n";
		} else {
			out+="This is not a 2-bipancyclic graph\n";
		}
		while (a < bipans.size()) {
			int b = 0;
			while (b < bipans[a].size()) {
				out+=alpha[b];
				out+=" = ";
				out+=change(bipans[a][b]);
				out+="\n";
				b++;
			}
			out+="\n";
			a++;
		}
	}
	out+="end graph \n";
	out+=end;
	out+=" ";
	return out;
}

bool proper(std::string line) {
	int a = 0;
	while (a < line.size()) {
		if (!isdigit(line[a])) {
			return false;
		}a++;
	}
	return true;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
	vertex_t last = -1;

	std::fstream file1;
	std::string filename = argv[1];
	file1.open(filename, std::fstream::in);
	std::fstream file2;
	filename = argv[2];
	std::vector<int> edges;
	std::string line;
	bool inGraph = false;
	std::string out;
	while (file1 >> line) {
		if (!inGraph) {
			if (line == "0") {
				inGraph = true;
				edges.push_back(0);
			} else {
				out+=line;
				out+=" ";
				std::cout << line << " ";
			}
		} else if (proper(line)) {
			std::stringstream convert(line);
			int mid;
			convert >> mid;
			edges.push_back(mid);
		} else {
			inGraph = false;
			out+= run(edges, line, file2);
			std::cout << line << std::endl;
			edges.clear();
		}
	}
	if (inGraph) {
		file2.open(filename, std::fstream::out);
		out+= run(edges, " ", file2);
		file2 << out;
		file2.close();
	}

}

