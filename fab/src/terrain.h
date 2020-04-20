/* terrain.h
   Code based on disjoint set class from James Plank
   The actual terrain is stored in an int vector in main.cpp
   0 = air
   1 = dirt
 */

#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"
#include "player.h"

using namespace std;

class Player;

/* The Disjoint Set API is implemented as a c++ interface, 
   because I am implementing it three ways.  Each subclass
   implementation is in its own cpp file. */

class DisjointSet {
	public:
	virtual ~DisjointSet(){};
	virtual int Union(int s1, int s2) = 0;
	virtual int Find(int element) = 0;
	//virtual void Print() = 0;
};

/* The first subclass implements Union-by-Size. */

class DisjointSetBySize : public DisjointSet {
	public:
	DisjointSetBySize(int nelements);
	int Union(int s1, int s2);
	int Find(int element);
	//void Print();

	//Terrain functions
	/*void generateTerrain(const Player p[], vector<int> &l);
	void genTer(vector<int> &l, vector<int> &toVisit, int &playerUnion, const int &playerCount, DisjointSet *u, const vector<int> &playerPositions);*/

	protected:
	vector<int> links;
	vector<int> sizes;
};

class Terrain {
	public:
	virtual ~Terrain() {
		l.clear();
	};
	Terrain(int x, int y) {
		l.resize(x * y, 1);
		levWidth = x;
		levHeight = y;
	}
	Terrain() {
	}
	void genTer(vector<int>& toVisit, int playerUnion, int playerCount, DisjointSet* u, const vector<int>& playerPositions);
	void generateTerrain(Player* p[4], int playerCount);
	void printLevel();
	int getValueAtIndex(int index) const;
	int getValueAtXY(int x, int y) const;
	void setValueAtIndex(int index, int value);
	void setValueAtXY(int x, int y, int value);

	private:
	vector<int> l;  //level
	int levWidth;
	int levHeight;
};

#endif