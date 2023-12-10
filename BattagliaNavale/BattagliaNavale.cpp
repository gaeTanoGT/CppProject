/*Battaglia navale
 *Creato da Gaetano Torrisi il 24/12/2021
*/
#include <iostream>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void sPlayer(int, int Player1[][11], int Player2[][11]);
void wPlayer(int, int wPlayer1[][10], int wPlayer2[][10]);
bool yN();
bool oV(int, int, int Player1[][11], int Player2[][11], int);
void stringaInt(int&, int&);

bool verifica(int, int Player1[][11], int Player2[][11], int, int, bool [][2], int, int);            //giocatore - matrice 1 - matrice 2 - rig - col - dir - i - x(lung nav)

void assegna(int, int Player1[][11], int Player2[][11], int, int, bool [][2], int, int);            //giocatore - matrice 1 - matrice 2 - rig - col - dir - i - x(lung nav)

bool wVerifica(int, int Player1[][11], int Player2[][11], int wPlayer1[][10], int wPlayer2 [][10], int, int);     //attaccante - matrice 1 - matrice 2 - visualeW1 - visualeW2 - rig - col 
        //wVerifica:  0: !z senza navi in campo;  1: !z con navi in campo

bool password(int, string[]);
void getPassword(int, string[]);

int main()
{
	int Player1[11][11] = { 0 };		// righe - colonne      0 = vuoto; 1 = nave; 2 = lanciato non colpito; 3 = colpito; 4: overflow
	int Player2[11][11] = { 0 };

    int wPlayer1[10][10] = { 0 };		// visuale del player 1 sulla mappa del player 2
	int wPlayer2[10][10] = { 0 };       // righe - colonne      0 = vuoto; 1 = lanciato; 2 = colpito

    //Indicazione dei confini
    for(int i = 0; i < 11; i++)
    {
        Player1[i][10] = 4;
        Player2[i][10] = 4;
        Player1[10][i] = 4;
        Player2[10][i] = 4;
    }

    //Stampo home screen
	sPlayer(0, Player1, Player2);
	sPlayer(1, Player1, Player2);

	cout << "--------------------------------------------------------------\n";

	cout << "Sei sicuro di voler iniziare? (Y/N): ";
	bool k = yN();

	if (k == false)
	{
		return 0;
	}

	system("cls");

    //Asseganzione posizione navi
	int z = 0;
	k = false;
    int a, b;
    int i;
    bool dir5[3][2];        // direzione (1 = orizzontale)(0 = verticale) - numero barca - numero giocatore
    bool dir3[3][2];
    bool dir2[4][2];

    int x;

    bool u = true;

    string p[2];
    
	while (k == false)
	{
        //Navi da 5
        for (int i = 0; i < 3; i++)
		{
            x = 5;
		    cout << "GIOCATORE " << z + 1 << ":\n\tSelezionare le proprie navi: \n     ---3 x Navi lunghezza 5---\n\t3 x Navi lunghezza 3\n\t4 x navi lunghezza 2\n\n";
            sPlayer(z, Player1, Player2);
			cout << "Nave " << i + 1 << " da 5.\nSelezionare verso (Orizziontale 'O' / Verticale 'V'): ";
			dir5[i][z] = oV(i, z, Player1, Player2, x);
			cout << "\n\nIndicare punto di partenza della nave (Alto a Sinistra), specificando le colonne e le righe. \nAttenzione ad usare bene gli spazi (es. 6 3): ";
			stringaInt(b, a);       // rig - col
            //Verificare e inserire parametri
            u = verifica(z, Player1, Player2, a, b, dir5, i, x);
            if(u == false)
            {
                cout << "\nI valori inseriti non entrano nella mappa. Riprovare\n";
                i--;
                system("pause");
            }else
                assegna(z, Player1, Player2, a, b, dir5, i, x);

            system("cls");
		}

        //Navi da 3
		for (int i = 0; i < 3; i++)
		{
            x = 3;
            cout << "GIOCATORE " << z + 1 << ":\n\tSelezionare le proprie navi: \n\t3 x Navi lunghezza 5\n     ---3 x Navi lunghezza 3---\n\t4 x navi lunghezza 2\n\n";
            sPlayer(z, Player1, Player2);
			cout << "Nave " << i + 1 << " da 3.\nSelezionare verso (Orizziontale 'O' / Verticale 'V'): ";
			dir3[i][z] = oV(i, z, Player1, Player2, x);
			cout << "\n\nIndicare punto di partenza della nave (Alto a Sinistra), specificando le colonne e le righe. \nAttenzione ad usare bene gli spazi (es. 6 3): ";
			stringaInt(b, a);       // righe - colonne
            //Verificare parametri
            u = verifica(z, Player1, Player2, a, b, dir3, i, x);
            if(u == false)
            {
                cout << "\nI valori inseriti non entrano nella mappa. Riprovare\n";
                i--;
                system("pause");
            }else
                assegna(z, Player1, Player2, a, b, dir3, i, x);

            system("cls");
		}

        //Navi da 2
		for (int i = 0; i < 4; i++)
		{
            x = 2;
            cout << "GIOCATORE " << z + 1 << ":\n\tSelezionare le proprie navi: \n\t3 x Navi lunghezza 5\n\t3 x Navi lunghezza 3\n     ---4 x navi lunghezza 2---\n\n";
            sPlayer(z, Player1, Player2);
			cout << "Nave " << i + 1 << " da 2.\nSelezionare verso (Orizziontale 'O' / Verticale 'V'): ";
			dir2[i][z] = oV(i, z, Player1, Player2, x);
			cout << "\n\nIndicare punto di partenza della nave (Alto a Sinistra), specificando le colonne e le righe. \nAttenzione ad usare bene gli spazi (es. 6 3): ";
			stringaInt(b, a);       // rig - col
            //Verificare parametri
            u = verifica(z, Player1, Player2, a, b, dir2, i, x);
            if(u == false)
            {
                cout << "\nI valori inseriti non entrano nella mappa. Riprovare\n";
                i--;
                system("pause");
            }else
                assegna(z, Player1, Player2, a, b, dir2, i, x);
            
            system("cls");
		}
        bool t = false;

    
        sPlayer(z, Player1, Player2);
        cout << "\nHai terminato la tua configurazione.\nInserisci la password necessaria per iniziare il gioco: ";
        getline(cin, p[z]);
        cout << "\n\n--Premi un tasto per nascondere la mappa--\n\n";
        char ww = getche();
        
        system("cls");

        if(z == 1)
            k = true;
        else
            z++;
	}
    
    cout << "\n--PREMI UN TASTO PER INIZIARE LA PARTITA--\n\n";
    char ww = getche();
    system("cls");

    //Inizio partita
    k = false;
    u = false;      // 0: player1;   1: player2
    bool t;         //stato delle navi dei giocatori

    
    while(k == false)
    {
        if(u == false)
            z = 0;
        else if(u == true)
            z = 1;
        
        getPassword(z, p);      //Chiede password per continuare    rimuovere questa riga se si desifìdera non utilizzare password

        sPlayer(z, Player1, Player2);
        wPlayer(z, wPlayer1, wPlayer2);

        cout << "\nSelezionare la casella da colpire (colonne - righe): ";
        stringaInt(b, a);       //rig - col

        //verifica coordinate
        t = wVerifica(z, Player1, Player2, wPlayer1, wPlayer2, a, b);       //dove z è l'attaccante
       
        system("cls");

        sPlayer(z, Player1, Player2);
        wPlayer(z, wPlayer1, wPlayer2);
        
        if(t == false)          //false se Tutte le navi sono state abbattute 
        {
            cout << "\n\n--GIOCATORE " << z + 1 << " HA VINTO--\n\n";
            k = true;
        }
        
        cout << "Premere un tasto per procedere...\n";
        char ww = getche();
        system("cls");
        
        u = !u;
    }

    cout << "Vuoi giocare un'altra partita? (Y/N): ";
    k = yN();

	if (k == false)
		return 0;
	else
        main();

}

