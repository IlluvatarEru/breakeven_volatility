#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>
#include <vector>
#include <string>

class option
    {
		
    public:
		option(double v,double s,double m);
		//~option();
		
		//double get_price();
		//void print_price();
		//double get_strike();
		//void print_strike();
		//double get_volatility();
		//void print_volatility();
		//double get_maturity();
		/* void print_maturity();
		double get_spot();
		void print_spot();
		double get_interestrate();
		void print_interestrate();
		 */
	private:
		std::string m_type;
		double m_price;
		double m_volatility;
		double m_maturity;
		double m_strike;
		double m_spot;
		double m_dividendyield;
		/* greek m_interestrate;
		greek m_delta;
		greek m_gamma; */

	};	
	
#endif