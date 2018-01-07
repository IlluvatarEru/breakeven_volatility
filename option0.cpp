#include "option.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

	option::option(double v, double s, double m)
	: m_volatility(v), m_strike(s), m_maturity(m) 
	{
		std::cout<<"option constructor"<<std::endl;
	}
	
	/*option::~option()
	{
        std::cout << "option destructor" << std::endl;
    }*/
	
	/*const double& option::get_price() const 
    {
        return m_price;
    }*/
	
	/*const double option::get_maturity() const 
    {
        return m_maturity;
    }
	
	const double option::get_strike() const 
    {
        return m_strike;
    }*/
	
	const double option::compute_price()
	{
		double d1 = (log(m_spot/m_strike) + (m_interestrate +0.5*m_volatility**2)*m_maturity)/(m_volatility*m_maturity**0.5);
		double d2 = d1-m_volatility*m_maturity**0.5;
		if (option->type="Call"){
			sign = 1;}
		else if (option->type="Put"){
			sign = -1;
		}
		return sign * m_spot*normalDistribution(sign*d1) - sign * exp(- m_interestrate* m_maturity) * m_strike*normalDistribution(sign*d2);
	}