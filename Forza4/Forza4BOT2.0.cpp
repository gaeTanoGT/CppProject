//FORZA 4 In C++ con BOT Torrisi Gaetano

#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include <vector>

#include <windows.h>

using namespace std;

void stampaMat(int[][7], int);		//mat, livello
int conv(char);						//conversione da carattere a numero intero

string cond(bool);					//condizione per la stampa del giocatore

bool riempiCol(bool, int, int[][7], int);		//data una colonna inserisce un gettone

int verificaMat(int[][7]);						//Verifica la presenza di un vincitore

bool verificaEnd(int[][7]);						//Verifica se tutte le celle sono piene

void sceltaBot(int[][7], bool);      	//Verifica se player1 puo' vincere
void sceltaBot2(int[][7], bool);		//Verifica se player2(bot) puo' vincere
void sceltaBot3(int[][7]);				//Previene la formazione di sequenza di 4 da entrambi i lati della riga
void sceltaBot4(int[][7], bool);		//cerca casualmente (liv: 0, 1) oppure nella cella con piu' possibilita' di vittoria il player2(bot) (liv: 2, 3)

bool assegnaBot(int[][7], int, int&, int);			//sceglie la mossa di player2 basandosi su un sistema di priorita'
void pulisciVet(bool);

bool continuare();			//vuoi sfidarmi un'altra volta?

bool notCol[7];         //array che ha 1 se il bot non deve entrare nella colonna
void pulisciVet();		//porta tutti gli elementi dell'array notCol a 0

void assegna(int[][7], int, int, int, int);		//assegna i gettoni nella matrice effettuando un' animazione

vector<int> vetPrior;           //livello priorità della sequenza       (1 = mossa vittoria, 2 = mossa per non vittoria avversario, 3 = creazione di una possibile sequenza, 4 = colonna adiacente ad avversario, 5 = random)
vector<int> vetRig;             //riga in cui nel caso inserire il gettone player2
vector<int> vetCol;             //colonna in cui nel caso inserire il gettone player2

int selLivello(int&);
/*
    -xx- -->  algoritmica che permette la vittoria di player1
        liv 3 = default
        liv 2 = -sceltaBot[rig & col]- && sceltaBot4 default && sceltaBot2 && sceltabot3 && -!col-
        liv 1 = -sceltaBot[rig & col]- && -rand- && -!notCol- && -sceltaBot2[rig & col] solo prior1-
        liv 0 = -only rand-

    numero di mosse che bloccano la vittoria di player1:
        liv 3 = infinito
        liv 2 = 2 - 4
        liv 1 = 2 - 4
        liv 0 = 0 - 2
*/

int main()
{
    srand(time(NULL));

    int Mat[6][7];
/*
    Mat[0][0] = 2;
    Mat[1][0] = 2;
    Mat[2][0] = 1;
    Mat[3][0] = 2;
    Mat[4][0] = 2;
    Mat[5][0] = 1;

    Mat[0][1] = 2;
    Mat[1][1] = 2;
    Mat[2][1] = 1;
    Mat[3][1] = 2;*/
    //Mat[4][1] = 1;
    Mat[5][1] = 1;

    Mat[0][2] = 0;
    Mat[1][2] = 0;
    Mat[2][2] = 1;
    Mat[3][2] = 2;
    Mat[4][2] = 1;
    Mat[5][2] = 2;

    Mat[0][3] = 0;
    Mat[1][3] = 1;
    Mat[2][3] = 2;
    Mat[3][3] = 2;
    Mat[4][3] = 1;
    Mat[5][3] = 1;

    Mat[0][4] = 0;
    Mat[1][4] = 2;
    Mat[2][4] = 1;
    Mat[3][4] = 2;
    Mat[4][4] = 2;
    Mat[5][4] = 1;
/*
    Mat[0][5] = 2;
    Mat[1][5] = 1;
    Mat[2][5] = 1;
    Mat[3][5] = 2;*/
    //Mat[4][5] = 2;
    Mat[5][5] = 2;
/*
    Mat[0][6] = 1;
    Mat[1][6] = 1;
    Mat[2][6] = 1;
    Mat[3][6] = 2;
    Mat[4][6] = 1;
    Mat[5][6] = 1;*/


    bool z = true;	//per il ciclo iniziale
    bool pl;        // 0 = player 1;  1 = player 2(BOT);

    bool alt = true;        //stato per far alternare il primo player tra una partita e l'altra

    cout << "Benvenuto a FORZA 4\nGioco creato da Gaetano Torrisi\nPer uscire digitare tasto 'c'.\n\tBUON DIVERTIMENTO!\n\n";

    do{
        //pulisco la matrice
        for(int i = 0; i < 6; i++)
            for(int j = 0; j < 7; j++)
                Mat[i][j] = 0;

        /*
         *  area test
        */

        int lev;
        int nF = selLivello(lev);     //livello di gioco
        int nK = 0;         //numero corrente di mosse bloccate
        if(lev == -1)
            return 0;
        system("cls");

        stampaMat(Mat, lev);

        bool k = true;

        if(alt)
        {
            pl = false;
            alt = false;
        }else{
            pl = true;
            alt = true;
        }

        while(k)		//ciclo che termina con vittoria o pareggio di un giocatore
        {
            if(!pl)         //se player 1
            {
                cout << "Giocatore: " << pl + 1 << cond(pl) << "Indicami la colonna: ";
                char ch = getche();

                int x = conv(ch);

                if(x == -2)
                    return 0;

                if(x < 0 || x > 6)		//se viene digitato un carattere diverso da quelli di gioco
                {
                    cout << "\nCoordinate indicate errate. Riprovare.\n";
                    pl = !pl;
                    system("pause");
                }else
                {
                    bool u = riempiCol(pl, x, Mat, lev);

                    if(!u)      //se il gettone non e' stato inserito
                    {
                        cout << "\nIl gettone non puo' entrare nella colonna indicata.\n";
                        pl = !pl;
                        system("pause");
                    }
                }
            }else           //player2 (BOT)
            {
            	if(lev == 0)
                {
                    sceltaBot(Mat, false);
            		sceltaBot4(Mat, false);
                }

				if(lev == 1)
				{
					sceltaBot(Mat, true);
					sceltaBot2(Mat, false);			//limitata ad assegnaBot
					sceltaBot4(Mat, false);
				}

				if(lev == 2 || lev == 3)
				{
					sceltaBot(Mat, true);
					sceltaBot2(Mat, true);
                    sceltaBot3(Mat);
                    sceltaBot4(Mat, true);
				}

                assegnaBot(Mat, lev, nK, nF);
            }

            int y = verificaMat(Mat);           //verifica se esiste un vincitore

            cout << "nK: " << nK << "\tnF: " << nF << endl;
            system("pause");        //******
            system("cls");
            stampaMat(Mat, lev);

            if(y == 3 || y == 4)
            {
                cout << "HA VINTO GIOCATORE " << y - 2;
                if(y == 4)
                    cout << " (BOT)\n";
                else
                    cout << endl;
                k = false;
                system("pause");
            }

            bool end = verificaEnd(Mat);        //verifica se non esitono più celle libere
            if(end)
            {
                cout << "\nIL GIOCO E' TERMINATO IN PAREGGIO.\n\n";
                k = false;
                system("pause");
            }
            else
            {
                system("cls");
                stampaMat(Mat, lev);
            }
            pl = !pl;
        }

        z = continuare();
        system("cls");
    }while(z);

    return 0;
}

