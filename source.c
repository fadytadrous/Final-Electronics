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

int result, ADC_value;
int i =0;
//target is lowpass 150hz, high pass 1hz,notch filter at 50hz
float coeff[5]= {0.2397,0.02625,0.02704,0.02785,0.02837}; 
void delay(unsigned int count) 
{
    int i,j;
    for(i=0;i<count;i++)
    for(j=0;j<100;j++);
}

   
// void filter(int n)
// {
// 	if (i<n)
// 	{
// 		//multiply by coefficeint and store it in result 
// 		result+=255*coeff[i]*ADC_value;
// 		i++;
// 	}
// 	else
// 		DAC_port= result;
// }

   
void filter(int x)  // x is the input from adc
{   static const in N = 5 ;
    static int z[N] ; // static 3lshan b3d m y5ls elfunction de w yrg3lha tany yrg3 yla2y el array fe a5r values
    static int last_input =-1 ; // initial state flag
    int oldest_input = 0 ;

    int n ;
    int result = 0 ;
    int oldest_input = 0 ;


    if (last_input == -1 ) {
        for (n=0 ; n<M ;n++)
            z[n] = 0 ;
        last_input = 0 ; //reset 
        
    }
    oldest_input = (last_input+N) % N +1 ;
    z[last_input] = x ; 
    last_input =  (last_input +1) % N ; // circular buffer pointer ----> last einputut entry location
    for (n= oldest_input ; n< last_input ;  n++)  // idx =1
    {                                   
        result += z[n] * coeff[ abs( n-(N-1) ) ] * 255 ;
    }
	
	DAC_port= result;
}


void read_adc() //Function to drive ADC
{
    ADC_value=0;
    ALE=1;
    START=1;
    delay(1);
    ALE=0;
    START=0;
    while(EOC==1);
    while(EOC==0);
    OE=1;
    ADC_value = Adc_Data;
   // DAC_port = ADC_value; 
    filter(5);
    delay(1);
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
        ADC_value=0;
    }
    
}