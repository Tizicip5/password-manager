#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct Haslo{
    string imie;
    string haslo;
    string kategoria;
    string serwis;
    string login;
};

bool hasloByloWykarzystane(string basicString, vector<Haslo> &vector){
    for(const auto& haslo : vector){
        if(haslo.haslo == basicString){
            return true;
        }
    }
    return false;
}

bool sprawdzenieHasla(string& basicString) {
    if (basicString.length() < 5) {
        cout << "Twoje hasło jest krótkie.Min. długość-5 znaków";
        return false;
    }
    bool czySaMaleLit = false;
    for (char a: basicString) {
        if (islower(a)) {
            czySaMaleLit = true;
            break;
        }
    }
    if (!czySaMaleLit) {
        cout << "Hasło muśi zawierac małe litere\n";
        return false;
    }
    bool czySaDuzeLit = false;
    for (char A: basicString) {
        if (isupper(A)) {
            czySaDuzeLit = true;
            break;
        }
    }
    if (!czySaDuzeLit) {
        cout << "Hasło muśi zawierac duże litere\n";
        return false;
    }
    bool czySaCyfry = false;
    for (char cyfry: basicString) {
        if (isdigit(cyfry)) {
            czySaCyfry = true;
            break;
        }
    }
    if (!czySaCyfry) {
        cout << "Hasło muśi zawierac cyfry\n";
        return false;
    }
    return true;
}

vector<Haslo> wyszykiwanieHasla(vector<Haslo> &hasla,const string& slowoKluczowe) {
    vector<Haslo> potrzebneHaslo;
    for (const auto& haslo: hasla) {
        if (haslo.imie.find(slowoKluczowe) != string::npos ||
            haslo.kategoria.find(slowoKluczowe) != string::npos ||
            haslo.serwis.find(slowoKluczowe) != string::npos ||
            haslo.login.find(slowoKluczowe) != string::npos) {
            potrzebneHaslo.push_back(haslo);
        }
    }
    return potrzebneHaslo;
}
void wyswietlenieHasla(const vector<Haslo>& has){
    for (const auto& haslo : has){
        cout << "Imię - " << haslo.imie << "\n";
        cout << "Hasło - " << haslo.haslo << "\n";
        cout << "Kategoria - " << haslo.kategoria << "\n";
        cout << "Serwis - " << haslo.serwis << "\n";
        cout << "Login - " << haslo.login << "\n";
    }
}
void dodawanieHasla(vector<Haslo> &haslo) {
    Haslo newHaslo;
    cout << "Imie hasła : ";
    cin >> newHaslo.imie;
    cout << "Wpisz haslo : ";
    cin >> newHaslo.haslo;
    if(hasloByloWykarzystane(newHaslo.haslo,haslo)){
        cout << "To hasło było już wykorzystane\n";
        return;
    }
    if (!sprawdzenieHasla(newHaslo.haslo)){
        return;}
    cout << "Kategoria hasła : ";
    cin >> newHaslo.kategoria;
    cout << "Serwis : ";
    cin >> newHaslo.serwis;
    cout << "Login do serwisu : ";
    cin >> newHaslo.login;

    if (!sprawdzenieHasla(newHaslo.haslo)) {
        cout << "Hasło jest slabe.Chcesz go zapisać?\n ";

        return;
    }

    if (hasloByloWykarzystane(newHaslo.haslo, haslo)) {
        cout << "Hasło zostało już użyte.Chcesz go zapisać? \n";
        return;
    }
    haslo.push_back(newHaslo);
    cout << "Twoje hasło zostawo dodane\n";
}