void stampaMat(int Mat[][7], int lev)
{
    cout << "Per uscire digitare tasto 'c'.\n\nLivello: ";
    switch(lev)
    {
        case 0:
            cout << "EASY\n";
            break;
        case 1:
            cout << "MEDIUM\n";
            break;
        case 2:
            cout << "HARD\n";
            break;
        case 3:
            cout << "IMPOSSIBLE\n";
            break;
    }
    cout << "  q   w   e   r   t   y   u\n";
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            if(Mat[i][j] == 0)
                cout << "|   ";
            else if(Mat[i][j] == 1)
                cout << "| X ";     //$ = alt + 36
            else if(Mat[i][j] == 2)
                cout << "| O ";
            else if(Mat[i][j] == 3)     //cella di vittoria player1
                cout << "||X|";
            else if(Mat[i][j] == 4)     //cella di vittoria player2
                cout << "||O|";
        }
        cout << "|" << endl;
    }
    cout << endl;
}

int conv(char ch)
{
    switch(ch)
        {
        case 'q':
            return 0;
            break;
        case 'w':
            return 1;
            break;
        case 'e':
            return 2;
            break;
        case 'r':
            return 3;
            break;
        case 't':
            return 4;
            break;
        case 'y':
            return 5;
            break;
        case 'u':
            return 6;
            break;
        case 'c':
            return -2;
            break;
        default:
            return -1;
            break;
        }
}

bool riempiCol(bool pl, int x, int Mat[][7], int lev)            //riempie la colonna restituendo true se effettuata correttamente
{
    if(x != -1)
        for(int i = 5; i >= 0; i--)
            if(Mat[i][x] == 0)
                if(!pl)
                {
                    assegna(Mat, 1, i, x, lev);
                    return true;
                }else
                {
                    assegna(Mat, 2, i, x, lev);
                    return true;
                }

    return false;
}

int verificaMat(int Mat[][7])       //verifica se qualcuno ha vinto
{
    //righe
    for(int i = 0; i < 6; i++)
        for(int j = 0; j <= 3; j++)
            if(Mat[i][j] == Mat[i][j + 1] && Mat[i][j] == Mat[i][j + 2] && Mat[i][j] == Mat[i][j + 3] && Mat[i][j] != 0)
            {
                for(int u = 0; u < 4; u++)
                    Mat[i][j + u] += 2;     //player1 = 3; player2 = 4
                return Mat[i][j];
            }


    //colonne
     for(int j = 0; j < 3; j++)
        for(int i = 0; i < 7; i++)
            if(Mat[j][i] == Mat[j + 1][i] && Mat[j][i] == Mat[j + 2][i] && Mat[j][i] == Mat[j + 3][i] && Mat[j][i] != 0)
            {
                for(int u = 0; u < 4; u++)
                    Mat[j + u][i] += 2;     //player1 = 3; player2 = 4
                return Mat[j][i];
            }

    //diagonale ++
    for(int i = 0; i <= 2; i++)
        for(int j = 0; j <= 3; j++)
            if(Mat[i][j] == Mat[i + 1][j + 1] && Mat[i][j] == Mat[i + 2][j + 2] && Mat[i][j] == Mat[i + 3][j + 3] && Mat[i][j] != 0)
            {
                for(int u = 0; u < 4; u++)
                    Mat[i + u][j + u] += 2;     //player1 = 3; player2 = 4
                return Mat[i][j];
            }

    //diagonale -+
    for(int i = 3; i <= 5; i++)
        for(int j = 0; j <= 3; j++)
            if(Mat[i][j] == Mat[i - 1][j + 1] && Mat[i][j] == Mat[i - 2][j + 2] && Mat[i][j] == Mat[i - 3][j + 3] && Mat[i][j] != 0)
            {
                for(int u = 0; u < 4; u++)
                    Mat[i - u][j + u] += 2;     //player1 = 3; player2 = 4
                return Mat[i][j];
            }

    return 0;
}

