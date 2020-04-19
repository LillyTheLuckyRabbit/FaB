/* terrain.cpp
   Disjoint code based on disjoint set class from James Plank
   The terrain class uses a vector to store and manipulate level data.
   Moreover, the class uses disjoint sets for level generation (e.g., generating air pockets that connect all players)
 */


#include "terrain.h"
using namespace std;

DisjointSetByRankWPC::DisjointSetByRankWPC(int nelements)
{
  links.resize(nelements, -1);
  ranks.resize(nelements, 1);
}

int DisjointSetByRankWPC::Union(int s1, int s2)
{
  int p, c;

  if (links[s1] != -1 || links[s2] != -1) {
    cerr << "Must call union on a set, and not just an element.\n";
    exit(1);
  }

  if (ranks[s1] > ranks[s2]) {
    p = s1;
    c = s2;
  } else {
    p = s2;
    c = s1;
  }
  
  links[c] = p;
  if (ranks[p] == ranks[c]) ranks[p]++;
  return p;
}

int DisjointSetByRankWPC::Find(int e)
{
  int p, c;   // P is the parent, c is the child.

  /* Find the root of the tree, but along the way, set
     the parents' links to the children. */

  c = -1;
  while (links[e] != -1) {
    p = links[e];
    links[e] = c;
    c = e;
    e = p;
  }

  /* Now, travel back to the original element, setting
     every link to the root of the tree. */

  p = e;
  e = c;
  while (e != -1) {
    c = links[e];
    links[e] = p;
    e =c;
  }
  return p;
}

void DisjointSetByRankWPC::Print()
{
  int i;

  printf("\n");
  printf("Node:  ");
  for (i = 0; i < links.size(); i++) printf("%3d", i);  
  printf("\n");

  printf("Links: ");
  for (i = 0; i < links.size(); i++) printf("%3d", links[i]);  
  printf("\n");

  printf("Ranks: ");
  for (i = 0; i < links.size(); i++) printf("%3d", ranks[i]);  
  printf("\n\n");
}

