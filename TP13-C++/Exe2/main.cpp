#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <iomanip>

template <typename T, std::size_t N, std::size_t M>
class Matrice {
protected:
    T m_data[N][M];

public:
    Matrice() {
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                m_data[i][j] = T{};
            }
        }
    }

    Matrice(std::initializer_list<std::initializer_list<T>> liste) : Matrice() {
        std::size_t i = 0;
        for (const auto& ligne : liste) {
            if (i >= N) break;
            std::size_t j = 0;
            for (const auto& val : ligne) {
                if (j >= M) break;
                m_data[i][j] = val;
                ++j;
            }
            ++i;
        }
    }

    T& operator()(std::size_t i, std::size_t j) { return m_data[i][j]; }
    const T& operator()(std::size_t i, std::size_t j) const { return m_data[i][j]; }

    Matrice<T, N, M> operator+(const Matrice<T, N, M>& autre) const {
        Matrice<T, N, M> resultat;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                resultat(i, j) = m_data[i][j] + autre(i, j);
            }
        }
        return resultat;
    }

    Matrice<T, N, M> operator-(const Matrice<T, N, M>& autre) const {
        Matrice<T, N, M> resultat;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                resultat(i, j) = m_data[i][j] - autre(i, j);
            }
        }
        return resultat;
    }

    template <std::size_t P>
    Matrice<T, N, P> operator*(const Matrice<T, M, P>& autre) const {
        Matrice<T, N, P> resultat;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < P; ++j) {
                T somme = T{};
                for (std::size_t k = 0; k < M; ++k) {
                    somme += m_data[i][k] * autre(k, j);
                }
                resultat(i, j) = somme;
            }
        }
        return resultat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrice<T, N, M>& mat) {
        for (std::size_t i = 0; i < N; ++i) {
            os << "| ";
            for (std::size_t j = 0; j < M; ++j) {
                os << std::setw(6) << mat(i, j) << " ";
            }
            os << "|\n";
        }
        return os;
    }
};

template <std::size_t N, std::size_t M>
class Matrice<bool, N, M> {
private:
    bool m_data[N][M];

public:
    Matrice() {
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                m_data[i][j] = false;
            }
        }
    }

    Matrice(std::initializer_list<std::initializer_list<bool>> liste) : Matrice() {
        std::size_t i = 0;
        for (const auto& ligne : liste) {
            if (i >= N) break;
            std::size_t j = 0;
            for (const auto& val : ligne) {
                if (j >= M) break;
                m_data[i][j] = val;
                ++j;
            }
            ++i;
        }
    }

    bool& operator()(std::size_t i, std::size_t j) { return m_data[i][j]; }
    const bool& operator()(std::size_t i, std::size_t j) const { return m_data[i][j]; }

    Matrice<bool, N, M> operator+(const Matrice<bool, N, M>& autre) const {
        Matrice<bool, N, M> resultat;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                resultat(i, j) = m_data[i][j] || autre(i, j);
            }
        }
        return resultat;
    }

    template <std::size_t P>
    Matrice<bool, N, P> operator*(const Matrice<bool, M, P>& autre) const {
        Matrice<bool, N, P> resultat;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < P; ++j) {
                bool val = false;
                for (std::size_t k = 0; k < M; ++k) {
                    val = val || (m_data[i][k] && autre(k, j));
                }
                resultat(i, j) = val;
            }
        }
        return resultat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrice<bool, N, M>& mat) {
        for (std::size_t i = 0; i < N; ++i) {
            os << "| ";
            for (std::size_t j = 0; j < M; ++j) {
                os << (mat(i, j) ? " 1 " : " 0 ");
            }
            os << "|\n";
        }
        return os;
    }
};

int main() {
    std::cout << "=== 1. Test Matrices Int (2x3 et 3x2) ===\n";
    Matrice<int, 2, 3> A = {
        {1, 2, 3},
        {3, 4, 5}
    };

    Matrice<int, 2, 3> B = {
        {8, 7, 6},
        {3, 2, 1}
    };

    std::cout << "Matrice A :\n" << A;
    std::cout << "Matrice B :\n" << B;
    std::cout << "A + B :\n" << (A + B);

    Matrice<int, 3, 2> C = {
        {7, 8},
        {9, 1},
        {2, 3}
    };

    std::cout << "Matrice C (3x2) :\n" << C;
    std::cout << "Produit A * C (2x2) :\n" << (A * C);

    std::cout << "\n=== 2. Test Matrices Double (2x2) ===\n";
    Matrice<double, 2, 2> D1 = {
        {2.5, 2.5},
        {4.0, 4.0}
    };
    Matrice<double, 2, 2> D2 = {
        {3.0, 0.5},
        {6.0, 2.0}
    };

    std::cout << "D1 :\n" << D1;
    std::cout << "D2 :\n" << D2;
    std::cout << "D1 * D2 :\n" << (D1 * D2);

    std::cout << "\n=== 3. Test Spécialisation Bool (Produit Logique) ===\n";
    Matrice<bool, 2, 3> B1 = {
        {true,  false, true},
        {false, true,  false}
    };

    Matrice<bool, 3, 2> B2 = {
        {false, true},
        {true,  false},
        {true,  true}
    };

    std::cout << "Matrice Bool B1 (2x3) :\n" << B1;
    std::cout << "Matrice Bool B2 (3x2) :\n" << B2;
    std::cout << "Produit Logique B1 * B2 (2x2) :\n" << (B1 * B2);

    return 0;
}