string cond(bool pl)
{
    if(pl)
        return " (O). ";
    else
        return " (X). ";
}

void sceltaBot(int Mat[][7], bool p)
{
    pulisciVet();
    //verifico possibilità di vittoria da parte dell'avversario
    for(int i = 0; i <= 5; i++)
        for(int j = 0; j <= 6; j++)
        {
            //righe
            if((((Mat[i][j] == Mat[i][j + 1] || Mat[i][j] == Mat[i][j + 2]) && Mat[i][j] == Mat[i][j + 3])
                || (Mat[i][j] == Mat[i][j + 1] && (Mat[i][j] == Mat[i][j + 2] || Mat[i][j] == Mat[i][j + 3])))
                && (Mat[i][j] != 0 ) && (Mat[i][j] != 2 && Mat[i][j + 1] != 2 && Mat[i][j + 2] != 2 && (Mat[i][j + 3] != 2 || Mat[i][j - 1] != 2)))
            {
                //creo array con possibile formazione di quartetto
                int Vet[5];     //-1 <-to-> 3
                for(int v = 0; v < 5; v++)
                    if(j + v - 1 < 0 || j + v - 1 > 6)      //se la colonna corrispondente è al di fuori della matrice stabilisci player2
                        Vet[v] = 2;
                    else
                        Vet[v] = Mat[i][j + v - 1];

                int e = 0;
                //conto il numero di uno nel vettore
                for(int p = 0; p < 5; p++)
                    if(Vet[p] == 1)
                        e++;

                if(e == 3)      //se il vettore abbia 3 uno
                {
                    bool r = true;      //1 = c'è possibilità che l'avversario alla prossima mossa possa vincere

                    for(int vv = 0; vv < 5; vv++)
                    {
                        //scorro il vettore partendo dal centro:    53124
                        int nV = -1;         // colonna in cui inserire il gettone player2  ||NB BASANDOSI SULL'ARRAY -1 <-to-> 3
                        if(vv == 0 && Vet[2] == 0)
                            nV = 2;
                        else if(vv == 1 && Vet[3] == 0)
                            nV = 3;
                        else if(vv == 2 && Vet[1] == 0)
                            nV = 1;
                        else if(vv == 3 && Vet[4] == 0)
                            nV = 4;
                        else if(vv == 4 && Vet[0] == 0)
                            nV = 0;

                        if(nV != -1)        //se esiste almeno un buco per impedire la sequenza di 4
                        {
                            if(nV == 0)
                                cout << Mat[i + 1][j + nV - 1] << " " << Mat[i][j + nV - 1] << endl;
                            if(Mat[i + 1][j + nV - 1] != 0 && Mat[i][j + nV - 1] == 0 && (r || nV == 0))          // && j > 0
                            {
                                vetPrior.push_back(2);      //livello priorità = 2
                                vetRig.push_back(i);
                                vetCol.push_back(j + nV - 1);
                            }
                            if(Mat[i + 1][j + nV - 1] == 0 && (Mat[i + 2][j + nV - 1] != 0 || i + 1 == 5))         //se colonna ha base vuota, oppure è all'ultima riga: dico al bot di non selezionare quella colonna
                            {
                                notCol[j + nV - 1] = 1;
                                r = false;
                            }
                        }
                    }
                }
            }

            //colonne
            if((Mat[i][j] == Mat[i + 1][j] && Mat[i][j] == Mat[i + 2][j])
                && (Mat[i][j] != 2 && Mat[i + 1][j] != 2 && Mat[i + 2][j] != 2
                && (Mat[i + 3][j] != 2 || Mat[i - 1][j] != 2)) && (Mat[i][j] != 0 || Mat[i + 1][j] != 0))
                if((i <= 3 && i >= 1) && Mat[i - 1][j] == 0)
                {
                    vetPrior.push_back(2);      //livello priorità = 2
                    vetRig.push_back(i - 1);
                    vetCol.push_back(j);
                }

            if(p)		//se siamo al livello 3
            {
                //diagonale ++
                if((((Mat[i][j] == Mat[i + 1][j + 1] || Mat[i][j] == Mat[i + 2][j + 2])
                    && Mat[i][j] == Mat[i + 3][j + 3]) || (Mat[i][j] == Mat[i + 1][j + 1] && (Mat[i][j] == Mat[i + 2][j + 2]
                    || Mat[i][j] == Mat[i + 3][j + 3]))) && (Mat[i][j] != 0) && (Mat[i][j] != 2 && Mat[i + 1][j + 1] != 2
                    && Mat[i + 2][j + 2] != 2 && (Mat[i + 3][j + 3] != 2 || Mat[i - 1][j - 1] != 2)))
                {
                    //creo array con possibile formazione di quartetto
                    int Vet[5];     //-1 <-to-> 3
                    for(int v = 0; v < 5; v++)
                        if((j + v - 1 < 0 || j + v - 1 > 6) || (i + v - 1 < 0 || i + v - 1 > 5))        //se la colonna o la riga corrispondente è al di fuori della matrice stabilisci player2
                            Vet[v] = 2;
                        else
                            Vet[v] = Mat[i + v - 1][j + v - 1];

                    int e = 0;
                    //conto il numero di uno nel vettore
                    for(int p = 0; p < 5; p++)
                        if(Vet[p] == 1)
                            e++;

                    if(e == 3)          //se ci sono aleno 3 uno
                    {
                        bool r = true;

                        for(int vv = 0; vv < 5; vv++)
                        {
                            //scorro il vettore partendo dal centro:    53124
                            int nV = -1;         // colonna in cui inserire il gettone player2  ||NB BASANDOSI SULL'ARRAY -1 <-to-> 3
                            if(vv == 0 && Vet[2] == 0)
                                nV = 2;
                            else if(vv == 1 && Vet[3] == 0)
                                nV = 3;
                            else if(vv == 2 && Vet[1] == 0)
                                nV = 1;
                            else if(vv == 3 && Vet[4] == 0)
                                nV = 4;
                            else if(vv == 4 && Vet[0] == 0)
                                nV = 0;

                            if(nV != -1)        //se esiste almeno un buco per impedire il 4tetto
                            {
                                if((Mat[i + nV][j + nV - 1] != 0 || (i + nV - 1) == 5) && Mat[i + nV - 1][j + nV - 1] == 0 && (r || nV == 0))        //se hai la base sotto la diagonale oppure se sei alla riga 5
                                {
                                    vetPrior.push_back(2);      //livello priorità = 2
                                    vetRig.push_back(i + nV - 1);
                                    vetCol.push_back(j + nV - 1);
                                }                             //altrimenti se nella colonna precedente sotto è vuoto guarda le altre colonne
                                if(Mat[i + nV][j + nV - 1] == 0 && (Mat[i + nV + 1][j + nV - 1] != 0 || i + nV == 5))         //se colonna ha base vuota, dico al bot di non selezionare quella colonna
                                {
                                    notCol[j + nV - 1] = 1;
                                    r = false;
                                }
                            }
                        }
                    }
                        
                }

                //diagonale -+
                if((((Mat[i][j] == Mat[i - 1][j + 1] || Mat[i][j] == Mat[i - 2][j + 2])
                    && Mat[i][j] == Mat[i - 3][j + 3]) || (Mat[i][j] == Mat[i - 1][j + 1]
                    && (Mat[i][j] == Mat[i - 2][j + 2] || Mat[i][j] == Mat[i - 3][j + 3])))
                    && (Mat[i][j] != 0 ) && (Mat[i][j] != 2 && Mat[i - 1][j + 1] != 2 && Mat[i - 2][j + 2] != 2
                    && (Mat[i - 3][j + 3] != 2 || Mat[i + 1][j - 1] != 2)))
                {
                    //creo array con possibile formazione di quartetto
                    int Vet[5];     //-1 <-to-> 3
                    for(int v = 0; v < 5; v++)
                        if((j + v - 1 < 0 || j + v - 1 > 6) || (i - v + 1 < 0 || i - v + 1 > 5))        //se la colonna o la riga corrispondente è al di fuori della matrice stabilisci player2
                            Vet[v] = 2;
                        else
                            Vet[v] = Mat[i - v + 1][j + v - 1];

                    int e = 0;
                    //conto il numero di uno nel vettore
                    for(int p = 0; p < 5; p++)
                        if(Vet[p] == 1)
                            e++;

                    if(e == 3)          //se ci sono aleno 3 uno
                    {
                        bool r = true;

                        for(int vv = 0; vv < 5; vv++)
                        {
                            //scorro il vettore partendo dal centro:    53124
                            int nV = -1;         // colonna in cui inserire il gettone player2  ||NB BASANDOSI SULL'ARRAY -1 <-to-> 3
                            if(vv == 0 && Vet[2] == 0)
                                nV = 2;
                            else if(vv == 1 && Vet[3] == 0)
                                nV = 3;
                            else if(vv == 2 && Vet[1] == 0)
                                nV = 1;
                            else if(vv == 3 && Vet[4] == 0)
                                nV = 4;
                            else if(vv == 4 && Vet[0] == 0)
                                nV = 0;

                            if(nV != -1)
                            {
                                if((Mat[i - nV + 2][j + nV - 1] != 0 || (i - nV + 1) == 5) && Mat[i - nV + 1][j + nV - 1] == 0 && (r || nV == 0))        //se hai la base sotto la diagonale oppure se sei alla riga 5
                                {
                                    vetPrior.push_back(2);      //livello priorità = 2
                                    vetRig.push_back(i - nV + 1);
                                    vetCol.push_back(j + nV - 1);
                                }                      //altrimenti se nella colonna precedente sotto è vuoto guarda le altre colonne
                                if(Mat[i - nV + 2][j + nV - 1] == 0 && (Mat[i - nV + 3][j + nV - 1] != 0 || i - nV + 2 == 5))         //se colonna ha base vuota, dico al bot di non selezionare quella colonna
                                {
                                    notCol[j + nV - 1] = 1;
                                    r = false;
                                }
                            }
                        }
                    }
                }
            }
        }
}

