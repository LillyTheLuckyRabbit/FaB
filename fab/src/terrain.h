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

/* The Disjoint Set API is implemented as a c++ interface, 
   because I am implementing it three ways.  Each subclass
   implementation is in its own cpp file. */

class DisjointSet {
  public:
    virtual ~DisjointSet() {};
    virtual int Union(int s1, int s2) = 0;
    virtual int Find(int element) = 0;  
    virtual void Print() = 0;
};

/* The first subclass implements Union-by-Size. */

class DisjointSetBySize : public DisjointSet {
  public:
    DisjointSetBySize(int nelements);
    int Union(int s1, int s2);
    int Find(int element); 
    void Print();

  protected:
    vector <int> links;
    vector <int> sizes;
};

/* The second subclass implements Union-by-Height. */

class DisjointSetByHeight : public DisjointSet {
  public:
    DisjointSetByHeight(int nelements);
    int Union(int s1, int s2);
    int Find(int element); 
    void Print();

  protected:
    vector <int> links;
    vector <int> heights;
};

/* The third subclass implements Union-by-Rank with path compression. */

class DisjointSetByRankWPC : public DisjointSet {
  public:
    DisjointSetByRankWPC(int nelements);
    int Union(int s1, int s2);
    int Find(int element); 
    void Print();

  protected:
    vector <int> links;
    vector <int> ranks;
};

#endif