//Stampa mappa giocatore
void sPlayer(int n, int Player1[][11], int Player2[][11])
{
    cout << "GIOCATORE " << n + 1 << "\nLa tua mappa:\n";
    cout << "  0  1  2  3  4  5  6  7  8  9\n";
	for (int i = 0; i < 10; i++)
	{
        cout << i << " ";
		for (int j = 0; j < 10; j++)
		{
			if (n == 0)
			{
				switch (Player1[i][j])
				{
				case 0: cout << "[] "; break;      //vuoto
				case 1: cout << "*  "; break;       //nave
                case 2: cout << "O  "; break;       //lanciato non colpito
                case 3: cout << "X  "; break;       //colpito   
				default: break;
				}
			}
			else if (n == 1)
			{
				switch (Player2[i][j])
				{
				case 0: cout << "[] "; break;      //vuoto
				case 1: cout << "*  "; break;       //nave
                case 2: cout << "O  "; break;       //lanciato non colpito
                case 3: cout << "X  "; break;       //colpito          
				default: break;
				}
			}
		}
		cout << endl;
	}
    cout << endl;
}

//Stampa mappa dell'avversario
void wPlayer(int n, int wPlayer1[][10], int wPlayer2[][10])
{
    cout << "\nLa mappa del tuo avversario:\n";
    cout << "  0  1  2  3  4  5  6  7  8  9\n";
	for (int i = 0; i < 10; i++)
	{
        cout << i << " ";
		for (int j = 0; j < 10; j++)
		{
			if (n == 0)
			{
				switch (wPlayer1[i][j])
				{
				case 0: cout << "[] "; break;      //vuoto
				case 1: cout << "O  "; break;       //lanciato non colpito
                case 2: cout << "X  "; break;       //colpito
				default: break;
				}
			}
			else if (n == 1)
			{
				switch (wPlayer2[i][j])
				{
				case 0: cout << "[] "; break;      //vuoto
				case 1: cout << "O  "; break;       //lanciato non colpito
                case 2: cout << "X  "; break;       //colpito
				default: break;
				}
			}
		}
		cout << endl;
	}
    cout << endl;
}

