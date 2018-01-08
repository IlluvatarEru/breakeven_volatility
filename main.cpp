#include <iostream>
#include "tsh.hpp"
#include "volatility.hpp"
#include "option.hpp"

#include "portfolio.hpp"
#include <time.h>


namespace BEV{

		/*void testoption()
		{
				option c(0.2,100,1.0,100.0,0.05,0, "Call", "SPX");
				option p(0.2,100,1.0,100.0,0.05,0, "Put", "SPX");
				double t1 = c.get_price();
				double t2 = p.get_price();
				std::cout << t1 << std::endl;
		}
				std::cout << t2 << std::endl;*/
		
		/*void testcsv()
		{
			std::string path="C:\\Users\\Arthur\\Documents\\Studies\\ParisDauphineUniversity\\Master203\\M2\\cpp\\project\\data\\";
			std::ifstream MyCSV(path + "data.csv");
			std::size_t t = csvf::count_lines(MyCSV);
			std::cout << t <<std::endl;
		}*/

		void testTSH()
		{	
			std::string path="C:\\Users\\Arthur\\Documents\\Studies\\ParisDauphineUniversity\\Master203\\M2\\cpp\\project\\data\\";
			std::ifstream MyCSV(path + "data.csv");
			TSH::tsh spx("SPX",MyCSV);
			std::vector<struct std::tm> mydates =spx.get_dates();
			
			option c(0.5,2000,mydates[500],spx,0.01,0, "Call", "SPX");
			c.set_delta_hedging("daily");
			double callprice = c.compute_price(mydates[0],0.5);
			std::cout << "t to M is " << time_diff(mydates[250],mydates[0]) <<std::endl;
			std::cout << "call price at t0 " << callprice <<std::endl;
			std::cout << "payoff at T is: " << c.payoff() <<std::endl;
			double pnl = c.delta_hedging_pnl(mydates[0],c.get_maturity());
			std::cout << "My unhedged  Pnl is " << c.compute_price(mydates[0],0.5)- c.payoff() <<std::endl;
			std::cout << "My delta hedged  Pnl is " << pnl <<std::endl;
			
			std::cout << " my delta is " << c.get_delta(mydates[0], 0.5,2000) << std::endl;
			std::vector<double> strikes = {2000,2500,2700};
			//v_skew::volatility_skew v = c.get_volatility_skew(mydates[0],strikes,"BE_vol");
			std::vector<struct std::tm> mat ={mydates[100],mydates[200],mydates[300],mydates[400]};
			v_surface::volatility_surface vv=c.get_volatility_surface(mydates[0],strikes,mat,"BE_vol");
			
		}
}

int main(int artc, char* argv[]){
	BEV::testTSH();
	return 0;
}