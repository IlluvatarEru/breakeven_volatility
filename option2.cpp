#include "option.hpp"
#include <iostream>

namespace fairvol{
	
	option::option(const double v, const double s, const double m)
	: m_volatility(v), m_strike(s), m_maturity(m) 
	{
		std::cout<<"option constructor"<<std::endl;
	}
	
}