void sceltaBot2(int Mat[][7], bool p)
{
    //verifico possibilità di sequenza di 4 player2(bot)
    for(int i = 5; i >= 0; i--)         //decrescente per far analizzare prima le possibili combinazioni più accessibili al bot
        for(int j = 0; j <= 6; j++)
        {
            //righe
            if((Mat[i][j] == 2) && ((Mat[i][j] == Mat[i][j + 1] || Mat[i][j] == Mat[i][j + 2])
                || Mat[i][j] == Mat[i][j + 3]) && (Mat[i][j] != 1 && Mat[i][j + 1] != 1 && Mat[i][j + 2] != 1
                && (Mat[i][j + 3] != 1 || Mat[i][j - 1] != 1)))     //j + 3 < 7
            {
                //cout << "SUCCHIAMI IL CAZZO\t" << i << "\t"  << j << endl;
                
                //creo array con possibile formazione di quartetto
                int Vet[5];     //-1 <-to-> 3
                for(int v = 0; v < 5; v++)
                {
                    if(j + v - 1 < 0 || j + v - 1 > 6)      //se la colonna corrispondente è al di fuori della matrice stabilisci player1(da non considerare)
                        Vet[v] = 1;
                    else
                        Vet[v] = Mat[i][j + v - 1];
                    //cout << Vet[v] << "\t";

                }
                //cout << endl;


                //verifico livello di priorità contando il numero di 1
                int num2[2] = {0, 0};       //[0] = contatore di numero di 2 nel vettore; [1] = contatore numero 0
                for(int kk = 0; kk < 5; kk++)
                {
                    if(Vet[kk] == 2)
                        num2[0] += 1;
                    if(Vet[kk] == 0)
                        num2[1] += 1;
                }

                //cout << "Num: " << num2[0] << "\t" << num2[1] << endl;      //3  2

                bool r = true;          //1 = possibilità di vittoria, 0 = caso in cui non è presente la base al di sotto della cella in cui si deve fare la mossa

                for(int vv = 0; vv < 5; vv++)			//scorro il vettore co nbsequenza   53124
                {
                    //cout << "vv " << vv << r << endl;
                    int nV = -1;
                    if(vv == 0 && Vet[2] == 0)
                        nV = 2;
                    else if(vv == 1 && Vet[3] == 0)
                        nV = 3;
                    else if(vv == 2 && Vet[1] == 0)
                        nV = 1;
                    else if(vv == 3 && Vet[4] == 0)
                        nV = 4;
                    else if(vv == 4 && Vet[0] == 0)
                        nV = 0;

                    //cout << "il nV di "<< vv << "\t" << nV << endl;

                    if(nV != -1)
                    {
                        if(Mat[i + 1][j + nV - 1] != 0)         //se esiste una base
                        {
                            if(num2[0] == 3 && num2[1] >= 1 && r)     //se ci sono tre 2 e almeno una cella vuota
                            {
                                vetPrior.push_back(1);      //livello priorità = 1
                                vetRig.push_back(i);
                                vetCol.push_back(j + nV - 1);
                                break;
                            }else if((num2[0] == 2 || !r) && num2[1] >= 2)      //se ci sono due 2 e almeno due celle vuote     //((num2[0] == 2 || (!r && num2[0] == 3)) && num2[1] >= 2)
                            {
                                vetPrior.push_back(4);      //livello priorità = 4
                                vetRig.push_back(i);
                                vetCol.push_back(j + nV - 1);
                                break;
                            }
                        }else if(Mat[i + 1][j + nV - 1] == 0 && num2[1] == 1)   //&& Mat[i][j + nV - 1] == 0   //se non è presente la base al di sotto ed è presente una sola cella vuota non considerare la situazione
                        {
                            break;
                            //cout << "OPS COGLIONE" << endl;
                        }
                        else if(Mat[i + 1][j + nV - 1] == 0 && nV != 4)        //caso num2[1] == 1 && no base al disotto nel mezzo del vettore
                            r = false;      //se non hai la base al disotto la priorità corrisponderà a 4
                    }
                }

                //system("pause");
            }

            //colonne       (da alto a basso)
            if((Mat[i][j] == 2) && (Mat[i][j] == Mat[i + 1][j] || Mat[i][j] == Mat[i + 2][j])
                && (Mat[i][j] != 1 && Mat[i + 1][j] != 1 && Mat[i + 2][j] != 1 && Mat[i - 1][j] != 1))
            {
                if((i <= 3 && i >= 1) && Mat[i - 1][j] == 0)        //se entra nel range della matrice
                {
                    int Vet[3];
                    for(int v = 0; v < 3; v++)
                        Vet[v] = Mat[i + v][j];

                    //verifico livello di priorità contando il numero di 1
                    int num2 = 0;       //contatore di numero di 2 nel vettore
                    for(int kk = 0; kk < 3; kk++)
                        if(Vet[kk] == 2)
                            num2++;

                    if(num2 == 3)
                    {
                        vetPrior.push_back(1);      //livello priorità = 1
                        vetRig.push_back(i - 1);
                        vetCol.push_back(j);
                        break;
                    }else if(num2 == 2)
                    {
                        vetPrior.push_back(4);      //livello priorità = 4
                        vetRig.push_back(i - 1);
                        vetCol.push_back(j);
                        break;
                    }
                }
            }

			if(p)
			{
				//diagonale ++
	            if((Mat[i][j] == 2) && (((Mat[i][j] == Mat[i + 1][j + 1] || Mat[i][j] == Mat[i + 2][j + 2])
	                || Mat[i][j] == Mat[i + 3][j + 3]) ) && (Mat[i][j] != 1 && Mat[i + 1][j + 1] != 1
	                && Mat[i + 2][j + 2] != 1 && (Mat[i + 3][j + 3] != 1 || Mat[i - 1][j - 1] != 1)))
	            {
	                int Vet[5];     //-1 <-to-> 3
	                for(int v = 0; v < 5; v++)
	                    if((j + v - 1 < 0 || j + v - 1 > 6) || (i + v - 1 < 0 || i + v - 1 > 5))        //se la colonna o la riga corrispondente è al di fuori della matrice stabilisci player1
	                        Vet[v] = 1;
	                    else
	                        Vet[v] = Mat[i + v - 1][j + v - 1];

	                int num2[2] = {0, 0};
	                for(int kk = 0; kk < 5; kk++)
	                {
	                    if(Vet[kk] == 2)
	                        num2[0] += 1;
	                    if(Vet[kk] == 0)
	                        num2[1] += 1;
	                }

                    bool r = true;          //1 = possibilità di vittoria, 0 = caso in cui non è presente la base al di sotto della cella in cui si deve fare la mossa

	                //scorro vettore in ordine 53124
	                for(int vv = 0; vv < 5; vv++)
	                {
	                    int nV = -1;
	                    if(vv == 0 && Vet[2] == 0)
	                        nV = 2;
	                    else if(vv == 1 && Vet[3] == 0)
	                        nV = 3;
	                    else if(vv == 2 && Vet[1] == 0)
	                        nV = 1;
	                    else if(vv == 3 && Vet[4] == 0)
	                        nV = 4;
	                    else if(vv == 4 && Vet[0] == 0)
	                        nV = 0;

	                    if(nV != -1)
	                    {
	                        if((Mat[i + nV][j + nV - 1] != 0 || (i + nV - 1) == 5) && Mat[i + nV - 1][j + nV - 1] == 0)        //se hai la base sotto la diagonale oppure se sei alla riga 5
	                        {

	                            if(num2[0] == 3 && num2[1] >= 1 && r)
	                            {
	                                vetPrior.push_back(1);      //livello priorità = 1
	                                vetRig.push_back(i + nV - 1);
	                                vetCol.push_back(j + nV - 1);
	                                break;
	                            }else if((num2[0] == 2 || !r) && num2[1] >= 2)
	                            {
	                                vetPrior.push_back(4);      //livello priorità = 4
	                                vetRig.push_back(i + nV - 1);
	                                vetCol.push_back(j + nV - 1);
	                                break;
	                            }
	                        }else if(Mat[i + nV][j + nV - 1] == 0 && num2[1] == 1)   //se non è presente la base al di sotto non considerare la situazione
	                            break;
                            else if(Mat[i + nV][j + nV - 1] == 0 && nV != 4)
                                r = false;      //se non hai la base al disotto la priorità corrisponderà a 4
	                    }
	                }
	            }

	            //diagonale -+
	            if((Mat[i][j] == 2) && (((Mat[i][j] == Mat[i - 1][j + 1] || Mat[i][j] == Mat[i - 2][j + 2])
	                || Mat[i][j] == Mat[i - 3][j + 3]) ) && (Mat[i][j] != 1 && Mat[i - 1][j + 1] != 1
	                && Mat[i - 2][j + 2] != 1 && (Mat[i - 3][j + 3] != 1 || Mat[i + 1][j - 1] != 1)))
	            {
	                //creo array con possibile formazione di quartetto
	                int Vet[5];     //-1 <-to-> 3
	                for(int v = 0; v < 5; v++)
	                    if((j + v - 1 < 0 || j + v - 1 > 6) || (i - v + 1 < 0 || i - v + 1 > 5))        //se la colonna o la riga corrispondente è al di fuori della matrice stabilisci player1
	                        Vet[v] = 1;
	                    else
	                        Vet[v] = Mat[i - v + 1][j + v - 1];

	                int num2[2] = {0, 0};               //dA MEttere fuori dal for(vv)
	                for(int kk = 0; kk < 5; kk++)
	                {
	                    if(Vet[kk] == 2)
	                        num2[0] += 1;
	                    if(Vet[kk] == 0)
	                        num2[1] += 1;
	                }

                    bool r = true;          //1 = possibilità di vittoria, 0 = caso in cui non è presente la base al di sotto della cella in cui si deve fare la mossa

	                for(int vv = 0; vv < 5; vv++)
	                {
	                    int nV = -1;
	                    if(vv == 0 && Vet[2] == 0)
	                        nV = 2;
	                    else if(vv == 1 && Vet[3] == 0)
	                        nV = 3;
	                    else if(vv == 2 && Vet[1] == 0)
	                        nV = 1;
	                    else if(vv == 3 && Vet[4] == 0)
	                        nV = 4;
	                    else if(vv == 4 && Vet[0] == 0)
	                        nV = 0;

	                    if(nV != -1)
	                    {
	                        if((Mat[i - nV + 2][j + nV - 1] != 0 || (i - nV + 1) == 5) && Mat[i - nV + 1][j + nV - 1] == 0)        //se hai la base sotto la diagonale oppure se sei alla riga 5
	                        {

	                            if(num2[0] == 3 && num2[1] >= 1 && r)
	                            {
	                                vetPrior.push_back(1);      //livello priorità = 1
	                                vetRig.push_back(i - nV + 1);
	                                vetCol.push_back(j + nV - 1);
	                            }else if((num2[0] == 2 || !r) && num2[1] >= 2)
	                            {
	                                vetPrior.push_back(4);      //livello priorità = 4
	                                vetRig.push_back(i - nV + 1);
	                                vetCol.push_back(j + nV - 1);
	                            }
	                        }else if(Mat[i - nV + 2][j + nV - 1] == 0 && num2[1] == 1)   //se non è presente la base al di sotto non considerare la situazione
	                            break;
                            else if(Mat[i - nV + 2][j + nV - 1] == 0 && nV != 4)
                                r = false;      //se non hai la base al disotto la priorità corrisponderà a 4
	                    }
	                }
	            }
			}
        }
}

