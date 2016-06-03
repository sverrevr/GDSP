#include "Astart.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

/* Jeg har modifisert input teksten. La til to linjer på starten. Den første er antall vardier bortover (x), og den andre er antall verdier nedover(y). Deretter kommer den vanlige inputen*/


//leser fra input filen, og genererer brettet deretter. Tar inn filnavnet og en refferanse til brett pekeren
bool fillBoard(string filename, Board** brett, solutiontype solution_type) {
	string line;		//en tekst linje
	char tegn;			//ett tegn
	stringstream ssLine;//String stream hjelper meg med å konvertere fra streng til int

	ifstream myfile(filename);	//åpner filen
	if (myfile.is_open()) {

		unsigned ySize = 0;
		unsigned xSize = 0;
		while (getline(myfile, line) ){
			ySize++;
		}
		xSize = line.length();
		myfile.clear();
		myfile.seekg(0, std::ios_base::beg);


		//Lager ett tomt brett
		*brett = new Board(xSize, ySize);
		for (unsigned y = 0; y < ySize; ++y) {	//for alle linjer
			getline(myfile, line);				//henter ut en linje
			for (unsigned x = 0; x < xSize; ++x) {	//for alle tegn bortover en linje
				tegn = line[x];					//henter ut ett tegn
				if (tegn != '#') {				//om det er vegg ('#') så trenger vi ikke gjøre noe, standard verdien til en node er vegg. 
					if (tegn == '.' || tegn == 'A' || tegn == 'B') {	//Setter start (A), slutt (B) og . fra oppgave A.1
						//vanlig rute
						(*brett)->generateNode(coordinate(x, y));		//costen til en standard rute er 1, så da beholdes dennel. 
						//start rute
						if (tegn == 'A') {
							(*brett)->start(coordinate(x, y));
						}
						//mål rute
						else if (tegn == 'B') {
							(*brett)->goal(coordinate(x, y));
						}
					}
				}
			}

		}
		myfile.close(); //lukker fila.
		return 1; //suksess
	}
	else {
		return 0; //klarte ikke å lese fila
	}
}


/*Skriver ut et ferdig utfylt brett. wasSucsess forteller om A* klarte å løse problemet, ellers skrives ut en feil. showState brukes i oppgave A.3, og viser lukkede og åpne noder.*/
bool writeBoard(string filename, Board* brett, bool wasSucsess, bool showState) {
	int sumCost = 0; //Pris summen
	int sumOpen = 0;
	int sumClosed = 0;

	ofstream file(filename); //åpner fil
	if (file.is_open()){
		if (!wasSucsess) {
			//Om A* feilet
			file << "FAILED\n";
			return 1;
		}
		string line = "";
		Node* current;
		for (int y = 0; y < brett->dim().y; ++y) {
			for (int x = 0; x < brett->dim().x; ++x) {
				current = brett->getNode(x, y);					//Går gjennom alle nodene og egger til en char i stringen avhengig av verdien deres

				if (current == nullptr) {						//nullptr == vegg
					line += '#';
				}
				else if (current->path()) {						//om det er en del av veien skal vi istedet legge til en O	
					line += 'O';
				}
				else if (showState && current->isClosed()) {	//Om vi skal vise åpne og lukkede, og den er lukket, vises en x istedet
					line += 'x';
					sumClosed++;
				}
				else if (showState && current->isOpen()) {		//Om vise åpne og lukkete, og ikke lukket og åpen, da vises en * istedet
					line += '*';
					sumOpen++;
				}
				else{	
					line += '.';
				}

			}
			file << line << endl;	//legger linja inn i filen
			
			line = "";				//Tømmer strengen som holder linja
		}
		if (showState) {
			file << "TODO Cost:\t" << sumCost << endl;
			file << "#Open:\t" << sumOpen << endl;
			file << "#Closed:" << sumClosed << endl;
		}
		file.close();	//Lukker filen
		return 1;		//suksess
	}
	else {
		return 0;		//Om filen ikke ble åpnet.
	}
}

int writeOptimalPath(string filename, vector<coordinate>* optimalPath) {
	ofstream file(filename);
	if (file.is_open()) {
		for (auto it = optimalPath->begin(); it != optimalPath->end(); it++) {
			file << it->x << ',' << it->y << endl;
		}
		file.close();
		return 1;
	}
	else{
		return 0;
	}
}

int main() {
	Board* brett;

	fillBoard("gameBoard.txt", &brett, Astar);
	vector<coordinate> optimalPath;
	if (aStar(brett,&optimalPath)) { //kjører A*
						//om den var suksessful skrives ut brettet
		writeBoard("solved.txt", brett, 1, 0);
		writeBoard("solved-debug.txt", brett, 1, 1);
		writeOptimalPath("solution.txt", &optimalPath);
	}
	else {
		writeBoard("solved.txt", brett, 0, 0);
	}
	delete brett;
	
	return 0;
}