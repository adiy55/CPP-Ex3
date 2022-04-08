#include "sources/Matrix.hpp"

using namespace zich;
using std::cin;
using std::cout;
using std::exception;

void preset();

void userInput(Matrix &);

// todo: finish main
// todo: add documentation
// todo: cim

int main() {
//    std::cout << Matrix({{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10, 1});
//    Matrix matrix{{1, 2, 3}, 3, 1};
//    std::cout << "Enter Matrix values:\n";
//    std::cin >> matrix;
//    std::cout << "Output:\n" << matrix;
    Matrix matrix{{1, 0, 0, 0, 1, 0, 0, 0, 1}, 3, 3};
    int mode;
    while (cin) {
        cout << "For preset input enter 1\nFor user input enter 2\nEnter any other character to exit\n";
        cin >> mode;
        if (mode == 1) {
            preset();
        } else if (mode == 2) {
            userInput(matrix);
        } else {
            cout << "Exiting program...\n";
            return 0;
        }
    }
    return 0;
}


void preset() {
    int selection;
    cout << "Select:\n1 = 5x5 Identity Matrix | 2 = 4x4 Diagonal Matrix | 3 = 3x3 Symmetric Matrix\n";
    cin >> selection;
    if (selection == 1) {
        cout << Matrix{{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 5, 5} << '\n';
    } else if (selection == 2) {
        cout << Matrix{{16, 0, 0, 0, 0, 30, 0, 0, 0, 0, 24, 0, 0, 0, 0, 23}, 4, 4} << '\n';
    } else if (selection == 3) {
        cout << Matrix{{1, 7, 3, 7, 4, 5, 3, 5, 0}, 3, 3} << '\n';
    } else {
        cout << "Invalid selection!\n";
    }
}


void userInput(Matrix &matrix) {
    cout << "Enter (with spaces in between):\n";
    try {
        cin >> matrix;
        cout << "Current Matrix:\n";
        cout << matrix << '\n';
    } catch (exception &ex) {
        cout << "Invalid input: " << ex.what() << '\n';
    }
}