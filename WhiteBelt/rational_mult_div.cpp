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

Rational operator+(Rational lhs, Rational rhs) {
    int new_denominator = lcm(lhs.Denominator(), rhs.Denominator());
    return Rational(lhs.Numerator() *  new_denominator / lhs.Denominator() +
                    rhs.Numerator() *  new_denominator / rhs.Denominator(), new_denominator);
}

Rational operator-(Rational lhs, Rational rhs) {
    int new_denominator = lcm(lhs.Denominator(), rhs.Denominator());
    return Rational(lhs.Numerator() *  new_denominator / lhs.Denominator() -
                    rhs.Numerator() *  new_denominator / rhs.Denominator(), new_denominator);
}

Rational operator*(Rational lhs, Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator/(Rational lhs, Rational rhs) {
    return Rational(lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator());
}

int main() {
    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }

    cout << "OK" << endl;
    return 0;
}
