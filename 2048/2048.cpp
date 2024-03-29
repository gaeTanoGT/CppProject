//2048      Torrisi Gaetano

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

void stampaMat(int [][4]);
void cond(int, int);                    //Condizione per stampaMat
int getDir();                           // 0 = (UP);   1 = (SX);    2 = (DW);    3 = (DX);
bool traslazione(int, int [][4]);       //False = nessuna traslazione eseguita; True almeno una effettuata
bool somma(int, int [][4]);             //False = nessuna somma eseguita; True almeno una effettuata

bool yN();

bool verificaContinuaggio(int [][4]);       //0 = game over
bool verificaSomma(int [][4]);              //1 = è possibile eseguire una somma
bool haiVinto(int [][4]);                   //1 = 2048

int main()
{
    srand(time(NULL));

    int Mat[4][4];
    /*
    *   0 = vuoto
    *   2
    *   4
    *   8
    *   16
    *   ...
    */

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            Mat[i][j] = 0;

    
    /*//Per il test di vincita a 2048 immediato:
    Mat[0][0] = 1024;
    Mat[1][0] = 1024;
    */

    bool z = 1;         //Variabile booleana per il ciclo while
    bool up2048 = 0;    //True se si è superato il valore 2048

    bool h;             //Variabile booleana per condizioni di verifica vincita e proseguimento

    bool t, s, sV;      //Traslazione e somma (True eseguita; False non eseguita)

    bool p;
    cout << "2048 in C++ creato da Torrisi Gaetano.\n\nPer uscire utilizzare il tasto esc.\n\nSei sicuro di voler iniziare? (Y/N): ";
    p = yN();

    if(!p)
        return 0;
    else
    {
        while(z)
        {
            bool k = 1;
            int x, y;
            while(k)
            {
                //Genero casualmente la posizione per l'asseganzione del valore 2
                x = rand() % 4;
                y = rand() % 4;
                
                //Verifico che la casella sia vuota
                if(Mat[y][x] == 0)
                    k = false;
            }
            
            Mat[y][x] = 2;

            system("cls");
            stampaMat(Mat);

            if(!up2048)         //Se il valore 2048 non è ancora stato raggiunto
            {
                h = haiVinto(Mat);                          //True = 2048
                if(h)
                {
                    up2048 = true;
                    cout << "Complimenti hai vinto!\n\nVuoi continuare a giocare? (Y/N): ";
                    p = yN();
                    if(!p)
                        return 0;
                }
            }

            int dir;
            do{
                sV = verificaSomma(Mat);             //Verifica se è possibile effettuare una somma (1: si somma)
                h = verificaContinuaggio(Mat);       //False: tutte le caselle sono piene
                if(sV || h)               //Se si può fare almeno una somma o almeno uno spostamento (almeno una cella della matrice è vuota)
                {
                    dir = getDir();     //Leggo la direzione
                    if(dir == -1)       //Caso di digitazione tasto esc
                        return 0;
                }else{
                    cout << "\tGAME OVER!\n\n";
                    system("pause");
                    system("pause");
                    return 0;
                }
                //Eseguo somma e traslazione dei numeri
                t = traslazione(dir, Mat);      //True se è stata eseguita almeno una traslazione
                s = somma(dir, Mat);            //True se è stata eseguita almeno una somma

                //Condizione di errore
                if(!(t || s))                   //Se nè traslazione nè somma sono stati eseguiti
                    cout << "\nImpossibile eseguire la traslazione indicata. Utilizzane un'altra." << endl;

            }while(!t && !s);              //Ripeti fino a quando viene effettuata almeno una somma e/o una traslazione

            if(s)       //Se è stata effetuata almeno una somma
                traslazione(dir, Mat);
        }
    }

    return 0;
}

void stampaMat(int Mat[][4])
{
    for(int i = 0; i < 4; i++)
    {
        cout << "\t\t|\t\t|\t\t|\n\t";
        for(int j = 0; j < 4; j++)
        {
            if(Mat[i][j] == 0)
                cout << " ";
            else
                cout << Mat[i][j]; 
            cond(0, j);
        }
        cond(1, i);
    }
    cout << "\n\t\t|\t\t|\t\t|\n\n";
}

