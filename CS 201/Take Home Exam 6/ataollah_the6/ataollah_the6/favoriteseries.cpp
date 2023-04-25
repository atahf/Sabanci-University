#include "favoriteseries.h"

using namespace std;

FavoriteSeries::FavoriteSeries(string s1, string s2) {
	userName = UpperString(s1);
	userSurname = UpperString(s2);
}

string FavoriteSeries::getFullName() const {
	return userName + " " + userSurname;
}

void FavoriteSeries::addSeries(const series & S) {
	favSeries.push_back(S);
}

bool FavoriteSeries::ifExists(int seriesCode) {
	for (int i = 0; i < favSeries.size(); i++) {
		if (favSeries[i].seriesCode == seriesCode) {
			return true;
		}
	}
	return false;
}

bool FavoriteSeries::isListEmpty() const {
	if (favSeries.size() == 0) {
		return true;
	}
	return false;
}

void FavoriteSeries::dropSeriesSorted(int seriesCode) {
	series temp;
	int pos, end = favSeries.size() - 1;
	bool _continue = true;
	for (int i = 0; i < favSeries.size() && _continue; i++) {
		if (favSeries[i].seriesCode == seriesCode) {
			pos = i;
			_continue = false;
		}
	}
	temp = favSeries[pos];
	favSeries[pos] = favSeries[end];
	favSeries[end] = temp;
	favSeries.pop_back();
}

void FavoriteSeries::displayFavoriteList() const {
	vector<TV_shows> tvShows;
	TV_shows temp;
	for (int i = 0; i < favSeries.size(); i++) {
		for (int j = 0; j < favSeries[i].episodes.size(); j++) {
			temp.actors = favSeries[i].actorNames;
			temp.code = favSeries[i].seriesCode;
			temp.date = favSeries[i].episodes[j].date;
			temp.nameS = favSeries[i].seriesName;
			temp.nameE = favSeries[i].episodes[j].episodeName;
			temp.slot = favSeries[i].episodes[j].slot;
			tvShows.push_back(temp);
		}
	}
	sort(tvShows);
	cout << "CODE,SERIES,EPISODE,DATE,SLOT" << endl;
	for (int i = 0; i < tvShows.size(); i++) {
		cout << tvShows[i].code << "," << tvShows[i].nameS << "," << tvShows[i].nameE << "," << tvShows[i].date << "," << tvShows[i].slot << endl;
	}
}

void FavoriteSeries::displayFavoriteActors() const {
	int temp, minIndex, numElts;
	string tempS, step;
	vector<string> tmp, actors;
	vector<int> counts;
	for (int x = 0; x < favSeries.size(); x++) {
		for (int y = 0; y < favSeries[x].actorNames.size(); y++) {
			step = UpperString(favSeries[x].actorNames[y]);
			StripPunc(step);
			StripWhite(step);
			tmp.push_back(step);
		}
	}
	for (int e = 0; e < tmp.size(); e++) {
		bool exist = false;
		for (int i = 0; i < actors.size() && !exist; i++) {
			if (actors[i] == tmp[e]) {
				counts[i]++;
				exist = true;
			}
		}
		if (exist == false) {
			actors.push_back(tmp[e]);
			counts.push_back(1);
		}
	}
	numElts = actors.size();
	for (int k = 0; k < numElts - 1; k++) {
		minIndex = k;
		for (int j = k + 1; j < numElts; j++)
			if (counts[j] > counts[minIndex])
				minIndex = j;
			else if (counts[j] == counts[minIndex] && actors[j] < actors[minIndex])
				minIndex = j;
		temp = counts[k];
		counts[k] = counts[minIndex];
		counts[minIndex] = temp;
		tempS = actors[k];
		actors[k] = actors[minIndex];
		actors[minIndex] = tempS;
	}
	for (int n = 0; n < actors.size(); n++) {
		cout << actors[n] << ": " << counts[n] << endl;
	}
}

bool FavoriteSeries::isConflict(const series & S) const {
	for (int i = 0; i < favSeries.size(); i++) {
		for (int j = 0; j < favSeries[i].episodes.size(); j++) {
			for (int k = 0; k < S.episodes.size(); k++) {
				if (favSeries[i].episodes[j].date == S.episodes[k].date && favSeries[i].episodes[j].slot == S.episodes[k].slot) {
					return true;
				}
			}
		}
	}
	return false;
}

string FavoriteSeries::getSeriesName(int code) const {
	for (int i = 0; i < favSeries.size(); i++) {
		if (favSeries[i].seriesCode == code) {
			return favSeries[i].seriesName;
		}
	}
	return "Not Found!";
}

void FavoriteSeries::sort(vector<TV_shows> & a) const {
	int j, k, minIndex, numElts = a.size();
	string order = "MORNING AFTERNOON EVENING NIGHT";
	TV_shows temp;
	for (k = 0; k < numElts - 1; k++) {
		minIndex = k;
		for (j = k + 1; j < numElts; j++)
			if (a[j].date < a[minIndex].date)
				minIndex = j;
			else if (a[j].date == a[minIndex].date && order.find(a[j].slot) < order.find(a[minIndex].slot))
				minIndex = j;
			else if (a[j].date == a[minIndex].date && order.find(a[j].slot) == order.find(a[minIndex].slot) && a[j].code < a[minIndex].code)
				minIndex = j;
		temp = a[k];
		a[k] = a[minIndex];
		a[minIndex] = temp;
	}
}