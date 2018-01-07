#ifndef TSH_HPP
#define TSH_HPP

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

namespace BEV
{
	/* Method to substract two std:: tm variables */
	double time_diff(struct std::tm& t1, struct std::tm& t2);
	
	/* Operators to comapre std::tm variables */
	bool operator==(const struct std::tm& lhs, const struct std::tm& rhs);
	bool operator>( struct std::tm& lhs, struct std::tm& rhs);
	bool operator<( struct std::tm& lhs, struct std::tm& rhs);
	bool operator<=( struct std::tm& lhs, struct std::tm& rhs);
	
	namespace TSH
	{
		struct std::tm str_to_tm(std::string date_string);
		
		/************************************************
		************** Time Series Handler **************
		*************************************************/
		
		/* This class is meant to handle time series data */
		class tsh
		{
			public:
				
				/* constructors and destructors */
				explicit tsh(std::string udl, std::ifstream& csvf);
				explicit tsh::tsh(std::string udl,std::size_t l);
				explicit tsh::tsh(std::string udl);
				//explicit tsh::tsh(tsh& other_tsh);
				virtual tsh::~tsh();
				
				/* getters */
				std::string tsh::get_udl();
				std::size_t tsh::get_size();
				int tsh::get_pos(struct std::tm day);
				std::vector<double> tsh::get_data();
				std::vector<struct std::tm> tsh::get_dates();
				struct std::tm tsh::get_date(size_t i);
				double tsh::operator[](std::size_t i) const;
				
				/* setters */
				void tsh::set_dates(std::vector<struct std::tm> dates);
				void tsh::set_data(std::vector<double> data);
				void tsh::read_data(std::ifstream& csvf);
				
				bool tsh::is_in(struct std::tm day);
				
			private:
			
				std::string m_udl;
				std::vector<struct std::tm> m_dates;
				std::vector<double> m_data;
				
		};
	}
	
	namespace csvf
	{
		/************************************************
		*************** CSV Files Handling **************
		*************************************************/
		
		std::size_t count_lines(std::ifstream& csvf);
		void back_to_start(std::ifstream& csvf);
	}
}

#endif