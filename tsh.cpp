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
	
	/* Method to substract two std:: tm variables get a result in days */
	double time_diff(struct std::tm& t1, struct std::tm& t2)
	{
		return std::difftime(std::mktime(&t1), std::mktime(&t2))/ 3600 / 24;
	}
	
	/* Method to convert a string date to a tm object */
	struct std::tm str_to_date(const std::string& date_str)
	{
		struct std::tm date;
		
		std::istringstream datestream(date_str);
		datestream >> std::get_time(&date, "%d/%m/%Y");
		
		date.tm_hour=0;
		date.tm_min=0;
		date.tm_sec=0;
		
		return date;
	}
	
	std::string date_to_str(struct std::tm& date)
	{
		std::string str_date= std::to_string(date.tm_mday)+"/"
								+std::to_string(date.tm_mon)+"/"
								+std::to_string(1900+date.tm_year);
		return str_date;
	}
	
	namespace TSH
	{
		
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
			
			std::cout<<"tsh constructor"<<std::endl;
		}
		
		tsh::tsh(std::string udl,std::size_t l)
		: m_udl(udl)
		{
			m_dates.resize(l);
			m_data.resize(l);
			
			std::cout<<"tsh constructor"<<std::endl;
		}
		
		tsh::tsh(std::string udl)
		: m_udl(udl)
		{
			m_dates.resize(1);
			m_data.resize(1);
			
			std::cout<<"tsh constructor"<<std::endl;
		}
		
		tsh::~tsh()
		{
			std::cout << "tsh destructor" <<std::endl;
		}
		
		/* getters */
		
		std::string tsh::get_udl() const
		{
			return m_udl;
		}
		
		std::size_t tsh::get_size() const
		{
			return m_data.size();
		}
		
		const std::vector<double>& tsh::get_data() const
		{
			return m_data;
		}
		
		const std::vector<struct std::tm>& tsh::get_dates() const
		{
			return m_dates;
		}
		
		struct std::tm tsh::get_date(size_t i) const
		{
			return m_dates[i];
		}
		
		size_t tsh::get_pos(struct std::tm day) const
		{
			size_t pos=0;
			std::size_t nb_inst = get_size();
			int i=0;
			while ((i< nb_inst) && (pos<=0))
			{
				if (m_dates[i]==day)
				{
					return i;
				}
				i++;
			}
			std::cout << "TSH: Element not found. First element returned" << std::endl;
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
			if(m_dates[0]==day)
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