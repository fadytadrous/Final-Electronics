#include<reg51.h>
#include<stdio.h>
sbit ALE=P3^3;  
sbit OE=P3^6; 
sbit START=P3^4; 
sbit EOC=P3^5;  
sbit clk=P3^7;  
sbit ADDA=P3^0;  //Address pins for selecting input channels.
sbit ADDB=P3^1;
sbit ADDC=P3^2;
#define Adc_Data P1  //ADC
 
int result[3],ADC_value;


void timer0() interrupt 1 
{
    clk=~clk;
}



void delay(unsigned int count) 
{
    int i,j;
    for(i=0;i<count;i++)
    for(j=0;j<100;j++);
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
    ADC_value=Adc_data;
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
    EOC=1;
    ALE=0;
    OE=0;
    START=0;
    TMOD=0x02;
    TH0=0xFD;
    IE=0x82;
    TR0=1;
    while(1)
    {
        adc();
        ADC_value=0;
    }
    
}