// qube txt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>

struct qube					// struktura opisujaca ekcje gry
{
	std::string infoQ;	//opis tekstowy boxa
	int hpQ;			//obrazenia lub leczenie (hp+ hp-)
	bool exitQ[4];		//tablica wyjsc | 0-W | 1-D | 2-S | 3-A
	int timeQ;			//zmienna rund zabieranych przez box
	int beInsideQ;		//licznik ilosci pobytow w boxie
};

struct gdata				//struktura zmiennych gry
{
	int timer;		// deklaracja ilosci rund
	int hp;			//zdrowie gracza
	int mov;		//ilosc skończonych rund
	int startX;		//pozycja startowa
	int startY;		//pozycja startowa
	int endX;		//wyjscie
	int endY;		//wyjscie
	int endgame;	//znacznik końca gry
	bool hardMode;	//tryb gry poczatkujacy/ekspert
	int moveI;		//oznacznik ruchu
	int x;			// współrzędna x
	int y;			//współrzędna Y
};

const int xM = 10;			// deklaracja rozmiaru gry
const int yM = xM;			// deklaracja rozmiaru gry

// zmienne globalne

qube matrix[xM][yM];		//tblica struktur boxow wraz z inicjacja
gdata allData;				//struktura danych głównych

// prototypy funkcji

void defineQ();				//inicjacja i wprowadzanie danych do tablicy struktur planszy gry
int mainGame();				//glowny modul gry (tabela boxow, start X Y, end X Y, allData.hp
int move();					//analiza ruchu (polozenie gracza X Y )
void opisQ();				//opis pomieszczenia po wejscu
int menuQ();				//blok poruszania sie
int gameOver();				//rozliczenie końcowe gry (allData.endgame, hp, losc ruchow, allData.timer)
void hardmode();			//realizacja wysokiej trudnosci gry

int main()
{
	using std::cin;
	using std::cout;
	using std::endl;

	srand(time(NULL));

	int end = 0; // !=0 - koniec petli pomocniczej

	do  //petla pomocnicza
	{
		allData.mov = 0;		//ilosc skończonych rund
		int rndX = (xM / 3);
		int rndY = (yM / 3);

		allData.startX = (std::rand() % rndX);					   //pozycja startowa
		allData.startY = (std::rand() % rndY);					   //pozycja startowa
		allData.endX = (std::rand() % rndX) + (rndX * 2);			   //wyjscie
		allData.endY = (std::rand() % rndY) + (rndY * 2);			   //wyjscie

		allData.endgame = 0;		//znacznik końca gry

		system("cls");

		cout << "Witam w qube txt" << endl;
		cout << "\nzaraz wejdziesz do losowego boxu " << endl;
		cout << "twoim zadaniem bedzie wydostanie sie na zewnatrz" << endl;
		cout << "w tym celu uzyj klawiszy WSAD :" << endl;
		cout << "\n W - polnoc\n E - poludnie\n A - zachod\n D - wschod" << endl;
		cout << "spokojnie - nie musisz tego zapamietac - bedzie to podawane podczas gry :)" << endl;
		cout << "\nNa poczatek masz 10 punktow zycia.\nCzesc pokojow zadaje obrazena a odpoczynek w czesci pokojow leczy rany" << endl;
		cout << "odpoczywasz uzywajac klawisza Q" << endl;
		cout << "masz " << allData.timer << " jednostek czasu a kazdy pokoj zabiera pewna ilosc czasu na ture" << endl;
		cout << "\nzycze milej zabawy, nacisnij dowolny klawisz aby zaczac." << endl;
		cout << "koncowy wynik zalezy od czasu przejscia i pozostalgo zdrowia" << endl;
		cout << "\n\nwybierz trudnosc gry: 1 - lawa 2- trudna - uwaga, gra moze byc sporadycznie nie mozliwa do ukonczenia " << endl;
		// trzeba zaimplementowac sprawdzanie możliwosci ukończenia gry
		int tmp;
		do
		{
			tmp = 0;
			char tempMode;
			tempMode = _getch();
			if (tempMode == '1')allData.hardMode = false;
			else if (tempMode == '2')allData.hardMode = true;
			else tmp = 1;
		} while (tmp == 1);

		if (allData.hardMode)allData.hp = 40;			//zdrowie gracza
		else allData.hp = 30;

		if (allData.hardMode)allData.timer = 150;		// deklaracja ilosci rund
		else allData.timer = 30;

		defineQ();			//uzupełnianie danych
		if (allData.hardMode) hardmode();
		allData.endgame = mainGame(); //pętla główna gry
		end = gameOver(); //koniec gry
	} while (end == 0);

	return 0;
}

