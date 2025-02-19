#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

vector <int> zbiorA;
vector<int> mapa;

void OdczytZPliku(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cout << "Nie mozna otworzyc pliku!" << endl;
        return;
    }
    int k;
    while (plik >> k) {
        zbiorA.push_back(k);
    }
    plik.close();
}

int tmp = 0;
auto lastPart = make_shared<int>(tmp);

void wypisz(vector<int> solution) {
    cout << endl << "MAPA: ";
    for (int num : solution) {
        cout << num << " ";
    }
    cout << *lastPart;
    cout << endl;
}

pair<bool, vector<int>> Czykontynuowac(vector<int> A, vector<int> solution, int length, int number) {
    int sum = 0;
    vector<int> toRemove;
    for (int i = solution.size() - 1; i >= 0; i--) {
        int toFind = number + solution[i] + sum;
        auto it = find(A.begin(), A.end(), toFind);
        if (it != A.end()) {
            toRemove.push_back(distance(A.begin(), it));
        }
        else {
            return make_pair(false, toRemove);
        }
        sum += solution[i];
    }
    int sum2 = 0;
    for (int i = 0; i < solution.size(); i++) {
        sum2 += solution[i];
    }
    int toFind2 = length - (number + sum2);
    auto it2 = find(A.begin(), A.end(), toFind2);
    auto it3 = find(A.begin(), A.end(), number);
    if (it2 != A.end()) {
        *lastPart = A[distance(A.begin(), it2)];
        toRemove.push_back(distance(A.begin(), it2));
        toRemove.push_back(distance(A.begin(), it3));
        return make_pair(true, toRemove);
    }
    else {
        return make_pair(false, toRemove);
    }
}
void szukaj(vector<int> A, vector<int> solution, int ind, int maxind, int length, shared_ptr<int> jest) {
    if (ind == maxind) {
        wypisz(solution);
        *jest = 1;
    }
    else {
        for (int i = 0; i < A.size(); i++) {
            int number = A[i];
            pair<bool, vector<int>> info = Czykontynuowac(A, solution, length, number);
            if (info.first == true) {
                vector<int> afterRemove;
                for (int j = 0; j < A.size(); j++) {
                    auto it = find(info.second.begin(), info.second.end(), j);
                    if (it == info.second.end()) {
                        afterRemove.push_back(A[j]);
                    }
                }
                vector<int> newA = afterRemove;
                vector<int> newSolution = solution;
                newSolution.push_back(number);
                szukaj(newA, newSolution, ind + 1, maxind, length, jest);
            }
            if (*jest == 1)
                break;
        }
    }
}

int CzyFormat() {
    int length = zbiorA.size();
    for (int i = 0; ; i++)
    {
        int number = (i + 1) * (i + 2) / 2;
        if (number == length)
            return i ;
        if (number > length)
            return -1;
    }
}

int main() {
    OdczytZPliku("zbior.txt");
    vector<int> A = zbiorA;
    vector<int> solution;
    auto jest = make_shared<int>(0);
    sort(A.begin(), A.end());

    int Maxind = CzyFormat();
    if (Maxind == -1){
        cout << "Niewlasciwy format multizbioru!\n";
        return 0;
    }

    cout << "A: ";
    for (int num : A) {
        cout << num << " ";
    }
    cout << endl;

    auto maxElementIter = max_element(A.begin(), A.end());
    auto secondMaxElementIter = max_element(A.begin(), maxElementIter);
    int maxElement = *maxElementIter;
    int secondMaxElement = *secondMaxElementIter;
    int startingElement = maxElement - secondMaxElement;

    auto start = high_resolution_clock::now();

    solution.push_back(startingElement);

    auto it = find(A.begin(), A.end(), startingElement);
    A.erase(it);
    it = find(A.begin(), A.end(), maxElement);
    A.erase(it);
    it = find(A.begin(), A.end(), secondMaxElement);
    A.erase(it);

    szukaj(A, solution, 1, Maxind, maxElement, jest);

    auto stop = high_resolution_clock::now();
    auto czas = duration_cast<milliseconds>(stop - start);
    if (*jest == 0) {
        cout << "Mapa nie zostala znaleziona.";
    }

    cout << "Czas: " << czas.count() << " ms" << endl;

    return 0;
}