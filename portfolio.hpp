#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <iostream>
#include <vector>
#include <algorithm>

namespace BEV
{
	
	
	namespace pf
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
				
				explicit portfolio::portfolio(std::string pf_name);
				portfolio::portfolio(std::string pf_name, std::vector<instrument_holder> InstrumentsHolder);
				portfolio::~portfolio();
				//void portfolio::add_instrument(instrument inst);
				/*int portfolio::get_pos(instrument instr);
				std::size_t portfolio::get_number_of_instruments();
				double portfolio::get_quantity(instrument inst);
				std::vector<double> portfolio::get_prices();
				double portfolio::compute_value();*/
				
			private:
				TSH::tsh m_pnl;
				std::vector<instrument_holder> m_InstrumentsHolder;
				
		};
	}
}

#endif