string szyfrowanieHasla(string& haslo, string klucz) {
    string zaszyfrowaneHaslo = haslo;
    for (char& c : zaszyfrowaneHaslo) {
        c += klucz.length();
    }
    return zaszyfrowaneHaslo;
}
string odszyfrowanieHasla(string szyfrowanieHasla, string klucz) {
    string haslo = szyfrowanieHasla;
    for (char& c : haslo) {
        c -= klucz.length();
    }
    return haslo;
}
void zapisywanieHaslaDoPliku(Haslo haslo, const string &nazwaPliku, const string &kluczSzyfrowania) {
    ofstream outFile;
    outFile.open(nazwaPliku, ios::app);
    if (outFile.is_open()) {
        time_t teraz = time(0);
        tm *czas = ::localtime(&teraz);
        char timestamp[80];
        ::strftime(timestamp, 80, "%H%M%S", czas);
        outFile << timestamp << szyfrowanieHasla(haslo.imie, kluczSzyfrowania);
        outFile << szyfrowanieHasla(haslo.haslo, kluczSzyfrowania);
        outFile << szyfrowanieHasla(haslo.kategoria, kluczSzyfrowania);
        outFile << szyfrowanieHasla(haslo.serwis, kluczSzyfrowania);
        outFile << szyfrowanieHasla(haslo.login, kluczSzyfrowania);
        outFile.close();
        cout << "Hasło dodano do pliku";
    } else {
        cout << "Błąd";
    }
}
vector<Haslo> odczytHasla(string nazwaPliku, string kluczSzyfrowania) {
    vector<Haslo> hasla;
    ifstream doPliku;
    doPliku.open(nazwaPliku);
    if (!doPliku.is_open()) {
        cout << "Błąd otwierania pliku\n ";
        return hasla;
    }
    string linia;
    Haslo haslo;
    int liczbaLinij = 0;
    while (getline(doPliku, linia)) {
        liczbaLinij++;
        switch (liczbaLinij % 5) {
            case 0 :
                haslo.serwis = odszyfrowanieHasla(linia, kluczSzyfrowania);
                hasla.push_back(haslo);
                break;
            case 1 :
                break;
            case 2:
                haslo.imie = odszyfrowanieHasla(linia, kluczSzyfrowania);
                break;
            case 3 :
                haslo.haslo = odszyfrowanieHasla(linia, kluczSzyfrowania);
            case 4 :
                haslo.kategoria = odszyfrowanieHasla(linia, kluczSzyfrowania);
            default:
                break;
        }
    }
    doPliku.close();
    cout << "Dane z pliku są odczytane\n";
    return hasla;
}

void edycjaHasla(vector<Haslo>& hasla) {
    string imie;
    cout << "Podaj imię hasła do edycji: ";
    cin >> imie;

    for (auto &haslo: hasla) {
        if (haslo.imie == imie) {
            cout << "Imię: " << haslo.imie << endl;
            cout << "Wpisz hasło: ";
            cin >> haslo.haslo;
            cout << "Kategoria hasła: ";
            cin >> haslo.kategoria;
            cout << "Serwis hasła: ";
            cin >> haslo.serwis;
            cout << "Login do serwisu: ";
            cin >> haslo.login;
            if (!sprawdzenieHasla(haslo.haslo)) {
                cout << "Haslo jest niepoprawne";
                return;
            }
            cout << "Hasło jest zaktualizowane";
            return;
        }
    }
}
void odczytHaslaZPliku(vector<Haslo>& hasla, const string& nazwaPliku, const string& kluczSzyfrowania) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cout << "Błąd podczas otwierania pliku" << endl;
        return;
    }
    hasla.clear();
    string imie;
    string zaszyfrowaneHaslo;
    string kategoria;
    string serwis;
    string login;

    while (plik >> imie >> zaszyfrowaneHaslo >> kategoria >> serwis >> login) {
        string odszyfrowaneHaslo = zaszyfrowaneHaslo;
        for (char& c : odszyfrowaneHaslo) {
            c -= kluczSzyfrowania.length();
        }

        Haslo haslo;
        haslo.imie = imie;
        haslo.haslo = odszyfrowaneHaslo;
        haslo.kategoria = kategoria;
        haslo.serwis = serwis;
        haslo.login = login;

        hasla.push_back(haslo);
    }

    plik.close();
    cout << "Odczytano dane z pliku." << endl;
}


