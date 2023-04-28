#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap (a, b);
    }
    return a;
}

int lcm (int a, int b) {
    return a / gcd (a, b) * b;
}

class Rational {
public:
    Rational() : p(0), q(1) {}

    int gcd(int a, int b) {
        while (b) {
            a %= b;
            swap (a, b);
        }
        return a;
    }

    Rational(int numerator, int denominator) {
        int nod = gcd(numerator, denominator);
        p = numerator / nod;
        q = denominator / nod;
        if (q < 0) {
            p = -p;
            q = -q;
        }
        if (p == 0) {
            q = 1;
        }
    }

    int Numerator() const {
        return p;
    }

    int Denominator() const {
        return q;
    }

private:
    int q, p;
};

bool operator==(Rational lhs, Rational rhs) {
    return lhs.Numerator() == rhs.Numerator() &&  lhs.Denominator() == rhs.Denominator();
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    int new_denominator = lcm(lhs.Denominator(), rhs.Denominator());
    return Rational(lhs.Numerator() *  rhs.Denominator() +
                    rhs.Numerator() * lhs.Denominator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator-(Rational lhs, Rational rhs) {
    int new_denominator = lcm(lhs.Denominator(), rhs.Denominator());
    return Rational(lhs.Numerator() *  rhs.Denominator() -
                    rhs.Numerator() * lhs.Denominator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator*(Rational lhs, Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator/(Rational lhs, Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator());
}

istream& operator>>(istream& in, Rational& fraction) {
    if (in.eof() || !in) {
        return in;
    }
    int numerator = 0, denominator = 1;
    in >> numerator;
    if (!in) {
        return in;
    }
    in.ignore(1);
    in >> denominator;
    if (!in) {
        return in;
    }
    fraction = Rational(numerator, denominator);
    return in;
}

ostream& operator<<(ostream& out, const Rational& fraction) {
    out << fraction.Numerator();
    out << '/';
    out << fraction.Denominator();
    return out;
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}

inline bool operator<(const Rational& lhs, const Rational& rhs) {
    int left = 0, right = 0;
    int common_denominator = lcm(lhs.Denominator(), rhs.Denominator());
    left = lhs.Numerator() * common_denominator / lhs.Denominator();
    right = rhs.Numerator() * common_denominator / rhs.Denominator();
    return left < right;
}

inline bool operator> (const Rational& lhs, const Rational& rhs){ return rhs < lhs; }

inline bool operator<=(const Rational& lhs, const Rational& rhs){ return !(lhs > rhs); }

inline bool operator>=(const Rational& lhs, const Rational& rhs){ return !(lhs < rhs); }

int main() {
    {
        const set<Rational> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{{1, 25}, {1, 2}, {3, 4}}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }

    cout << "OK" << endl;
    return 0;
}
