/* 1 vs BOT  con Win32 API
    Torrisi Gaetano
    10-08-22
*/

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

//strutture interfaccia grafica
HWND hwndWNDMain;
HWND hwndTXT;
HWND hwndTXT1;
HWND hwndTXT2;
HWND hwndMat[6][7];

int Mat[6][7];

bool set1 = false;      //settaggio livello
bool set2 = false;      //file cache
int pp = 0;         //numero partite
int p1 = 0;         //vittorie p1
int p2 = 0;         //vittorie p2

//variabili bot
int lev, nF, nK = 0;    //livello gioco, mosse da bloccare, mosse bloccate
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

bool notCol[7];         //array che ha 1 se il bot non deve entrare nella colonna

vector<int> vetPrior;           //livello priorità della sequenza       
        //[1 = mossa vittoria; 2 = mossa per parare avversario; 3 = colonna adiacente ad avversario; 4 = mossa per creazione di una possibile sequenza; 5 = scelta strategica; 6 = random]
vector<int> vetRig;             //riga in cui nel caso inserire il gettone player2
vector<int> vetCol;             //colonna in cui nel caso inserire il gettone player2

//-----prototipi
bool riempiCol(bool, int, int[][7]);
int verificaMat(int[][7]);              //verifica se qualcuno ha vinto
bool verificaEnd(int[][7]);             //verifica se si può continuare
void assegna(int[][7], int, int, int);  //assegna alla matrice dell'interfaccia i valori di Mat
void aggiorna(int[][7]);                //aggiorna la matrice in caso di vittoria
void sceltaBot(int[][7], bool);      	//Verifica se player1 puo' vincere
void sceltaBot2(int[][7], bool);		//Verifica se player2(bot) puo' vincere
void sceltaBot3(int[][7]);				//Previene la formazione di sequenza di 4 da entrambi i lati della riga
void sceltaBot4(int[][7], bool);		//trova coordinate casuali(liv: 0, 1) oppure nella cella con piu' possibilita' di vittoria il player2(bot) (liv: 2, 3)
bool assegnaBot(int[][7]);			    //sceglie la mossa di player2 basandosi su un sistema di priorita'
void pulisciVet(bool);                  //pulizia vettori: vetPrior, vetRig e vetCol
void pulisciVet();		                //porta tutti gli elementi dell'array notCol a 0
void mossaBot();
bool verificaVittoria();
int testCoordinate(int);                //esegue mossa player e bot alla pressione di un tasto      
        //[-1: errore assegnazione; 0: assegnazione effettuata correttamente; 1: vittoria pl1; 2: vittoria bot; 3: pareggio]


