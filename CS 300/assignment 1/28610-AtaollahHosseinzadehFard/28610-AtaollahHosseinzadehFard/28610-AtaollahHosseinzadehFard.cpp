#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

struct Process {
	string pName;
	queue<int> pQueue;

	Process() {};
	Process(string name, queue<int> q): pName(name), pQueue(q) {};
};

class MLFQ {
	public:
		MLFQ() {};

		void initialize() {
			cout << "Please enter the process folder name: ";
			cin >> folderName;
			eCounter = 0;
			string configFileName = folderName + "/configuration.txt";
			ifstream config;
			config.open(configFileName.c_str());
			config >> qNum >> pNum >> sSize;
			config.close();

			for(int i = 0; i < qNum; i++) {
				queue<Process> tmpQ;
				Q.push_back(tmpQ);
			}

			for(int i = 1; i <= pNum; i++) {
				queue<int> tmp;
				string pFileName = folderName + "/p" + to_string(i) + ".txt";
				ifstream pFile;
				pFile.open(pFileName.c_str());
				string line;
				while(getline(pFile, line))
					if(line != "-")
						tmp.push(stoi(line));
				pFile.close();
				Process pq("PC" + to_string(i), tmp);
				Q[0].push(pq);
			}
		}

		void run() {
			ofstream outFile(folderName + "/output.txt");
			int counter = 0;
			while(eCounter < pNum){
				if(counter == sSize){
					for(int i = 1; i < Q.size(); i++){
						while(!Q[i].empty()){
							Process tmp = Q[i].front();
							Q[i].pop();
							Q[0].push(tmp);
							outFile << "B, " << tmp.pName << ", Q" << qNum << endl;
						}
					}
					counter = 0;
				}
				else{
					for(int i = 0; i < Q.size(); i++){
						while(!Q[i].empty()){
							if (counter == sSize)
								break;
							counter++;
							Process tmp = Q[i].front();
							if(tmp.pQueue.size() == 1){
								QX.push_back(tmp);
								eCounter++;
								Q[i].pop();
								outFile << "E, " << tmp.pName << ", QX" << endl;
							}
							else if(tmp.pQueue.front() == 1 && tmp.pQueue.size() != 1){
								Q[i].pop();
								int idx = i;
								if(i < Q.size() - 1)
									idx++;
								tmp.pQueue.pop();
								Q[idx].push(tmp);
								outFile << "1, " << tmp.pName << ", Q" << qNum - idx << endl; 
							}
							else if(tmp.pQueue.front() == 0 && tmp.pQueue.size() != 1){
								Q[i].pop();
								tmp.pQueue.pop();
								Q[i].push(tmp);
								outFile << "0, " << tmp.pName << ", Q" << qNum - i << endl; 
							}
						}
					}
				}
			}
			outFile.close();
			cout << "When all processes are completed, you can find execution sequence in \"" << folderName << "/output.txt\"." << endl;
		}

	private:
		string folderName;
		int qNum, pNum, sSize, eCounter;
		vector<queue<Process>> Q;			//the main Qs (index 0 high - last index low priority)
		vector<Process> QX;					//the trash vector which stores the exterminated Processes
};

int main() {
	MLFQ mlfq;
	mlfq.initialize();
	mlfq.run();
	return 0;
}