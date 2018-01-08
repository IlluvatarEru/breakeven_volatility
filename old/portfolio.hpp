#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <iostream>
#include <vector>
#include <algorithm>

namespace BEV
{
		
		class instrument_holder{
			
			public:
				instrument_holder::instrument_holder(instrument inst,TSH::tsh quantity);
				instrument_holder::~instrument_holder();
				
			private:
				TSH::tsh m_quantity; 
				TSH::tsh m_prices;
				instrument* m_instrument;
		};
		
		class portfolio
		{
			public:
				
				explicit portfolio::portfolio(option opt, struct std::tm start, struct std::tm end);
				portfolio::~portfolio();
				
				v_skew::volatility_skew portfolio::get_skew(std::vector<double> strikes);
				/*void portfolio::add_instrument(instrument inst,TSH::tsh quantity);
				int portfolio::get_pos(instrument instr);
				std::size_t portfolio::get_number_of_instruments();
				double portfolio::get_quantity(instrument inst);
				std::vector<double> portfolio::get_prices();
				double portfolio::compute_value();*/
				
			private:
				double m_pnl;
				struct std::tm m_start;
				struct std::tm m_end;
				option* m_option;
				// vs_skew::volatility_skew vsk;
		};
}

#endif