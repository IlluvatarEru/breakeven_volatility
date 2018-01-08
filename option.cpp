#include "tsh.hpp"
#include "volatility.hpp"
#include "option.hpp"

namespace BEV{
	
	
	/************************************************
	****************** Instrument	*****************
	*************************************************/
	/* operator to compare if two instruments are the same */
	bool operator ==(const instrument inst1, const instrument inst2)
	{
		return (inst1.GetUid()== inst2.GetUid());
	}
	
	int instrument::newUID=0;
	
	/* constructors and destrucor */
	instrument::instrument(std::string udl, double div=0)
	: m_udl(udl), m_dividendyield(div), uid(newUID++)
	{
		std::cout<<"instrument constructor"<<std::endl;
	}

	instrument::~instrument()
	{
		std::cout << "instrument destructor" << std::endl;
	}
	
	/* getters */
	std::string instrument::get_udl() const
	{
		return m_udl;
	}
	
	double instrument::get_price() const
	{
		return m_price;
	}
	
	double instrument::get_dividend_yield() const
	{
		return m_dividendyield;
	}
	
	int instrument::GetUid() const
	{
		return uid;
	}
	
	/* setters*/
	void instrument::set_price(double price)
	{
		m_price = price;
	}
	
	/* computers */
	double instrument::compute_price() const
	{
		return m_price;
	}
	
	/************************************************
	********************* Stock *********************
	*************************************************/
	
	/* constructors and destrucor */
	stock::stock(std::string udl, double div)
	: instrument(udl,div)
	{
		std::cout<<"stock constructor"<<std::endl;
	}
	
	stock::~stock()
	{
        std::cout << "stock destructor" << std::endl;
	}
	
	/************************************************
	******************** Option *********************
	*************************************************/
	
	/* constructors and destrucor */
	option::option(const double vol, const double strike, const struct std::tm mat, TSH::tsh spot, const double rate, 
	const double div, const std::string& optiontype, const std::string& udl, const std::string& hedge_type)
	: instrument(udl,div), m_volatility(vol), m_strike(strike), m_maturity(mat), m_spot(spot), m_interestrate(rate),  m_type(optiontype), m_hedge_type(hedge_type)
	{
		std::cout<<"option constructor"<<std::endl;
	}
	
	option::~option()
	{
        std::cout << "option destructor" << std::endl;
    }
	
	/* getters */
	std::string option::get_type() const
	{
		return m_type;
	}
	
	struct std::tm option::get_maturity() const 
    {
        return m_maturity;
    }
	
	double option::get_volatility() const 
    {
        return m_volatility;
    }
	
	double option::get_strike() const 
    {
        return m_strike;
    }
	
	double option::get_spot(struct std::tm day) const
	{
		return m_spot[m_spot.get_pos(day)];
	}
	
	double option::get_interestrate() const 
    {
        return m_interestrate;
    }
	TSH::tsh option::get_spot() const
	{
		return m_spot;
	}
	
	/* setters */
	void option::set_volatility(double vol)
	{
		m_volatility = vol;
	}
	
	void option::set_maturity(struct std::tm maturity)
	{
		m_maturity = maturity;
	}
	
	void option::set_delta_hedging(std::string hedge_type)
	{
		std::vector<std::string> mylist{"unhedged", "daily"};
		if (std::find(std::begin(mylist), std::end(mylist), hedge_type) != std::end(mylist))
		{	
			m_hedge_type = hedge_type;
		}
		else
		{
			std::cout << "Type of hedge not recognized" << std::endl;
			m_hedge_type = "unhedged";
		}
	}
	void option::set_strike(double strike)
	{
		m_strike = strike;
	}
	
	/* computers */
	double option::compute_price(struct std::tm day)
	{
		double vol = get_volatility();
		return compute_price(day,vol);
	}
	