void sceltaBot3(int Mat[][7])
{
    //verifico possibilità di fregatura da parte di player1
    for(int i = 5; i >= 0; i--)         //decrescente per far analizzare prima le possibili combinazioni più accessibili al bot
        for(int j = 0; j <= 6; j++)
        {
            //righe
            if((Mat[i][j] == 1) && ((Mat[i][j] == Mat[i][j + 1] || Mat[i][j] == Mat[i][j + 2])
                && (Mat[i][j] != 2 && Mat[i][j + 1] != 2 && Mat[i][j + 2] != 2
                && (Mat[i][j + 3] != 2 || Mat[i][j - 1] != 2)) && (j + 2 < 7)))     //j + 3 < 7
            {
                bool kk = true;
                //verifico che esista una base
                for(int qq = 0; qq < 7; qq++)
                    if(Mat[i + 1][j + qq - 1] == 0)
                    {
                        kk = false;
                        break;
                    }

                //se la base esiste allora procedi
                if(i == 5 || kk)
                {
                    //creo array con possibile formazione di quartetto
                    int Vet[5];     //-1 <-to-> 3
                    for(int v = 0; v < 5; v++)
                        if(j + v - 1 < 0 || j + v - 1 > 6)      //se la colonna corrispondente è al di fuori della matrice stabilisci player2(da non considerare)
                            Vet[v] = 2;
                        else
                            Vet[v] = Mat[i][j + v - 1];

                    int num2[2] = {0, 0};       //contatore di numero di 2 nel vettore,  [1] = contatore di 0
                    for(int kk = 0; kk < 5; kk++)
                    {
                        if(Vet[kk] == 1)
                            num2[0] += 1;
                        if(Vet[kk] == 0)
                            num2[1] += 1;
                    }

                    for(int vv = 0; vv < 5; vv++)           //  53124
                    {
                        int nV = -1;
                        if(vv == 0 && Vet[2] == 0)
                            nV = 2;
                        else if(vv == 1 && Vet[3] == 0)
                            nV = 3;
                        else if(vv == 2 && Vet[1] == 0)
                            nV = 1;
                        else if(vv == 3 && Vet[4] == 0)
                            nV = 4;
                        else if(vv == 4 && Vet[0] == 0)
                            nV = 0;

                        if(nV != -1)
                        {
                            if(((Mat[i + 1][j + nV - 1] != 0 || i == 5) && Mat[i][j + nV - 1] == 0) && (num2[0] >= 2 && num2[1] >= 2))
                            {
                                vetPrior.push_back(3);      //livello priorità = 3
                                vetRig.push_back(i);
                                vetCol.push_back(j + nV - 1);
                                break;
                            }
                        }
                    }
                }
            }
        }
}

