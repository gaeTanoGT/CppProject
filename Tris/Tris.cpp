//Tris2          Torrisi Gaetano    19/01/2022
#include <iostream>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void stampa(int[][3]);
void stringaInt(int&, int&, int[][3], bool);
void cond(int, int);
int verificaTris(int[][3]);
bool verificaVincitore(int, int[][3]);
void errore(int[][3], int&, int&, bool, bool);
bool yN();
bool verificaContinuaggio(int[][3]);

int main()
{
    int Mat[3][3];          //0 vuoto  -  1 Player0 - 2 Player1
    
    //Pulisco la matrice
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            Mat[i][j] = 0;

    int r, c;
    int who;                        //vincitore: 0 nessuno - 1 Player0 - 2 Player1

    bool k = true;
    bool z = true;
    bool Player = 0;
    while(k && z)
    {   
        stampa(Mat);

        z = verificaContinuaggio(Mat);
        if(z == false)
        {
            cout << "\n\nGIOCO TERMINATO SENZA VINCITORE\n\n";
            break;
        }

        if(Player == 0)
            cout << "\n\n   --Player 1 = O--\n     Player 2 = X\n";
        if(Player == 1)
            cout << "\n\n     Player 1 = O\n   --Player 2 = X--\n";

        cout << "Indicare la casella da occupare con coordinata x y (es. ''1 0''): ";
        stringaInt(r, c, Mat, Player);      //Leggo coordinate e verifico correttezza
        int u = (int)Player + 1;
        Mat[r][c] = u;                      //Giocatore occupa casella
        who = verificaTris(Mat);            //Verifica la presenza di un tris
        k = verificaVincitore(who, Mat);    //controlla se who ha un vincitore restituendo 0 se ha vinto qualcuno

        system("cls");
        Player = !Player;
    }

    cout << "Vuoi giocare ancora? (Y/N): ";
    k = yN();

	if (k == false)
		return 0;
	else
    {
        system("cls");
        main();
    }
}

void stampa(int Mat[][3])
{
    cout << "    0     1     2\n";
    for(int i = 0; i < 3; i++)
    {
        cout << "       |     |\n" << i << "   ";
        for(int j = 0; j < 3; j++)
        {
            if(Mat[i][j] == 0)
                cout << " ";
            else if(Mat[i][j] == 1)
                cout << "O";
            else if(Mat[i][j] == 2)
                cout << "X"; 
            cond(0, j);
        }
        cond(1, i);
    }
    cout << "\n       |     |";
}

void cond(int z, int i)         //Condizioni per la stampa della matrice
{
    if(z == 0)
    {
        if(i != 2)
            cout << "  |  ";
    }
    else if(z == 1)
        if(i != 2)
            cout << "\n  _____|_____|_____\n";
}

//Lettura coordinate e controllo
void stringaInt(int &y, int &x, int Mat[][3], bool Player)         // colonne - righe       invertiti perchè la metrice lavora in y x
{
	string stringa;
	getline(cin, stringa);

    int n1 = 0;         //col
    int n2 = 0;         //rig

    string s1, s2;

    if(stringa.length() >= 2 && stringa[1] == ' ')
    {
        s1 = stringa.substr(0, 1);
        s2 = stringa.substr(2, 3);
    }

    n1 = (int)s1[0];
    n2 = (int)s2[0];

    if((n1 >= 48 && n1 <= 50) && (n2 >= 48 && n2 <= 50))
    {
        x = stoi(s1);
        y = stoi(s2);
    }
    else
        errore(Mat, y, x, 0, Player);

    //Verifico coordinate
    if(Mat[y][x] != 0)
        errore(Mat, y, x, 1, Player);
}

int verificaTris(int Mat[][3])
{
    // Scorri tra le righe e colonne
    for(int i = 0; i < 3; i++)          
    {
        if((Mat[i][0] == Mat[i][1]) && (Mat[i][1] == Mat[i][2]))        //Righe
            return Mat[i][0];

        if((Mat[0][i] == Mat[1][i]) && (Mat[1][i] == Mat[2][i]))        //Colonne
            return Mat[0][i];
    }

    // Scorri diagonale 1
    if((Mat[0][0] == Mat[1][1]) && (Mat[1][1] == Mat[2][2]))
        return Mat[0][0];

    // Scorri diagonale 2
    if((Mat[0][2] == Mat[1][1]) && (Mat[1][1] == Mat[2][0]))
        return Mat[0][2];

    return 0;
}

bool verificaVincitore(int who, int Mat[][3])
{
    if(who == 0)
        return true;        //Continua a giocare
    if(who == 1 || who == 2)
    {
        system("cls");
        stampa(Mat);
        cout << "\n\nHa vinto il Giocatore " << who << "!!\nCOMPLIMENTI!!\n\n";
        system("pause");
        return false;       //Fine gioco uscita while
    }
}

void errore(int Mat[][3], int &r, int &c, bool s, bool Player)      //s: tipo errore
{
    system("cls");
    stampa(Mat);
    if(s == 0)
        cout << "\nValori inseriti in modo errato...";
    if(s == 1)
        cout << "\nCoordinate gia' utilizzate...";

    if(Player == 0)
        cout << "\n   --Player 1 = O--\n     Player 2 = X\n";
    if(Player == 1)
        cout << "\n     Player 1 = O\n   --Player 2 = X--\n";

    cout << "Indicare la casella da occupare con coordinata x y (es. ''1 0'')";
    if(s == 0)
        cout << ". Attenzione ad usare bene gli spazi: ";
    if(s == 1)
        cout << ": ";
    stringaInt(r, c, Mat, Player);
}

bool verificaContinuaggio(int Mat[][3])
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(Mat[i][j] == 0)
                return true;

    return false;
}

bool yN()       //Sicuro di voler uscire?
{
	char state;
	state = getche();
	switch (state) {
	case 'y': return true; break;
	case 'n': return false; break;
	default: break;
	}
}