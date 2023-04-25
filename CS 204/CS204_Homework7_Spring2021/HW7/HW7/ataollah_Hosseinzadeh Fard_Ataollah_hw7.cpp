#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <time.h>
#include <chrono>
#include <iomanip>
#include "ataollah_Hosseinzadeh Fard_Ataollah_hw7_HW7DynIntQueue.h"

using namespace std;

mutex mtxF, mtxP, mtxCout;
//any place that filling queue is editing, mtxF is used
//any place that packing queue is editing, mtxP is used
//any place that doing cout, mtxCout is used

HW7DynIntQueue Q_FILLING, Q_PACKING;

int TOTAL_BOX_COUNT, CREATED = 0, FILLED = 0, PACKED = 0; 

int random_range(const int & min, const int & max) {
	static mt19937 generator(time(0));
	uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}
//prints current local time in HH:MM:SS format
void getTime() {
	time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()); 
	struct tm *ptm = new struct tm; 
	localtime_s(ptm, &tt); 
	cout << put_time(ptm, "%X") << endl;
}

//creates boxes in total amount
void produce(int min, int max) {
	//until number of created boxes are the same with total number runs loop.
	while(CREATED < TOTAL_BOX_COUNT) {
		this_thread::sleep_for(chrono::seconds(random_range(min, max)));

		mtxF.lock();					//to avoid any problem with other running fill functions
		Q_FILLING.enqueue(++CREATED);	//first increaments the created number and then creates a box (added a box to filling queue)
		mtxF.unlock();

		mtxCout.lock();					//to avoid any cout mixture
		cout << "Producer has enqueued a new box " << CREATED << " to filling queue (filling queue size is " << Q_FILLING.getCurrentSize() << "): ";
		getTime();
		mtxCout.unlock();
	}
}

void fill(int id, int min, int max) {
	int tmp;
	//until number of filled boxes are the same with total number runs loop.
	while(FILLED < TOTAL_BOX_COUNT) {
		mtxF.lock();					//to avoid any problem with other running produce and fill functions
		if(!Q_FILLING.isEmpty()) {
			Q_FILLING.dequeue(tmp);		//a single box removed from filled queue
			FILLED++;					//increamented filled number to update the condition of loop
			mtxF.unlock();

			mtxCout.lock();				//to avoid any cout mixture
			cout << "Filler " << id << " started filling the box " << tmp << " (filling queue size is " << Q_FILLING.getCurrentSize() << "): ";
			getTime();
			mtxCout.unlock();

			this_thread::sleep_for(chrono::seconds(random_range(min, max)));

			mtxP.lock();
			Q_PACKING.enqueue(tmp);		//single box added to packing queue
			mtxP.unlock();

			mtxCout.lock();				//to avoid any cout mixture
			cout << "Filler " << id << " finished filling the box " << tmp << ": ";
			getTime();
			cout << "Filler " << id << " put box " << tmp << " into packaging queue (packaging queue size is " << Q_PACKING.getCurrentSize() << "): ";
			getTime();
			mtxCout.unlock();
		}
		else
			mtxF.unlock();
	}
}

void pack(int id, int min, int max) {
	int tmp;
	//until number of packed boxes are the same with total number runs loop.
	while(PACKED < TOTAL_BOX_COUNT) {
		mtxP.lock();					//to avoid any problem with other running fill and pack functions
		if(!Q_PACKING.isEmpty()) {
			Q_PACKING.dequeue(tmp);		//a box removed from packing queue
			PACKED++;					//increamented packed number to update the condition of loop
			mtxP.unlock();

			mtxCout.lock();				//to avoid any cout mixture
			cout << "Packager " << id << " started packaging the box " << tmp << " (packaging queue size is " << Q_PACKING.getCurrentSize() << "): ";
			getTime();
			mtxCout.unlock();

			this_thread::sleep_for(chrono::seconds(random_range(min, max)));

			mtxCout.lock();				//to avoid any cout mixture
			cout << "Packager " << id << " finished packaging the box " << tmp << ": ";
			getTime();
			mtxCout.unlock();
		}
		else
			mtxP.unlock();
	}
}

int main() {
	cout << "Please enter the total number of items: ";
	cin >> TOTAL_BOX_COUNT;
	
	int min_production, max_production;
	cout << "Please enter the min-max waiting time range of producer:\nMin: ";
	cin >> min_production;
	cout << "Max: ";
	cin >> max_production;
	
	int min_filling_time, max_filling_time;
	cout << "Please enter the min-max waiting time range of filler workers:\nMin: ";
	cin >> min_filling_time;
	cout << "Max: ";
	cin >> max_filling_time;
	
	int min_packaging_time, max_packaging_time;
	cout << "Please enter the min-max waiting time range of packager workers:\nMin: ";
	cin >> min_packaging_time;
	cout << "Max: ";
	cin >> max_packaging_time;
	
	cout << "Simulation starts ";
	getTime();

	thread producer, workerF[2], workerP[2];

	producer = thread(&produce, min_production, max_production);			//producer thread assigned and begins to run

	workerF[0] = thread(&fill, 1, min_filling_time, max_filling_time);		//filler worker 1 thread assigned and begins to run
	workerF[1] = thread(&fill, 2, min_filling_time, max_filling_time);		//filler worker 2 thread assigned and begins to run

	workerP[0] = thread(&pack, 1, min_packaging_time, max_packaging_time);	//packer worker 1 thread assigned and begins to run
	workerP[1] = thread(&pack, 2, min_packaging_time, max_packaging_time);	//packer worker 2 thread assigned and begins to run
	
	//all threads first checked whether they are joinable to avoid any compiler issues encountered during initializing threads

	if(producer.joinable())
		producer.join();

	if(workerF[0].joinable())
		workerF[0].join();
	if(workerF[1].joinable())
		workerF[1].join();

	if(workerP[0].joinable())
		workerP[0].join();
	if(workerP[1].joinable())
		workerP[1].join();
	
	cout << "End of the simulation ends: ";
	getTime();

	return 0;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610