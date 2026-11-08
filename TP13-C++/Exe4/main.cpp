#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cassert>
#include <algorithm>

template <typename T>
struct Asc {
    constexpr bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <typename T>
struct Desc {
    constexpr bool operator()(const T& a, const T& b) const {
        return b < a;
    }
};


template <typename T, typename Comp = std::less<T>>
int partitionner(std::vector<T>& vec, int bas, int haut, Comp comp = Comp{}) {
    T pivot = vec[haut];
    int i = bas - 1;

    for (int j = bas; j < haut; ++j) {
        if (comp(vec[j], pivot)) {
            ++i;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[haut]);
    return i + 1;
}

template <typename T, typename Comp = std::less<T>>
void quickSortImpl(std::vector<T>& vec, int bas, int haut, Comp comp = Comp{}) {
    if (bas < haut) {
        int pi = partitionner(vec, bas, haut, comp);
        quickSortImpl(vec, bas, pi - 1, comp);
        quickSortImpl(vec, pi + 1, haut, comp);
    }
}

template <typename T, typename Comp = std::less<T>>
void quickSort(std::vector<T>& vec, Comp comp = Comp{}) {
    if (!vec.empty()) {
        quickSortImpl(vec, 0, static_cast<int>(vec.size()) - 1, comp);
    }
}

struct Etudiant {
    std::string nom;
    double note;

    bool operator<(const Etudiant& autre) const {
        return note < autre.note;
    }

    friend std::ostream& operator<<(std::ostream& os, const Etudiant& e) {
        return os << e.nom << " (" << e.note << "/20)";
    }
};

template <typename T>
void afficherVecteur(const std::vector<T>& vec) {
    std::cout << "[ ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "]\n";
}

void executerTestsUnitaires() {
    std::cout << "--- Execution des tests unitaires ---\n";

    std::vector<int> v1 = {5, 2, 9, 1, 7};
    quickSort(v1);
    assert(std::is_sorted(v1.begin(), v1.end()));

    std::vector<std::string> v2 = {"Zebra", "Apple", "Mango", "Banana"};
    quickSort(v2, Desc<std::string>{});
    assert(std::is_sorted(v2.begin(), v2.end(), std::greater<std::string>{}));

    std::vector<Etudiant> v3 = {{"Ahmed", 14.5}, {"Aya", 18.0}, {"Youssef", 11.0}};
    quickSort(v3);
    assert(std::is_sorted(v3.begin(), v3.end()));

    std::cout << "Tous les tests unitaires ont reussi avec succes (asserts OK) !\n\n";
}

int main() {
    executerTestsUnitaires();

    std::cout << "=== Demonstration du QuickSort ===\n\n";

    std::vector<int> nombres = {42, 12, 88, 3, 27, 64};
    std::cout << "Nombres originaux   : "; afficherVecteur(nombres);

    quickSort(nombres, Asc<int>{});
    std::cout << "Tri Ascendant (Asc) : "; afficherVecteur(nombres);

    quickSort(nombres, Desc<int>{});
    std::cout << "Tri Descendant (Desc): "; afficherVecteur(nombres);
    std::cout << "\n";

    std::vector<std::string> fruits = {"Fraise", "Ananas", "Kiwi", "Orange", "Banane"};
    std::cout << "Fruits originaux    : "; afficherVecteur(fruits);

    quickSort(fruits);
    std::cout << "Tri par defaut (<)  : "; afficherVecteur(fruits);
    std::cout << "\n";

    std::vector<Etudiant> promo = {
        {"Adil", 15.5},
        {"Salma", 19.0},
        {"Islam", 12.0},
        {"Hiba", 17.25}
    };

    std::cout << "Promotion originale :\n";
    for (const auto& e : promo) std::cout << " - " << e << "\n";

    quickSort(promo);
    std::cout << "\nPromotion triee par note croissante :\n";
    for (const auto& e : promo) std::cout << " - " << e << "\n";

    quickSort(promo, Desc<Etudiant>{});
    std::cout << "\nPromotion triee par note decroissante :\n";
    for (const auto& e : promo) std::cout << " - " << e << "\n";

    return 0;
}