	double option::compute_price(struct std::tm day, double vol)
	{
		double spot = get_spot(day);
		double TimeToMaturity = time_diff(m_maturity, day)/365;
		double d1 = (log(spot/m_strike) + (m_interestrate + 0.5*vol*vol)*TimeToMaturity)/(vol*pow(TimeToMaturity,0.5));
		double d2 = d1-vol*pow(TimeToMaturity,0.5);
		double sign =0;
		if (m_type=="Call"){
			sign = 1;}
		else if (m_type=="Put"){
			sign = -1;
		}
		double price = sign * spot*norm_cdf(sign*d1) - sign * exp(- m_interestrate* TimeToMaturity) * m_strike*norm_cdf(sign*d2);
		return price;
	}
	
	double option::delta_hedging_pnl(struct std::tm start, struct std::tm end)
	{
		if(m_spot.is_in(start) && m_spot.is_in(end) && (start<end) && (end<=m_maturity))
		{
			if(m_hedge_type=="daily")
			{
				return daily_delta_hedging_pnl(start,end);
			}
			else
			{
				std::cout << "Type of hedge is invalid" << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "Invalid dates" << std::endl;
			return 0;
		}
	}
	
	double option::get_rf_return(struct std::tm start,struct std::tm end)
	{
		return std::exp(m_interestrate * time_diff(end, start)/365);
	}
	
	double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end)
	{
		double vol = get_volatility();
		double strike = get_strike();
		struct std::tm mat = get_maturity();
		
		return daily_delta_hedging_pnl(start,end,mat,vol,strike);
	}
	
