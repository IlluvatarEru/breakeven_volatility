#include <iostream>
#include "tsh.hpp"
#include "volatility.hpp"
#include "option.hpp"
#include "portfolio.hpp"
#include <time.h>


namespace BEV{

		

		void testTSH()
		{	
			std::string path="C:\\Users\\Arthur\\Documents\\Studies\\ParisDauphineUniversity\\Master203\\M2\\cpp\\project\\data\\";
			std::ifstream MyCSV(path + "data.csv");
			TSH::tsh spx("SPX",MyCSV);
			std::vector<struct std::tm> mydates =spx.get_dates();
			option c(0.5,2000,mydates[500],spx,0.01,0, "Put", "SPX");
			c.set_delta_hedging("daily");
			double callprice = c.compute_price(mydates[0],0.5);
			std::cout << "t to M is " << c.get_type() <<std::endl;
			std::cout << "call price at t0 " << callprice <<std::endl;
			std::cout << "payoff at T is: " << c.payoff(c.get_maturity()) <<std::endl;
			double pnl = c.delta_hedging_pnl(mydates[0],c.get_maturity());
			std::cout << "My unhedged  Pnl is " << c.compute_price(mydates[0],0.5)- c.payoff(c.get_maturity()) <<std::endl;
			std::cout << "My delta hedged  Pnl is " << pnl <<std::endl;
			double vol = c.get_BE_vol(mydates[0],2000,mydates[500]);
			std::cout << "my BE VOL IS " << vol << std::endl;
			c.set_volatility(vol);
			std::cout << "My delta hedged  Pnl is " << c.delta_hedging_pnl(mydates[0],c.get_maturity()) <<std::endl;
			
			/*std::cout << " my delta is " << c.get_delta(mydates[0],c.get_maturity(), 0.5,2000) << std::endl;
			std::vector<double> strikes = {2000,2500,2700};
			v_skew::volatility_skew v = c.get_volatility_skew(mydates[0],strikes,"BE_vol");
			std::vector<struct std::tm> mat ={mydates[100],mydates[200],mydates[300],mydates[400]};
			v_surface::volatility_surface vv=c.get_volatility_surface(mydates[0],strikes,mat,"BE_vol");*/
			
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
	double K = 2000 ;
	BEV::option Call(0.5, K, T, spx_ts, 0.01, 0, "Call", "SPX");
	
	// Compute Pnl of delta hedged option with cosntant BS vol
	std::cout << "My BS-vol delta-hedged Pnl is " << Call.delta_hedging_pnl(start,T) <<std::endl;
	
	// Compute Pnl with Break Even vol
	double BEvol = Call.get_BE_vol(start,K,T);
	Call.set_volatility(BEvol);
	std::cout << "My BE-vol delta-hedged  Pnl is " << Call.delta_hedging_pnl(start,T) << " With a BE vol of " << BEvol <<std::endl;
	
	//Approximate BE-vol with robust
	// double BEvol_robust = Call.get_BE_vol_robust(start,K,T);
	// Call.set_volatility(BEvol_robust);
	// std::cout << "Pnl is  "<< Call.daily_delta_hedging_pnl_robust(start,T) << " with robust vol is " << BEvol_robust <<std::endl;
	
	/*
	//Define ranges of maturities and strikes and compute volatility surface
	std::vector<double> strikes = {2000,2500,2700};
	std::vector<struct std::tm> mat ={spx_ts.get_date(100),spx_ts.get_date(200),spx_ts.get_date(300),spx_ts.get_date(400)};
	BEV::v_surface::volatility_surface vv=Call.get_volatility_surface(start,strikes,mat,"BE_vol");
	*/
	
	return 0;
}