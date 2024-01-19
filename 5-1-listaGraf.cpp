#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename TipKljuca> class GranaIterator;
template <typename TipKljuca> class Grana;
template <typename TipKljuca> class Cvor;
template <typename TipKljuca> class ListaGraf;

template <typename TipKljuca> class UsmjereniGraf {
public:
  UsmjereniGraf() {}
  UsmjereniGraf(int a) {}
  virtual ~UsmjereniGraf() {}
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int a) = 0;
  virtual void dodajGranu(int p, int d, double t) = 0;
  virtual void obrisiGranu(int p, int d) = 0;
  virtual bool postojiGrana(int p, int d) = 0;
  virtual void postaviTezinuGrane(int p, int d, double t) = 0;
  virtual TipKljuca dajOznakuGrane(int p, int d) = 0;
  virtual TipKljuca dajOznakuCvora(int b) = 0;
  virtual double dajTezinuGrane(int p, int d) = 0;
  virtual void postaviOznakuCvora(int b, TipKljuca a) = 0;
  virtual void postaviOznakuGrane(int p, int d, TipKljuca a) = 0;
  Grana<TipKljuca> dajGranu(int p, int d) { return Grana<TipKljuca>(this, p, d); }
  Cvor<TipKljuca> dajCvor(int b) { return Cvor<TipKljuca>(this, b); }

  GranaIterator<TipKljuca> dajGranePocetak() {
    GranaIterator<TipKljuca> it(this, 0, -1);
    return ++it;
  }

  GranaIterator<TipKljuca> dajGraneKraj() {
    GranaIterator<TipKljuca> it(this, -1, -1);
    return it;
  }
  virtual GranaIterator<TipKljuca> dajSljedecuGranu(int p, int d) = 0;
};

template <typename TipKljuca> class Cvor {

  UsmjereniGraf<TipKljuca> *u;
  TipKljuca oznaka;
  int broj;
  std::vector<Grana<TipKljuca>> grane;
  std::vector<Cvor<TipKljuca>> susjedi;

public:
  Cvor(UsmjereniGraf<TipKljuca> *a, int b) : u(a), broj(b) {}

  int dajRedniBroj() const { return broj; }

  void postaviOznaku(TipKljuca a) { u->postaviOznakuCvora(broj, a); }

  TipKljuca dajOznaku() const { return u->dajOznakuCvora(broj); }

  void dodajVezu(Grana<TipKljuca> g, Cvor<TipKljuca> s) {
    grane.push_back(g);
    susjedi.push_back(s);
  }
  void obrisiVezu(Grana<TipKljuca> g, Cvor<TipKljuca> s) {

    for (int i = 0; i < susjedi.size(); i++)
      if (s == susjedi[i]) {

        for (int j = i; j < susjedi.size() - 1; j++) {
          susjedi[j] = susjedi[j + 1];
          grane[j] = grane[j + 1];
        }
        break;
      }
    susjedi.pop_back();
    grane.pop_back();
  }

  std::vector<Grana<TipKljuca>> dajGrane() const { return grane; }
  std::vector<Cvor<TipKljuca>> dajSusjede() const { return susjedi; }
};

template <typename TipKljuca> class Grana {

  UsmjereniGraf<TipKljuca> *u;
  TipKljuca oznaka;
 double tezina;
  int p, d;

public:
  Grana(UsmjereniGraf<TipKljuca> *ug, int polazni, int dolazni)
      : u(ug), p(polazni), d(dolazni) {}
  double dajTezinu() const { return u->dajTezinuGrane(p, d); }
  void postaviTezinu(double a) { u->postaviTezinuGrane(p, d, a); }
  void postaviOznaku(TipKljuca a) { u->postaviOznakuGrane(p, d, a); }
  TipKljuca dajOznaku() const { return u->dajOznakuGrane(p, d); }
  Cvor<TipKljuca> dajPolazniCvor() const { return u->dajCvor(p); };
  Cvor<TipKljuca> dajDolazniCvor() const { return u->dajCvor(d); };
};

