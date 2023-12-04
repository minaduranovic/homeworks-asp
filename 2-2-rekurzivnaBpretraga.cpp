#include <iostream>
#include <vector>

using namespace std;

template <typename tip>
class Stek {
private:
    tip **lniz;
    int kapacitet;
    int Vrh;

    void realociraj() {
        int noviKap = kapacitet * 2;
        tip **noviLniz = new tip *[noviKap];

        for (int i = 0; i <= Vrh; i++) {
            noviLniz[i] = lniz[i];
        }

        delete[] lniz;
        lniz = noviLniz;
        kapacitet = noviKap;
    }

public:
    Stek() {
        kapacitet = 10;
        Vrh = -1;
        lniz = new tip *[kapacitet];
        for (int i = 0; i < kapacitet; i++) {
            lniz[i] = nullptr;
        }
    }

    ~Stek() {
        for (int i = 0; i <= Vrh; i++) {
            delete lniz[i];
        }
        delete[] lniz;
    }

    int brojElemenata() const { return Vrh + 1; }

    tip &vrh() {
        if (Vrh == -1) {
            throw std::runtime_error("Prazan stek");
        }

        return *(lniz[Vrh]);
    }

    void stavi(const tip &el) {
        if (Vrh + 1 >= kapacitet) {
            realociraj();
        }
        lniz[++Vrh] = new tip(el);
    }

    tip skini() {
        if (Vrh == -1) {
            throw std::runtime_error("Prazan stek");
        }
        tip el = *(lniz[Vrh]);
        delete lniz[Vrh];
        lniz[Vrh] = nullptr;
        Vrh--;

        return el;
    }
};

void pretraga(Stek<vector<int>> &s, int trazeni) {
  bool static nadjen = false;
  if (s.brojElemenata() == 0) {
    if (!nadjen)
      std::cout << "Nema elementa";
    return;
  }
  vector<int> vektor = s.skini();
  int dno = 0;
  int vrh = vektor.size()-1;
  int  i = (vrh+dno) / 2;
  while(vrh >= dno) {
    if (vektor[i] == trazeni) {
      cout << i << " " << s.brojElemenata() << std::endl;
      nadjen = true;
      break;
    } else if (vektor[i] > trazeni)
      vrh = i - 1;
    else
      dno = i + 1;
    i = (vrh+dno) / 2;
  }
  pretraga(s, trazeni);
  s.stavi(vektor);
  return;
}

int main() {
    Stek<vector<int>> stek;

    vector<int> v1 = {1, 2, 3, 4, 5};
    stek.stavi(v1);
    int trazeni1 = 3;
    pretraga(stek, trazeni1);

    vector<int> v2 = {10, 20, 30, 40, 50};
    stek.stavi(v2);
    int trazeni2 = 15;
    pretraga(stek, trazeni2);

    int trazeni3 = 7;
    pretraga(stek, trazeni3);

    return 0;
}
