#ifndef VOLATILITY_HPP
#define VOLATILITY_HPP

#include <vector>
#include <string>


namespace BEV{
	
	/************************************************
	*************** Volatility Surface **************
	*************************************************/
	
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
				std::vector<double> get_strikes();
				std::vector<double> get_volatilities();
				double volatility_skew::get_volatility(size_t index);
				double volatility_skew::get_volatility(double strike);
				ptrdiff_t  volatility_skew::get_pos(double strike);
				
				/* setters */
				void volatility_skew::set_volatilities(std::vector<double> volatilities);
				void volatility_skew::set_strikes(std::vector< double> strikes);
				void volatility_skew::set_maturity(double maturity);
				
			private:
				double m_maturity;
				std::vector<double> m_strikes;
				std::vector<double> m_volatilities;
		};
	}
	
	namespace v_surface
	{
		class volatility_surface
		{
			public:
			
				/* constructors and destructors */
				explicit volatility_surface::volatility_surface(std::vector<double> maturities, std::vector<double> strikes, std::vector< std::vector<double> >  volatilities);
				volatility_surface::volatility_surface(std::vector<double> maturities, std::vector<v_skew::volatility_skew> vs);
				virtual volatility_surface::~volatility_surface();
				
				/* getters */
				double volatility_surface::get_volatility(double strike, double maturity);
				double volatility_surface::get_volatility_from_index(std::size_t index1, std::size_t index2);
				size_t volatility_surface::get_pos_strike(double strike);
				size_t volatility_surface::get_pos_maturity(double maturity);
				
				/* setters */
				void volatility_surface::set_strikes(std::vector< double> strikes);
				void volatility_surface::set_maturities(std::vector< double> maturity);
				void volatility_surface::set_volatilities(std::vector< std::vector<double> > volatilities);
				
				/* computers */
				void compute_surface();
				
			private:
				std::vector<double> m_strikes;
				std::vector<double> m_maturities;
				std::vector< std::vector<double> > m_volatilities;
				
		};
	}
	
}

#endif
