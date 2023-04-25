/*

			int counter = 0;
			while(ex < pNum){
				if(counter == sSize){
					for(int i = 1; i < Q.size(); i++){
						while(!Q[i].empty()){
							Process tmp = Q[i].front();
							Q[i].pop();
							Q[0].push(tmp);
							cout << "B, " << tmp.pName << ", Q" << qNum << endl;
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
							if(tmp.p_vector[0] == 1 && tmp.p_vector.size() == 1){
								QX.push_back(tmp);
								ex++;
								Q[i].pop();
								cout << "E, " << tmp.pName << ", QX" << endl;
							}
							else if(tmp.p_vector[0] == 0 && tmp.p_vector.size() == 1){
								QX.push_back(tmp);
								ex++;
								Q[i].pop();
								cout << "E, " << tmp.pName << ", QX" << endl;
							}
							else if(tmp.p_vector[0] == 1 && tmp.p_vector.size() != 1){
								Q[i].pop();
								int idx = i;
								if(i < Q.size() - 1){
									idx++;
								}
								tmp.p_vector.erase(tmp.p_vector.begin());
								Q[idx].push(tmp);
								cout << "1, " << tmp.pName << ", Q" << qNum - idx << endl; 
							}
							else if(tmp.p_vector[0] == 0 && tmp.p_vector.size() != 1){
								Q[i].pop();
								tmp.p_vector.erase(tmp.p_vector.begin());
								Q[i].push(tmp);
								cout << "0, " << tmp.pName << ", Q" << qNum - i << endl; 
							}
						}
					}
				}
			}

*/