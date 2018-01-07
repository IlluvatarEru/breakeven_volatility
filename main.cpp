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
			struct std::tm t1 = {};
			t1.tm_year = 2015 - 1900;
			t1.tm_mon = 5;
			t1.tm_mday = 6;
			t1.tm_hour = 0;    //  To avoid any doubts about summer time, etc.
			mktime( &t1 );
			t2.tm_year = 2015 - 1900;
			t2.tm_mon = 5 ;
			t2.tm_mday = 16;
			t2.tm_hour = 0;    //  To avoid any doubts about summer time, etc.
			mktime( &t2 );
			
			std::cout << "T1<T2 " << t2.tm_mday-t1.tm_mday <<std::endl;
			std::cout << "diff " << time_diff(mydates[15],mydates[5]) <<std::endl;
			option c(0.3,1200,mydates[400],spx,0.05,0, "Call", "SPX");
			c.set_delta_hedging("daily");
			double pnl = c.delta_hedging_pnl(mydates[1],mydates[365]);
			std::cout << pnl <<std::endl;
			std::vector<double> strikes = {100,110,120};
			v_skew::volatility_skew vs(10,strikes);
			double mystrike = 110;
			std::cout << vs.get_volatility(mystrike) << std::endl;
			//daily_delta_hedging_pnl
			//double toto=spx.get_data()[spx.get_pos(day)];
			//std::cout << c.compute_price(day) << std::endl;
			//option p(0.2,100,1.0,spx,0.05,0, "Put", "SPX");
			//double t1 = c.get_price();
			//double t2 = p.get_price();
			//std::cout << t1 << std::endl;
			//std::cout << t2 << std::endl;
			//pf::portfolio Mypf;
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