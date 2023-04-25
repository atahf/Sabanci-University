#ifndef favoriteseriest_h
#define favoriteseries_h

/******************************************************************** 

 Models a user watching TV Series in a particular schedule 

 This code is freely distributable and modifiable providing you
 leave this notice in it.

 Copyright @ Anil

********************************************************************/


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "date.h"
#include "strutils.h"
using namespace std;

struct episode 
{
	string episodeName;
	int seriesCode;
	Date date;
	string slot;
};

struct series
{
    string seriesName;
    int seriesCode;
    vector<episode> episodes;
	vector<string> actorNames;
};

struct TV_shows //custumized struct to make printing easier!!
{
	int code;
	string nameS, nameE, slot;
	vector<string> actors;
	Date date;
};

class FavoriteSeries
{
	public:
		FavoriteSeries(string, string);		// constructor with parameter
    
		string getFullName() const;			// returns the full name (name&surname) of the user 
		void addSeries(const series &);		// add a TV series to the favorite list
		bool ifExists(int seriesCode);		// check if a series is in the favorite list or not
		bool isListEmpty() const;			// return trues if the favorite list is empty
		void dropSeriesSorted(int);			// drops the TV series with the given TV series code 
											// from the vector (favSeries) while keeping the vector sorted
		void displayFavoriteList() const;		// displays the favorite TV series list
		void displayFavoriteActors() const;		// calculates and displays the sorted favorite actors list
		bool isConflict(const series &) const;	// returns true if a given TV series has a time conflict
												// with another series from the user's schedule
			 // time conflict means same date and same time slot among (morning, afternoon, evening, night)
		
		// if needed you may add more functions here
		string getSeriesName(int) const;
		void sort(vector<TV_shows> &) const;

	private:
		string userName;
		string userSurname;
		vector<series> favSeries;
};

#endif
