#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <atomic>
#include <cmath>

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
			std::string instrument::get_udl() const;
			virtual double instrument::get_price() const;
			double instrument::get_dividend_yield() const;
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
			explicit option(const double vol, const double strike, const struct std::tm mat,  TSH::tsh spot, const double rate, const double div,
			const std::string& optiontype, const std::string& udl, const std::string& hedge_type="daily");
			virtual~option();
			
			/* getters */
			std::string option::get_type() const;
			struct std::tm option::get_maturity() const;
			double option::get_volatility() const;
			double option::get_strike() const;
			double option::get_spot(struct std::tm day) const;
			double option::get_interestrate() const;
			TSH::tsh option::get_spot() const;
			
			/* setters */
			void option::set_delta_hedging(std::string hedge_type);
			void option::set_maturity(struct std::tm maturity);
			void option::set_volatility(double vol);
			
			/* computers */
			double option::compute_price(struct std::tm day, double vol);
			double option::compute_price(struct std::tm day);
			
			double option::delta_hedging_pnl(struct std::tm start, struct std::tm end);
			double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end);
			double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end, struct std::tm mat, double vol, double strike);
			double option::daily_delta_hedging_pnl_robust(struct std::tm start,struct std::tm end,struct std::tm mat,double vol,double strike);
			double option::daily_delta_hedging_pnl_robust(struct std::tm start,struct std::tm mat);
			
			double option::get_delta(struct std::tm day,  struct std::tm mat, double vol, double strike);
			double option::get_gamma(struct std::tm day,  struct std::tm mat, double vol, double strike);
			
			double option::get_rf_return(struct std::tm start,struct std::tm end);
			double option::payoff(struct std::tm mat);
			
			v_skew::volatility_skew option::get_volatility_skew(struct std::tm start, std::vector<double> strikes, std::string vol_type);
			v_surface::volatility_surface option::get_volatility_surface(struct std::tm start, std::vector<double> strikes, std::vector<struct std::tm > maturities, std::string vol_type);
			
			std::vector<double>  option::get_BE_vol(struct std::tm start, std::vector<double> strikes, double lb=0,double hb=1, double tol=0.005);
			double option::get_BE_vol(struct std::tm start, double strike, double lb=0.0,double hb=1.0, double tol=0.005);
			std::vector<std::vector<double>>  option::get_BE_vol(struct std::tm start, std::vector <struct std::tm > maturities, std::vector<double> strikes, double lb=0, double hb=1, double tol=0.005);
			double option::get_BE_vol(struct std::tm start, double strike, struct std::tm mat, double lb=0, double hb=1, double tol=0.005);
			
			double option::get_BE_vol_robust(struct std::tm start, double strike, struct std::tm mat, double lb=0, double hb=1, double tol=0.005);
			
			double option::get_realized_vol(struct std::tm start, struct std::tm end);
			
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