//Sicuro di voler uscire?
bool yN()
{
	char state;
	state = getche();
	switch (state) {
	case 'y': return true; break;
	case 'n': return false; break;
	default: break;
	}
}

//Orizzontale o verticale input
bool oV(int i, int z, int Player1[][11], int Player2[][11], int x)
{
	char state;
	state = getche();
	if(state == 'o')
        return true;
    else if(state == 'v')
        return false;
    else
    {
        system("cls");
        cout << "GIOCATORE " << z + 1 << ":\n\tSelezionare le proprie navi: \n\t3 x Navi lunghezza 5\n\t3 x Navi lunghezza 3\n\t4 x navi lunghezza 2.\n";
        sPlayer(z, Player1, Player2);
        cout << "Nave " << i + 1 << " da " << x <<".\nSelezionare verso (Orizziontale 'O' / Verticale 'V'): ";
        oV(i, z, Player1, Player2, x);     
    }
}

//Converte la stringa di input in coordinate x y
void stringaInt(int &m, int &n)         // righe - colonne
{
	string stringa;
	getline(cin, stringa);

    string s1, s2;

    int n1 = 0;         //rig
    int n2 = 0;         //col

    if(stringa.length() >= 2 && stringa[1] == ' ')
    {
        s1 = stringa.substr(0, 1);
        s2 = stringa.substr(2, 3);
    }

    n1 = (int)s1[0];
    n2 = (int)s2[0];
    
    if((n1 >= 48 && n1 <= 57) && (n2 >= 48 && n2 <= 57))
    {
        m = stoi(s1);
        n = stoi(s2);
    }
    else
    {
        system("cls");
        cout << "Valori errati...";
        cout << "\n\nIndicare le colonne e le righe. Attenzione ad usare bene gli spazi (es. 6 3): ";
		stringaInt(m, n);
    }
}

//Controllo se le coordinate in input sono valide per l'inserimento della nave
bool verifica(int z, int Player1[][11], int Player2[][11], int rig, int col, bool dir[][2], int q, int x)     //giocatore - matrice 1 - matrice 2 - rig - col - dir - i - lungh nave
{
    if(z == 0)          //giocatore 1
    {
        if(dir[q][z] == 0)      //verticale
        {
            for(int i = 0; i < x; i++)
            {
                if((Player1[rig][col] != 0) || (Player1[rig - 1][col - 1] == 1) || (Player1[rig][col - 1] == 1) || (Player1[rig + 1][col - 1] == 1) || (Player1[rig - 1][col] == 1) || (Player1[rig + 1][col] == 1) || (Player1[rig + 1][col + 1] == 1) || (Player1[rig][col + 1] == 1) || (Player1[rig - 1][col + 1] == 1))
                    return false;
                
                rig++;
            }
        }
        else if(dir[q][z] == 1)       //se orizzontale
        {
            for(int i = 0; i < x; i++)
            {
                if((Player1[rig][col] != 0) || (Player1[rig - 1][col - 1] == 1) || (Player1[rig][col - 1] == 1) || (Player1[rig + 1][col - 1] == 1) || (Player1[rig - 1][col] == 1) || (Player1[rig + 1][col] == 1) || (Player1[rig + 1][col + 1] == 1) || (Player1[rig][col + 1] == 1) || (Player1[rig - 1][col + 1] == 1))
                    return false;
                
                col++;
            }
        }
    }
    else if(z == 1)    //giocarore 2
    {
        if(dir[q][z] == 0)      //verticale
        {
            for(int i = 0; i < x; i++)
            {
                if((Player2[rig][col] != 0) || (Player2[rig - 1][col - 1] == 1) || (Player2[rig][col - 1] == 1) || (Player2[rig + 1][col - 1] == 1) || (Player2[rig - 1][col] == 1) || (Player2[rig + 1][col] == 1) || (Player2[rig + 1][col + 1] == 1) || (Player2[rig][col + 1] == 1) || (Player2[rig - 1][col + 1] == 1))
                    return false;
                
                rig++;
            }
        }
        else if(dir[q][z] == 1)       //se orizzontale
        {
            for(int i = 0; i < x; i++)
            {
                if((Player2[rig][col] != 0) || (Player2[rig - 1][col - 1] == 1) || (Player2[rig][col - 1] == 1) || (Player2[rig + 1][col - 1] == 1) || (Player2[rig - 1][col] == 1) || (Player2[rig + 1][col] == 1) || (Player2[rig + 1][col + 1] == 1) || (Player2[rig][col + 1] == 1) || (Player2[rig - 1][col + 1] == 1))
                    return false;
                
                col++;
            }
        }
    }
    return true;
}

