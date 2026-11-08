#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class StackOverflow : public std::runtime_error {
public:
    explicit StackOverflow(const std::string& msg = "Erreur : La pile est pleine (StackOverflow) !")
        : std::runtime_error(msg) {}
};

class StackUnderflow : public std::runtime_error {
public:
    explicit StackUnderflow(const std::string& msg = "Erreur : La pile est vide (StackUnderflow) !")
        : std::runtime_error(msg) {}
};

template <typename T>
class Stack {
private:
    std::size_t m_capacity;
    std::size_t m_top;
    std::unique_ptr<T[]> m_data;

public:
    explicit Stack(std::size_t capacity = 10)
        : m_capacity(capacity), m_top(0), m_data(std::make_unique<T[]>(capacity)) {}

    template <typename InputIterator>
    Stack(InputIterator begin, InputIterator end)
        : m_top(0) {
        std::size_t dist = 0;
        for (auto it = begin; it != end; ++it) {
            ++dist;
        }

        m_capacity = (dist == 0) ? 10 : dist;
        m_data = std::make_unique<T[]>(m_capacity);

        for (auto it = begin; it != end; ++it) {
            push(*it);
        }
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    Stack(Stack&&) noexcept = default;
    Stack& operator=(Stack&&) noexcept = default;

    ~Stack() = default;

    void push(const T& element) {
        if (m_top >= m_capacity) {
            throw StackOverflow();
        }
        m_data[m_top++] = element;
    }

    T pop() {
        if (empty()) {
            throw StackUnderflow();
        }
        return m_data[--m_top];
    }

    const T& peek() const {
        if (empty()) {
            throw StackUnderflow();
        }
        return m_data[m_top - 1];
    }

    bool empty() const {
        return m_top == 0;
    }

    std::size_t size() const {
        return m_top;
    }

    std::size_t capacity() const {
        return m_capacity;
    }
};

struct ObjetTest {
    static int compteur;
    int id;

    ObjetTest() : id(++compteur) {}
    ~ObjetTest() {}
};
int ObjetTest::compteur = 0;

int main() {
    try {
        std::cout << "=== Test 1 : Operations basiques et exceptions ===\n";
        Stack<int> pileInt(3);

        pileInt.push(10);
        pileInt.push(20);
        pileInt.push(30);

        std::cout << "Element au sommet (peek) : " << pileInt.peek() << "\n";

        std::cout << "Tentative de push dans une pile pleine...\n";
        try {
            pileInt.push(40);
        } catch (const StackOverflow& e) {
            std::cout << "Exception capturee : " << e.what() << "\n";
        }

        std::cout << "Depilage : ";
        while (!pileInt.empty()) {
            std::cout << pileInt.pop() << " ";
        }
        std::cout << "\n";

        std::cout << "Tentative de pop dans une pile vide...\n";
        try {
            pileInt.pop();
        } catch (const StackUnderflow& e) {
            std::cout << "Exception capturee : " << e.what() << "\n";
        }

        std::cout << "\n=== Test 2 : Constructeur par iterateurs ===\n";
        std::vector<std::string> mots = {"C++", "Templates", "UniquePtr", "RAII"};

        Stack<std::string> pileStrings(mots.begin(), mots.end());

        std::cout << "Taille de la pile : " << pileStrings.size() << "\n";
        std::cout << "Contenu depile : ";
        while (!pileStrings.empty()) {
            std::cout << pileStrings.pop() << " | ";
        }
        std::cout << "\n";

        std::cout << "\n=== Test 3 : Gros volume d'objets (Test d'allocation) ===\n";
        {
            constexpr std::size_t GROS_VOLUME = 1'000'000;
            Stack<ObjetTest> grossePile(GROS_VOLUME);

            for (std::size_t i = 0; i < GROS_VOLUME; ++i) {
                grossePile.push(ObjetTest());
            }

            std::cout << GROS_VOLUME << " objets empiles avec succes.\n";
            std::cout << "Taille au sommet : " << grossePile.size() << "\n";
        }

        std::cout << "Memoire liberee sans fuite (Destruction automatique RAII).\n";

    } catch (const std::exception& e) {
        std::cerr << "Erreur inattendue : " << e.what() << std::endl;
    }

    return 0;
}
