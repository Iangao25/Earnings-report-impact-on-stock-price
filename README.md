# Financial Computing

# Goal
We see surprises in company's quater earning reports. Sometimes companies are doing better than they expected and sometimes they miss the expectation. The release of earning report can be considered as one sentiment signal, therefore it could cause flucutation in stock prices.

# Methodology: Bootstrapping
We divide the suprise factors into three groups based on whether the EPS beats Wall Street's predictions: meet(-5% to 5%), beat (over +5%), and miss (below -5%). We use 453 stocks to form our stock pool. After we divide them into 3 big groups by their earning surprises, we randomly select 40 stocks with replacement and calculate their Average Abnormal Return (AAR) and Cumulative Average Abnormal Return (CAAR), for N resample times, and finally take the average.

# Architechture
We have three subsystem: a DataHandle (process all data extracted from Yahoo Finance and companies' constituents information, like the date of releasing earnings report, from Bloomberg), Core (STL design, sample data and store calculation), ExcelDriver (connect to Excel and display all calculation and plotting result).

I am reponsible for the core part.
