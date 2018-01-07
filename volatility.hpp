#ifndef VOLATILITY_HPP
#define VOLATILITY_HPP

#include <vector>
#include <string>


namespace BEV{
	
	/************************************************
	*************** Volatility Surface **************
	*************************************************/
	
	/*
	namespace v_surface
	{
		class volatility_surface
		{
			public:
				explicit volatility_surface(std::vector<double> maturities, std::vector<double> strikes, std::vector<double> volatilities);
				virtual ~volatility_surface();
				double get_volatility(size_t index);
				double get_volatility(double strike, double maturity);
				int get_pos(double strike);
				int get_pos(double maturity);
				int get_pos(double volatility);
				
				
			private:
				std::vector<double> m_strikes;
				std::vector<double> m_maturities;
				std::vector< std::vector<double> > m_volatilities;
				
		};
	}*/
	
	namespace v_skew
	{
		
		/************************************************
		**************** Volatility Skew ****************
		*************************************************/
		
		class volatility_skew
		{
			public:
			
				/* constructors and destructors */
				explicit volatility_skew::volatility_skew(double maturity, std::vector<double> strikes, std::vector<double> volatilities);
				explicit volatility_skew::volatility_skew(double maturity, std::vector<double> strikes);
				virtual volatility_skew::~volatility_skew();
				
				/* getters */
				double volatility_skew::get_volatility(size_t index);
				double volatility_skew::get_volatility(double strike);
				ptrdiff_t  volatility_skew::get_pos(double strike);
				
				/* setters */
				void volatility_skew::set_volatilities(std::vector<double> volatilities);
				
				/* computers */
				std::vector<double> volatility_skew::get_break_even_volatility();

			private:
				double m_maturity;
				std::vector<double> m_strikes;
				std::vector<double> m_volatilities;
		};
	}
}

#endif
