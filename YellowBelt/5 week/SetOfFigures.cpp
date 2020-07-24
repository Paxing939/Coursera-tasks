#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

class Figure {
public:
    virtual string Name() = 0;

    virtual double Perimeter() = 0;

    virtual double Area() = 0;

protected:
    const double PI = 3.14;
};

class Triangle : public Figure {
public:

    Triangle(int a, int b, int c) : a_(a), b_(b), c_(c) {}

    string Name() override {
        return "TRIANGLE";
    }

    double Perimeter() override {
        return a_ + b_ + c_;
    }

    double Area() override {
        return sqrtf(Perimeter() / 2 * (Perimeter() / 2 - a_) * (Perimeter() / 2 - b_) * (Perimeter() / 2 - c_));
    }

private:
    int a_, b_, c_;
};

class Rect : public Figure {
public:

    Rect(int width, int height) : width_(width), height_(height) {}

    string Name() override {
        return "RECT";
    }

    double Perimeter() override {
        return 2 * (width_ + height_);
    }

    double Area() override {
        return width_ * height_;
    }

private:
    int width_, height_;
};

class Circle : public Figure {
public:

    Circle(int radius) : radius_(radius) {}

    string Name() override {
        return "CIRCLE";
    }

    double Perimeter() override {
        return 2 * PI * radius_;
    }

    double Area() override {
        return PI * radius_ * radius_;
    }

private:
    int radius_;
};

shared_ptr<Figure> CreateFigure(istringstream &is) {
    string type;
    is >> type;
    if (type == "RECT") {
        double width, height;
        is >> width >> height;
        return make_shared<Rect>(width, height);
    } else if (type == "TRIANGLE") {
        double a, b, c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    } else if (type == "CIRCLE") {
        double radius;
        is >> radius;
        return make_shared<Circle>(radius);
    }
    return nullptr;
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line);) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            // Пропускаем "лишние" ведущие пробелы.
            // Подробнее об std::ws можно узнать здесь:
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto &current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}