void cond(int z, int i)         //Condizioni per la stampa della matrice
{
    if(z == 0)              //Separazione verticale
    {
        if(i != 3)
            cout << "\t|\t";
    }
    else if(z == 1)         //Separazione orizzontale
        if(i != 3)
            cout << "\n________________|_______________|_______________|_______________\n";
}

int getDir()
{
    cout << "Dammi la traslazione da eseguire (Frecce Direzionali): ";
    int ch = getch();

    if(ch == 27)            //Tasto esc
        return -1;  

    if (ch == 224){         //Se si utilizzano caratteri di controllo:
        while (ch != 13)
        {             
            ch = getch();
            switch (ch)
            {
                case 72 : return 0; break;      //UP
                case 0x4B: return 1; break;     //SX
                case 80 : return 2; break;      //DW
                case 0x4D: return 3; break;     //DX
                default: break;
            }
        }
    }
}

bool traslazione(int dir, int Mat[][4])     //False = nessuna traslazione è stata eseguita nella funzione; True = almeno una
{
    bool state = false;             
    int k = 0;
    int i, j;
    switch(dir)
    {
        case 0:     //UP        //Lavoro per le colonne
            for(i = 0; i < 4; i++)      //Scorro tra le COLONNE     1 colonna alla volta
            {
                bool z = 1;
                while(z)        //Ciclo che si ripete fino a quando abbiamo raggiunto l'ultima riga della colonna
                {
                    k = 0;
                    for(j = 0; j < 4; j++)          //Scorro tra le RIGHE
                    {
                        if(Mat[j][i] == 0)
                            k++;
                        else if(k != 0 && Mat[j][i] != 0)      //Se hai già avuto uno spazio vuoto nella colonna esci dal for
                            break;

                        if(j == 3)          //Se hai raggiunto l'ultima riga esce dal ciclo while
                            z = false;      
                    }

                    if(k > 0 && k != 4 && j < 4)     //Se hai buchi vuoti && se c'è almeno un elemento nella riga && se non si è superato confine righe:
                    {
                        Mat[j - k][i] = Mat[j][i];      //Traslaggio con distanza k
                        Mat[j][i] = 0;
                        k = 0;
                        state = true;
                    }
                }
            }
            break;
        case 1:     //SX        //Lavoro per le righe
            for(i = 0; i < 4; i++)      //Scorro tra le RIGHE     1 riga alla volta
            {
                bool z = 1;
                while(z)        //Ciclo che si ripete fino a quando abbiamo raggiunto l'ultima colonna della riga
                {
                    k = 0;
                    for(j = 0; j < 4; j++)          //Scorro tra le COLONNE
                    {
                        if(Mat[i][j] == 0)
                            k++;
                        else if(k != 0 && Mat[i][j] != 0)      //Se hai già avuto un buco nella riga esci dal for e trasla di k
                            break;

                        if(j == 3)
                            z = false;      //Esce dal ciclo while condizione di ripetizione per tutti i valori "fluttuanti"
                    }

                    if(k > 0 && k != 4 && j < 4)     //Se hai buchi vuoti, se c'è almeno un elemento e se non si è superato confine colonne:
                    {
                        Mat[i][j - k] = Mat[i][j];
                        Mat[i][j] = 0;
                        k = 0;
                        state = true;
                    }
                }
            }
            break;
        case 2:     //DW
            for(i = 0; i < 4; i++)      //Scorro tra le COLONNE     1 col alla volta
            {
                bool z = 1;
                while(z)
                {
                    k = 0;
                    for(j = 3; j >= 0; j--)          //scorro tra le RIGHE in senso decrescente
                    {
                        if(Mat[j][i] == 0)
                            k++;
                        else if(k != 0 && Mat[j][i] != 0)      //Se hai già avuto un buco nella colonna esci dal for e trasla di k
                            break;

                        if(j == 0)
                            z = false;      //Esce dal ciclo while condizione di ripetizione per tutti i valori "fluttuanti"
                    }

                    if(k > 0 && k != 4 && j >= 0)     //Se hai buchi vuoti, se c'è almeno un elemento e se non si è superato confine righe:
                    {
                        Mat[j + k][i] = Mat[j][i];
                        Mat[j][i] = 0;
                        k = 0;
                        state = true;
                    }
                }
            }
            break;
        case 3:     //DX
            for(i = 0; i < 4; i++)      //Scorro tra le RIGHE     1 rig alla volta
            {
                bool z = 1;
                while(z)
                {
                    k = 0;
                    for(j = 3; j >= 0; j--)          //Scorro tra le COLONNE in senso decrescente
                    {
                        if(Mat[i][j] == 0)
                            k++;
                        else if(k != 0 && Mat[i][j] != 0)      //Se hai già avuto un buco nella riga esci dal for e trasla di k
                            break;

                        if(j == 0)
                            z = false;      //Esce dal ciclo while condizione di ripetizione per tutti i valori "fluttuanti"
                    }

                    if(k > 0 && k != 4 && j >= 0)     //Se hai buchi vuoti e se c'è almeno un elemento e non si è superato confine colonne:
                    {
                        Mat[i][j + k] = Mat[i][j];
                        Mat[i][j] = 0;
                        k = 0;
                        state = true;
                    }
                }
            }
            break;

        default: break;
    }

    return state;
}

