#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  virtual ~Mapa() {}
  Mapa<TipKljuca, TipVrijednosti>() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
  virtual const TipVrijednosti operator[](TipKljuca kljuc) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
protected:
  int brEl;
  std::vector<std::list<std::pair<TipKljuca, TipVrijednosti> *>> lista;
  unsigned int (*hashFunkcija)(TipKljuca, unsigned int);

public:
  HashMapaLan<TipKljuca, TipVrijednosti>()
      : Mapa<TipKljuca, TipVrijednosti>(), brEl(0), hashFunkcija(nullptr) {
    lista.resize(10000);
  }

  ~HashMapaLan<TipKljuca, TipVrijednosti>() { obrisi(); }

  HashMapaLan<TipKljuca, TipVrijednosti>(
      const HashMapaLan<TipKljuca, TipVrijednosti> &other) {

    brEl = other.brEl;
    hashFunkcija = other.hashFunkcija;

    for (int i = 0; i < other.lista.size(); i++) {
      lista.push_back(std::list<std::pair<TipKljuca, TipVrijednosti> *>());
      auto iter = other.lista[i].begin();
      while (iter != other.lista[i].end()) {
        lista[i].push_back(new std::pair<TipKljuca, TipVrijednosti>(
            (*iter)->first, (*iter)->second));
        iter++;
      }
    }
  }

  HashMapaLan<TipKljuca, TipVrijednosti> &
  operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &other) {

    if (this == &other)
      return *this;
    obrisi();

    while (lista.size() < other.lista.size()) {
      lista.push_back(std::list<std::pair<TipKljuca, TipVrijednosti> *>());
    }

    brEl = other.brEl;
    hashFunkcija = other.hashFunkcija;

    for (int i = 0; i < other.lista.size(); i++) {
      auto iter = other.lista[i].begin();

      while (iter != other.lista[i].end()) {
        lista[i].push_back(new std::pair<TipKljuca, TipVrijednosti>(
            (*iter)->first, (*iter)->second));
        iter++;
      }
    }

    return *this;
  }

  void obrisi() {
    if (!hashFunkcija) {
      return;
    }
    if (brEl == 0) {
      return;
    }

    for (int i = 0; i < lista.size(); i++) {
      auto iter = lista[i].begin();
      while (iter != lista[i].end()) {
        delete *iter;
        iter++;
      }
      lista[i].resize(0);
    }

    brEl = 0;
  }

  void obrisi(const TipKljuca &kljuc) {

    if (!hashFunkcija) {
      return;
    }
    int in = hashFunkcija(kljuc, lista.size());
    typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator iter =
        lista[in].begin();

    while (iter != lista[in].end()) {

      if ((*iter)->first == kljuc) {
        delete *iter;
        lista[in].erase(iter);
        brEl--;
        return;
      }
      iter++;
    }

    std::domain_error("Nema kljuca");
  }

  int brojElemenata() const { return brEl; }

  int TraziHash(const TipKljuca &k) const {

    int in = hashFunkcija(k, lista.size());
    typename std::list<std::pair<TipKljuca, TipVrijednosti> *> l = lista[in];
    typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator iter =
        l.begin();
    int i = 0;

    while (iter != l.end()) {
      if ((*iter)->first == k)
        return i;

      i++;
      iter++;
    }

    return -1;
  }

  TipVrijednosti &operator[](TipKljuca kljuc) {

    if (!hashFunkcija) {
      static TipVrijednosti default_value;
      return default_value;
    }
    int in = hashFunkcija(kljuc, lista.size());
    typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator iter =
        lista[in].begin();

    if (lista[in].empty()) {
      lista[in].push_back(
          new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()));
      brEl++;
      iter = lista[in].begin();
      return (*iter)->second;
    }

    if ((*iter)->first == kljuc)
      return (*iter)->second;
    else {
      int q = TraziHash(kljuc);
      if (q == -1) {
        brEl++;
        lista[in].push_back(
            new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()));
        lista[in].sort(
            [](const std::pair<TipKljuca, TipVrijednosti> *a,
               const std::pair<TipKljuca, TipVrijednosti> *b) -> bool {
              return a->first < b->first;
            });
        q = TraziHash(kljuc);
        iter = lista[in].begin();
      }
      while (q > 0) {
        iter++;
        q--;
      }
    }

    return (*iter)->second;
  }

  const TipVrijednosti operator[](TipKljuca kljuc) const {

    if (!hashFunkcija) {
      static TipVrijednosti default_value;
      return default_value;
    }
    int in = hashFunkcija(kljuc, lista.size());
    typename std::list<std::pair<TipKljuca, TipVrijednosti> *> l = lista[in];
    typename std::list<std::pair<TipKljuca, TipVrijednosti> *>::iterator iter =
        l.begin();

    if (!lista[in].empty() && (*iter)->first == kljuc)
      return (*iter)->second;

    else if (lista[in].empty())
      return TipVrijednosti();
    else {
      in = TraziHash(kljuc);
      if (in == -1)
        return TipVrijednosti();
      else {
        while (in > 0) {
          iter++;
          in--;
        }
      }
    }

    return (*iter)->second;
  }

  void definisiHashFunkciju(unsigned int (*f)(TipKljuca, unsigned int)) {
    hashFunkcija = f;
    return;
  }
};