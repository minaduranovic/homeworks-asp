#include <iostream>

using namespace std;

template <typename tipKljuca, typename tipVrijednosti> class Mapa {
public:
  virtual ~Mapa() {}
  virtual tipVrijednosti &operator[](const tipKljuca &kljuc) = 0;
  virtual tipVrijednosti operator[](const tipKljuca &kljuc) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const tipKljuca &kljuc) = 0;
};

template <typename tipKljuca, typename tipVrijednosti>
class AVLStabloMapa : public Mapa<tipKljuca, tipVrijednosti> {
  struct Cvor {
    tipKljuca kljuc;
    tipVrijednosti vrijednost;
    Cvor *lijevo;
    Cvor *desno;
    Cvor *roditelj;
    int balans; 

    Cvor(const tipKljuca &k, const tipVrijednosti &v, Cvor *l, Cvor *d, Cvor *r)
        : kljuc(k), vrijednost(v), lijevo(l), desno(d), roditelj(r), balans(0) {}
  };

  Cvor *korijen;
  int brojEl;
  tipVrijednosti prazan;

  void obrisiSve(Cvor *p) {
    if (p == nullptr)
      return;
    obrisiSve(p->lijevo);
    obrisiSve(p->desno);
    delete p;
    p = nullptr;
    brojEl--;
  }

  void azurirajBalans(Cvor *cvor) {
    if (cvor != nullptr) {
      int visinaLijevo = visina(cvor->lijevo);
      int visinaDesno = visina(cvor->desno);
      cvor->balans = visinaLijevo - visinaDesno;
    }
  }

  int visina(Cvor *cvor) const {
    if (cvor == nullptr)
      return -1;
    return 1 + max(visina(cvor->lijevo), visina(cvor->desno));
  }

  void rotacijaLL(Cvor *&cvor) {
    Cvor *temp = cvor->lijevo;
    cvor->lijevo = temp->desno;
    if (temp->desno != nullptr) {
      temp->desno->roditelj = cvor;
    }
    temp->desno = cvor;
    temp->roditelj = cvor->roditelj;
    cvor->roditelj = temp;

    azurirajBalans(cvor);
    azurirajBalans(temp);

    cvor = temp;
  }

  void rotacijaRR(Cvor *&cvor) {
    Cvor *temp = cvor->desno;
    cvor->desno = temp->lijevo;
    if (temp->lijevo != nullptr) {
      temp->lijevo->roditelj = cvor;
    }
    temp->lijevo = cvor;
    temp->roditelj = cvor->roditelj;
    cvor->roditelj = temp;

    azurirajBalans(cvor);
    azurirajBalans(temp);

    cvor = temp;
  }

  void rotacijaLR(Cvor *&cvor) {
    rotacijaRR(cvor->lijevo);
    rotacijaLL(cvor);
  }

  void rotacijaRL(Cvor *&cvor) {
    rotacijaLL(cvor->desno);
    rotacijaRR(cvor);
  }

  void balansiraj(Cvor *&cvor) {
    azurirajBalans(cvor);

    if (cvor->balans == -2) {
      if (cvor->desno->balans <= 0) {
        rotacijaRR(cvor);
      } else {
        rotacijaRL(cvor);
      }
    } else if (cvor->balans == 2) {
      if (cvor->lijevo->balans >= 0) {
        rotacijaLL(cvor);
      } else {
        rotacijaLR(cvor);
      }
    }
  }

  void kopiraj(Cvor *&trenutni, Cvor *other, Cvor *roditelj) {
    if (other == nullptr)
      return;
    trenutni = new Cvor(other->kljuc, other->vrijednost, nullptr, nullptr, roditelj);
    kopiraj(trenutni->lijevo, other->lijevo, trenutni);
    kopiraj(trenutni->desno, other->desno, trenutni);
    brojEl++;
  }

public:
  ~AVLStabloMapa() {
    obrisiSve(korijen);
    brojEl = 0;
  }
  AVLStabloMapa() : brojEl(0), korijen(nullptr), prazan(tipVrijednosti()) {}

  AVLStabloMapa(const AVLStabloMapa<tipKljuca, tipVrijednosti> &b)
      : korijen(0), brojEl(0), prazan(tipVrijednosti()) {
    kopiraj(korijen, b.korijen, nullptr);
  }

