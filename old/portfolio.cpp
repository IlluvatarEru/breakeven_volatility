#include "tsh.hpp"
#include "volatility.hpp"
#include "option.hpp"
#include "portfolio.hpp."


bool greater0(double i) {return i>0; }

namespace BEV{
	
		instrument_holder::instrument_holder(instrument inst,TSH::tsh quantity)
		: m_instrument(&inst),m_prices(quantity.get_udl(),quantity.get_size()), m_quantity(quantity.get_udl(),quantity.get_size())
		{
			m_prices.set_dates(quantity.get_dates());
			m_quantity.set_dates(quantity.get_dates());
			m_quantity.set_data(quantity.get_data());
			std::cout << "instrument holder constructor" << std::endl;
		}
		
		instrument_holder::~instrument_holder()
		{
			std::cout << "instrument holder destructor" << std::endl;
		}
		
		portfolio::portfolio(option opt, struct std::tm start, struct std::tm end)
		: m_start(start), m_end(end), m_option(&opt) 
		{
			
			struct std::tm t1 = opt.get_spot().get_date(0);
			struct std::tm t2 = opt.get_spot().get_date(opt.get_spot().get_size()-1);
			if(t1> start)
			{
				std::cout << "Start date not in data" << std::endl;
				m_start = t1;
			}
			if(t2 < end)
			{
				std::cout << "End date not in data" << std::endl;
				m_end = t2;
			}
			std::cout << "pf constructor" << std::endl;
		}
		
		portfolio::~portfolio()
		{
			std::cout << "portfolio destructor" << std::endl;
		}
		
		v_skew::volatility_skew portfolio::get_skew(std::vector<double> strikes)
		{
			return m_option->get_volatility_skew(m_start,strikes,"BE_vol");
		} 
		
		
		/*void portfolio::add_instrument(instrument inst,TSH::tsh quantity)
		{
			instrument_holder new_instrument_holder(inst, quantity);
			m_InstrumentsHolder.push_back(new_instrument_holder);
		}
		
		std::size_t portfolio::get_number_of_instruments()
		{
			std::size_t mycount = count_if (m_InstrumentsHolder.quantity.begin(), m_InstrumentsHolder.quantity.end(), greater0);
			return mycount;
		}
		
		int portfolio::get_pos(const instrument inst)
		{
			int pos=-1;
			std::size_t nb_inst = m_InstrumentsHolder.instruments.size();
			int i=0;
			while ((i< nb_inst) && (pos<0))
			{
				if (m_InstrumentsHolder.instruments[i].GetUid()==inst.GetUid())
				{
					pos=i;
				}
				i++;
			}
			return pos;
		}
		
		double portfolio::get_quantity(const instrument inst)
		{

			return m_InstrumentsHolder.quantity[get_pos(inst)];
		}
		
		std::vector<double> portfolio::get_prices()
		{
			std::vector<double> prices;
			std::size_t nb_inst = m_InstrumentsHolder.instruments.size();
			int i=0;
			while(i< nb_inst)
			{
				if (get_quantity(m_InstrumentsHolder.instruments[i])>0)
				{
					prices.push_back(m_InstrumentsHolder.instruments[i].get_price());
				}
				i++;
			}
			return prices;
		}
		
		double portfolio::compute_value()
		{
			double pf_value=0;
			std::vector<double> prices = get_prices();
			std::vector<double> temp;
			std::transform( prices.begin(), prices.end(),m_InstrumentsHolder.quantity.begin(), std::back_inserter(temp), std::multiplies<double>() );
			for (auto& n : temp)
			{
				pf_value += n;
			}
			return pf_value;
		}

	}*/
}