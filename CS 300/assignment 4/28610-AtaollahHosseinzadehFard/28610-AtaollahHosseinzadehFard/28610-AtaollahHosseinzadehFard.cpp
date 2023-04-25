/*
	Ataollah Hosseinzadeh Fard		ID: 28610		CS300 HW4
	some parts of this code's algorithm are inspired from codes in recitation.
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <vector>
#include <unordered_map>

#define INF 2<<22

using namespace std;

//finds minimum of a and b
int min(int a, int b) {
	return (a > b)? b: a;
}

//finds minimum of a and b and c
int min(int x, int y, int z) {
	return min(min(x, y), z); 
}

//finds the amount of changes needed to get to str2 from str1
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

/*
	for the str1 and str2 which they have one change 
	difference, classifies that difference and returns it.
*/
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
						return "insert " + str2.substr(i, 1) + " after position " + to_string(i);	
					}
				}
				return "insert " + str2.substr(str2.length() - 1, 1) + " after position " + to_string(str2.length() - 1);
			}
			else {
				for(int i = 0; i < str2.length(); i++) {
					if(str1.at(i) != str2.at(i)) {
						return "delete " + str1.substr(i, 1) + " at position " + to_string(i + 1);
					}
				}
				return "delete " + str1.substr(str1.length() - 1, 1) + " at position " + to_string(str1.length());
			}
		}
	}
	return "";
}
 
//a tool struct which is inspired from recitation
struct node{
	int v, d;

	node() {};
	node(int e, int dest = INF): v(e), d(dest) {};
};

class Graph{
	private:
		vector<string> words;
		unordered_map<int, vector<int>> graph;

		bool singleMisMatch(string a, string b) const;
		bool addVertex(int e);
		void addEdge(int a, int b);
		int getIdx(const string s) const;

	public:
		Graph(const vector<string> v);

		bool exists(const string s) const;
		int bfs(string start, string dest);
		void getPath(string start, string end);
};

Graph::Graph(const vector<string> v): words(v) {
	for(int i = 0; i < words.size(); i++)
		for(int j = 0; j < words.size(); j++)
			if(i != j)
				if(singleMisMatch(words[i], words[j]))
					addEdge(i, j);
}

bool Graph::addVertex(int e) {
	if(graph.find(e) != graph.end())
		return false;
	vector<int> adjLst;
	graph[e] = adjLst;
	return true;
};

bool Graph::exists(const string s) const {
	for(string e: words)
		if(e == s)
			return true;
	return false;
};

void Graph::addEdge(int a, int b) {
	if(graph.find(a) == graph.end())
		addVertex(a);
	if(graph.find(b) == graph.end())
		addVertex(b);
	graph[a].push_back(b);
};

//returns true if a and b has one change difference
bool Graph::singleMisMatch(string a, string b) const {
	return editDist(a, b, a.length(), b.length()) == 1;
};

int Graph::getIdx(const string s) const {
	int res = 0;
	for(string e: words) {
		if(e == s)
			return res;
		else
			res++;
	}
	return -1;
};

//calculates minimum distance of dest from start
//usage of dequeue inspired from recitation
int Graph::bfs(string start, string dest) {
	int s = getIdx(start), d = getIdx(dest);
	deque<node> Q;
	vector<bool> visited(words.size(), false);
	Q.push_front(node(s, 0));
	while(!Q.empty()) {
		node currentN = Q.back();
		int current = currentN.v, currentDist = currentN.d;
		Q.pop_back();
		visited[current] = true;
		for(int i = 0; i < graph[current].size(); i++) {
			if(visited[graph[current][i]] == false) {
				if(graph[current][i] == d)
					return currentDist + 1;
				else
					Q.push_front(node(graph[current][i], currentDist + 1));
			}
		}
	}
	return -1;
}

//print shortest path by comparing bfs distances of each vertex
void Graph::getPath(string start, string end) {
	int maxDist = bfs(start, end);
	int startPos = getIdx(start);
	int currentPos = getIdx(end);
	string output = "";
	for(int i = maxDist; i >= 0; i--) {
		for(int e: graph[currentPos]) {
			if(bfs(start, words[e]) == i - 1) {
				output = words[currentPos] + " (" + getEditDist(words[e], words[currentPos]) + ")\n" + output ;
				currentPos = e;
				break;
			}
		}
	}
	output = start + "\n" + output;
	cout << output << endl;
}

int main() {
	vector<string> words;
	string tmp;
	ifstream inFile;
	inFile.open("words.txt");
	while (inFile >> tmp)
		words.push_back(tmp);
	inFile.close();

	Graph graph(words);
	string A, B;
	bool _continue = true;
	while(_continue) {
		cout << "Enter A and B: ";
		cin >> A >> B;
		if(A.at(0) == '*') {
			_continue = false;
		}
		else {
			cout << endl;
			if(!graph.exists(A) && !graph.exists(B)) {
				cout << "both \"" <<  A << "\" and \"" << B << "\" are not in wordlist" << endl;
			}
			else if(!graph.exists(A)) {
				cout << "\"" << A << "\" is not in wordlist" << endl;
			}
			else if(!graph.exists(B)) {
				cout << "\"" << B << "\" is not in wordlist" << endl;
			}
			else{
				if(graph.bfs(A, B) == -1) {
					cout << "there is no path between \"" <<  A << "\" and \"" << B << "\"" << endl;
				}
				else {
					graph.getPath(A, B);
				}
			}
			cout << endl << endl;
		}
	}
	cout << "\nProgram Terminated." << endl;
	return 0;
}