	double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end, struct std::tm mat, double vol, double strike)
	{	
		double TmT = time_diff(mat,start)/365;
		set_maturity(mat);
		double pnl = compute_price(start,vol);
		double pos_in_stock = get_delta(start,mat,vol,strike);
		double pos_in_rf = pnl - get_spot(start) * pos_in_stock;
		std::vector<struct std::tm> dates = m_spot.get_dates();
		
		
		for(size_t i=m_spot.get_pos(start)+1;i<m_spot.get_pos(end)+1;++i)
		{
			TmT = time_diff(mat,dates[i])/365;
			
			pnl+= pos_in_stock * (m_spot[i]-m_spot[i-1]) + pos_in_rf * (get_rf_return(dates[i-1],dates[i])-1);
			
			if(TmT!=0)
				pos_in_stock = get_delta(dates[i],mat,vol,strike);

			pos_in_rf = pnl- pos_in_stock * m_spot[i];
		}
		
		return pnl - payoff(mat,strike);
	}
	
	double option::get_gamma(struct std::tm day,  struct std::tm mat, double vol, double strike)
	{
		if(m_spot.is_in(day))
		{
			double spot = m_spot[m_spot.get_pos(day)];
			double TimeToMaturity = time_diff(mat, day)/365;
			double d1 = ((log(spot/strike) + (m_interestrate + 0.5*vol*vol)*TimeToMaturity))/(vol*pow(TimeToMaturity,0.5));
			
			return norm_pdf(d1) / (spot*vol*pow(TimeToMaturity,0.5));
		}
		else
		{
			std::cout << "Invalid date" << std::endl;
			return 0;
		}
	}
	double option::get_delta(struct std::tm day, struct std::tm mat, double vol, double strike)
	{
		if(m_spot.is_in(day))
		{
			
			double TimeToMaturity = time_diff(mat, day)/365;
			double spot = m_spot[m_spot.get_pos(day)];
			double d1 = ((log(spot/strike) + (m_interestrate + 0.5*vol*vol)*TimeToMaturity))/(vol*pow(TimeToMaturity,0.5));
			if(m_type=="Call")
			{
				return norm_cdf(d1);
			}
			else if(m_type=="Put")
			{
				return  norm_cdf(d1)-1;
			}
			else
			{
				std::cout << "invalid option type" << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "Invalid date" << std::endl;
			return 0;
		}
	}
	
	double option::payoff(struct std::tm mat, double strike)
	{
		if(m_type=="Call")
		{
			return std::max(0.0, m_spot[m_spot.get_pos(mat)]-strike);
		}
		else if(m_type=="Put")
		{
			return std::max(0.0, strike-m_spot[m_spot.get_pos(mat)]); 
		}
		else
		{
			std::cout << "Option type is invalid" <<std::endl;
			return 0;
		}
		
	}
	
	v_skew::volatility_skew option::get_volatility_skew(struct std::tm start, std::vector<double> strikes, std::string vol_type)
	{
		if(vol_type=="BE_vol")
		{
			std::vector<double> BE_vol = get_BE_vol(start, strikes);
			return v_skew::volatility_skew(time_diff(m_maturity,start), strikes, BE_vol);
		}
		else
		{
			std::vector<double> constant_vol(strikes.size(), m_volatility);
			return v_skew::volatility_skew(time_diff(m_maturity,start), strikes,constant_vol);
		}
	}
	
	v_surface::volatility_surface option::get_volatility_surface(struct std::tm start, std::vector<double> strikes, std::vector<struct std::tm > maturities, std::string vol_type)
	{
		std::vector<double> mat;
		std::vector<struct std::tm> valid_mat;
		for(size_t i=0; i< maturities.size(); ++i)
		{
			if(m_spot.is_in(maturities[i]))
			{
				mat.push_back(time_diff(maturities[i],start)/365);
				valid_mat.push_back(maturities[i]);
			}
			else
			{
				std::cout << "Invalid Maturity" << std::endl;
			}
		}
		
		if(vol_type=="BE_vol")
		{
			std::vector<std::vector<double>> BE_vol = get_BE_vol(start, valid_mat, strikes);
			return v_surface::volatility_surface(mat, strikes, BE_vol);
		}
		else
		{
			std::vector<std::vector<double>> constant_vol(strikes.size(), std::vector<double>(valid_mat.size(),m_volatility));
			return v_surface::volatility_surface(mat, strikes,constant_vol);
		}	
	}
	
	std::vector<std::vector<double>>  option::get_BE_vol(struct std::tm start, std::vector <struct std::tm > maturities, std::vector<double> strikes, double lb, double hb, double tol)
	{
		std::vector<std::vector<double>> vol(strikes.size(), std::vector<double>(maturities.size(),0));
		for(size_t j=0;j<maturities.size();++j)
		{
			for(size_t i=0;i<strikes.size();++i)
			{
				vol[i][j] = get_BE_vol(start,strikes[i],maturities[j],lb,hb,tol);
				//std::cout << "strike " << strikes[i] << ", Maturity " << date_to_str(maturities[j]) << ", Vol " << vol[i][j] <<std::endl;
			}
		}
		return vol;
	}
	
	std::vector<double>  option::get_BE_vol(struct std::tm start, std::vector<double> strikes, double lb, double hb, double tol)
	{
		std::vector<double> vol(strikes.size(),0);
		for(size_t i=0;i<strikes.size();++i)
		{
			set_strike(strikes[i]);
			vol[i] = get_BE_vol(start,strikes[i],lb,hb,tol);
		}
		return vol;
	}
	
	double option::get_BE_vol(struct std::tm start, double strike, struct std::tm mat, double lb, double hb, double tol)
	{
		if(m_spot.is_in(start) && m_spot.is_in(mat) && (start<mat))
		{
			struct std::tm end =m_spot.get_date(m_spot.get_pos(mat));
			double vol = (hb+lb)/2;
			double pnl = daily_delta_hedging_pnl(start,end,mat,vol,strike);
			
			int i=0;
			
			double pnl_lb = daily_delta_hedging_pnl(start,end,mat,lb,strike);
			double pnl_hb = daily_delta_hedging_pnl(start,end,mat,hb,strike);
			
			if(pnl_lb*pnl_hb>0)
			{
					std::cout<< "Error on lower and upper bounds vol P&Ls for dichotomy method." << std::endl;
					return vol;
			}
			
			while (std::abs(pnl) > tol)
			{
				if(pnl <0)
				{
					lb = vol;
				}
				else
				{
					hb = vol;
				}
				vol = (hb+lb)/2;
				pnl = daily_delta_hedging_pnl(start,end,mat,vol,strike);
				i++;
				if(i > 50)
				{
					return vol;
				}
			}
		
			//std::cout << "Final PnL is " << pnl <<std::endl;
			//std::cout << "Final vol is " << vol <<std::endl;
			
			return vol;
		}
		else
		{
			std::cout<< "Invalid dates" << std::endl;
			return get_volatility();
		}
	}
	
	double option::get_BE_vol(struct std::tm start, double strike, double lb, double hb, double tol)
	{
		struct std::tm mat = get_maturity();
		return get_BE_vol(start, strike, mat,lb,hb,tol);
	}
	
	double option::daily_delta_hedging_pnl_robust(struct std::tm start,struct std::tm mat)
	{
		double strike = get_strike();
		double vol = get_volatility();
		return daily_delta_hedging_pnl_robust(start,mat,mat, vol,strike);

	}
	double option::daily_delta_hedging_pnl_robust(struct std::tm start,struct std::tm end,struct std::tm mat,double vol,double strike)
	{
		double pnl = 0;
		double TmT;
		double dt;
		std::vector<struct std::tm> dates = m_spot.get_dates();
		double spot;
		double r_vol;
		double gamma = get_gamma(start,mat,vol,strike);
		for(size_t i=m_spot.get_pos(start)+1;i<m_spot.get_pos(end);++i)
		{
			TmT = time_diff(mat,dates[i])/365;
			dt = time_diff(dates[i],dates[i-1])/365;
			spot = m_spot[i-1];
			r_vol= get_realized_vol(dates[i-1],dates[i]);
			pnl+=  gamma * pow(spot,2) * (pow(r_vol,2) - pow(vol,2) * dt);
			
			if(TmT != 0)
			{
				gamma = get_gamma(dates[i],mat,vol,strike);
			}
		}
		
		return 0.5*pnl;
	}
	
	double option::get_realized_vol(struct std::tm d1, struct std::tm d2)
	{
		return (m_spot[m_spot.get_pos(d1)]-m_spot[m_spot.get_pos(d2)])/m_spot[m_spot.get_pos(d2)];
	}
	
	double option::get_BE_vol_robust(struct std::tm start, double strike, struct std::tm mat, double lb, double hb, double tol)
	{
		double vol = (hb+lb)/2;
		double pnl = daily_delta_hedging_pnl_robust(start,mat,mat,vol,strike);
		int i=0;
		
		double pnl_lb = daily_delta_hedging_pnl_robust(start,mat,mat,lb,strike);
		double pnl_hb = daily_delta_hedging_pnl_robust(start,mat,mat,hb,strike);
		
		if(pnl_lb*pnl_hb>0)
		{
				std::cout<< "Error on lower and upper bounds for dichotomy method." << std::endl;
		}
		
		while (std::abs(pnl) > tol)
		{
			if(pnl >0)
			{
				lb = vol;
			}
			else
			{
				hb = vol;
			}
			vol = (hb+lb)/2;
			pnl = daily_delta_hedging_pnl_robust(start,mat,mat,vol,strike);
			i++;
			if(i > 20)
			{
				return vol;
			}
		}
		
		//std::cout << "Final PnL is " << pnl <<std::endl;
		//std::cout << "Final vol is " << vol <<std::endl;
		
		return vol;
	}
	
	/* Normal distribution methods */
	
	const double pi = 3.14159265358979323846;
	
	double norm_pdf(const double& x)
	{
		return (1.0/(pow(2*pi,0.5)))*exp(-0.5*x*x);
	}

	double norm_cdf(const double& x)
	{
		double k = 1.0/(1.0 + 0.2316419*x);
		double k_sum = k*(0.319381530 + k*(-0.356563782 + k*(1.781477937 + k*(-1.821255978 + 1.330274429*k))));

		if (x >= 0.0) {
			return (1.0 - (1.0/(pow(2*pi,0.5)))*exp(-0.5*x*x) * k_sum);
		} else {
			return 1.0 - norm_cdf(-x);
		}
	}

	
}