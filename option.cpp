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
	double instrument::get_price() const
	{
		return m_price;
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
	const double div, const std::string& optiontype, const std::string& udl)
	: instrument(udl,div), m_volatility(vol), m_strike(strike), m_maturity(mat), m_spot(spot), m_interestrate(rate),  m_type(optiontype)
	{
		std::cout<<"option constructor"<<std::endl;
	}
	
	option::~option()
	{
        std::cout << "option destructor" << std::endl;
    }
	
	/* getters */
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
	
	double option::get_spot(struct std::tm day)
	{
		return m_spot[m_spot.get_pos(day)];
	}
	
	double option::get_interestrate() const 
    {
        return m_interestrate;
    }
	TSH::tsh option::get_spot()
	{
		return m_spot;
	}
	
	/* setters */
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
	
	/* computers */
	double option::compute_price(struct std::tm day)
	{
		double spot = get_spot(day);
		double TimeToMaturity = time_diff(m_maturity, day);
		double d1 = (log(spot/m_strike) + (m_interestrate + 0.5*m_volatility*m_volatility)*TimeToMaturity)/(m_volatility*pow(TimeToMaturity,0.5));
		double d2 = d1-m_volatility*pow(TimeToMaturity,0.5);
		double sign =0;
		if (m_type=="Call"){
			sign = 1;}
		else if (m_type=="Put"){
			sign = -1;
		}
		return sign * spot*norm_cdf(sign*d1) - sign * exp(- m_interestrate* TimeToMaturity) * m_strike*norm_cdf(sign*d2);
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
		return std::exp(m_interestrate * time_diff(end, start));
	}
	
	double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end)
	{
		double pnl = -compute_price(start);
		double vol = get_volatility();
		double strike = get_strike();
		for(size_t i=m_spot.get_pos(start);i<m_spot.get_pos(end)-1;++i)
		{
			struct std::tm day = m_spot.get_date(i-1);
			pnl-=get_delta(day, vol, strike)*(m_spot[i]-m_spot[i-1]);
		}
		return pnl + payoff(end);
	}
	/*{
		double vol = get_volatility();
		double strike = get_strike();
		struct std::tm day = start;
		struct std::tm prev_day;
		int end_pos = m_spot.get_pos(end);
		int start_pos = m_spot.get_pos(start);
		double pf_value = compute_price(day);
		double delta = get_delta(day,vol,strike);
		double risk_free = pf_value - delta * m_spot[m_spot.get_pos(day)];
		
		for(size_t i=start_pos;i<end_pos-1;++i)
		{
			prev_day = day;
			day = m_spot.get_date(i);
			pf_value += delta * (m_spot[i]-m_spot[m_spot.get_pos(prev_day)]) 
				+ risk_free * (get_rf_return(prev_day,day)-1);
			double delta = get_delta(day,vol,strike);
			risk_free = pf_value - delta * m_spot[i];
		}
		return pf_value - payoff(day);
	}
*/	
	double option::daily_delta_hedging_pnl(struct std::tm start, struct std::tm end, double vol, double strike)
	{
		double pnl = -compute_price(start);
		
		for(size_t i=m_spot.get_pos(start);i<m_spot.get_pos(end)-1;++i)
		{
			struct std::tm day = m_spot.get_date(i-1);
			double delta = get_delta(day, vol, strike);
			pnl-=delta*(m_spot[i]-m_spot[i-1])
				+(compute_price(day)-delta*m_spot[i])*(get_rf_return(prev_day,day)-1);
		}
		return pnl + payoff(end);
	}
	
	double option::get_delta(struct std::tm day,double vol, double strike)
	{
		if(m_spot.is_in(day))
		{
			double TimeToMaturity = time_diff(m_maturity, day);
			double spot = m_spot.get_pos(day);
			double d1 = (log(spot/strike) + (m_interestrate + 0.5*vol*vol)*TimeToMaturity)/(vol*pow(TimeToMaturity,0.5));
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
	
	double option::payoff(struct std::tm day)
	{
		if(m_type=="Call")
		{
			return std::max(0.0, m_spot[m_spot.get_pos(day)]-m_strike);
		}
		else if(m_type=="Put")
		{
			return std::max(0.0, m_strike-m_spot[m_spot.get_pos(day)]); 
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
			return v_skew::volatility_skew(time_diff(m_maturity,start), strikes,BE_vol);
		}
		else
		{
			std::vector<double> constant_vol(strikes.size(), m_volatility);
			return v_skew::volatility_skew(time_diff(m_maturity,start), strikes,constant_vol);
		}
	}
	std::vector<double>  option::get_BE_vol(struct std::tm start, std::vector<double> strikes, double lb, double hb, double tol)
	{
		std::vector<double> vol(strikes.size(),0);
		for(int i=0;i<strikes.size();++i)
		{
			vol[i] = get_BE_vol(start,m_maturity,strikes[i]);
		}
		return vol;
	}
	
	double option::get_BE_vol(struct std::tm start, double strike, double lb, double hb, double tol)
	{
		double vol = get_volatility();
		double pnl = daily_delta_hedging_pnl(start,m_maturity,vol,strike);
		while (std::abs(pnl) >tol)
		{
			std::cout << "Pnl is " <<pnl << std::endl;
			if(pnl >0)
			{
				vol = (vol + lb)/2;
			}
			else
			{
				vol = (vol + hb)/2;
			}
			pnl = daily_delta_hedging_pnl(start,m_maturity,vol,strike);
		}
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