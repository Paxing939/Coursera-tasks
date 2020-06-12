#include <iostream>
using namespace std;

int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap (a, b);
    }
    return a;
}

int lcm (int a, int b) {
    return a / gcd(a, b) * b;
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

int main() {
    //auto r = Rational(0, 10) + Rational(1, 10);
    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    cout << "OK" << endl;
    return 0;
}
