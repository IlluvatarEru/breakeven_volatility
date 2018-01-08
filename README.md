# project_cpp
# Break Even Volatility

The goal of this project is to find the break even volatility that cancels the PnL of a delta hedged option.
From this break even volatility we produce volatility skews and surfaces.

Another method is to find the break even volatility that cancels the Robust formula P&L. 

#Future Work

##Getting the right BEV
The first thing to do would be to make the break-even volatility method more robust, especially to high and low strikes for which we cannot get a consistent volatility.

##Graphic representation
Graphic representation is very often an important and useful feature as it allows to see directly from the plot the different implications for the trader. For this project, I outputted the surface and the skew in csv files and used excel to get the graphs. However it would be interesting to automatically design the surface and the skew (using gnuplot for example).

##Portfolio class
I started to build a class portfolio which would allow to hold multiple instruments in various quantities, each instrument being hold through an instrumentholder that define the instrument, its hold quantities over time and its price over time using tsh objects.
Then we could compute the different hedging strategies for a portfolio holding multiple assets, and compute the break-even volatility for each option. This would be useful to get a more realistic view of a portfolio and its hedging etc. See my work in the repo old/

##Other pricing methods
In this project I used the Black Scholes formula to compute the price of the option, but we could easily imagine adding other pricing methods (Heston, SABR etc). This would give the user more flexibility in terms of pricing. I would also add methods that take into account dividends, which I did not do here.

##Backtesting framework
Finally, we could extend the tool for backtesting purposes. Creating a class backtesting that would hold a portfolio built by the user during a given period, using a given hedging strategy and adding other fine-tuning features could make this tool more powerfull. Moreover we could add different performance measures (Sharpe ratio, max drawdown etc) to really allow the user to see the complete results of his backtesting portfolio.
