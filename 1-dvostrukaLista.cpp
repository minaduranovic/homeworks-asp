#include <iostream>

using namespace std;
template <typename tip> class Lista {
public:
  Lista() {}
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual tip &trenutni() = 0;
  virtual tip trenutni() const = 0;
  virtual bool sljedeci() = 0;
  virtual bool prethodni() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const tip &el) = 0;
  virtual void dodajIza(const tip &el) = 0;
  virtual tip &operator[](int i) = 0;
  virtual tip operator[](int i) const = 0;
};
template <typename tip> class DvostrukaLista : public Lista<tip> {
private:
  struct Cvor {
    tip element;
    Cvor *prethodni;
    Cvor *sljedeci;
    Cvor(const tip &el) : element(el), prethodni(nullptr), sljedeci(nullptr) {}
  };
  Cvor *prvi;
  Cvor *zadnji;
  Cvor *trenutniCvor;
  int brojElemenataListe;

public:
  DvostrukaLista()
      : prvi(nullptr), zadnji(nullptr), trenutniCvor(nullptr),
        brojElemenataListe(0) {}

  ~DvostrukaLista() {
    while (prvi != nullptr) {
      Cvor *temp = prvi;
      prvi = prvi->sljedeci;
      delete temp;
    }
    brojElemenataListe = 0;
  }

  DvostrukaLista(const DvostrukaLista &other)
      : prvi(nullptr), zadnji(nullptr), trenutniCvor(nullptr),
        brojElemenataListe(0) {
    Cvor *temp = other.prvi;
    while (temp != nullptr) {
      Cvor *noviCvor = new Cvor(temp->element);
      if (prvi == nullptr) {
        prvi = noviCvor;
        zadnji = noviCvor;
        trenutniCvor = noviCvor;
      } else {
        zadnji->sljedeci = noviCvor;
        noviCvor->prethodni = zadnji;
        zadnji = noviCvor;
      }
      temp = temp->sljedeci;
      brojElemenataListe++;
    }
  }

  DvostrukaLista &operator=(const DvostrukaLista &other) {
    if (this == &other) {
      return *this;
    }

    while (prvi != nullptr) {
      Cvor *temp = prvi;
      prvi = prvi->sljedeci;
      delete temp;
    }

    prvi = nullptr;
    zadnji = nullptr;
    trenutniCvor = nullptr;
    brojElemenataListe = 0;

    Cvor *temp = other.prvi;
    while (temp != nullptr) {
      Cvor *noviCvor = new Cvor(temp->element);
      if (prvi == nullptr) {
        prvi = noviCvor;
        zadnji = noviCvor;
        trenutniCvor = noviCvor;
      } else {
        zadnji->sljedeci = noviCvor;
        noviCvor->prethodni = zadnji;
        zadnji = noviCvor;
      }
      temp = temp->sljedeci;
      brojElemenataListe++;
    }

    return *this;
  }

  int brojElemenata() const override { return brojElemenataListe; }

  bool prethodni() override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    if (trenutniCvor == nullptr || trenutniCvor->prethodni == nullptr) {
      return false;
    }
    trenutniCvor = trenutniCvor->prethodni;
    return true;
  }

  bool sljedeci() override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    if (trenutniCvor == nullptr || trenutniCvor->sljedeci == nullptr) {
      return false;
    }
    trenutniCvor = trenutniCvor->sljedeci;
    return true;
  }

  tip &trenutni() override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    if (trenutniCvor == nullptr) {
      throw std::out_of_range("Trenutni element ne postoji.");
    }
    return trenutniCvor->element;
  }

  tip trenutni() const override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    if (trenutniCvor == nullptr) {
      throw std::out_of_range("Trenutni element ne postoji.");
    }
    return trenutniCvor->element;
  }

  void pocetak() override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    trenutniCvor = prvi;
  }

  void kraj() override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    trenutniCvor = zadnji;
  }

  void obrisi() override {
    if (brojElemenataListe == 0) {
      throw std::out_of_range("Lista je prazna.");
    }
    if (trenutniCvor == nullptr) {
      throw std::out_of_range("Trenutni element ne postoji.");
    }

    if (trenutniCvor->prethodni != nullptr) {
      trenutniCvor->prethodni->sljedeci = trenutniCvor->sljedeci;
    } else {
      prvi = trenutniCvor->sljedeci;
    }

    if (trenutniCvor->sljedeci != nullptr) {
      trenutniCvor->sljedeci->prethodni = trenutniCvor->prethodni;
    } else {
      zadnji = trenutniCvor->prethodni;
    }

    Cvor *zaBrisanje = trenutniCvor;
    trenutniCvor = trenutniCvor->sljedeci;

    delete zaBrisanje;
    brojElemenataListe--;
  }
  void dodajIspred(const tip &el) override {
    Cvor *noviCvor = new Cvor(el);
    // noviCvor->sljedeci = trenutniCvor;

    if (prvi == nullptr) {
      prvi = noviCvor;
      zadnji = noviCvor;
      trenutniCvor = noviCvor;
      prvi->sljedeci = nullptr;
      trenutniCvor->sljedeci = nullptr;
    } else if (prvi == trenutniCvor) {
      noviCvor->sljedeci = prvi;
      prvi->prethodni = noviCvor;
      prvi = noviCvor; ///
    } else {
      noviCvor->prethodni = trenutniCvor->prethodni;
      trenutniCvor->prethodni = noviCvor;
      noviCvor->sljedeci = trenutniCvor;
      noviCvor->prethodni->sljedeci = noviCvor;
    }

    brojElemenataListe++;
  }
  void dodajIza(const tip &el) override {
    Cvor *noviCvor = new Cvor(el);
    // noviCvor->prethodni = trenutniCvor;

    if (brojElemenataListe == 0) {
      prvi = noviCvor;
      zadnji = noviCvor;
      trenutniCvor = noviCvor;
    } else {
      noviCvor->sljedeci = trenutniCvor->sljedeci;
      trenutniCvor->sljedeci = noviCvor;

      if (noviCvor->sljedeci != nullptr) {
        noviCvor->sljedeci->prethodni = noviCvor;
      } else {
        zadnji = noviCvor;
      }
      noviCvor->prethodni = trenutniCvor;
    }

    brojElemenataListe++;
  }

  tip &operator[](int i) override {
    if (i < 0 || i >= brojElemenataListe) {
      throw std::out_of_range("Indeks izvan granica liste.");
    }

    Cvor *trenutni = prvi;
    for (int j = 0; j < i; j++) {
      trenutni = trenutni->sljedeci;
    }
    return trenutni->element;
  }
  tip operator[](int i) const override {
    if (i < 0 || i >= brojElemenataListe) {
      throw std::out_of_range("Indeks izvan granica liste.");
    }

    Cvor *trenutni = prvi;
    for (int j = 0; j < i; j++) {
      trenutni = trenutni->sljedeci;
    }

    return trenutni->element;
  }
};
template <typename Tip> Tip dajMaksimum(const Lista<Tip> &n) {
  const DvostrukaLista<Tip> *nova =
      dynamic_cast<const DvostrukaLista<Tip> *>(&n);
  DvostrukaLista<Tip> nije(*nova);

  nije.pocetak();

  Tip maksimum = nije.trenutni();
  while (nije.sljedeci()) {

    if (nije.trenutni() > maksimum) {
      maksimum = nije.trenutni();
    }
  }
  return maksimum;
}

int main() {

  DvostrukaLista<string> l;
  cout << "Testiranje JednostrukaLista:" << endl;
  l.dodajIspred("A");
  l.dodajIza("B");
  l.dodajIza("C");
  cout << "Broj elemenata: " << l.brojElemenata() << endl;
  l.pocetak();
  cout << "Trenutni element: " << l.trenutni() << endl;
  l.sljedeci();
  cout << "Sljedeci element: " << l.trenutni() << endl;
  l.prethodni();
  cout << "Prethodni element: " << l.trenutni() << endl;
  l.kraj();
  cout << "Krajnji element: " << l.trenutni() << endl;
  l.obrisi();
  cout << "Broj elemenata nakon brisanja: " << l.brojElemenata() << endl;
  cout << "indeks 1: " << l[1] << endl;
  cout << "Maksimum:  " << dajMaksimum(l) << endl;

  return 0;
}