#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
	// create the filter structure for 3rd order
	Iir::Butterworth::LowPass<3> f;

	// filter parameters
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 5; // Hz

	// calc the coefficients
	f.setup (samplingrate, cutoff_frequency);
	
	double b = 0;
	double b2 = 0;
	for(int i=0;i<1000000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b2 = b;
		b = f.filter(a);
		assert_print(!isnan(b),"Lowpass output is NAN\n");
		if ((i>20) && (i<100))
			assert_print((b != 0) || (b2 != 0),
				     "Lowpass output is zero\n");
	}
	assert_print(fabs(b) < 1E-25,"Lowpass value for t->inf to high!");

	Iir::Butterworth::BandStop<4,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (samplingrate, center_frequency, frequency_width);
	bs.reset ();
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = bs.filter(a);
		assert_print(!isnan(b),"Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	assert_print(fabs(b) < 1E-25,"Bandstop value for t->inf to high!");


	Iir::Butterworth::BandStop<4> bs2;
	const float norm_center_frequency = 0.1019;
	const float norm_frequency_width = 0.01;	
	bs2.setupN(norm_center_frequency, norm_frequency_width);
	for(int i=0;i<10000;i++) 
	{
		b = fabs(bs2.filter(sin(2*M_PI*norm_center_frequency*i)));
		if (i > 5000) {
			assert_print(b < 1E-5,"Bandstop not removing sine.\n");
		}
	}
	
	return 0;
}
