#include "volatility.hpp"
#include <iostream>
#include <vector>

option::option(const double volatility, const double strike, const double maturity)
	: m_volatility(volatility), m_strike(strike), m_maturity(maturity)  //constructor to initialize name
	{
		std::cout<<"option constructor"<<std::endl;
	}
	
	const double& option::get_price() const 
    {
        return m_price;
    }
	
	const double& option::get_maturity() const 
    {
        return m_maturity;
    }
	
	const double& option::get_strike() const 
    {
        return m_strike;
    }
	
	const double option::compute_price()
	{
		if (option->type="Call"){
			return exp(-rT)*;}
		else if (option->type="Put"){
			return exp(-rT)*;
		}
	}