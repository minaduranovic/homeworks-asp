#include <iostream>

template <typename T>
class DvostraniRed {
private:
    struct Cvor {
        T element;
        Cvor* sljedeci;
        Cvor* prethodni;
        Cvor(const T& el, Cvor* sled = nullptr, Cvor* pret = nullptr)
            : element(el), sljedeci(sled), prethodni(pret) {}
    };

    Cvor* pocetak;
    Cvor* kraj;
    int brElemenata;

public:
    DvostraniRed() : pocetak(nullptr), kraj(nullptr), brElemenata(0) {}

    DvostraniRed(const DvostraniRed& other) : pocetak(nullptr), kraj(nullptr), brElemenata(0) {
        kopiraj(other);
    }

    DvostraniRed& operator=(const DvostraniRed& other) {
        if (this != &other) {
            brisi();
            kopiraj(other);
        }
        return *this;
    }

    ~DvostraniRed() {
        brisi();
    }

    void staviNaVrh(const T& element) {
        Cvor* novi = new Cvor(element);
        if (kraj == nullptr) {
            pocetak = kraj = novi;
        } else {
            Cvor* p=kraj;
            p->sljedeci=novi;
            novi->prethodni=p;
            kraj=novi;

        }
        brElemenata++;
    }

    T skiniSaVrha() {
          if (brElemenata == 0)
      throw std::domain_error("Prazan red");
        T el{};
        if (kraj != nullptr) {
            Cvor* temp = kraj;
            el = temp->element;
            kraj = kraj->prethodni;
            delete temp;
            brElemenata--;

            if (kraj == nullptr) {
                pocetak = nullptr;
            } else {
                kraj->sljedeci = nullptr;
            }
        }
        return el;
    }

    T vrh() const {
        if (pocetak!= nullptr) {
            return kraj->element;
        } else {
            throw std::out_of_range("Red je prazan.");
        }
    }


   

    int brojElemenata() const {
        return brElemenata;
    }

    void brisi() {
        while (pocetak != nullptr) {
            Cvor* temp = pocetak;
            pocetak= pocetak->sljedeci;
            delete temp;
        }
        kraj = nullptr;
        brElemenata = 0;
    }
     T celo() const {
        if (kraj != nullptr) {
            return pocetak->element;
        } else {
            throw std::out_of_range("Red je prazan.");
        }
    }

 void staviNaCelo(const T& element) {
        Cvor* novi = new Cvor(element);
        if (pocetak == nullptr) {
            pocetak = kraj = novi;
        } else {
            novi->sljedeci = pocetak;
            pocetak->prethodni = novi;
            pocetak = novi;
        }
        brElemenata++;
    }

    T skiniSaCela() {
          if (brElemenata == 0)
      throw std::domain_error("Prazan red");
        T el{};
        if (pocetak != nullptr) {
            Cvor* temp = pocetak;
            el = temp->element;
            pocetak = pocetak->sljedeci;
            delete temp;
            brElemenata--;

            if (pocetak == nullptr) {
                kraj = nullptr;
            } else {
                pocetak->prethodni = nullptr;
            }
        }
        return el;
    }
private:

    void kopiraj(const DvostraniRed& other) {
        Cvor* iter = other.pocetak;
        while (iter != nullptr) {
            staviNaVrh(iter->element);
            iter = iter->sljedeci;
        }
    }
};



int main() {

    DvostraniRed<int> red;

    red.staviNaVrh(1);
    red.staviNaVrh(2);
    red.staviNaVrh(3);

    std::cout << "Vrh: " << red.vrh() << std::endl;
    std::cout << "Broj elemenata: " << red.brojElemenata() << std::endl;

    int skini = red.skiniSaVrha();
    std::cout << "Skini sa vrha: " << skini << std::endl;
    std::cout << "Vrh nakon skidanja: " << red.vrh() << std::endl;
    std::cout << "Broj elemenata nakon skidanja: " << red.brojElemenata() << std::endl;

    red.staviNaCelo(4);
    red.staviNaCelo(5);
    red.staviNaCelo(6);

    std::cout << "Celo: " << red.celo() << std::endl;
    std::cout << "Broj elemenata: " << red.brojElemenata() << std::endl;

    int skiniCelo = red.skiniSaCela();
    std::cout << "Skini sa cela: " << skiniCelo << std::endl;
    std::cout << "Celo nakon skidanja: " << red.celo() << std::endl;
    std::cout << "Broj elemenata nakon skidanja: " << red.brojElemenata() << std::endl;

    DvostraniRed<int> kopija = red;
    std::cout << "Vrh kopije: " << kopija.vrh() << std::endl;
    std::cout << "Broj elemenata kopije: " << kopija.brojElemenata() << std::endl;

   return 0;
}