bool somma(int dir, int Mat[][4])       //False = no somma effettuata
{
    bool state = false;         
    switch(dir)
    {
        case 0:     //UP
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    if(Mat[i][j] == Mat[i + 1][j] && Mat[i][j] != 0)        //Se la cella inferiore è uguale alla cella stessa ed è diversa da 0 (vuota):
                    {
                        Mat[i][j] = Mat[i][j] * 2;
                        Mat[i + 1][j] = 0;
                        state = true;
                    }
                }
            }
            break;
        case 1:     //SX
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    if(Mat[i][j] == Mat[i][j + 1] && Mat[i][j] != 0)
                    {
                        Mat[i][j] = Mat[i][j] * 2;
                        Mat[i][j + 1] = 0;
                        state = true;
                    }
                }
            }
            break;
        case 2:     //DW
            for(int i = 3; i > 0; i--)
            {
                for(int j = 0; j < 4; j++)
                {
                    if(Mat[i][j] == Mat[i - 1][j] && Mat[i][j] != 0)
                    {
                        Mat[i][j] = Mat[i][j] * 2;
                        Mat[i - 1][j] = 0;
                        state = true;
                    }
                }
            }
            break;
        case 3:     //DX
            for(int i = 0; i < 4; i++)
            {
                for(int j = 3; j > 0; j--)
                {
                    if(Mat[i][j] == Mat[i][j - 1] && Mat[i][j] != 0)
                    {
                        Mat[i][j] = Mat[i][j] * 2;
                        Mat[i][j - 1] = 0;
                        state = true;
                    }
                }
            }
            break;
        default: break;
    }

    return state;
}

bool yN()       //funzione con getch() yes or no
{
	char state;
	state = getche();
	cout << endl;
    switch (state) 
    {
        case 'y': return true; break;
        case 'n': return false; break;
        case 'Y': return true; break;
        case 'N': return false; break;
        default: break;
	}
}

bool verificaContinuaggio(int Mat[][4])         //Verifica se si può continuare a giocare, cercando la presenza di celle vuote
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(Mat[i][j] == 0)
                return 1;
        }
    }

    return 0;
}

bool verificaSomma(int Mat[][4])                //Verifica se si può continuare a giocare, verificando la possibilità di effettuare somme
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(Mat[i][j] == Mat[i][j + 1] && Mat[i][j] != 0)
                return 1;
            if(Mat[i][j] == Mat[i + 1][j] && Mat[i][j] != 0)
                return 1;
        }
    }

    return 0;
}

bool haiVinto(int Mat[][4])                     //Verifica se si è raggiunto il valore 2048
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(Mat[i][j] == 2048)
                return 1;
        }
    }

    return 0;
}