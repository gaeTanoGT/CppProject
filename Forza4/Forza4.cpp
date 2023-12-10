//FORZA 4 In C++ Torrisi
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

using namespace std;

void stampaMat(int[][7], bool);
int conv(char);
string cond(bool);
bool riempiCol(bool, int, int[][7]);
int verificaMat(int[][7]);
bool verificaEnd(int[][7]);
bool continuare();
void assegna(int[][7], int, int, int);

int main()
{
    int Mat[6][7];
/*
    Mat[0][0] = 1;
    Mat[1][0] = 2;
    Mat[2][0] = 2;
    Mat[3][0] = 2;
    Mat[4][0] = 1;
    Mat[5][0] = 1;

    Mat[0][1] = 2;
    Mat[1][1] = 2;
    Mat[2][1] = 1;
    Mat[3][1] = 2;
    Mat[4][1] = 1;
    Mat[5][1] = 2;

    Mat[0][2] = 2;
    Mat[1][2] = 1;
    Mat[2][2] = 2;
    Mat[3][2] = 2;
    Mat[4][2] = 1;
    Mat[5][2] = 2;

    Mat[0][3] = 1;
    Mat[1][3] = 1;
    Mat[2][3] = 2;
    Mat[3][3] = 1;
    Mat[4][3] = 2;
    Mat[5][3] = 1;

    Mat[0][4] = 2;
    Mat[1][4] = 1;
    Mat[2][4] = 2;
    Mat[3][4] = 1;
    Mat[4][4] = 2;
    Mat[5][4] = 1;

    Mat[0][5] = 1;
    Mat[1][5] = 2;
    Mat[2][5] = 1;
    Mat[3][5] = 2;
    Mat[4][5] = 1;
    Mat[5][5] = 2;

    Mat[0][6] = 1;
    Mat[1][6] = 1;
    Mat[2][6] = 1;
    Mat[3][6] = 2;
    Mat[4][6] = 1;
    Mat[5][6] = 2;*/

    stampaMat(Mat, true);

    bool z = true;
    bool pl;        // 0 = player 1;  1 = player 2(BOT);

    bool alt = true;        //stato per far alternare il primo player tra una partita e l'altra

    do{
        //pulisco la matrice
        for(int i = 0; i < 6; i++)
            for(int j = 0; j < 7; j++)
                Mat[i][j] = 0;

        /*
         *  area test
        */

        system("cls");

        stampaMat(Mat, false);

        bool k = true;

        if(alt)
        {
            pl = false;
            alt = false;
        }else{
            pl = true;
            alt = true;
        }


        while(k)
        {
            cout << "Giocatore: " << pl + 1 << cond(pl) << "Indicami la colonna: ";
            char ch = getche();

            int x = conv(ch);

            if(x == -2)
                return 0;

            if(x < 0 || x > 6)
            {
                cout << "\nCoordinate indicate errate. Riprovare.\n";
                system("pause");
            }else
            {
                bool u = riempiCol(pl, x, Mat);
                if(u)
                    pl = !pl;
                else
                {
                    bool end = verificaEnd(Mat);
                    if(!end)
                    {
                        cout << "\nIl gettone non puo' entrare nella colonna indicata.\n";

                    }else{
                        cout << "\n\nIL GIOCO E' TERMINATO IN PAREGGIO.\n\n";
                        k = false;
                    }
                    system("pause");
                }
            }

            int y = verificaMat(Mat);

            system("cls");
            stampaMat(Mat, false);

            if(y == 3 || y == 4)
            {
                cout << "HA VINTO GIOCATORE " << y - 2 << endl;
                system("pause");
                k = false;
            }
        }

        z = continuare();
        system("cls");
    }while(z);
    return 0;
}

void stampaMat(int Mat[][7], bool st)
{
    if(st)
    {
        cout << "Benvenuto a FORZA 4\nGioco creato da Gaetano Torrisi\nPer uscire digitare tasto 'c'.\n\tBUON DIVERTIMENTO!\n\n";
        system("pause");
    }
    else
    {
        cout << "Per uscire digitare tasto 'c'.\n\n";
        cout << "  q   w   e   r   t   y   u\n";
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < 7; j++)
            {
                if(Mat[i][j] == 0)
                    cout << "|   ";
                else if(Mat[i][j] == 1)
                    cout << "| X ";
                else if(Mat[i][j] == 2)
                    cout << "| O ";
                else if(Mat[i][j] == 3)     //cella di vittoria player1
                    cout << "||X|";
                else if(Mat[i][j] == 4)     //cella di vittoria player1
                    cout << "||O|";
            }
            cout << "|" << endl;
        }
        cout << endl;
    }
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

bool riempiCol(bool pl, int x, int Mat[][7])
{
    if(x != -1)
    {
        for(int i = 5; i >= 0; i--)
        {
            if(Mat[i][x] == 0)
            {
                if(pl)
                {
                    assegna(Mat, 2, i, x);
                    return true;
                }else
                {
                    assegna(Mat, 1, i, x);
                    return true;
                }
            }
        }
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

bool verificaEnd(int Mat[][7])
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            if(Mat[i][j] == 0)
                return false;
        }
    }
    return true;
}

bool continuare()
{
    cout << "\n\nVuoi fare un'altra partita? (Y/N): ";
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

void assegna(int Mat[][7], int pl, int rig, int col)
{
    for(int i = 0; i <= rig; i++)
    {
        if(pl == 1)
            Mat[i][col] = 1;
        else if(pl == 2)
            Mat[i][col] = 2;

        system("cls");
        stampaMat(Mat, false);

        if(i != rig)
        {
            Sleep(100);
            Mat[i][col] = 0;
        }
    }
}
