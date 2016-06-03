#include "gameClasses.h"
#include <algorithm>
#include <math.h>
#include <iostream>
using namespace std;

Board::Board(unsigned dimX, unsigned dimY){
	brett = new Node**[dimX];				//lager brettet
	for (unsigned x = 0; x < dimX; ++x) {
		brett[x] = new Node*[dimY];			//2 dimmensjoner
		for (unsigned y = 0; y < dimY; ++y) {
			brett[x][y] = nullptr;			//Setter alle verdiene til nullptr, denne verdien kommer til å representere vegg.
		}
	}
	dim_val = coordinate(dimX, dimY);	//oppdaterer dimensjonen til lista

	//Setter standar verdier
	startPos = coordinate(0,0);
	goalPos = coordinate(0,0);
}

Board::~Board(){
	//frigir minnet vi alokerte. 
	for (int x = 0; x < dim_val.x; ++x) {
		for (int y = 0; y < dim_val.y; ++y) {
			delete brett[x][y];
		}
		delete[] brett[x];
	}
	delete[] brett;
}

Node* Board::generateNode(coordinate pos) {
	if (brett[pos.x][pos.y] != nullptr) {
		delete brett[pos.x][pos.y];
	}
	brett[pos.x][pos.y] = new Node(pos, this);
	return brett[pos.x][pos.y];					//Returnerer den nylig genererte noden
}

Node::Node(coordinate pos, Board* brett){
	//lager en standard node
	this->pos_val = pos;
	this->brettet = brett;

	parent_val = nullptr;
	g_val = 0;
	h_val = 0;
	isClosed_val = false;
}

double Node::h_beregn() {
	//manhatten distance
	int dx = this->pos().x - brettet->goal().x;
	int dy = this->pos().y - brettet->goal().y;
	h_val = sqrt(pow(dx, 2) + pow(dy, 2));
	return h_val;
}

void Node::getAlleBarn(vector<Node*>* barn) {
	Node* temp;
	int x = this->pos().x;
	int y = this->pos().y;

	for (int dx=-1; dx <=1; ++dx) {

		for (int dy = -1; dy <= 1; ++dy) {

			if (x+dx >= 0 && y+dy >= 0 && x+dx < brettet->dim().x && y+dy < brettet->dim().y && !(dx==0 && dy==0)) {

				temp = brettet->getNode(x+dx, y+dy);
				if (temp != nullptr) {
					barn->push_back(temp);
				}
			}
		}
	}
}


//Settr inn elementet på sortert plassering utifra f()
void sortedNodeList::insert(Node* elem) {
	elem->h_beregn();
	for (vector<Node*>::iterator it = elementer.begin(); it != elementer.end(); it++) {
		if (elem->f() < (*it)->f()) {	//om vi finner ett element med høyere f
			elementer.insert(it, elem);	//settes dette elementet inn foran, og funksjonen avsluttes
			return;
		}
	}
	//ellers puttes det inn på slutten. 
	elementer.push_back(elem);
}

//Fjerner element og returnerer det
Node* sortedNodeList::pop() {
	Node* popedElem = elementer[0];
	elementer.erase(elementer.begin());
	return popedElem;
}

//Bruker funksjonen "find" fra "<algorithms>" for å finne elementet
bool sortedNodeList::contains(Node* elem) {
	if ( find(elementer.begin(), elementer.end(), elem) != elementer.end()) {
		return true;
	}
	return false;
}


//En debugging funksjon som skriver ut brettet under genereringen. Man ser kunn åpne, lukkede og ikke beregnede ruter, ikke beste veien så langt
void Board::printBoard() {
	for (int y = 0; y < this->dim_val.y; ++y) {
		for (int x = 0; x < this->dim_val.x; ++x) {
			if (brett[x][y] == nullptr) {
				cout << "#";
			}
			else if (brett[x][y]->isClosed()) {
				cout << 'X';
			}
			else if (brett[x][y]->isOpen()) {
				cout << '*';
			}
			else {
				cout << ".";
			}
		}
		cout << endl;
	}
	cout << endl;
}