

	greek::greek(std::string name)
	:m_name(name)
	{
		stdd::cout<<"greek constructor"<<std::endl;
	}
	
	double greek::get_value()
	{
		if(m_name=="Delta")
		{
			return 1;
		}
		else if (m_name=="Gamma")
		{
			return 2;
		}
	}
