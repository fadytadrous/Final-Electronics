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

//filtration constants
unsigned char N = 5;
unsigned char last_input = 0;		// initial state flag
unsigned char oldest_input = 0;
unsigned char full = 0;
int z[5]={0};

//target is lowpass 150hz, high pass 1hz,notch filter at 50hz
float coeff[5]= {0.2397,0.02625,0.02704,0.02785,0.02837}; 

void delay(unsigned int count) 
{
    unsigned char i,j;
    for(i=0;i<count;i++)
    for(j=0;j<100;j++);
}

// void Filter(int x)
// {
// 	static const unsigned char N = 5 ;
// 	unsigned char n;
// 	int result=0; 
// 	if(i<N)
// 	{
// 		testarray[i]= x;  
// 		i++;
// 	}
// 	else{
// 		for ( n = 0; n < (N-1); n++)
// 		{
// 			testarray[n] = testarray[n+1]; //shift w 7otkol value fi ely ganbha
// 		}
// 		testarray[N-1] = x;
// 	}
// 	for (n= 0 ; n< N ;  n++)  // idx =1
//     	{ 
//     	//fih hena casting ha y7sl momkn y2sr 3al output                                 
//             result += testarray[n] * coeff[ -n+N-1 ] * 255 ; //bamshy el coeff bmen el a5r lel awl
//     	}		
//     result = (result>>8); // take Higher 8 bits i.e shifting to right , 
// 	DAC_port= result;
// }   

void filter (int x)			// x is the input from adc
{
  unsigned char n;
  int result = 0;


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
   // filter(Adc_Data);  // or ------> filter(Adc_Data); // I think this is a little bit faster , we will save variable assignment operation 
		DAC_port = Adc_Data ;
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
    while(1)
    {
        adc();
    }
    
}