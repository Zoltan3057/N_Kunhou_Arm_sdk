//Cordic in 32 bit signed fixed point math
//Function is valid for arguments in range -pi/2 -- pi/2
//for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
//
// 1.0 = 1073741824
// 1/k = 0.6072529350088812561694
// pi = 3.1415926535897932384626
//Constants

#ifndef _CORDIC_KYOSHO_20160327_
#define _CORDIC_KYOSHO_20160327_


#define cordic_1K 0x26DD3B6A
#define half_pi 0x6487ED51
#define MUL 1073741824.000000
#define CORDIC_NTAB 32

class Fast_Sin_Cos{
public:
	Fast_Sin_Cos();
	~Fast_Sin_Cos();

	static void cordic(int theta, int *s, int *c, int n);
};


// void test{
// 	double p;
// 	int s,c;
// 	int i;    
// 	for(i=0;i<50;i++)
// 	{
// 		p = (i/50.0)*M_PI/2;        
// 		//use 32 iterations
// 		cordic((p*MUL), &s, &c, 32);
// 		//these values should be nearly equal
// 		printf("%f : %f\n", s/MUL, sin(p));
// 	}   
// };
#endif//_CORDIC_KYOSHO_20160327_
