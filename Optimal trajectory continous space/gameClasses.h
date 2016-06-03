#pragma once
#include <vector>
#include <math.h>

enum solutiontype { Astar, BFS, Djikstra }; //De forskjellige løsningstypene for oppgave A.3
class Node; //Forward declaration siden node avhenger av board og omvendt. 

//Lagde en kordinat klasse så jeg slapp å sende så mange elementer. 
struct coordinate {
	int x;
	int y;

	coordinate() { x = 0; y = 0; }
	coordinate(int x, int y) {
		this->x = x;
		this->y = y;
	}
	bool operator ==(coordinate& rhs) {
		if ((this->x == rhs.x) && (this->y == rhs.y)) {
			return 1;
		}
		return 0;
	}
};

/*Boredet holder styr på all nodenen i en dimX*dimY matrise. Den husker og hvor start og slutt er*/
class Board {
	//Formatert brett[x][y]
	Node*** brett;
	coordinate dim_val;	//størrelsen på matrisen

	coordinate startPos;
	coordinate goalPos;

public:
	//lage matrisen
	Board(unsigned dimX, unsigned dimY);
	~Board();	//Destruktør, håndterer at minnet slettes rett.

				//sette start og goal noder
	void start(coordinate startPos) { this->startPos = startPos; }
	void goal(coordinate goalPos) { this->goalPos = goalPos; }

	//hente start og goal possisjon
	coordinate start() { return startPos; }
	coordinate goal() { return goalPos; }
	//hente start og goal node
	Node* startNode() { return brett[startPos.x][startPos.y]; }
	Node* goalNode() { return brett[goalPos.x][goalPos.y]; }

	//hente en spesifikk node
	Node* getNode(unsigned x, unsigned y) { return brett[x][y]; }
	Node* getNode(coordinate pos) { return brett[pos.x][pos.y]; }

	//hente dim. Denne kan ikke endres, siden vi da måtte endret brettet.
	coordinate dim() { return dim_val; }

	//Lager en ny node på en eksisterende plass. Hvis det er noe der fra før av, slettes den gamle noden.
	Node* generateNode(coordinate pos);

	//debugging funksjon, skriver ut bordet underveis.
	void printBoard();

};



/*Hver rute er en node med sine tilhørige verdier og funksjoner. */
class Node{
private:
	Node* parent_val;	//sin forelder
	double g_val;		//current g verdi
	double h_val;		//h verdien
	bool isClosed_val;	//om den er lukket
	bool isOpen_val;	//om den har blitt åpnet. Hvis isClosed så er også isOpen. Brukes kunn i A.3

	coordinate pos_val;	//Hvor på brettet den befinner seg

	Board* brettet;		//En peker til brettet for enkelhets skyld

	bool isPath;		//Om den er med på den valgte veien til målet
public:
	Node(coordinate pos, Board* brett);					//Lager noden

	//hent pos
	coordinate pos() { return pos_val; }

	//hent og endre g
	double g() { return g_val; }
	double g(double g_inn) { return (g_val = g_inn); }

	//hent og oppdater h
	double h() { return h_val; }
	double h_beregn();

	//hent f
	double f() { return (g() + h()); }

	//hent og set closed og open. Kan ikke resettes. 
	void close() { isClosed_val = 1; }
	bool isClosed() { return isClosed_val; }
	void open() { isOpen_val = 1; }
	bool isOpen() { return isOpen_val; }

	//hent og set parent
	void parent(Node* parent) { parent_val = parent; }
	Node* parent() { return parent_val; }

	//Funksjon som henter alle barna og putter den inn i listen (barn) som kommer inn som en refferanse
	void getAlleBarn(std::vector<Node*>* barn);

	//beregner prisen å gå herifra til child som sendes inn. Det antas at child et att av this sine child. 
	double arcCost(Node* child) {
		return sqrt(pow((child->pos().x - this->pos().x),2) + pow((child->pos().y - this->pos().y),2));
	}

	//henter og setter om dette er en del av pathen.
	void path(bool state) { isPath = state; }
	bool path() { return isPath; }

};


/*En liste type som legger inn elementer i sortert rekkefølge etter f()*/
class sortedNodeList {
	//basert på std::vector
	std::vector<Node*> elementer;
	
public:
	void insert(Node* elem);	//Legger inn element på rett sted
	Node* pop();				//Henter ut og fjerner element
	unsigned size() { return elementer.size(); }
	bool contains(Node* elem);	//Forteller om ett element er medlem av lista.
};