#include <iostream>
#include <chrono>
#include<iomanip>
#include "BL_Calculator.h"
using namespace std;

/*
Type of velocity profile available here (just copy from this) :
Pohlhausen 2nd order
Pohlhausen 3rd order
Pohlhausen 4th order
Schlichting's sinusoidal
Majdalani-Xuan quartic

else, it's not defined
*/

int main() {
    cout << "Number of points = " << number_points << "\n";
    cout << "Number of threads used = " << num_threads << "\n\n";
    auto time_begin = chrono::high_resolution_clock::now();

    {
    BL_Calculator Solver("Pohlhausen 2nd order");
    Solver.MainMenu(1);
    cout << endl;
    }

    {
    BL_Calculator Solver("Pohlhausen 3rd order");
    Solver.MainMenu();
    cout << endl;
    }

    {
    BL_Calculator Solver("Pohlhausen 4th order");
    Solver.MainMenu();
    cout << endl;
    }

    {
    BL_Calculator Solver("Schlichting's sinusoidal");
    Solver.MainMenu();
    cout << endl;
    }

    {
    BL_Calculator Solver("Majdalani-Xuan quartic");
    Solver.MainMenu(0, 1);
    cout << endl;
    }

    auto time_end = chrono::high_resolution_clock::now();
    double time_elapsed_ms = chrono::duration_cast<chrono::milliseconds>(time_end - time_begin).count();
    cout << "\n\nTime needed for calculation = " << fixed << setprecision(3) << time_elapsed_ms << " ms\n";
}
