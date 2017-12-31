#include <iostream>
#include <vector>

class option
    {
		
    public:
		
		double get_price();
		void print_price();
		double get_strike();
		void print_strike();
		double get_volatility();
		void print_volatility();
		double get_maturity();
		void print_maturity();
		
		explicit volatility(const std::string& udlname=""); //prevents implicit conversion like converting a string to volatility
        const std::string& underlying_name() const;
		virtual double get_volatility(size_t index) const;
		virtual ~volatility();
		
		
	private:
		std::string m_type;
		double m_price;
		double m_volatility;
		double m_maturity;
		double m_strike;
		double m_spot;
		double m_dividendyield;
		double m_interestrate;
		double m_delta;
		double m_gamma;
			
	protected:	//can be access by all children classes
	//usually include only methods in protected, not data members
		int id;
	};	