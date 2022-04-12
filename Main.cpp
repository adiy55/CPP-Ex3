#include "sources/Matrix.hpp"
#include <limits>

using namespace zich;
using std::cin;
using std::cout;
using std::exception;

void preset(const std::vector<double> &identity);

void userInput(Matrix &);

int main() {
    const std::vector<double> identity{1, 0, 0, 0, 1, 0, 0, 0, 1};
    Matrix matrix{identity, 3, 3};
    int mode;
    while (cin) {
        cout << "For preset input enter 1\nFor user input enter 2\nEnter any other character to exit\n";
        cin >> mode;
        // https://www.tutorialspoint.com/what-is-the-use-of-cin-ignore-in-cplusplus
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clear buffer before taking new line
        if (mode == 1) {
            preset(identity);
        } else if (mode == 2) {
            userInput(matrix);
        } else {
            cout << "Exiting program...\n";
            return 0;
        }
    }
    return 0;
}

void preset(const std::vector<double> &identity) {
    int selection;
    cout << "Select:\n1 = 5x5 Identity Matrix | 2 = 4x4 Diagonal Matrix | 3 = 3x3 Symmetric Matrix | "
            "4 = Matrix Multiplication Demo\n";
    cin >> selection;
    if (selection == 1) {
        cout << Matrix{{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 5, 5} << '\n';
    } else if (selection == 2) {
        cout << Matrix{{16, 0, 0, 0, 0, 30, 0, 0, 0, 0, 24, 0, 0, 0, 0, 23}, 4, 4} << '\n';
    } else if (selection == 3) {
        cout << Matrix{{1, 7, 3, 7, 4, 5, 3, 5, 0}, 3, 3} << '\n';
    } else if (selection == 4) {
        Matrix matrix1{identity, 3, 3};
        Matrix matrix2{{1, 2, 3}, 3, 1};
        cout << "Multiplying:\n" << matrix1 << "\nWith:\n" << matrix2 << "\nResult:\n" << matrix1 * matrix2 << '\n';
    } else {
        cout << "Invalid selection!\n";
    }
}

void userInput(Matrix &matrix) {
    cout << "Enter Matrix Values:\nValid Format Example: [1 0 0], [0 1 0], [0 0 1]\n";
    try {
        cin >> matrix;
        cout << "Updated Matrix:\n";
        cout << matrix << '\n';
    } catch (exception &ex) {
        cout << "Invalid input: " << ex.what() << '\n';
    }
}