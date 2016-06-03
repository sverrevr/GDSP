#include "Astart.h"
#include "gameClasses.h"
#include <iostream>

using namespace std;


bool aStar(Board* brett, vector<coordinate>* optimalPath) {
	/*Dropper closed liste, siden listesøk er unødvendig komplisert. Har heller en
	privat variabel hos nodeklassen som husker om noden er åpen eller lukket*/
	sortedNodeList open;

	brett->startNode()->g(0);
	//h beregnes inne i sortedNodeList::insert()
	//f beregnes nå man kaller på f

	open.insert(brett->startNode());//Insert funksjonen plasserer elementet sortert etter f.
	brett->startNode()->open();		//For å visualisere åpne noder til slutt, oppg A.3

	Node* X = nullptr;
	while (open.size()) {	//Kjører så lenge vi har åpne noder
		//brett->printBoard();

		X = open.pop();		//Neste element
		X->close();
		//cout << X->h() << endl;

		if (X->pos() == brett->goal()) {
			//hvis vi har når målet, så markerer vi hvilke noder som var en del av ruta
			markPathNodes(brett);
			getOptimalPath(brett,optimalPath);
			return 1;
		}

		vector<Node*> barnListe; //SUCC
		X->getAlleBarn(&barnListe);

		for (vector<Node*>::iterator barn = barnListe.begin(); barn != barnListe.end(); barn++) {
			//iteratoren blir en peker til hver Node* så til hvert barn refferanse

			if (!(*barn)->isClosed() && !open.contains(*barn)){
				attachAndEval(X,*barn);
				open.insert(*barn);
				(*barn)->open();	//For å visualisere åpne noder til slutt, oppg. A.3
			}
			else if ( X->g() + X->arcCost(*barn) < (*barn)->g() ) {
				attachAndEval(X, *barn);
				if ((*barn)->isClosed()) {
					propagatePathImprovements(*barn, &open);
				}
			}
		}
	}
	return 0;
}

void attachAndEval(Node* parent, Node* child) {
	child->parent(parent);							//Setter child sin parent  = parent
	child->g(parent->g() + parent->arcCost(child));
	//child->h_beregn();
	//h beregnes inn i insert.
	//som tidligere blir f beregnet når den kalles
}

void propagatePathImprovements(Node* parent, sortedNodeList* open) {
	vector<Node*> barnListe;
	parent->getAlleBarn(&barnListe);

	//går gjennom alle barna til parent. Disse er lagret i barnListe. 
	for ( vector<Node*>::iterator barn = barnListe.begin(); barn != barnListe.end(); barn++) {
		//Barn som ikke er i open, eller closed kommer til å bli reevaluert når de setes i open, så da er det ikke vits å propegate til dem
		if ((*barn)->isClosed() || open->contains(*barn)) { 
			if (parent->g() + parent->arcCost(*barn) < (*barn)->g()) {
				(*barn)->parent(parent);
				(*barn)->g(parent->g() + parent->arcCost(*barn));
				propagatePathImprovements(*barn, open);
			}
		}
	}
}

/*Når A* er ferdig må vi traverse den valgte veien fra slutt noden gjennom alle foreldre til startnoden. 
Hvert element på veien markeres som en del av veien, for å gjøre utskrift enklere*/
void markPathNodes(Board* brett) {
	Node* current = brett->goalNode();
	while (current != nullptr) {		//Parent til start noden er NULL, ingen andre på veien kan mangle parent
		current->path(1);
		current = current->parent();
	}
}

void getOptimalPath(Board* brett, vector<coordinate>* optimalPath) {
	optimalPath->clear();
	Node* current = brett->goalNode();
	while (current != nullptr) {		//Parent til start noden er NULL, ingen andre på veien kan mangle parent
		optimalPath->push_back(current->pos());
		current = current->parent();
	}
}