//Inserisce la nave nella mappa
void assegna(int z, int Player1[][11], int Player2[][11], int rig, int col, bool dir[][2], int q, int x)     //giocatore - matrice 1 - matrice 2 - rig - col - dir - i - x(lungh nav)
{
    if(z == 0)              //giocatore 1
    {
        if(dir[q][z] == 1)          // se orizzontale        
            for(int i = 0; i < x; i++)
                Player1[rig][col + i] = 1;
        
        else if(dir[q][z] == 0)     // se verticale
            for(int i = 0; i < x; i++)
                Player1[rig + i][col] = 1;
        
    }else if(z == 1)         //giocatore 2
    {
        if(dir[q][z] == 1)             // se orizzontale
            for(int i = 0; i < x; i++)
                Player2[rig][col + i] = 1;
        
        else if(dir[q][z] == 0)       // se verticale
            for(int i = 0; i < x; i++)
                Player2[rig + i][col] = 1;  
        
    }
}

//Modifica la mappa in base al colpo dell'attaccante
bool wVerifica(int z, int Player1[][11], int Player2[][11], int wPlayer1[][10], int wPlayer2 [][10], int a, int b)         //attaccante - matrice 1 - matrice 2 - visualeW1 - visualeW2 - rig - col 
{
    if(z == 0)      //giocatore 0 attacca -->  player2
    {
        if(Player2[a][b] == 0)
        {
            Player2[a][b] = 2;      //lanciato non colpito
            wPlayer1[a][b] = 1;     //lanciato
        }
        else if(Player2[a][b] == 1)
        {
            Player2[a][b] = 3;      //colpito
            wPlayer1[a][b] = 2;     //colpito
        }
        
        //verifica di navi
        bool cj = false;
        for(int i = 0; i < 10; i++)
            for(int j = 0; j < 10; j++)
                if(Player2[i][j] == 1)
                {
                    cj = true;
                    return true;
                }
        if(cj == false)
            return false;
    }
    else if(z == 1)      //giocatore 1 attacca -->  player1 (0)
    {
        if(Player1[a][b] == 0)
        {
            Player1[a][b] = 2;      //lanciato non colpito
            wPlayer2[a][b] = 1;     //lanciato
        }
        else if(Player1[a][b] == 1)
        {
            Player1[a][b] = 3;      //colpito
            wPlayer2[a][b] = 2;     //colpito
        }

        //verifica della presenza di navi nella mappa
        bool cj = false;
        for(int i = 0; i < 10; i++)
            for(int j = 0; j < 10; j++)
                if(Player1[i][j] == 1)
                {
                    cj = true;
                    return true;
                }
        if(cj == false)
            return false;
    }
}

//verifica correttezza Input password
bool password(int z, string p[])
{
    string g;
    getline(cin, g);

    if(g == p[z])
        return true;
    else 
        return false;
}

//Input password
void getPassword(int z, string p[])
{
    cout << "\nInserisci password GIOCATORE " << z + 1 << " per procedere: ";
    bool pq = password(z, p);
    if(pq == true)
        system("cls");
    else
    {
        cout << "\nPassword errata. Riprovare...\n";
        getPassword(z, p);
    }
}