//-----interfaccia grafica
LRESULT CALLBACK wndMyProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){      
    switch(message){
        case WM_CREATE:         //2
        {
            //creazione interfaccia grafica
            hwndTXT = CreateWindow(       //textbox
                WC_EDIT,
                "FORZA 4\t\t1 VS BOT",
                WS_CHILD | WS_VISIBLE | ES_LEFT,
                15, 10,       //distanza da bordo X, y
                300, 25,       //dimenzioni box
                hwnd, NULL, NULL, NULL
            );

            hwndTXT1 = CreateWindow(       //textbox1
                WC_EDIT,
                "BUON DIVERTIMENTO!",
                WS_CHILD | WS_VISIBLE | ES_LEFT,
                15, 40,       //distanza da bordo X, y
                300, 25,       //dimenzioni box
                hwnd, NULL, NULL, NULL
            );

            hwndTXT2 = CreateWindow(       //textbox2
                WC_EDIT,
                "Selezionare il livello:",
                WS_CHILD | WS_VISIBLE | ES_LEFT,
                15, 65,       //distanza da bordo X, y
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
                        15 + (j * 63), 100 + (i * 63),      //distanza da bordo X, y
                        60, 60,                             //dimenzioni box
                        hwnd, (HMENU)j, NULL, NULL          // , identificativo del tasto, 
                    );
                }
            }

            //presentazione livelli di gioco al player
            LPCSTR met[6][7];

            for(int i = 0; i < 6; i++)
                for(int j = 0; j < 7; j++)
                    met[i][j] = "*";

            met[0][0] = "E";
            met[1][0] = "A";
            met[2][0] = "S";
            met[3][0] = "Y";
            met[4][0] = " ";
            met[5][0] = " ";
            met[0][2] = "M";
            met[1][2] = "E";
            met[2][2] = "D";
            met[3][2] = "I";
            met[4][2] = "U";
            met[5][2] = "M";
            met[0][4] = "H";
            met[1][4] = "A";
            met[2][4] = "R";
            met[3][4] = "D";
            met[4][4] = " ";
            met[5][4] = " ";
            met[0][6] = "I";
            met[1][6] = "M";
            met[2][6] = "P";
            met[3][6] = " ";
            met[4][6] = " ";
            met[5][6] = " ";

            for(int i = 0; i < 6; i++)
                for(int j = 0; j < 7; j++)
                    Edit_SetText(hwndMat[i][j], met[i][j]); 

            set2 = true;        //per avviare settaggio livello dopo messaggio di pressione tasto
            
            cout << "Interfaccia creata\n";
            break;
        }

        case WM_COMMAND:        //loop ad ogni tasto premuto  (anche prima della pressione)
        {                           
            if(set2)
            {
                //scelta livello
                switch(HIWORD(wParam)){
                    case BN_CLICKED:
                    {
                        switch(LOWORD(wParam)){
                            case 0:
                                cout << "lev 0\n";
                                lev = 0;
                                nF = rand() % 3;        //0 - 2
                                break;
                            case 2:
                                cout << "lev 1\n";
                                lev = 1;
                                nF = rand() % 3 + 2;    //2 - 4
                                break;
                            case 4:
                                cout << "lev 2\n";
                                lev = 2;
                                nF = rand() % 3 + 2;    //2 - 4
                                break;
                            case 6:
                                cout << "lev 3\n";
                                lev = 3;
                                nF = -2;
                                break;
                            default:
                                cout << "Errore selezione\n";
                                set1 = true;     //per bypassare set1
                                set2 = false;   //per far ripetere la funzione set2
                                Edit_SetText(hwndTXT1, "Errore selezione. Ripriovare..."); 
                                break;
                        }
                        break;
                    }
                }

                set1 = !set1;        //per passare al settaggio file cache se selezione livello è avvenuta con successo
                set2 = !set2;

                if(set1)        //se la selezione del livello è andata a buon fine
                {
                    Edit_SetText(hwndTXT, "FORZA 4\t\t1 VS BOT"); 

                    switch(lev){
                        case 0:
                            Edit_SetText(hwndTXT1, "Livello EASY");
                            break;
                        case 1:
                            Edit_SetText(hwndTXT1, "Livello MEDIUM");
                            break;
                        case 2:
                            Edit_SetText(hwndTXT1, "Livello HARD");
                            break;
                        case 3:
                            Edit_SetText(hwndTXT1, "Livello IMPOSSIBLE");
                            break; 
                    }

                    //pulisco la matrice
                    for(int i = 0; i < 6; i++)
                        for(int j = 0; j < 7; j++)
                            Mat[i][j] = 0;
                            
                    for(int i = 0; i < 6; i++)
                        for(int j = 0; j < 7; j++)
                            Edit_SetText(hwndMat[i][j], " "); 
                    
                    bool k = true;

                    //creo file txt dove salvare i dati
                    fstream f1, f2;
                    f1.open("cache.txt", ios::in);

                    if(!f1.is_open())   //se il file non esiste
                    {
                        f1.close();

                        system("type null > cache.txt");        //creo file cache.txt
                        cout << "file cache creato\n";

                        pp = 0;
                        p1 = 0;
                        p2 = 0;
                    }else{      //altrimenti aggiorna il file
                        int n = 0;
                        string s;
                        //Ciclo lettura caratteri
                        while(!f1.eof())
                        {
                            getline(f1, s);     //leggo il file riga per riga
                            switch(n)
                            {
                                case 0:
                                    pp = atoi(s.c_str());       //converto la stringa in un intero e la assegno alla variabile
                                    break;
                                case 1:
                                    p1 = atoi(s.c_str());
                                    break;
                                case 2:
                                    p2 = atoi(s.c_str());
                                    break;
                                default:
                                    break;
                            }
                            n++;
                        }
                        pp++;
                        f1.close();
                    }
                    //aggiorno il file
                    f1.open("cache.txt", ios::out);
                    f1 << pp << endl << p1 << endl << p2 << endl;

                    f1.close();

                    cout << "file cache aggiornato\n";
                    
                    string stringa = "Inizia ";
                    if(pp % 2 != 0)     //se numero è dispari -> inizia bot
                    {     
                        stringa += "BOT (O):";
                        Edit_SetText(hwndTXT2, &stringa[0]);
                        mossaBot();
                    }else
                    {
                        stringa += "giocatore 1 (X):";
                        Edit_SetText(hwndTXT2, &stringa[0]);
                    }

                    set1 = false;
                }

            }else{      //altrimenti se settaggi sono già stati effettuati
                switch(HIWORD(wParam)){
                    case BN_CLICKED:
                    {                   
                        Edit_SetText(hwndTXT2, "Buona Fortuna!");    
                        int zyx = testCoordinate(LOWORD(wParam));       //-1 = errore, 0 = no winner, 1 = pl1, 2 = bot, 3 = pareggio

                        if(zyx > 0)         //se qualcuno ha vinto oppure non è possibile continuare
                        {
                            aggiorna(Mat);
                            int val;

                            LPCWSTR str = L"Vuoi fare un'altra partita?";
                            wstring str1(str);
                            wstring str2(str);

                            //aggiorno le vittorie sul file cache.txt
                            fstream f1;
                            f1.open("cache.txt", ios::out);
                            if(zyx == 1){
                                str1 = L"Ha vinto Giocatore 1!";
                                p1++;
                            }
                            else if(zyx == 2){
                                str1 = L"Ha vinto BOT!";
                                p2++;
                            }
                            else if(zyx == 3)
                                str1 = L"Gioco terminato in pareggio!";
                            
                            f1 << pp << endl << p1 << endl << p2 << endl;

                            f1.close();

                            //finestra resoconto e continuare
                            str1 = str1 + L"\n\n  Resoconto punti:\n     Tu(X): "; 
                            wchar_t buf[2];
                            wsprintfW(buf, L"%d", p1);      //assegno a stringa di char "buf" il valore intero di p1
                            str1 += buf;
                            str1 += L"  -  BOT(O): ";
                            wsprintfW(buf, L"%d", p2);
                            str1 += buf;
                            str = str1.c_str();
                            MessageBoxW(hwnd, str, L"Forza 4 BOT dice:", MB_OK | MB_ICONWARNING);
                            
                            str = str2.c_str();
                            val = MessageBoxW(hwnd, str, L"Forza 4 BOT dice:", MB_YESNO | MB_ICONQUESTION);

                            if(val == IDNO)     //selezione "NO"
                            {
                                system("del cache.txt");    //elimino file cache
                                cout << "USCITA";
                                DestroyWindow(hwnd);        //distruggo la finestra
                            }else{              //selezione "SI"
                                DestroyWindow(hwnd);

                                //ottengo la stringa con il nome dell'eseguibile partendo dal percorso di cartella
                                string f = __FILE__;
                                int x =  f.length() - 4;
                                f.replace(x, 4, ".exe");        //sostituisco l'estenzione da .cpp a .exe alla fine della stringa

                                //estraggo solamente il nome del file dal full path
                                int w = -1;
                                for(int h = f.length(); h >= 0; h--)        //conto quanti caratteri distano dalla fine al '\'
                                    if(f[h] != '\\')
                                        w++;
                                    else
                                        break;

                                char s[100];
                                int dif = f.length() - w;       //differenza numerica dei caratteri dall'inizio del pathname al nome del file
                                for(int h = 0; h < w; h++)
                                    s[h] = f[h + dif];
                                s[w] = '\0';    //fine stringa
                                system(s);      //apre nuovamente l'eseguibile del file corrente
                            }
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
        "FORZA 4 BOT",
        WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 520,
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
        for(int i = 5; i >= 0; i--)
            if(Mat[i][x] == 0)
                if(pl){
                    assegna(Mat, 2, i, x);
                    return true;
                }else{
                    assegna(Mat, 1, i, x);
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

bool verificaEnd(int Mat[][7])
{
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 7; j++)
            if(Mat[i][j] == 0)
                return false;
    
    return true;
}

void assegna(int Mat[][7], int p, int rig, int col)
{
    for(int i = 0; i <= rig; i++)
    {
        if(p == 1){
            Mat[i][col] = 1;
            Edit_SetText(hwndMat[i][col], "X"); 
        }
        else if(p == 2){
            Mat[i][col] = 2;
            Edit_SetText(hwndMat[i][col], "O");
        }

        if(i != rig){
            Sleep(100);
            Mat[i][col] = 0;
            Edit_SetText(hwndMat[i][col], "");
        }
    }
}

int testCoordinate(int x)       //x corrisponde alla colonna selezionata
{
    //player
    bool xyz = false;
    xyz = riempiCol(false, x, Mat);

    bool end = verificaEnd(Mat);

    if(xyz && end){
        string str = "IL GIOCO E' TERMINATO IN PAREGGIO.";
        Edit_SetText(hwndTXT2, &str[0]); 
        return 3;
    }else if(!xyz && !end){
        string str = "Il gettone non puo' entrare nella colonna indicata.";
            Edit_SetText(hwndTXT2, &str[0]);
            return -1;
    }

    if(!verificaVittoria())
        return 1;

    //bot
    mossaBot();
    end = verificaEnd(Mat);
    if(end)
    {
        string str = "IL GIOCO E' TERMINATO IN PAREGGIO.";
        Edit_SetText(hwndTXT2, &str[0]); 
        return 3;
    }else if(!verificaVittoria())
        return 2;
    
    return 0;
}

void mossaBot()
{
    if(lev == 0){
        sceltaBot(Mat, false);
        sceltaBot4(Mat, false);
    }

    if(lev == 1){
        sceltaBot(Mat, true);
        sceltaBot2(Mat, false);			//limitata dentro assegnaBot
        sceltaBot4(Mat, false);
    }

    if(lev == 2 || lev == 3){
        sceltaBot(Mat, true);
        sceltaBot2(Mat, true);
        sceltaBot3(Mat);
        sceltaBot4(Mat, true);
    }

    assegnaBot(Mat);
    cout << " - nK: " << nK << "\tnF: " << nF << endl;
}

bool verificaVittoria()
{
    int y = verificaMat(Mat);
    if(y == 3)
    {
        string str = "HA VINTO GIOCATORE 1!";
        Edit_SetText(hwndTXT2, &str[0]); 
        return false;
    }
    if(y == 4)
    {
        string str = "HA VINTO GIOCATORE 2(BOT)!";
        Edit_SetText(hwndTXT2, &str[0]); 
        return false;
    }

    return true;
}

void aggiorna(int Mat[][7])
{
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 7; j++)
        {
            if(Mat[i][j] == 3)
                Edit_SetText(hwndMat[i][j], "| X |"); 
            else if(Mat[i][j] == 4)
                Edit_SetText(hwndMat[i][j], "| O |");
        }
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
                            if((Mat[i + 1][j + nV - 1] != 0 || i == 5) && Mat[i][j + nV - 1] == 0 && (r || nV == 0))
                            {
                                vetPrior.push_back(2);      //livello priorità = 2
                                vetRig.push_back(i);
                                vetCol.push_back(j + nV - 1);
                            }                             
                            if((Mat[i + 1][j + nV - 1] == 0 && i != 5) && (Mat[i + 2][j + nV - 1] != 0 || i + 1 == 5))         //se colonna ha base vuota, oppure è all'ultima riga: dico al bot di non selezionare quella colonna
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
                    || Mat[i][j] == Mat[i + 3][j + 3]))) && (Mat[i][j] != 0 ) && (Mat[i][j] != 2 && Mat[i + 1][j + 1] != 2
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
                                if((Mat[i + nV][j + nV - 1] == 0 && i + nV != 5) && (Mat[i + nV + 1][j + nV - 1] != 0 || i + nV == 5))         //se colonna ha base vuota, dico al bot di non selezionare quella colonna
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
                                if((Mat[i - nV + 2][j + nV - 1] == 0 && i - nV + 1 != 5) && (Mat[i - nV + 3][j + nV - 1] != 0 || i - nV + 2 == 5))         //se colonna ha base vuota, dico al bot di non selezionare quella colonna
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
                //creo array con possibile formazione di quartetto
                int Vet[5];     //-1 <-to-> 3
                for(int v = 0; v < 5; v++)
                {
                    if(j + v - 1 < 0 || j + v - 1 > 6)      //se la colonna corrispondente è al di fuori della matrice stabilisci player1(da non considerare)
                        Vet[v] = 1;
                    else
                        Vet[v] = Mat[i][j + v - 1];
                }

                //verifico livello di priorità contando il numero di 1
                int num2[2] = {0, 0};       //[0] = contatore di numero di 2 nel vettore; [1] = contatore numero 0
                for(int kk = 0; kk < 5; kk++)
                {
                    if(Vet[kk] == 2)
                        num2[0] += 1;
                    if(Vet[kk] == 0)
                        num2[1] += 1;
                }
                bool r = true;          //1 = possibilità di vittoria, 0 = caso in cui non è presente la base al di sotto della cella in cui si deve fare la mossa

                for(int vv = 0; vv < 5; vv++)			//scorro il vettore co nbsequenza   53124
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
                        if(Mat[i + 1][j + nV - 1] != 0 || i == 5)         //se esiste una base oppure sei all'ultima riga
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
                        }else if((Mat[i + 1][j + nV - 1] == 0 && i != 5) && num2[1] == 1)   //&& Mat[i][j + nV - 1] == 0   //se non è presente la base al di sotto ed è presente una sola cella vuota non considerare la situazione
                            break;
                        else if((Mat[i + 1][j + nV - 1] == 0 && i != 5) && nV != 4)        //caso num2[1] == 1 && no base al disotto nel mezzo del vettore
                            r = false;      //se non hai la base al disotto la priorità corrisponderà a 4
                    }
                }
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
	                        }else if((Mat[i + nV][j + nV - 1] == 0 && i + nV != 5) && num2[1] == 1)   //se non è presente la base al di sotto non considerare la situazione
	                            break;
                            else if((Mat[i + nV][j + nV - 1] == 0 && i + nV != 5) && nV != 4)
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

	                int num2[2] = {0, 0};               //da mettere fuori dal for(vv)
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
	                        }else if((Mat[i - nV + 2][j + nV - 1] == 0 && i - nV + 1 != 5) && num2[1] == 1)   //se non è presente la base al di sotto non considerare la situazione
	                            break;
                            else if((Mat[i - nV + 2][j + nV - 1] == 0 && i - nV + 1 != 5) && nV != 4)
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
        /*
            scelta seguendo possibilità max di combianzioni
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

        if(max != -1)       //se è stato possibile trovare una cella libera
        {
            vetPrior.push_back(5);      //livello priorità = 5
            vetRig.push_back(rig);
            vetCol.push_back(col);
        }else               //altrimenti esegui scelta casuale
            sceltaBot4(Mat, false);

    }else
    {
        //altrimenti random -- livello 0, 1
        int c = rand() % 7;
        if(Mat[0][c] == 0)      //se c'è sempre spazio nella colonna corrispondente per il gettone
        {
            vetPrior.push_back(6);      //livello priorità = 6
            vetRig.push_back(-1);       //la riga viene rilevata al momento dell'assegnazione
            vetCol.push_back(c);
        }else
            sceltaBot4(Mat, false);
    }

}

void pulisciVet()
{
    for(int i = 0; i < 7; i++)
        notCol[i] = 0;
}

void pulisciVet(bool z)
{
    vetPrior.clear();
    vetRig.clear();
    vetCol.clear();
}

bool assegnaBot(int Mat[][7])
{
    //verifico esisitenza di coordinate a priorità 1
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 1 && lev != 0)
        {
            int rig = vetRig[i];
            int col = vetCol[i];
            assegna(Mat, 2, rig, col);
        	cout << "PRIOR 1\t " << vetRig[i] << "  " << vetCol[i];
            pulisciVet(true);
            break;
            return false;
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
                if(nK <= nF || lev == 3)        //se hai superato nK(numero mosse bloccate) oppure sei al livello 3
                {
                    assegna(Mat, 2, rig, col);
                    cout << "PRIOR 2\t " << vetRig[i] << "  " << vetCol[i];
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
                assegna(Mat, 2, rig, col);
                cout << "PRIOR 3\t " << vetRig[i] << "  " << vetCol[i];
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
                assegna(Mat, 2, rig, col);
                cout << "PRIOR 4\t "<< vetRig[i] << "  " << vetCol[i];
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
            assegna(Mat, 2, rig, col);
            cout << "PRIOR 5\t " << vetRig[i] << "  " << vetCol[i];
            pulisciVet(true);
            break;
            return false;
        }

    //verifico esisitenza di coordinate a priorità 6
    for(int i = 0; i < vetPrior.size(); i++)
        if(vetPrior[i] == 6)
        {
            riempiCol(true, vetCol[i], Mat);
            cout << "PRIOR 6\t" << vetRig[i] << "  " << vetCol[i];
            pulisciVet(true);
            break;
            return false;
        }
}
