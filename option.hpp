#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <atomic>

namespace BEV{

	/* Normal distribution methods */
	double norm_pdf(const double& x);
	double norm_cdf(const double& x);
	
	
	/************************************************
	****************** Instrument	*****************
	*************************************************/
	
	/* This class is meant to represent a financial instrument*/
	class instrument
	{
		public:
			/* constructors and destrucor */
			explicit instrument(std::string udl, double div);
			virtual~instrument();
			
			/* getters */
			virtual double instrument::get_price() const;
			int instrument::GetUid() const;

			/* setters*/
			void set_price(double price);
			
			/* computers */
			virtual double compute_price() const;
			
		private:
			
			std::string m_udl;
			double m_price;
			double m_dividendyield;
			static int newUID;
    		const int uid;
	};

	/************************************************
	********************* Stock *********************
	*************************************************/
	
	class stock : public instrument
	{		
		public:
			/* constructors and destrucor */
			explicit stock(std::string udl, double div);
			virtual~stock();
	};
	
	/************************************************
	******************** Option *********************
	*************************************************/
	
	class option : public instrument
	{
		public:
			/* constructors and destrucor */
			explicit option(const double vol, const double strike, const struct std::tm mat,  TSH::tsh spot, const double rate, const double div,const std::string& optiontype, const std::string& udl);
			virtual~option();
			
			/* getters */
			struct std::tm option::get_maturity() const;
			double option::get_volatility() const;
			double option::get_strike() const;
			double option::get_spot(struct std::tm day);
			double option::get_interestrate() const;
			
			/* setters */
			void option::set_delta_hedging(std::string hedge_type);
			
			/* computers */
			double option::compute_price(struct std::tm day);
			double option::delta_hedging_pnl(struct std::tm start, struct std::tm end);
			double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end);
			
			double option::get_delta(struct std::tm day);
			double option::get_rf_return(struct std::tm start,struct std::tm end);
			double option::payoff(struct std::tm day);
			v_skew::volatility_skew option::get_volatility_skew(struct std::tm day, std::vector<double> strikes,std::string vol_type);
			std::vector<double>  get_BE_vol(struct std::tm day, std::vector<double> strikes);
			double get_BE_vol(struct std::tm day, double strike);
			
		private:
		
			std::string m_type;
			double m_volatility;
			struct std::tm m_maturity;
			double m_strike;
			TSH::tsh m_spot;
			double m_interestrate;
			std::string m_hedge_type;
	};
	
	
}
#endif