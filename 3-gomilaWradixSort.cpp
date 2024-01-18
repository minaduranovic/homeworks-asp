#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

void swap(vector<int> &a, int i, int j) {
  int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}

void popraviDolje(vector<int> &a, int i, int vel) {
  int najveci = i;
  int lijevo = 2 * i + 1;
  int desno = 2 * i + 2;

  if (lijevo < vel && a[lijevo] > a[najveci]) {
    najveci = lijevo;
  }

  if (desno < vel && a[desno] > a[najveci]) {
    najveci = desno;
  }

  if (najveci != i) {
    swap(a, i, najveci);
    popraviDolje(a, najveci, vel);
  }
}
int roditelj(int i) { return (i - 1) / 2; }

void popraviGore(vector<int> &v, int i) {

  int rod = (i - 1) / 2;

  while (i != 0 && (v[i] > v[rod])) {
    swap(v, i, rod);
    i = rod;
  }
}
void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
  a.push_back(umetnuti);
  velicina++;

  popraviGore(a, velicina - 1);
}

int maks(vector<int> &a) {
  int m = a[0];
  for (int i = 1; i < a.size(); ++i) {
    if (a[i] > m) {
      m = a[i];
    }
  }
  return m;
}
void stvoriGomilu(vector<int> &a) {
  int vel = a.size();

  for (int i = vel / 2 - 1; i >= 0; i--) {
    popraviDolje(a, i, vel);
  }
}
int izbaciPrvi(vector<int> &a, int &velicina) {

  if (a.size() == 0)  std::domain_error("Nema gomile");

  velicina--;
  swap(a, 0, velicina);

  if (velicina != 0){
    popraviDolje(a, 0, velicina);
  }

  return a[velicina];
}

void gomilaSort(vector<int> &a) {
  int vel = a.size();
  stvoriGomilu(a);

  for (int i = vel - 1; i > 0; i--) {
    swap(a, 0, i);
    vel--;
    popraviDolje(a, 0, vel);
  }
}

void radixSort(vector<int> &a) {
  int maksEl = maks(a);
  int stepen = 1;

  while (maksEl / stepen > 0) {
    vector<vector<int>> v1(10, vector<int>());

    for (int i = 0; i < a.size(); ++i) {
      v1[(a[i] / stepen) % 10].push_back(a[i]);
    }
    a.clear();
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < v1[i].size(); ++j) {
        a.push_back(v1[i][j]);
      }
    }
    stepen *= 10;
  }
}
int main() {
  vector<int> a;
  a.push_back(545465);
  a.push_back(1556445);
  a.push_back(454636);
  a.push_back(1254121);
  a.push_back(15665656);
  a.push_back(95468);
  int velicina = a.size();
  stvoriGomilu(a);
  umetniUGomilu(a, 100000, velicina);
  umetniUGomilu(a, 3, velicina);
  umetniUGomilu(a, 1, velicina);
  umetniUGomilu(a, 100, velicina);
  umetniUGomilu(a, 200, velicina);
  izbaciPrvi(a, velicina);
  for (int i(0); i < a.size(); i++) {
    cout << a[i] << " ";
  }

  return 0;
}
