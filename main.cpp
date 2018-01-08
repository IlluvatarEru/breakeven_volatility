#include <iostream>
#include "tsh.hpp"
#include "volatility.hpp"
#include "option.hpp"
#include <time.h>
#include <fstream> 

namespace BEV{

		void write_2d_to_csv(std::vector<double> v1, std::vector<double> v2, std::string& path, std::string& filename)
		{
			std::ofstream myfile(path+filename+".csv");
			int vsize = v1.size();
			for(int n=0; n<vsize; n++)
			{
				myfile << v1[n];
				myfile << "," ;
			}
			myfile << std::endl;
			vsize = v2.size();
			for(int n=0; n<vsize; n++)
			{
				myfile << v2[n];
				myfile << "," ;
			}

		}
		
		void write_3d_to_csv(std::vector<double> v1, std::vector<std::string> v2, std::vector<std::vector<double>> v3, std::string& path, std::string& filename)
		{
			std::ofstream myfile(path+filename+".csv");
			int vsize_k = v1.size();
			int vsize_t = v2.size();
			for(int n=0; n<vsize_k; n++)
			{
				for(int m=0; m<vsize_t; m++)
				{
					myfile << v1[n];
					myfile << "," ;
				}
			}
			myfile << std::endl;
			for(int n=0; n<vsize_k; n++)
			{
				for(int m=0; m<vsize_t; m++)
				{
					myfile << v2[n];
					myfile << "," ;
				}
			}
			myfile << std::endl;
			for(int n=0; n<vsize_k; n++)
			{
				for(int m=0;m<vsize_t;m++)
				{
					myfile << v3[n][m];
					myfile << "," ;
				}
			}

		}


		
}

int main(int artc, char* argv[]){
	
	// Let us import the data from a CSV file to a time_serie object
	std::string path="C:\\Users\\Arthur\\Documents\\Studies\\ParisDauphineUniversity\\Master203\\M2\\cpp\\project\\data\\";
	std::ifstream MyCSV(path + "data.csv");
	BEV::TSH::tsh spx_ts("SPX",MyCSV);
	MyCSV.close();
	
	// Define start date
	struct std::tm start = BEV::str_to_date("02/01/2015");
	
	// Define maturity at 1 y from start
	struct std::tm T =  BEV::str_to_date("04/01/2016");
	
	// Let us define a 1 year maturity option with some parameters
	double K = 2100 ;
	BEV::option Call(0.5, K, T, spx_ts, 0.01, 0, "Call", "SPX");
	
	// Compute Pnl of delta hedged option with cosntant BS vol
	std::cout << "My BS-vol delta-hedged Pnl is " << Call.daily_delta_hedging_pnl(start,T) <<std::endl;
	
	// Compute Pnl with Break Even vol
	double BEvol = Call.get_BE_vol(start,K,T);
	Call.set_volatility(BEvol);
	std::cout << "My BE-vol delta-hedged Pnl is " << Call.daily_delta_hedging_pnl(start,T) << " with a BE vol of " << BEvol <<std::endl;
	
	//Approximate BE-vol with robust
	double BEvol_robust = Call.get_BE_vol_robust(start,K,T);
	Call.set_volatility(BEvol_robust);
	std::cout << "My BE-vol Robust delta-hedged Pnl is "<< Call.daily_delta_hedging_pnl_robust(start,T) << " with a robust BE vol of " << BEvol_robust <<std::endl;
	
	
	//Define ranges of maturities and strikes and compute volatility skew and surface
	std::vector<double> strikes = {1950,2000,2050};
	std::vector<struct std::tm> mat ={BEV::str_to_date("02/06/2015"),BEV::str_to_date("02/11/2016"),BEV::str_to_date("04/01/2016")};
	std::vector<std::string> mat_str = {"02/06/2015","02/11/2015","04/01/2016"};
	
	BEV::v_skew::volatility_skew v1 = Call.get_volatility_skew(start, strikes, "BE_vol");
	std::string f ="Skew";
	BEV::write_2d_to_csv(v1.get_strikes(),v1.get_volatilities(),path,f);
	
	BEV::v_surface::volatility_surface v2 = Call.get_volatility_surface(start,strikes,mat,"BE_vol");
	f = "Surface";
	BEV::write_3d_to_csv(v2.get_strikes(),mat_str,v2.get_volatilities(),path,f);
	
	return 0;
}