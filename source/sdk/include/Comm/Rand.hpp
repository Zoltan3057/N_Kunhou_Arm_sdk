#ifndef _RAND_WANGHONGTAO_20151024_
#define _RAND_WANGHONGTAO_20151024_

#include <boost/random.hpp>


class MyRand{

public:

	MyRand(){};
	~MyRand(){};

	int randInt(int min,int max)
	{
		boost::uniform_int<> real(min, max);
		return real(gen);
	};

	double randFloat(double min,double max)
	{
		boost::uniform_real<double> real(min, max);
		return real(gen);
	};
	double randND(double mid,double dis){
		boost::normal_distribution<> nd(mid,dis);
		return nd(gen);
	};

	double sampleNormal() {
		double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
		double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
		double r = u * u + v * v;
		if (r == 0 || r > 1){ 
			return sampleNormal();
		}
		double c = sqrt(-2 * log(r) / r);
		return u * c;
	}; 



private:
	boost::random::mt19937 gen;

};

class TimeRand{

public:

	TimeRand(){};
	~TimeRand(){};

	int randInt(int min,int max,boost::random::mt19937 &gen)
	{
		boost::uniform_int<> real(min, max);
		return real(gen);
	};

	double randFloat(double min,double max,boost::random::mt19937 &gen)
	{
		boost::uniform_real<double> real(min, max);
		return real(gen);
	};

	double randND(double mid,double dis,boost::random::mt19937 &gen){
		boost::normal_distribution<> nd(mid,dis);
		return nd(gen);
	};


};
#if defined(_WINDOWS)
#else
// Draw randomly from a zero-mean Gaussian distribution, with standard
// deviation sigma.
// We use the polar form of the Box-Muller transformation, explained here:
//   http://www.taygeta.com/random/gaussian.html
class BoxMuller{
	static double ran_gaussian(double sigma)
	{
		double x1, x2, w, r;

		do
		{
			do { r = drand48(); } while (r==0.0);
			x1 = 2.0 * r - 1.0;
			do { r = drand48(); } while (r==0.0);
			x2 = 2.0 * r - 1.0;
			w = x1*x1 + x2*x2;
		} while(w > 1.0 || w==0.0);

		return(sigma * x2 * sqrt(-2.0*log(w)/w));
	};
}; 
#endif

#endif//_RAND_WANGHONGTAO_20151024_
