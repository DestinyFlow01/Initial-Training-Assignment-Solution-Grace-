#include "BL_Calculator.h"
#include<iostream>
#include<cmath>
#include<iomanip>
#include<omp.h>

// Main Function in BL_Calculator
void BL_Calculator::MainMenu(bool header, bool reference) {
	omp_set_num_threads(num_threads);
	Setting_f();
	Calculate_a1();
	Calculate_etastar_thetastar();
	Calculate_ShapeFactor();
	Calculate_delta99_Ratio();
	Calculate_deltastar_Ratio();
	Calculate_theta_Ratio();
	Calculate_Cf_Ratio();
	Calculate_Cd_Ratio();
	Calculate_Error();

	//Printing Initial results : 
	if (header) Print_Header();
	Print_BL_Params();
	if (reference) Print_BL_Params_Ref();

}




//Other functions needed in BL Calculator
BL_Calculator::BL_Calculator(string vel_type) : velocity_type(vel_type) {}

void BL_Calculator::Setting_f() {
	if (velocity_type == "Pohlhausen 2nd order") {
		equation = "2x - x^2";
		f1 = 2 * dx - dx * dx;
	}
	else if (velocity_type == "Pohlhausen 3rd order") {
		equation = "1.5x - 0.5x^3";
		f1 = 1.5 * dx - 0.5 * dx * dx * dx;
	}
	else if (velocity_type == "Pohlhausen 4th order") {
		equation = "2x - 2x^3 + x^4";
		double x3 = dx * dx * dx;
		double x4 = x3 * dx;
		f1 = 2 * dx - 2 * x3 + x4;
	}
	else if(velocity_type == "Schlichting's sinusoidal") {
		equation = "sin(0.5*pi*x)";
		f1 = sin(0.5 * PI * dx);
	}
	else if (velocity_type == "Majdalani-Xuan quartic") {
		equation = "5x/3 - x^3 + x^4/3";
		double x3 = dx * dx * dx;
		double x4 = x3 * dx;
		f1 = 5 * dx / 3.0 - x3 + x4 / 3.0;
	}
	else cerr << "Profile not found, change it to the available one";
}

void BL_Calculator::Calculate_a1() { a1 = (f1 - f0) / dx; } //Finite difference

void BL_Calculator::Calculate_etastar_thetastar() {
	double sum_eta = 0, sum_theta = 0; 
	#pragma omp parallel for reduction (+:sum_eta, sum_theta)
	for (int i = 0; i < number_points; i++) {
		//Calculate x : 
		double x_val = i * dx;

		//Calculate f : 
		double f_val = 0;
		if (velocity_type == "Pohlhausen 2nd order") f_val = 2 * x_val - x_val * x_val;
		else if (velocity_type == "Pohlhausen 3rd order") f_val = 1.5 * x_val - 0.5 * x_val * x_val * x_val;
		else if (velocity_type == "Pohlhausen 4th order") {
			double x3 = x_val * x_val * x_val;
			double x4 = x3 * x_val;
			f_val = 2 * x_val - 2 * x3 + x4;
		}
		else if (velocity_type == "Schlichting's sinusoidal") f_val = sinf(PI * x_val / 2);
		else if (velocity_type == "Majdalani-Xuan quartic") {
			double x3 = x_val * x_val * x_val;
			double x4 = x3 * x_val;
			f_val = 5 * x_val / 3.0 - x3 + x4 / 3.0;
		}

		sum_eta += 1 - f_val;
		sum_theta += f_val * (1 - f_val);
	}

	BL_Params[0] = dx * sum_eta;
	BL_Params[1] = dx * sum_theta;
}

void BL_Calculator::Calculate_ShapeFactor() { BL_Params[2] = BL_Params[0] / BL_Params[1]; }

void BL_Calculator::Calculate_delta99_Ratio() {	BL_Params[3] = sqrt(2*a1* BL_Params[2] / BL_Params[0]); }

void BL_Calculator::Calculate_deltastar_Ratio() { BL_Params[4] = BL_Params[3] * BL_Params[0]; }

void BL_Calculator::Calculate_theta_Ratio() { BL_Params[5] = BL_Params[4] / BL_Params[2]; }

void BL_Calculator::Calculate_Cf_Ratio() { BL_Params[6] = 2*a1 / BL_Params[3]; }

void BL_Calculator::Calculate_Cd_Ratio() { BL_Params[7] = 2 * BL_Params[6]; }

void BL_Calculator::Calculate_Error() {
	for (int i = 0; i < 8; i++) error_percent[i] = fabs(BL_Params[i] - BL_Params_ref[i]) / BL_Params_ref[i] * 100;
}

void BL_Calculator::Print_Header() const {
	// Print header
	for (int i = 0; i < 9; i++) {
		if (i == 0) cout << left << setw(40) << header_name[i];
		else cout << right << setw(10) << header_name[i];
	}
	cout << endl;
	cout << string(40 + 8*10, '-') << endl;
}

void BL_Calculator::Print_BL_Params() const {
	//Printing Results
	cout << left << setw(40) << velocity_type;
	cout << fixed << setprecision(5);
	for (int i = 0; i < 8; i++) cout << right << setw(10) << BL_Params[i];
	cout << endl;

	//Printing error :
	cout << left << setw(40) << equation;
	cout << fixed << setprecision(5);
	for (int i = 0; i < 8; i++) cout << right << setw(9) << error_percent[i] << "%";
	cout << endl;
}

void BL_Calculator::Print_BL_Params_Ref() const {
	//Printing Results
	cout << endl;
	cout << left << setw(40) << "Blausius";
	cout << fixed << setprecision(5);
	for (int i = 0; i < 8; i++) cout << right << setw(10) << BL_Params_ref[i];
	cout << endl;
}
