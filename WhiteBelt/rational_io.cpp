#include <iostream>
#include <sstream>

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
    if (in) {
        char c;
        int numerator, denominator;
        in >> numerator;
        in >> c;
        in >> denominator;
        if(in && c == '/') {
            fraction = Rational(numerator, denominator);
        }
    }
    return in;
}

ostream& operator<<(ostream& out, const Rational& fraction) {
    out << fraction.Numerator();
    out << '/';
    out << fraction.Denominator();
    return out;
}

int main() {
    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 4;
        }
    }

    cout << "OK" << endl;
    return 0;
}