void sceltaBot4(int Mat[][7], bool p)
{
    if(p)        //livello 3, 2
    {
        //scelta seguendo possibilità max di combianzioni
        /*
        3   4   5   7   5   4   3
        4   6   8   10  8   6   4
        5   8   11  13  11  8   5
        5   8   11  13  11  8   5
        4   6   8   10  8   6   4
        3   4   5   7   5   4   3
        */
        int comb[6][7] = {
            {3,  4,  5,  7,  5,  4,  3},
            {4,  6,  8,  10, 8,  6,  4},
            {5,  8,  11, 13, 11, 8,  5},
            {5,  8,  11, 13, 11, 8,  5},
            {4,  6,  8,  10, 8,  6,  4},
            {3,  4,  5,  7,  5,  4,  3}
        };

        //scorro colonne cercando la colonna col max numero di combinazioni
        int max = -1;
        int rig = 0;
        int col = 0;
        for(int j = 0; j < 7; j++)
            for(int i = 5; i >= 0; i--)
            {
                if(comb[i][j] > max && notCol[j] == 0 && Mat[i][j] == 0)
                {
                    max = comb[i][j];
                    rig = i;
                    col = j;
                }
                if(Mat[i][j] == 0)
                    break;
            }

        if(max != -1)
        {
            vetPrior.push_back(5);      //livello priorità = 5
            vetRig.push_back(rig);
            vetCol.push_back(col);
        }else
            sceltaBot4(Mat, false);

    }else
    {
        //altrimenti random -- livello 0, 1
        int c = rand() % 7;
        if(Mat[0][c] == 0)      //se c'è sempre spazio nella colonna corrispondente per il gettone
        {
            vetPrior.push_back(6);      //livello priorità = 6
            vetRig.push_back(-1);           //la riga deve essere rilevata al momento dell'assegnazione
            vetCol.push_back(c);
            //riempiCol(true, c, Mat);
        }else
            sceltaBot4(Mat, false);
    }

}

