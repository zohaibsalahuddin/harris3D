#include "Vertex.h"
#include "util.h"
#include <iostream>
#include <cmath>
#include <map>

using namespace std;
void Vertex::getNeighborhood(int rad, vector<Vertex*>& V, Vertex* vertices){
	queue<Vertex*> Q;
	vector<Vertex*> marked;
	
	Q.push(this);
	this->setMark(true);
	this->setDepth(0);
	marked.push_back(this);
	
	while(!Q.empty()){
		Vertex* v0 = Q.front();
		Q.pop();
		V.push_back(new Vertex(v0->getX(), v0->getY(), v0->getZ())); //Indeed, copy vertex information rather than return the same vertex
		
		int dep = v0->getDepth();
		if(dep <= rad){
			set<int> listVertices = v0->getAdjacentVertices();
			set<int> :: iterator it;
			for(it = listVertices.begin(); it!=listVertices.end(); it++){
				Vertex* v1 = &vertices[*it];
					if(!v1->isMarked()){
						Q.push(v1);
						v1->setMark(true);
						v1->setDepth(dep + 1);
						marked.push_back(v1);
					}
				}
			}
	}
	
	vector<Vertex*>::iterator ini = marked.begin();
	
	while(ini<marked.end()){
		(*ini)->setMark(false);
		(*ini)->setDepth(0);
		ini++;
	}
}

int Vertex :: getRadius(Vertex*  vertices, double radius, vector<Vertex*>& V){
	vector<Vertex*> marked; //Store the marked vertices
	map<int, double> distances; //Store the distances relatives to the current vertex
	map<int, Vertex*> markedRing; //Elements in a new ring
	queue<Vertex*> Q; 
	double maxDistance = 0.0; 
	int rad = -1; 
	
	Q.push(this);
	this->setMark(true);
	markedRing.insert(pair<int, Vertex*>(this->index, this));
		
	distances[this->index] = 0.0;
	
	while(!Q.empty()){
		Vertex* v0 = Q.front();
		Q.pop();
		
		int dep = v0->getDepth();
		if(dep != rad){ //First vertex in the new ring
			map<int, Vertex*>::iterator it;
			double max = 0.0;
			
			//Mark the previous ring
			for(it = markedRing.begin(); it!=markedRing.end(); it++){
				Vertex* mar = (*it).second;
				mar->setMark(true);
				marked.push_back(mar);
				V.push_back(new Vertex(mar->getX(), mar->getY(), mar->getZ()));
				if(distances[(*it).first] > max)
					max = distances[(*it).first];
			}
			
			rad++;
			markedRing.clear(); 
			maxDistance = max;
			if(maxDistance > radius)
				break;
		}
		
		set<int> listVertices = v0->getAdjacentVertices();
		set<int> :: iterator it;
		
		for(it = listVertices.begin(); it!=listVertices.end(); it++){
				Vertex* v1 = &vertices[*it];
				if(!v1->isMarked()){ 
					if(distances[v1->getIndex()] == 0.0){ //Distance is not set
						Q.push(v1);   	
						v1->setDepth(dep + 1);
					}
					markedRing.insert(pair<int, Vertex*>(v1->getIndex(), v1));
					double dist = distanceL2(v0, v1);
					double newDistance = distances[v0->getIndex()] + dist;
					if(distances[v1->getIndex()] == 0.0){ //First time on this vertex
						distances[v1->getIndex()] = newDistance;
					}else if(newDistance  < distances[v1->getIndex()]){
						distances[v1->getIndex()] = newDistance;
					}
				}
			}
		//}
	}
	
	if(!markedRing.empty()){
			map<int, Vertex*>::iterator it;
			double max = 0.0;
			
		
			for(it = markedRing.begin(); it!=markedRing.end(); it++){
				Vertex* mar = (*it).second;
				mar->setMark(true);
				marked.push_back(mar);
				V.push_back(new Vertex(mar->getX(), mar->getY(), mar->getZ()));
				if(distances[(*it).first] > max)
					max = distances[(*it).first];
			}
			
			rad++;
			markedRing.clear(); 
			maxDistance = max;
			
	}
	
	//Unmark all vertices
	vector<Vertex*>::iterator ini = marked.begin();
	while(ini  < marked.end()){
		(*ini)->setMark(false);
		(*ini)->setDepth(0);
		ini++;
	}
	return rad;
}

void Vertex::processMaximum(Vertex* vertices, int numRings){
		set<int> :: iterator it;
		for(it = adjacentVertices.begin(); it!=adjacentVertices.end(); it++){
			Vertex* v1 = &vertices[*it];
			if(v1!=this){
				if(response < v1->getResponse())
					return;
			}
		}
	isInterest = true;
}

ostream& operator<<(ostream& out, Vertex& point){
	out << "(" << point.x() <<" , "<<point.y()<<" , "<<point.z()<<") -> (" <<
		point.mark << " , "<<point.isInterest<<")";
	
	return out;
}
