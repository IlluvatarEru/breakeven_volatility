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
			//std::cout<< "main: "<<csvf::count_lines(MyCSV)<<std::endl;
			TSH::tsh spx("SPX",MyCSV);
			std::vector<struct std::tm> mydates =spx.get_dates();
			//struct std::tm t1 =  {};//mydates[5];
			struct std::tm t2 =  {};//mydates[122];
			
			
			option c(0.9,2700,mydates[365],spx,0.01,0, "Call", "SPX");
			c.set_delta_hedging("daily");
			std::cout << "TRU FALSE "<<(c.get_spot().is_in(c.get_maturity())) <<std::endl;
			double pnl = c.delta_hedging_pnl(mydates[1],c.get_maturity());
			std::cout << "My Pnl is " << pnl <<std::endl;
			
			std::vector<double> strikes = {100,110,120};
			//v_skew::volatility_skew v=c.get_volatility_skew(mydates[1],mydates[365],strikes, "BE_vol");
			//std::cout << pnl <<std::endl;
			
			
			//daily_delta_hedging_pnl
			//double toto=spx.get_data()[spx.get_pos(day)];
			//std::cout << c.compute_price(day) << std::endl;
			//option p(0.2,100,1.0,spx,0.05,0, "Put", "SPX");
			//double t1 = c.get_price();
			//double t2 = p.get_price();
			//std::cout << t1 << std::endl;
			//std::cout << t2 << std::endl;
			
			//portfolio Mypf(c,mydates[15],mydates[155]);
			//Mypf.add_instrument(c,0);
			//Mypf.add_instrument(p,1);
			//std::cout << "MAIN ID IS " << c.GetUid() << std::endl;
			//std::vector<double> prices=Mypf.get_prices();
			//std::cout<< "QT " << Mypf.compute_value()  << std::endl;
		}
}

int main(int artc, char* argv[]){
	BEV::testTSH();
	return 0;
}