void pulisciVet()
{
    for(int i = 0; i < 7; i++)
        notCol[i] = 0;
}

bool assegnaBot(int Mat[][7], int lev, int& nK, int nF)
{
    //verifico esisitenza di coordinate a priorità 1
    for(int i = 0; i < vetPrior.size(); i++)
    {
        if(vetPrior[i] == 1 && lev != 0)
        {
            int rig = vetRig[i];
            int col = vetCol[i];
            assegna(Mat, 2, rig, col, lev);
        	cout << "PRIOR 1\t"<< vetRig[i]<<"\t"<<vetCol[i]<< endl;
            pulisciVet(true);
            break;
            return false;
        }
    }
    //verifico esisitenza di coordinate a priorità 2
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 2)
        {
            int rig = vetRig[i];
            int col = vetCol[i];
            if((notCol[col] == 0 && lev == 3) || lev != 3)        //se la mossa non porta la vittoria di player1
            {
                nK++;
                if(nK <= nF || lev == 3)
                {
                    assegna(Mat, 2, rig, col, lev);
                    cout << "PRIOR 2\t"<< vetRig[i]<<"\t"<<vetCol[i]<< endl;
                    pulisciVet(true);
                    break;
                    return false;
                }
            }
        }

    //verifico esisitenza di coordinate a priorità 3
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 3 && lev != 1)
        {
            int rig = vetRig[i];
            int col = vetCol[i];
            if((notCol[col] == 0 && lev == 3) || lev != 3)
            {
                assegna(Mat, 2, rig, col, lev);
                cout << "PRIOR 3\t"<< vetRig[i]<<"\t"<<vetCol[i]<< endl;
                pulisciVet(true);
                break;
                return false;
            }
        }

    //verifico esisitenza di coordinate a priorità 4
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 4)
        {
            int rig = vetRig[i];
            int col = vetCol[i];
            if(notCol[col] == 0 && lev == 3)
            {
                nK++;
                assegna(Mat, 2, rig, col, lev);
                cout << "PRIOR 4\t"<< vetRig[i]<<"\t"<<vetCol[i]<< endl;
                pulisciVet(true);
                break;
                return false;
            }
        }

    //verifico esisitenza di coordinate a priorità 5
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 5)
        {
            int rig = vetRig[i];
            int col = vetCol[i];
            assegna(Mat, 2, rig, col, lev);
            cout << "PRIOR 5\t"<< vetRig[i]<<"\t"<<vetCol[i]<< endl;
            pulisciVet(true);
            break;
            return false;
        }

    //verifico esisitenza di coordinate a priorità 6
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 6)
        {
            riempiCol(true, vetCol[i], Mat, lev);
            cout << "PRIOR 6\t"<< vetRig[i]<<"\t"<<vetCol[i]<< endl;
            pulisciVet(true);
            break;
            return false;
        }
}

