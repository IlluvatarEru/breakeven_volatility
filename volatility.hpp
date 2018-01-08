#ifndef VOLATILITY_HPP
#define VOLATILITY_HPP

#include <vector>
#include <string>


namespace BEV{
	
	namespace v_skew
	{
		
		/************************************************
		**************** Volatility Skew ****************
		*************************************************/
		
		class volatility_skew
		{
			public:
			
				/* constructors and destructors */
				volatility_skew::volatility_skew(double maturity, std::vector<double> strikes, std::vector<double> volatilities);
				volatility_skew::volatility_skew(double maturity, std::vector<double> strikes);
				volatility_skew::~volatility_skew();
				
				/* getters */
				const std::vector<double>& get_strikes() const;
				const std::vector<double>& get_volatilities() const;
				double volatility_skew::get_maturity() const;
				double volatility_skew::get_volatility(size_t index) const;
				double volatility_skew::get_volatility(double strike) const;
				ptrdiff_t  volatility_skew::get_pos(double strike) const;
				
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
		
			/************************************************
			*************** Volatility Surface **************
			*************************************************/
			
		class volatility_surface
		{
			public:
			
				/* constructors and destructors */
				volatility_surface::volatility_surface(std::vector<double> maturities, std::vector<double> strikes, std::vector< std::vector<double> >  volatilities);
				volatility_surface::volatility_surface(std::vector<double> maturities, std::vector<v_skew::volatility_skew> vs);
				volatility_surface::~volatility_surface();
				
				/* getters */
				const std::vector<double>& volatility_surface::get_strikes() const ;
				const std::vector<double>& volatility_surface::get_maturities() const ;
				const std::vector<std::vector<double>>& volatility_surface::get_volatilities() const ;
				double volatility_surface::get_volatility(double strike, double maturity) const;
				double volatility_surface::get_volatility_from_index(std::size_t index1, std::size_t index2) const;
				size_t volatility_surface::get_pos_strike(double strike) const;
				size_t volatility_surface::get_pos_maturity(double maturity) const;
				
				/* setters */
				void volatility_surface::set_strikes(std::vector< double> strikes);
				void volatility_surface::set_maturities(std::vector< double> maturity);
				void volatility_surface::set_volatilities(std::vector< std::vector<double> > volatilities);
				
				/* computers */
				//void compute_surface();
				
			private:
				std::vector<double> m_strikes;
				std::vector<double> m_maturities;
				std::vector< std::vector<double> > m_volatilities;
				
		};
	}
	
}

#endif
