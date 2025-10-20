#pragma once
#include <string>
#include<vector>
using namespace std;

constexpr int number_points = 1e8 + 1;
constexpr double dx = 1.0 / (number_points - 1);
constexpr double PI = 3.1415926535897932384626433832795;
constexpr int num_threads = 12;

class BL_Calculator {
	private : 
		string velocity_type = "";
		double f0 = 0, f1 = 0;
		double a1 = 0; //First derivative of f at y = 0
		

		//Boundary Layer Parameters (need to be calculated) : 
		double BL_Params[8] = {};

		//Reference solution (Blausius): 
		double BL_Params_ref[8] = {
			0.344, // eta_star = δ*/δ			-- 0
			0.133, // theta_star = θ/δ			-- 1
			2.59,  // shape factor H			-- 2
			5.0,   // δ√Re_x / x  (using δ99)	-- 3
			1.72,  // δ*√Re_x / x				-- 4
			0.664, // θ√Re_x / x				-- 5
			0.664, // C_f√Re_x					-- 6
			1.328  // C_d√Re_x					-- 7
		};

		//Error compared to Blausius :
		double error_percent[8] = {};

		//Printing purpose : 
		string header_name[9] = {
			"u/U = F(x), x = y/delta",
			"Eta*",
			"Theta*",
			"Shape H",
			"delta",
			"delta*",
			"theta",
			"Cf",
			"Cd"
		};
		string equation;

	public : 
		//Constructor ; 
		BL_Calculator(string vel_type);

		//Calculator : 
		void Setting_f();
		void Calculate_a1();
		void Calculate_etastar_thetastar();	//Index 0 and 1
		void Calculate_ShapeFactor();		//Index 2
		void Calculate_delta99_Ratio();		//Index 3
		void Calculate_deltastar_Ratio();	//Index 4
		void Calculate_theta_Ratio();		//Index 5
		void Calculate_Cf_Ratio();			//Index 6
		void Calculate_Cd_Ratio();			//Index 7
		void Calculate_Error();

		//Printing : 
		void Print_Header() const;
		void Print_BL_Params() const;
		void Print_BL_Params_Ref() const;

		//Main Menu : 
		void MainMenu(bool header = 0, bool reference = 0);
};


