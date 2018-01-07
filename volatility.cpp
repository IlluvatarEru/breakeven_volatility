#include "tsh.hpp"
#include "volatility.hpp"

namespace BEV{
	
	/************************************************
	*************** Volatility Surface **************
	*************************************************/
	
	/*
	volatility_surface::volatility_surface(std::vector<double> maturities, std::vector<double> strikes, std::vector<double> volatilities)
	: m_strikes(strikes), m_maturities(maturities), m_volatilities(volatilities)
	{
		std::cout<<"volatility_surface constructor"<<std::endl;
	}
	
	volatility_surface::~volatility_surface()
	{
		std::cout<<"volatility_surface destructor"<<std::endl;
	}
	
	double volatility_surface::get_volatility(size_t index) const //copy so no need of const
	{
		return m_volatilities[index];
	}
	*/
	
	namespace v_skew
	{
		
		/************************************************
		**************** Volatility Skew ****************
		*************************************************/
		
		/* constructors and destructors */
		
		volatility_skew::volatility_skew(double maturity, std::vector<double> strikes, std::vector<double> volatilities)
		: m_strikes(strikes), m_maturity(maturity), m_volatilities(volatilities)
		{
			std::cout<<"volatility_skew constructor"<<std::endl;
		}
		
		volatility_skew::volatility_skew(double maturity, std::vector<double> strikes)
		: m_strikes(strikes), m_maturity(maturity),m_volatilities(strikes.size(),0)
		{
			std::cout<<"volatility_skew constructor"<<std::endl;
		}
		
		volatility_skew::~volatility_skew()
		{
			std::cout<<"volatility_skew destructor"<<std::endl;
		}
		
		/* getters */
		
		double volatility_skew::get_volatility(size_t index)
		{
			return m_volatilities[index];
		}

		double volatility_skew::get_volatility(double strike)
		{
			return m_volatilities[get_pos(strike)];
		}
		
		ptrdiff_t  volatility_skew::get_pos(double strike)
		{
			ptrdiff_t pos = distance(m_strikes.begin(), find(m_strikes.begin(), m_strikes.end(), strike));
			if(pos >= m_strikes.size())
			{
				std::cout<< "pos out of bound"<<std::endl;
			}
			return pos;
		}
		
		/* setters */
		void volatility_skew::set_volatilities(std::vector<double> volatilities)
		{
			m_volatilities = volatilities;
		}
		
		/* computers */
		
		std::vector<double> volatility_skew::get_break_even_volatility()
		{
			std::vector<double> v(1,0);
			return v;
		}
		
	
	}
	
	
}