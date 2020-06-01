#include<reg51.h>
#include<stdio.h>
sbit ALE=P0^4;  
sbit OE=P0^7; 
sbit START=P0^5; 
sbit EOC=P0^6;  

sbit ADDA=P0^0;  //Address pins for selecting input channels.
sbit ADDB=P0^1;
sbit ADDC=P0^2;
#define Adc_Data P2  //ADC
#define DAC_port P3  //DAC
#define state P1
unsigned char x=1;
if (P1 == 1 ){ // notch at 5000hz
float coeff[]={-0.000001, 
-0.000088, 
-0.000433, 
-0.001062, 
-0.001717, 
0.998000, 
-0.001717, 
-0.001062, 
-0.000433, 
-0.000088, 
-0.000001};	
}
if (P1 == 1 ){ // notch at 5000hz
float coeff[]={-0.000001, 
-0.000088, 
-0.000433, 
-0.001062, 
-0.001717, 
0.998000, 
-0.001717, 
-0.001062, 
-0.000433, 
-0.000088, 
-0.000001};	
}
if (P1 == 8 ){ // notch at 5000hz
float coeff[]={-0.000001, 
-0.000088, 
-0.000433, 
-0.001062, 
-0.001717, 
0.998000, 
-0.001717, 
-0.001062, 
-0.000433, 
-0.000088, 
-0.000001};	
}
if (P1 == 16 ){ // notch at 5000hz
float coeff[]={-0.000001, 
-0.000088, 
-0.000433, 
-0.001062, 
-0.001717, 
0.998000, 
-0.001717, 
-0.001062, 
-0.000433, 
-0.000088, 
-0.000001};	
}



 int result;
//filtration constants
unsigned char N = 11;
unsigned char last_input = 0;		// initial state flag
unsigned char oldest_input = 0;
unsigned char full = 0;
int z[11]={0};

//target is lowpass 150hz, high pass 1hz,notch filter at 50hz


void filter (int x)			// x is the input from adc
{
  unsigned char n;
  result = 0;


  z[last_input] = x;
  last_input = (last_input + 1) % N;

  if (last_input == 0)
    {
      full = 1;
    }

  if (full == 0)
    {
      for (n = 0; n < last_input; n++)	// idx =1
		{
	  	result += z[n] * coeff[-n+N-1] * 255;
		}
    }

  else
    {
      //  oldest_input = (last_input + 1) % N;
      for (n = 0; n < N; n++)	// idx =1
		{
       	result += z[(oldest_input + n) % N] * coeff[-n+N-1] * 255;  
	  // result = z[(oldest_input + n) % N];	// for just debugging
	  // printf ("current input is  %d  ", x);
	  // printf ("%d  %d \t", n, result);
	  // printf ("\n");
		}
      oldest_input = (last_input + 1) % N;

    }

  	result = (result>>8); // take Higher 8 bits i.e shifting to right , 
	DAC_port= result;
}

void read_adc() //Function to drive ADC
{
    ALE=1;
    START=1;
    ALE=0;
    START=0;
    while(EOC==1);
    while(EOC==0);
    OE=1;
 	filter(Adc_Data);
    OE=0;
}
void adc() 
{
    ADDC=0; // Selecting input channel IN0 using address lines
    ADDB=0;
    ADDA=0;
    read_adc();
}


void main()
{	
	Adc_Data = 0xFF ;
    EOC=1;
    ALE=0;
    OE=0;
    START=0;
    IE=0x82;
    TR0=1;
    while(1)
    {
        adc();
    }
    
}