  AVLStabloMapa operator=(const AVLStabloMapa<tipKljuca, tipVrijednosti> &b) {
    if (this != &b) {
      obrisiSve(korijen);
      brojEl = 0;
      kopiraj(korijen, b.korijen, nullptr);
    }
    return *this;
  }
  int brojElemenata() const { return brojEl; }

  void obrisi() {
    obrisiSve(korijen);
    brojEl = 0;
    korijen = nullptr;
  }

  void obrisi(const tipKljuca &k) {
    Cvor *p = korijen;
    while (p != nullptr) {
      if (p->kljuc < k) {
        p = p->desno;
      } else if (p->kljuc > k) {
        p = p->lijevo;
      } else if (p->kljuc == k) {
        if (p->lijevo == nullptr && p->desno == nullptr) {
          if (p != korijen) {
            if (p->roditelj->lijevo == p)
              p->roditelj->lijevo = nullptr;
            else
              p->roditelj->desno = nullptr;
            delete p;
            p = nullptr;
            brojEl--;
          } else if (p == korijen) {
            delete korijen;
            korijen = nullptr;
            brojEl--;
          }
        } else if (p->lijevo != nullptr && p->desno != nullptr) {
          Cvor *z = p->lijevo;
          Cvor *pz = nullptr;
          while (z->desno != nullptr) {
            pz = z;
            z = z->desno;
          }
          p->kljuc = z->kljuc;
          p->vrijednost = z->vrijednost;
          if (pz != nullptr)
            z->roditelj->desno = z->lijevo;
          else
            z->roditelj->lijevo = z->lijevo;
          delete z;
          z = nullptr;
          brojEl--;
        } else {
          Cvor *tmp = p->lijevo;
          if (tmp == nullptr)
            tmp = p->desno;
          if (p->roditelj == nullptr) {
            korijen = tmp;
            korijen->roditelj = nullptr;
          } else {
            if (p->roditelj->lijevo == p)
              p->roditelj->lijevo = tmp;
            else
              p->roditelj->desno = tmp;
            tmp->roditelj = p->roditelj;
          }
          delete p;
          p = nullptr;
          brojEl--;
        }

      } else
        throw std::out_of_range("Kljuc nije pronadjen!");
    }
  }

  tipVrijednosti &operator[](const tipKljuca &k) {
    Cvor *p1 = nullptr;
    Cvor *p2 = nullptr;
    if (brojEl != 0) {
      p1 = korijen;
      while (p1 != nullptr) {
        p2 = p1;
        if (p1->kljuc < k)
          p1 = p1->desno;
        else if (p1->kljuc > k)
          p1 = p1->lijevo;
        else if (p1->kljuc == k)
          return p1->vrijednost;
      }

      Cvor *temp = new Cvor(k, tipVrijednosti(), nullptr, nullptr, nullptr);

      if (p2->kljuc < k) {
        p2->desno = temp;
        temp->roditelj = p2;
      } else {
        p2->lijevo = temp;
        temp->roditelj = p2;
      }

      brojEl++;
      azurirajBalans(temp);
      balansiraj(temp);
      return temp->vrijednost;
    } else {
      korijen = new Cvor(k, tipVrijednosti(), nullptr, nullptr, nullptr);
      brojEl++;
      return korijen->vrijednost = tipVrijednosti();
    }
  }

  tipVrijednosti operator[](const tipKljuca &k) const {
    Cvor *p = korijen;
    while (p != nullptr) {
      if (p->kljuc < k)
        p = p->desno;
      else if (p->kljuc > k)
        p = p->lijevo;
      else if (p->kljuc == k)
        return p->vrijednost;
    }
    static tipVrijednosti t = tipVrijednosti();
    return t;
  }


  void Preorder() const {
    PreorderPomocna(korijen);
    cout << endl;
  }

  
  void PreorderPomocna(Cvor *cvor) const {
  if (cvor != nullptr) {
    cout << cvor->kljuc << " ";
    PreorderPomocna(cvor->lijevo);
    PreorderPomocna(cvor->desno);
  }
}

};

int main() {
AVLStabloMapa<int, int> avl;
  avl[3] = 3;
  avl[5] = 5;
  avl[8] = 8;
  avl[10] = 10;
  avl[9] = 9;
  avl[11] = 11;
  avl.Preorder();
  std::cout << endl; 
  return 0;
}
