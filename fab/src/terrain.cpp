/* File: terrain.cpp
 * Authors: David Butler and Lillian Coar
 * Description: Disjoint code based on disjoint set class from James Plank
 * The terrain class uses a vector to store and manipulate level data.
 * Moreover, the class uses disjoint sets for level generation (e.g., generating air pockets that connect all players)
*/


#include "terrain.h"
using namespace std;

DisjointSetBySize::DisjointSetBySize(int nelements){
  links.resize(nelements, -1);
  sizes.resize(nelements, 1);
}

int DisjointSetBySize::Union(int s1, int s2){
  int p, c;
  if (links[s1] != -1 || links[s2] != -1) {
    cerr << "Must call union on a set, and not just an element.\n";
    exit(1);
  }

  if (sizes[s1] > sizes[s2]) {
    p = s1;
    c = s2;
  } else {
    p = s2;
    c = s1;
  }
  
  links[c] = p;
  sizes[p] += sizes[c];
  return p;
}

int DisjointSetBySize::Find(int element)
{
  while (links[element] != -1) element = links[element];
  return element;
}

//void genTer(vector<int> &l, vector<int> &toVisit, int &playerUnion, const int &playerCount, DisjointSet *u, const vector<int> &playerPositions);


/* generateTerrain acts as a helper function for genTer
   Helper function initializes toVisit with player locations.
*/
void Terrain::generateTerrain(Player *p[4],int playerCount){
	vector<int> toVisit;
	vector<int> playerPositions;
	toVisit.reserve(LEVEL_WIDTH*LEVEL_HEIGHT);
	playerPositions.reserve(4);
	//int playerCount = (sizeof(p))/(sizeof(p[0]));
	for (int i = 0; i < playerCount; i++){ 
		int addPos = (p[i]->getY()* LEVEL_WIDTH) + p[i]->getX();
		if (i==0) toVisit.push_back(addPos);
		playerPositions.push_back(addPos);
		for(int y = -25; y <= 25; y++) {
			for(int x = -25; x <= 25; x++) {
				if(x * x + y * y < 25 * 25) {
					l[((p[i]->getY() + 15 + y) *LEVEL_WIDTH+1)+p[i]->getX() + 15 + x] = 0;
				}
			}
		}
	}
	DisjointSet *u; //Player union
	u = new DisjointSetBySize(LEVEL_WIDTH*LEVEL_HEIGHT);
	//Initialize union with 1st player's position
	int playerUnion = (p[0]->getY()*LEVEL_WIDTH)+p[0]->getX();
	genTer(toVisit, playerUnion, playerCount, u, playerPositions);
}
/* genTer
	Pseudocode:
Base case - If find(player2-4) == find(player1), the function is done.
Pick a random element from the list vector.
For each of the element's neighbors...
  Turn neighbors into air and call union on this element + neighbor
  Add neighbor into the list vector (if they were dirt previously)
*/
void Terrain::genTer( vector<int> &toVisit, int playerUnion, int playerCount, DisjointSet *u, const vector<int> &playerPositions){
bool terrainDone = false;
while (!terrainDone){
	//cout << "Before base cases\n";
	//Emergency base case: The toVisit vector is empty; end and print error
	if (toVisit.size() == 0){
		cout << "ERROR: Terrain generation did not finish properly.\n";
		terrainDone = true;
		break;
		//return;
	}
	//Base case: all players are in the same union and the function is done.
	if ((u->Find(playerUnion))==(u->Find(playerPositions[1]))){
		if (playerCount > 2){
			if ((u->Find(playerUnion))==(u->Find(playerPositions[2]))){
				if (playerCount > 3){
					if((u->Find(playerUnion))==(u->Find(playerPositions[3]))){
						terrainDone = true;
						break;
						//return;
					}
				}
				else {
					terrainDone = true;
					break;
					//return;
				}
			}
		}
		else return;
	}//End of totally-easy-to-read base case
	//DEBUG: print toVisit
	/*for (auto i = toVisit.begin(); i != toVisit.end(); i++) {
		cout << "(" << *i%LEVEL_WIDTH << "," << *i/LEVEL_WIDTH << ") ";
	}*/
	//cout << endl;
	//Choose a random element from toVisit
	int visitNext = rand() % toVisit.size();
	int visitPos = toVisit[visitNext];
	int visitX = visitPos % LEVEL_WIDTH;
	int visitY = visitPos / LEVEL_WIDTH;
	//cout << "At position: " << visitX << "," << visitY << endl;
	//Check which neighbor cells aren't already air and modify them
	//UP neighbor
	//cout << "\tBEFORE UP, checking at L: " << (visitY+1)*LEVEL_WIDTH+(visitX) << " or " << "(" << visitX << "," << visitY+1 << ")" << endl;
	//cout << "\tAlso, L has " << l[(visitY+1)*LEVEL_WIDTH+(visitX)] << endl;
	if ((visitY+1 < (LEVEL_HEIGHT))&&((u->Find(playerUnion)!=(u->Find((visitY+1)*LEVEL_WIDTH+(visitX)))))){
		//cout << "\tUP\n";
		l[(visitY+1)*LEVEL_WIDTH+(visitX)] = 0;
		toVisit.push_back((visitY+1)*LEVEL_WIDTH+(visitX));
		u->Union(u->Find(visitPos),u->Find((visitY+1)*LEVEL_WIDTH+(visitX)));
	}
	//cout << "1\n";
	//DOWN neighbor
	if ((visitY > 0)&&((u->Find(playerUnion)!=(u->Find((visitY-1)*LEVEL_WIDTH+(visitX)))))){
		//cout << "\tDOWN\n";
		l[(visitY-1)*LEVEL_WIDTH+(visitX)] = 0;
		toVisit.push_back((visitY-1)*LEVEL_WIDTH+(visitX));
		u->Union(u->Find(visitPos),u->Find((visitY-1)*LEVEL_WIDTH+(visitX)));
	}
	//cout << "2\n";
	//LEFT neighbor
	if ((visitX > 0)&&((u->Find(playerUnion)!=(u->Find((visitY)*LEVEL_WIDTH+(visitX-1)))))){
		//cout << "\tLEFT\n";
		l[(visitY)*LEVEL_WIDTH+(visitX-1)] = 0;
		toVisit.push_back((visitY)*LEVEL_WIDTH+(visitX-1));
		u->Union(u->Find(visitPos),u->Find((visitY)*LEVEL_WIDTH+(visitX-1)));
	}
	//cout << "3\n";
	//RIGHT neighbor
	if ((visitX+1 < (LEVEL_WIDTH))&&((u->Find(playerUnion)!=(u->Find((visitY)*LEVEL_WIDTH+(visitX+1)))))){
		//cout << "\tRIGHT\n";
		l[(visitY)*LEVEL_WIDTH+(visitX+1)] = 0;
		toVisit.push_back((visitY)*LEVEL_WIDTH+(visitX+1));
		u->Union(u->Find(visitPos),u->Find((visitY)*LEVEL_WIDTH+(visitX+1)));
	}

	//Remove visitNext from toVisit
	//cout << "Before erase\n";
	toVisit.erase(toVisit.begin()+visitNext);
	//cout << "Before recursive call\n";
	}//end of while	
	//Recursive call
	//genTer(toVisit, playerUnion, playerCount, u, playerPositions);
	return;
}

//Debug command to see if terrain generation worked
void Terrain::printLevel(){
	for (int i = 0; i < levHeight; i++){
		for (int j = 0; j < levWidth; j++){
			//if ((j%80)==0) 
				cout << l[i*levWidth+j] << " ";
		}
		//if ((i%80)==0) 
			cout << endl;
	}
	
}

int Terrain::getValueAtIndex(int index) const{
	if(index < l.size()) {
		return l[index];
	} else {
		return(-1);
	}
}

int Terrain::getValueAtXY(int x, int y) const {
	if(y * levWidth + x < l.size()) {
		return l[y * levWidth + x];
	} else {
		return(0);
	}
}

bool Terrain::setValueAtIndex(int index, int value) {
	if(index < l.size()) {
		l[index] = value;
		return true;
	}
	return false;
}

bool Terrain::setValueAtXY(int x, int y, int value) {
	if(y * levWidth + x < l.size()) {
		l[y * levWidth + x] = value;
		return true;
	}
	return false;
}