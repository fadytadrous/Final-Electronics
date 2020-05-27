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

   
void filter(int n)
{
	if (i<n)
	{
		//multiply by coefficeint and store it in result 
		result+=255*coeff[i]*ADC_value;
		i++;
	}
	else
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