void sortowanieHaslaImie(vector<Haslo> &hasla) {
    sort(hasla.begin(), hasla.end(), [](const Haslo &haslo_1, const Haslo &haslo_2) {
        return haslo_1.imie < haslo_2.imie;
    });
}
void sortowanieHaslaKategoria(vector<Haslo> &hasla) {
    sort(hasla.begin(), hasla.end(), [](const Haslo &haslo_1, const Haslo &haslo_2) {
        return haslo_1.kategoria < haslo_2.kategoria;
    });
}
void dodawanieKategorii(vector<string>& kategorii, string nowaKategoria) {
    cout << "Podaj nazwę kategorii : ";
    cin >> nowaKategoria;
    kategorii.push_back(nowaKategoria);
    cout << "Udało się dodać!\n";
}
void usuwanieKategorii(vector<Haslo> &hasla, vector<string> &kategorii, string usuwanieKategorii) {
    auto kat = find(kategorii.begin(), kategorii.end(), usuwanieKategorii);
    if (kat != kategorii.end()) {
        kategorii.erase(kat);
        auto haslo = hasla.begin();
        while (haslo != hasla.end()) {
            if (haslo->kategoria == usuwanieKategorii) {
                haslo = hasla.erase(haslo);
            } else {
                ++haslo;
            }
        }
        cout << "Kategoria została usunięta!" << endl;
    } else {
        cout << "Kategoria nie istnieje!" << endl;
    }
}

void usuwanieHasla(vector<Haslo>& hasla) {
    string imie;
    cout << "Podaj imię hasła które chcesz usunąć: ";
    cin >> imie;

    auto it = find_if(hasla.begin(), hasla.end(), [imie](const Haslo& haslo) {
        return haslo.imie == imie;
    });

    if (it != hasla.end()) {
        hasla.erase(it);
        std::cout << "Hasło zostało usunięte." << std::endl;
    } else {
        std::cout << "Hasło o podanym imieniu nie zostało znalezione." << std::endl;
    }
}


int main() {
    string nazwaPliku;
    string kluczSzyfrowania;
    vector<Haslo> hasla;
    vector<string> kategorii;

    cout << "Wprowadz nazwę pliku : ";
    cin >> nazwaPliku;
    cout << "Podaj twoje hasło : ";
    cin >> kluczSzyfrowania;
    hasla = odczytHasla(nazwaPliku, kluczSzyfrowania);
    while (true) {
        cout << "1.Wyszukaj hasła\n";
        cout << "2.Posortuj hasła\n";
        cout << "3.Dodaj hasło\n";
        cout << "4.Edytuj hasło\n";
        cout << "5.Usuń hasło\n";
        cout << "6.Dodaj kategorie\n";
        cout << "7.Wyszukaj hasła\n";
        cout << "8.Usuń kategorie\n";
        int wybor;
        cin >> wybor;
        switch (wybor) {
            case 1: {
                string slowoKluczowe;
                cout << "Podaj słowo kluczowe : ";
                cin >> slowoKluczowe;
                vector<Haslo> a = wyszykiwanieHasla(hasla, slowoKluczowe);
                wyswietlenieHasla(a);
                break;
            }
            case 2:
                sortowanieHaslaImie(hasla);
                cout << "Hasła są posortowane\n";
                wyswietlenieHasla(hasla);
                break;
            case 3:
                dodawanieHasla(hasla);
                break;
            case 4:
                edycjaHasla(hasla);
                break;
            case 5:
                usuwanieHasla(hasla);
                break;
            case 6:
                string nowaKategoria;
                cout << "Podaj nazwę kategorii,ktorą chcesz dodać\n";
                cin >> nowaKategoria;
                dodawanieKategorii(kategorii, nowaKategoria);
                break;
        }
    }
    return 0;
}