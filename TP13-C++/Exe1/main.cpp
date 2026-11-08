#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

template <typename T>
class ArrayDyn {
private:
    T* m_data;
    std::size_t m_taille;
    std::size_t m_capacite;

    void redimensionner(std::size_t nouvelleCapacite) {
        T* nouveauData = new T[nouvelleCapacite];
        for (std::size_t i = 0; i < m_taille; ++i) {
            nouveauData[i] = m_data[i];
        }
        delete[] m_data;
        m_data = nouveauData;
        m_capacite = nouvelleCapacite;
    }

public:
    explicit ArrayDyn(std::size_t capaciteInitiale = 4)
        : m_taille(0), m_capacite(capaciteInitiale) {
        m_data = new T[m_capacite];
    }


    ~ArrayDyn() {
        delete[] m_data;
    }

    ArrayDyn(const ArrayDyn& autre)
        : m_taille(autre.m_taille), m_capacite(autre.m_capacite) {
        m_data = new T[m_capacite];
        for (std::size_t i = 0; i < m_taille; ++i) {
            m_data[i] = autre.m_data[i];
        }
    }

    ArrayDyn& operator=(const ArrayDyn& autre) {
        if (this != &autre) {
            T* nouveauData = new T[autre.m_capacite];
            for (std::size_t i = 0; i < autre.m_taille; ++i) {
                nouveauData[i] = autre.m_data[i];
            }
            delete[] m_data;
            m_data = nouveauData;
            m_taille = autre.m_taille;
            m_capacite = autre.m_capacite;
        }
        return *this;
    }

    void push_back(const T& valeur) {
        if (m_taille >= m_capacite) {
            redimensionner(m_capacite == 0 ? 1 : m_capacite * 2);
        }
        m_data[m_taille++] = valeur;
    }

    void pop_back() {
        if (m_taille == 0) {
            throw std::underflow_error("Le tableau est vide !");
        }
        --m_taille;
    }

    T& operator[](std::size_t index) {
        if (index >= m_taille) {
            throw std::out_of_range("Index hors bornes !");
        }
        return m_data[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= m_taille) {
            throw std::out_of_range("Index hors bornes !");
        }
        return m_data[index];
    }

    std::size_t size() const { return m_taille; }
    std::size_t capacity() const { return m_capacite; }

    class Iterator {
    private:
        T* m_ptr;

    public:
        explicit Iterator(T* ptr) : m_ptr(ptr) {}

        T& operator*() const {
            return *m_ptr;
        }

        T* operator->() const {
            return m_ptr;
        }

        Iterator& operator++() {
            ++m_ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--() {
            --m_ptr;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator!=(const Iterator& autre) const {
            return m_ptr != autre.m_ptr;
        }

        bool operator==(const Iterator& autre) const {
            return m_ptr == autre.m_ptr;
        }
    };

    Iterator begin() {
        return Iterator(m_data);
    }

    Iterator end() {
        return Iterator(m_data + m_taille);
    }
};

struct Personne {
    std::string nom;
    int age;

    Personne() : nom("Anonyme"), age(0) {}
    Personne(std::string n, int a) : nom(std::move(n)), age(a) {}

    friend std::ostream& operator<<(std::ostream& os, const Personne& p) {
        return os << p.nom << " (" << p.age << " ans)";
    }
};

int main() {
    try {
        std::cout << "--- Test avec des int ---\n";
        ArrayDyn<int> nombres;
        nombres.push_back(10);
        nombres.push_back(20);
        nombres.push_back(30);

        for (auto &n : nombres) {
            std::cout << n << " ";
        }
        std::cout << "\n\n";

        std::cout << "--- Test avec std::string ---\n";
        ArrayDyn<std::string> mots;
        mots.push_back("C++");
        mots.push_back("Template");
        mots.push_back("Iterator");

        for (const auto &mot : mots) {
            std::cout << mot << " | ";
        }
        std::cout << "\n\n";

        std::cout << "--- Test avec un objet maison ---\n";
        ArrayDyn<Personne> equipe;
        equipe.push_back(Personne("Aya", 25));
        equipe.push_back(Personne("Badr", 30));
        equipe.push_back(Personne("Malak", 22));

        for (auto &p : equipe) {
            std::cout << p << "\n";
        }
        std::cout << "\n";

        std::cout << "--- Parcours inverse via itérateur ---\n";
        auto it = equipe.end();
        while (it != equipe.begin()) {
            --it;
            std::cout << *it << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