void pulisciVet(bool z)
{
    vetPrior.clear();
    vetRig.clear();
    vetCol.clear();
}

bool verificaEnd(int Mat[][7])
{
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 7; j++)
            if(Mat[i][j] == 0)
                return false;

    return true;
}

bool continuare()
{
    cout << "\nVuoi sfidarmi un'altra volta? (Y/N): ";
    char ch = getche();

    switch(ch)
    {
        case 'y':
            return true;
            break;
        case 'Y':
            return true;
            break;
        case 'n':
            return false;
            break;
        case 'N':
            return false;
            break;
        default:
            cout << "\nMmm non ho capito bene... Riprova.";
            return continuare();
            break;
    }
}

int selLivello(int& lev)
{
    /*
    -xx- -->  algoritmica che permette la vittoria di player1
        liv 3 = default
        liv 2 = -sceltaBot[rig & col]- && sceltaBot4 default && sceltaBot2 && sceltabot3 && -!col-
        liv 1 = -sceltaBot[rig & col]- && -rand- && -!notCol- && -sceltaBot2[rig & col] solo prior1-
        liv 0 = -only rand-

    numero di mosse che bloccano la vittoria di player1:
        liv 3 = infinito
        liv 2 = 2 - 4
        liv 1 = 2 - 4
        liv 0 = 0 - 2
    */

    cout << "Livello di gioco:\n\t0: Easy\n\t1: Medium\n\t2: Hard\n\t3: Impossible\n\nSelezionare il livello da tastiera: ";
    char ch = getche();

    int x;

    //calcolo il numero di volte in cui il bot dovrà impedire la vittoria di player1 in base al livello selezionato
    switch(ch)
    {
        case '0':
            lev = 0;
            return rand() % 3;      //0 - 2
            break;
        case '1':
            lev = 1;
            return rand() % 3 + 2;  //2 - 4
            break;
        case '2':
            lev = 2;
            return rand() % 3 + 2;  //2 - 4
            break;
        case '3':
            lev = 3;
            return -2;  //il bot dovrà coprire tutte le possibilità di vittoria di player1
            break;
        case 'c':       //uscita
            return -1;
        default:
            cout << "\nMmm non ho capito bene... Riprova.\n\n";
            system("pause");
            system("cls");
            x = selLivello(lev);
            break;
    }
}

void assegna(int Mat[][7], int pl, int rig, int col, int lev)
{
    for(int i = 0; i <= rig; i++)
    {
        if(pl == 1)
            Mat[i][col] = 1;
        else if(pl == 2)
            Mat[i][col] = 2;

        system("cls");
        stampaMat(Mat, lev);

        if(i != rig)
        {
            Sleep(100);
            Mat[i][col] = 0;
        }
    }
}
