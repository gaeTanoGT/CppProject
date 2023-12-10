//1vs1
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include <stdlib.h>
#include <string.h>

#include <fstream>


using namespace std;

HWND hwndWNDMain;

HWND hwndTXT;
HWND hwndTXT1;

HWND hwndMat[6][7];

int Mat[6][7];

bool pl;        // 0 = player 1;  1 = player 2(BOT);

bool k = true; //0 = gioco terminato
bool set = false;

bool h = true; //1 = si può giocare

int pp, p1, p2;     //numero partite, vittorie p1, vittorie p2

bool testCoordinate(int);

//-----prototipi
void stampaMat(int[][7], bool);
int conv(char);
bool riempiCol(bool, int, int[][7]);
int verificaMat(int[][7]);
bool verificaEnd(int[][7]);
bool continuare();
void assegna(int[][7], int, int, int);

void aggiorna(int[][7]);

//-----interfaccia grafica
LRESULT CALLBACK wndMyProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){      
    switch(message){
        case WM_CREATE:         //2
        {
            hwndTXT = CreateWindow(       //textbox
                WC_EDIT,
                "FORZA 4    1 VS 1    BUON DIVERTIMENTO!",
                WS_CHILD | WS_VISIBLE | ES_LEFT,
                15, 10,       //distanza da bordo X, y
                300, 25,       //dimenzioni box
                hwnd, NULL, NULL, NULL
            );

            hwndTXT1 = CreateWindow(       //textbox2
                WC_EDIT,
                NULL,
                WS_CHILD | WS_VISIBLE | ES_LEFT,
                15, 40,       //distanza da bordo X, y
                350, 25,       //dimenzioni box
                hwnd, NULL, NULL, NULL
            );

            for(int i = 0; i < 6; i++)
            {
                for(int j = 0; j < 7; j++)
                {
                    hwndMat[i][j] = CreateWindow(       //textbox
                        WC_BUTTON,
                        "",     //testo del bottone
                        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                        15 + (j * 63), 85 + (i * 63),       //distanza da bordo X, y
                        60, 60,       //dimenzioni box
                        hwnd, (HMENU)j, NULL, NULL      // , identificativo del tasto, 
                    );
                }
            }
            
            cout << "Interfaccia creata\n";

            if(!pl)
                Edit_SetText(hwndTXT1, "Inizia Giocatore 1 (X):"); 
            else
                Edit_SetText(hwndTXT1, "Inizia Giocatore 2 (O):");

            break;
        }
        case WM_COMMAND:        //1
        {                           //loop ad ogni tasto premuto  (anche prima della pressione)
            if(!set)        //se ancora non e' iniziata la partita
            {
                Edit_SetText(hwndTXT, "FORZA 4    1 VS 1"); 
                //pulisco la matrice
                for(int i = 0; i < 6; i++)
                    for(int j = 0; j < 7; j++)
                        Mat[i][j] = 0;
                
                bool k = true;

                //creo file txt dove salvare i dati
                fstream f1;
                f1.open("cache.txt", ios::in);

                int pp;     //contatore di numero di partite effettuate

                if(!f1.is_open())
                {
                    f1.close();

                    system("type null > cache.txt");
                    cout << "file cache creato\n";

                    pp = 0;
                }else{
                    f1 >> pp;
                    pp++;
                    f1.close();
                }            

                f1.open("cache.txt", ios::out);
                f1 << pp;
                f1.close();

                cout << pp << "file cache aggiornato\n";

                if(pp % 2 == 0)
                    pl = false;
                else
                    pl = true;

                set = true;
            }
            
            switch(HIWORD(wParam)){
                case BN_CLICKED:
                {
                    string stringa = "Mossa giocatore ";
                    if(pl)
                        stringa += " 1 (X):";
                    else
                        stringa += " 2 (O):";
                
                    Edit_SetText(hwndTXT1, &stringa[0]);                        
                    
                    if(!testCoordinate(LOWORD(wParam)))
                    {
                        aggiorna(Mat);
                        int val;
                        val = MessageBoxW(hwnd, L"Vuoi fare un'altra partita?", L"Forza 4 dice:", MB_YESNO | MB_ICONWARNING);
                        
                        if(val == IDNO)
                        {
                            system("del cache.txt");
                            cout << "USCITA";
                            DestroyWindow(hwnd);
                        }else{
                            DestroyWindow(hwnd);

                            string f = __FILE__;        //ottengo la stringa con il nome dell'eseguibile partendo dal nome del file
                            int x =  f.length() - 4;
                            f.replace(x, 4, ".exe");    
                            system(&f[0]);      //apre nuovamente l'eseguibile del file corrente*/
                        }
                    }
                }
            }
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lCmdLine, int nCmdShow){
    WNDCLASSEX wc;
    MSG msg;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.hInstance = hInstance;
    wc.cbWndExtra = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "wndMyClass";
    wc.lpfnWndProc = wndMyProc;

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL, "Errore", "ERRORE", MB_OK | MB_ICONERROR);
        return -1;
    }

    hwndWNDMain = CreateWindow(
        "wndMyClass",
        "FORZA 4",
        WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 500,
        NULL, NULL, hInstance, NULL
    );

    if(!hwndWNDMain)
    {
        MessageBox(NULL, "ERRORE FINESTRA", "ERRORE", MB_OK | MB_ICONERROR);
        return -2;
    }

    ShowWindow(hwndWNDMain, SW_SHOWNORMAL);

    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
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

void assegna(int Mat[][7], int p, int rig, int col)
{
    for(int i = 0; i <= rig; i++)
    {
        if(p == 1)
        {
            Mat[i][col] = 1;
            Edit_SetText(hwndMat[i][col], "X"); 
        }
        else if(p == 2)
        {
            Mat[i][col] = 2;
            Edit_SetText(hwndMat[i][col], "O");
        }

        if(i != rig)
        {
            Sleep(100);
            Mat[i][col] = 0;
            Edit_SetText(hwndMat[i][col], "");
        }
    }
}

bool testCoordinate(int x)
{
    bool u = riempiCol(pl, x, Mat);
    if(u)
        pl = !pl;
    else
    {
        bool end = verificaEnd(Mat);
        if(!end)
        {
            string str = "Il gettone non puo' entrare nella colonna indicata.";
            Edit_SetText(hwndTXT1, &str[0]); 
        }else{
            string str = "IL GIOCO E' TERMINATO IN PAREGGIO.";
            Edit_SetText(hwndTXT1, &str[0]); 
            return false;
        }
    }

    int y = verificaMat(Mat);
    if(y == 3 || y == 4)
    {
    	string s = (y == 3 ? "1" : "2");
        string str = "HA VINTO GIOCATORE " + s + "!";
        Edit_SetText(hwndTXT1, &str[0]); 
        return false;
    }

    return true;
}

void aggiorna(int Mat[][7])
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            if(Mat[i][j] == 3)
            {
                Edit_SetText(hwndMat[i][j], "| X |"); 
            }
            else if(Mat[i][j] == 4)
            {
                Edit_SetText(hwndMat[i][j], "| O |");
            }
        }
    }
}