void defineQ()
{
	int  p = 1; //numerator nazwy pokoju

	for (int i = 0; i < xM; i++)
		for (int j = 0; j < yM; j++)
		{
			matrix[i][j].beInsideQ = 1;
			matrix[i][j].infoQ = "pokoj numer  ";				//opis pokoju - moze z pliku
			matrix[i][j].infoQ += std::to_string(p); p++;		//numer pokoju
			matrix[i][j].hpQ = (std::rand() % 5) - 2;			//losowe okreslenie obrazem lub leczenia w zakresie +/-2
			matrix[i][j].timeQ = (std::rand() % 4) + matrix[i][j].hpQ;			//Im bezpieczniej, tym czas szybciej idzie
			matrix[i][j].exitQ[0] = true;
			matrix[i][j].exitQ[1] = true;
			matrix[i][j].exitQ[2] = true;
			matrix[i][j].exitQ[3] = true;
			if (i == 0) matrix[i][j].exitQ[0] = false;			//dla blokady w
			if (j == (yM - 1)) matrix[i][j].exitQ[1] = false;	//dla blokady d
			if (i == (xM - 1)) matrix[i][j].exitQ[2] = false;	//dla blokady s
			if (j == 0) matrix[i][j].exitQ[3] = false;			//dla blokady a
		}
	if (allData.hardMode) hardmode();
}

void hardmode()
{
	for (int i = 1; i < (xM - 1); i++)
		for (int j = 1; j < (yM - 1); j++)
		{
			int tmp = (std::rand() % 4);
			matrix[i][j].exitQ[tmp] = false;

			//std::cout << "i = " << i << "  j = " << j << std::endl;
		}
	//_getch();
}

int mainGame()
{
	allData.x = allData.startX;
	allData.y = allData.startY;
	allData.endgame = 0;	//znacznik końca gry 0-gramy | 1-zdrowie | 2-ucieczka | 3-wygrana! | 4-koniec czasu
	int door = 5;			//znacznik pokonanych ostatnio drzwi (odwrotny wdsa) 5 - pierwsze wejscie
	allData.moveI = 9;		//oznaczenie wyboru gracza 0-w |1-d |2-s |3-a |4-a |5-rest |6- exit | 9 - wartosc startowa

	system("cls");
	std::cout << "\n\n\n\nZaraz wejdziesz do gry!!!" << std::endl;
	std::cout << "\nzaczniesz w miejscu oznaczonym jako " << matrix[allData.x][allData.y].infoQ << std::endl;
	std::cout << "wyjscie znajduje sie w pokoju oznaczonym jako " << matrix[allData.endX][allData.endY].infoQ << std::endl;
	std::cout << "\nzapamietaj to, bo od teraz podawany bedzie tylko numer pokoju w ktorym aktualnie jestes!" << std::endl;
	std::cout << "\n\nWspinasz sie po drabince do pomieszczenia od ktorego zaczniesz gre,\ncelem jest dotrzec jak najszybciej do pomieszczenia z taka sama klapa w podlodze i wyjscie" << std::endl;
	std::cout << "\n\nNacisnij dowolny klawisz" << std::endl;
	_getch();

	do					//głowna pętla gry
	{
		system("cls");

		allData.hp += matrix[allData.x][allData.y].hpQ;  //zmiana HP gracza w nowym pokoju
		if (allData.hp <= 0) return 1;
		allData.mov += matrix[allData.x][allData.y].timeQ;  //zmiana rundy gracza w nowym pokoju
		if (allData.mov >= allData.timer) return 4;

		std::cout << "    HP  " << allData.hp << "   zakonczone rundy  " << allData.mov << "    Pozostalo czasu  " << allData.timer - allData.mov << std::endl << std::endl;

		opisQ();
		allData.moveI = menuQ();
		allData.endgame = move();
	} while (allData.endgame == 0);

	return allData.endgame;
}

void opisQ()
{
	using std::cin;
	using std::cout;
	using std::endl;

	cout << "wchodzisz do pomieszczenia przez ";
	if (allData.moveI == 9) cout << "klape w podlodze " << endl;
	if (allData.moveI == 0) cout << "poludniowe drzwi " << endl;
	if (allData.moveI == 1) cout << "zachodnie drzwi " << endl;
	if (allData.moveI == 2) cout << "polnocne drzwi " << endl;
	if (allData.moveI == 3) cout << "wschodnie drzwi " << endl;

	if (allData.moveI == 0 && matrix[allData.x][allData.y].exitQ[2] == false) cout << "Drzwi za toba zatrzaskuja sie" << endl;
	if (allData.moveI == 1 && matrix[allData.x][allData.y].exitQ[3] == false) cout << "Drzwi za toba zatrzaskuja sie" << endl;
	if (allData.moveI == 2 && matrix[allData.x][allData.y].exitQ[0] == false) cout << "Drzwi za toba zatrzaskuja sie" << endl;
	if (allData.moveI == 3 && matrix[allData.x][allData.y].exitQ[1] == false) cout << "Drzwi za toba zatrzaskuja sie" << endl;

	cout << "\nw jednolicie bialym pokoju widzisz tylko na kazdej scianie napis \n\n    \"" << matrix[allData.x][allData.y].infoQ << "\"" << endl;
	cout << "\no ile pamietasz jestes w tym pokoju po raz " << matrix[allData.x][allData.y].beInsideQ << endl;
	matrix[allData.x][allData.y].beInsideQ++;     //dodanie liczby odwiedzin w pokoju
	cout << "\nwychodza z tad drzwi na ";
	if (matrix[allData.x][allData.y].exitQ[0]) cout << "polnoc ";
	if (matrix[allData.x][allData.y].exitQ[1]) cout << "wschod ";
	if (matrix[allData.x][allData.y].exitQ[2]) cout << "poludnie ";
	if (matrix[allData.x][allData.y].exitQ[3]) cout << "zachod ";
	if (allData.x == allData.startX && allData.y == allData.startY) cout << "\nw podloze widzisz klape przez ktora sie tu dostales" << endl;
	if (allData.x == allData.endX && allData.y == allData.endY)cout << "\n w podlodze widzisz nieznana klape, czyzby upragnione wyjscie??" << endl;
	cout << "\nJednoczesnie czujesz ze w pokoju panuje ";
	if (matrix[allData.x][allData.y].hpQ == 0) cout << "neutralna atmosfera " << endl;
	if (matrix[allData.x][allData.y].hpQ > 0) cout << "przyjazna atmosfera i orzymujesz " << matrix[allData.x][allData.y].hpQ << " HP" << endl;
	if (matrix[allData.x][allData.y].hpQ < 0) cout << "szkodliwa atmosfera i orzymujesz " << matrix[allData.x][allData.y].hpQ << " HP" << endl;
	cout << "\nPobyt w tym pokoju kosztuje " << matrix[allData.x][allData.y].timeQ << " rund" << endl;
}

