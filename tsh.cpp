#include "tsh.hpp"

namespace BEV
{
	/* Operators to comapre std::tm variables */
	
	bool operator==(const struct std::tm& lhs, const struct std::tm& rhs)
	{
		return std::tie(lhs.tm_year, lhs.tm_mon, lhs.tm_mday) == std::tie(rhs.tm_year, rhs.tm_mon, rhs.tm_mday);
	}
	
	bool operator>( struct std::tm& lhs, struct std::tm& rhs)
	{
		return (time_diff(lhs,rhs)>0);
	}
	
	bool operator<( struct std::tm& lhs, struct std::tm& rhs)
	{
		return (time_diff(lhs,rhs)<0);
	}
	
	bool operator<=( struct std::tm& lhs, struct std::tm& rhs)
	{
		return (time_diff(lhs,rhs)<=0);
	}
	
	/* Method to substract two std:: tm variables */
	double time_diff(struct std::tm& t1, struct std::tm& t2)
	{
		return std::difftime(std::mktime(&t1), std::mktime(&t2))/ 3600 / 24;
	}
	
	namespace TSH
	{
		struct std::tm str_to_tm(std::string date_string)
		{
			struct std::tm date_tm;
			std::istringstream datestream(date_string);
			datestream >> std::get_time(&date_tm, "%d/%m/%Y");
			date_tm.tm_sec = 0;
			date_tm.tm_min = 0;
			date_tm.tm_hour = 0;
			return date_tm;
		}
		
		/************************************************
		************** Time Series Handling *************
		*************************************************/
		
		/* constructors and destructors */
		
		tsh::tsh(std::string udl, std::ifstream& csvf)
		: m_udl(udl)
		{
			std::size_t size = csvf::count_lines(csvf);
			m_dates.resize(size);
			m_data.resize(size);
			read_data(csvf);
		}
		
		tsh::tsh(std::string udl,std::size_t l)
		: m_udl(udl)
		{
			m_dates.resize(l);
			m_data.resize(l);
		}
		
		tsh::tsh(std::string udl)
		: m_udl(udl)
		{
			m_dates.resize(1);
			m_data.resize(1);
		}
		
		/*tsh::tsh(tsh& other_tsh)
		: m_udl(other_tsh.get_udl()),m_dates(other_tsh.get_dates()),m_data(other_tsh.get_data())
		{}*/
		
		tsh::~tsh()
		{
			std::cout << "tsh destructor" <<std::endl;
		}
		
		/* getters */
		
		std::string tsh::get_udl()
		{
			return m_udl;
		}
		
		std::size_t tsh::get_size()
		{
			return m_data.size();
		}
		
		std::vector<double> tsh::get_data()
		{
			return m_data;
		}
		
		std::vector<struct std::tm> tsh::get_dates()
		{
			return m_dates;
		}
		
		struct std::tm tsh::get_date(size_t i)
		{
			return m_dates[i];
		}
		
		int tsh::get_pos(struct std::tm day)
		{
			int pos=-1;
			std::size_t nb_inst = get_size();
			int i=0;
			while ((i< nb_inst) && (pos<0))
			{
				if (m_dates[i]==day)
				{
					pos=i;
				}
				i++;
			}
			return pos;
		}
		
		double tsh::operator[](std::size_t i) const
		{
			if(i<m_data.size())
			{
				return m_data[i];
			}
			else
			{
				std::cout<< "No data in time serie at the index: "<<i<<std::endl;
				return 0;
			}
		}
		
		/*setters*/
		
		void tsh::set_dates(std::vector<struct std::tm> dates)
		{
			m_dates = dates;
		}
		
		void tsh::set_data(std::vector<double> data)
		{
			m_data = data;
		}
	
		void tsh::read_data(std::ifstream& csvf)
		{
			if(!csvf.is_open())
			{
				std::cout << "failed to open file." << std::endl;
			}
			else if (csvf.is_open()) {
				std::string date;
				std::string data;
				std::size_t i = 0;
				while(csvf.good())
				{
					std::getline(csvf, date, ';');
					std::getline(csvf, data, '\n');
					if (!date.empty())
					{
						struct std::tm ss;
						std::istringstream datestream(date);
						datestream >> std::get_time(&ss, "%d/%m/%Y");
						ss.tm_sec = 0;
						ss.tm_min = 0;
						ss.tm_hour = 0;
						m_dates[i] = ss;
						m_data[i] = std::stod(data);
						i++;
					}
				}
				csvf::back_to_start(csvf);
			}
		}
		
		/* to tell if a day is in a time serie*/
		bool tsh::is_in(struct std::tm day)
		{
			bool b=false;
			if(get_pos(day)>0)
			{
				b=true;
			}
			return b;
		}
		
		
	}
	namespace csvf
	{
		/************************************************
		*************** CSV Files Handler ***************
		*************************************************/
		
		std::size_t count_lines(std::ifstream& csvf)
		{
			if(csvf.is_open())
			{
				csvf.unsetf(std::ios_base::skipws);
				std::size_t nbrows = std::count(std::istream_iterator<char>(csvf), std::istream_iterator<char>(),'\n');
				back_to_start(csvf);
				return nbrows;
			}
			else
			{
				std::cout << "File is not open." <<std::endl;
				return 0;
			}
		}
		
		void back_to_start(std::ifstream& csvf)
		{
			if(csvf.is_open())
			{
				csvf.clear();
				csvf.seekg(0, std::ios::beg);
			}
		}
		
	}
}