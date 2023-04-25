#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

#define INF 2<<22

using namespace std;

int min(int a, int b) {
	return (a > b)? b: a;
}

int min(int x, int y, int z) {
	return min(min(x, y), z); 
}
  
int editDist(string str1, string str2, int m, int n) {
	if (m == 0)
		return n;
	if (n == 0)
		return m;
	if (str1[m - 1] == str2[n - 1])
		return editDist(str1, str2, m - 1, n - 1);
	return 1 + min(editDist(str1, str2, m, n - 1),		// Insert
				   editDist(str1, str2, m - 1, n),		// Remove
			       editDist(str1, str2, m - 1, n - 1)); // Replace
}

string getEditDist(const string str1, const string str2) {
	if(editDist(str1, str2, str1.length(), str2.length()) == 1) {
		if(str1.length() == str2.length()) {
			for(int i = 0; i < str1.length(); i++) {
				if(str1.at(i) != str2.at(i)) {
					return "change " + str1.substr(i, 1) + " at position " + to_string(i + 1) + " to " + str2.substr(i, 1);
				}
			}
		}
		else {
			if(str1.length() < str2.length()) {
				for(int i = 0; i < str1.length(); i++) {
					if(str1.at(i) != str2.at(i)) {
						return "insert " + str2.substr(i, 1) + " after position " + to_string(i + 1);	
					}
				}
				return "insert " + str2.substr(str2.length() - 1, 1) + " after position " + to_string(str2.length());
			}
			else {
				for(int i = 0; i < str2.length(); i++) {
					if(str1.at(i) != str2.at(i)) {
						return "delete " + str1.substr(i, 1) + " sfter position " + to_string(i + 1);
					}
				}
				return "delete " + str1.substr(str1.length() - 1, 1) + " after position " + to_string(str1.length());
			}
		}
	}
	return "";
}

struct vertex{
	int end, dist, path;
	bool known;
	vector<int> adjV;

	vertex() {};
	vertex(int e, int d = INF, int p = INF, bool k = false): end(e), dist(d), path(p), known(k) {};
};

bool operator!=(vertex rhs, vertex lhs) {
	return (rhs.adjV != lhs.adjV ||
			rhs.dist != lhs.dist ||
			rhs.end != lhs.end ||
			rhs.known != lhs.known ||
			rhs.path != lhs.path);
}

class Graph{
	private:
		int currentSize;
		vector<string> words;
		vector<vertex> graph;
		unordered_map<int, string> map;

		void reset();
		bool singleMisMatch(string a, string b);
		void Dijkstra(vertex v);
		void getPath(string &trace, vertex v);

	public:
		Graph(const vector<string> v);

		bool exists(const string s) const;
		void findPath(const string s, const string d);
};

Graph::Graph(const vector<string> v) {
	currentSize = 0;
	words = v;
	for(string s: v) {
		if(map.find(currentSize) == map.end()) {
			map[currentSize] = s;
			vertex newV(currentSize, 0);
			graph.push_back(newV);
			currentSize++;
		}
	}
	for(int i = 0; i < currentSize; i++) {
		for(int j = 0; j < currentSize; j++) {
			if(i != j) {
				string A = map[i], B = map[j];
				if(singleMisMatch(A, B)) {
					graph[i].adjV.push_back(j);
				}
			}
		}
	}
}

bool Graph::exists(const string s) const {
	for(string w: words)
		if(w == s)
			return true;
	return false;
}

bool Graph::singleMisMatch(string a, string b) {
	return editDist(a, b, a.length(), b.length()) == 1;
};

void Graph::reset(){
	for(int e = 0; e < currentSize; e++) {
		graph[e].dist = INF;
		graph[e].path = INF;
		graph[e].known = false;
	}
}

void Graph::Dijkstra(vertex v){
	graph[v.end].dist = 0;
	graph[v.end].known = true;
	graph[v.end].path = v.end;
	int marked = 1;
	for(int e: v.adjV) {
		graph[e].dist = 1;
		graph[e].path = v.end;
		graph[e].known = true;
		marked++;
	}
	while(marked < currentSize) {
		for(int i = 0; i < currentSize; i++) {
			if(!graph[i].known) {
				int minD = INF, idxV = INF;
				for(int a: graph[i].adjV) {
					if(graph[a].dist < minD) {
						minD = graph[a].dist;
						idxV = a;
					}
				}
				graph[i].dist = minD;
				graph[i].path = idxV;
				graph[i].known = true;
				marked++;
			}
		}
	}
}

void Graph::getPath(string &trace, vertex v){
	if (v.path != INF && graph[v.path] != v) {
		getPath(trace, graph[v.path]);
		trace += "\n";
	}
	trace += words[v.end];
	if(v.path != INF && v.path != v.end)
		trace += " (" + getEditDist(words[v.path], words[v.end]) + ")";
}

void Graph::findPath(const string start, const string destination) {
	if(exists(start) && exists(destination)) {
		int idxS = 0, idxD = 0;
		for(; idxS < currentSize; idxS++)
			if(words[idxS] == start)
				break;
		for(; idxD < currentSize; idxD++)
			if(words[idxD] == destination)
				break;
		reset();
		Dijkstra(graph[idxS]);
		string path = "";
		getPath(path, graph[idxD]);
		if(path.length() == start.length())
			cout << "no path possible between \"" << start << "\" and \"" << destination << "\"." << endl;
		else
			cout << path << endl;
	}
}

int main() {
	vector<string> words;
	string tmp;
	ifstream inFile;
	inFile.open("words.txt");
	while (inFile >> tmp)
		words.push_back(tmp);
	inFile.close();

	Graph adjLst(words);
	string A, B;
	bool _continue = true;
	while(_continue) {
		cout << "A B: ";
		cin >> A >> B;
		if(A.at(0) == '*') {
			_continue = false;
		}
		else {
			cout << endl;
			if(!adjLst.exists(A) && !adjLst.exists(B)) {
				cout << "both \"" <<  A << "\" and \"" << B << "\" are not in wordlist" << endl;
			}
			else if(!adjLst.exists(A)) {
				cout << "\"" << A << "\" is not in wordlist" << endl;
			}
			else if(!adjLst.exists(B)) {
				cout << "\"" << B << "\" is not in wordlist" << endl;
			}
			else {
				adjLst.findPath(A, B);
			}
			cout << endl << endl;
		}
	}
	cout << "\nProgram Terminated." << endl;
	return 0;
}