int menuQ()
{
	std::cout << std::endl;
	std::cout << "nacisnij E zeby odpoczac w pokoju jedna runde;  R - zrezygnuj i wyjdz z \n" << std::endl;
	if (matrix[allData.x][allData.y].exitQ[0])  std::cout << "nacisnij W zeby isc na polnoc" << std::endl;
	if (matrix[allData.x][allData.y].exitQ[1])  std::cout << "nacisnij D zeby isc na wschod" << std::endl;
	if (matrix[allData.x][allData.y].exitQ[2])  std::cout << "nacisnij S zeby isc na poludie" << std::endl;
	if (matrix[allData.x][allData.y].exitQ[3])  std::cout << "nacisnij A zeby isc na zachod" << std::endl;
	if (allData.x == allData.endX && allData.y == allData.endY)  std::cout << "\n\a\a****** nacisnij Q zeby skorzystac z klapy w podlodze *****" << std::endl;

	int tst = 0;
	char moveC;				//znak z klawiatury
	allData.moveI = 9;			//oznaczenie wyboru gracza 0-w |1-d |2-s |3-a |4-a |5-rest |6- exit

	do
	{
		tst = 0;
		moveC = _getch(); //pobieranie znaku z klawiatury

		if ((moveC == 'w' || moveC == 'W') && matrix[allData.x][allData.y].exitQ[0]) allData.moveI = 0;
		else if ((moveC == 'd' || moveC == 'D') && matrix[allData.x][allData.y].exitQ[1]) allData.moveI = 1;
		else if ((moveC == 's' || moveC == 'S') && matrix[allData.x][allData.y].exitQ[2]) allData.moveI = 2;
		else if ((moveC == 'a' || moveC == 'A') && matrix[allData.x][allData.y].exitQ[3]) allData.moveI = 3;
		else if ((moveC == 'q' || moveC == 'Q') && (allData.x == allData.endX && allData.y == allData.endY)) allData.moveI = 4;
		else if (moveC == 'e' || moveC == 'E') allData.moveI = 5;
		else if (moveC == 'r' || moveC == 'R') allData.moveI = 6;
		else tst = 1;
	} while (tst == 1);
	return allData.moveI;
}

int move()
{
	switch (allData.moveI)
	{
	case 0: allData.x--; break;
	case 1: allData.y++; break;
	case 2: allData.x++; break;
	case 3: allData.y--; break;
	case 4: return 3;
	case 5: return 0;
	case 6: return 2;
	default: menuQ();
	}
	return 0;
}

int gameOver()
{
	using std::cout;
	using std::endl;

	system("cls");
	cout << "    HP  " << allData.hp << "   zakonczone rundy  " << allData.mov << "    Pozostalo czasu  " << allData.timer - allData.mov << endl << endl;
	if (allData.endgame == 1) cout << "\nNiestety twoje zdrowie spadlo do zera... niezyjesz :) ";
	if (allData.endgame == 2) cout << "\nUciekasz wyjsciem bezpieczenstwa przed zakonczeniem zadania.";
	if (allData.endgame == 4) cout << "\nNiestety koniec czasu - przegrales.";
	if (allData.endgame == 3)
	{
		cout << "\n\nGRATULACJE udalo ci sie wygrac !!!!";
		cout << "\nuzyskales " << allData.hp / 2 + (allData.timer - allData.mov) << " punktow" << endl;
	}
	cout << "\nKoniec gry nacisnij Q\nponowna gra nacisnij E" << endl;
	int tst = 0;
	do
	{
		char a = 0;
		a = _getch();
		if (a == 'e' || a == 'E')return 0;
		else if (a == 'q' || a == 'Q')return 1;
		else tst = 1;
	} while (tst == 1);
	return 1;
}