template <typename TipKljuca> class GranaIterator {

  int p, d;
  UsmjereniGraf<TipKljuca> *u;

public:
  GranaIterator(UsmjereniGraf<TipKljuca> *us, int a, int b) : p(a), d(b), u(us){};

  Grana<TipKljuca> operator*() { return u->dajGranu(p, d); }
  bool operator==(const GranaIterator &it) const {
    return it.u == u && it.p == p && it.d == d;
  }
  bool operator!=(const GranaIterator &it) const {
    return it.u != u || it.p != p || it.d != d;
  }
  GranaIterator &operator++() {
    GranaIterator<TipKljuca> sljedeci = u->dajSljedecuGranu(p, d);
    p = sljedeci.p;
    d = sljedeci.d;
    return *this;
  }
  GranaIterator operator++(int) {
    GranaIterator<TipKljuca> ovaj = *this;
    ++(*this);
    return ovaj;
  }
};

template <typename TipKljuca> class ListaGraf : public UsmjereniGraf<TipKljuca> {

  struct grana {
    TipKljuca oznaka;
   double tezina;
    bool postoji;
  };
  std::vector<std::vector<grana>> lista;
  std::vector<TipKljuca> oznake;

public:
  ListaGraf(int b) : UsmjereniGraf<TipKljuca>(b) { postaviBrojCvorova(b); }
  int dajBrojCvorova() const override { return lista.size(); }

  void postaviBrojCvorova(int a) override {
    grana g;
    g.postoji = false;
    for (int i = 0; i < lista.size(); i++)
      lista[i].resize(a, g);
    std::vector<grana> ubaci(a, g);
    lista.resize(a, ubaci);
    oznake.resize(a);
  }
  bool postojiGrana(int p, int d) override { return lista[p][d].postoji; }

  void dodajGranu(int p, int d, double t) override {
    grana g;
    g.tezina = t;
    g.postoji = true;
    lista[p][d] = g;
  }
  void obrisiGranu(int p, int d) override { lista[p][d].postoji = false; }
  void postaviTezinuGrane(int p, int d, double t) override {
    lista[p][d].tezina = t;
  }
  TipKljuca dajOznakuCvora(int b) override { return oznake[b]; }

  TipKljuca dajOznakuGrane(int p, int d) override { return lista[p][d].oznaka; }
  double dajTezinuGrane(int p, int d) override { return lista[p][d].tezina; }
  void postaviOznakuCvora(int b, TipKljuca a) override { oznake[b] = a; }
  void postaviOznakuGrane(int p, int d, TipKljuca a) override {
    lista[p][d].oznaka = a;
  }
  GranaIterator<TipKljuca> dajGranePocetak() {
    GranaIterator<TipKljuca> it(this, 0, -1);
    ++it;
    return it;
  }
  GranaIterator<TipKljuca> dajGraneKraj() {
    GranaIterator<TipKljuca> it(this, -1, -1);
    return it;
  }

  GranaIterator<TipKljuca> dajSljedecuGranu(int p, int d) override {
    for (int i = p; i < lista.size(); i++) {
      for (int j = 0; j < lista.size(); j++) {
        if (i == p && j <= d)
          continue;

        if (lista[i][j].postoji)
          return GranaIterator<TipKljuca>(this, i, j);
      }
    }
    return GranaIterator<TipKljuca>(this, -1, -1);
  }
};

template <typename TipKljuca>
void bfs(UsmjereniGraf<TipKljuca> *g, std::vector<Cvor<TipKljuca>> &b, Cvor<TipKljuca> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (b[i].dajRedniBroj() == c.dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int br = g->dajBrojCvorova();
  for (int i = 0; i < br; i++)
    if (!dodati)
      bfs(g, b, g->dajCvor(i));
}

template <typename TipKljuca>
void dfs(UsmjereniGraf<TipKljuca> *g, std::vector<Cvor<TipKljuca>> &b, Cvor<TipKljuca> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (c.dajRedniBroj() == b[i].dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int brojCvorova = g->dajBrojCvorova();
  for (int i = 0; i < brojCvorova; i++)
    if (g->postojiGrana(c.dajRedniBroj(), i) && !dodati)
      dfs(g, b, g->dajCvor(i));
}