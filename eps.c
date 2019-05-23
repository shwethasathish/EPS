#include<avr/io.h>
#include<util/delay.h>
#define SET_BIT(PORT,PIN) PORT|=(1<<PIN)
#define CLR_BIT(PORT,PIN) PORT&=~(1<<PIN)
struct
{
 volatile unsigned int FLAG_ISR1:1; 
}FLAG_BIT;
int adc1()
{
  ADMUX&=0x00;
  ADMUX|=(1<<REFS0);
  ADCSRA|=(1<<ADEN);
  ADCSRA|=(1<<ADSC);
while(ADCSRA & (1<<ADSC));
  return ADC;
}
int adc2()
{
  ADMUX&=0x00;
  ADMUX|=(1<<MUX0);
  ADMUX|=(1<<REFS0);
  ADCSRA|=(1<<ADEN);
  ADCSRA|=(1<<ADSC);
  ADC=0;
while(ADCSRA & (1<<ADSC));
  return ADC;
}
int adc3()
{
  ADMUX&=0x00;
   ADMUX|=(1<<MUX1);
  ADMUX|=(1<<REFS0);
  ADCSRA|=(1<<ADEN);
  ADCSRA|=(1<<ADSC);
  ADC=0;
while(ADCSRA & (1<<ADSC));
  return ADC;
}
 void extint1()
{
  EICRA|=(1<<ISC10);
    EICRA&=~(1<<ISC11);
    EIMSK|=(1<<INT1);
  sei();
} 
ISR(INT1_vect)
            {
   			cli();
            FLAG_BIT.FLAG_ISR1=!FLAG_BIT.FLAG_ISR1;
            sei(); 
            }
int main()
{
  CLR_BIT(DDRC,PC1);
  CLR_BIT(DDRC,PC2);
  CLR_BIT(DDRC,PC3);
  SET_BIT(DDRD,PD7);//motor
  SET_BIT(DDRB,PB1);//led
  
  uint16_t speed=0;
  uint16_t angle=0;
  uint16_t torque=0;
  extint1();
  while(1)
  {
    if(FLAG_BIT.FLAG_ISR1==1){
    speed= adc1();
   
    angle= adc2();
    
    torque= adc3();
    
    if((speed<=200) && (angle<=300)&& (torque<=100))
    {
      SET_BIT(PORTD,PD7);
      SET_BIT(PORTB,PB1);
      
    }
    else if((speed>200) && (angle>300) && (torque<50))
    {
      SET_BIT(PORTD,PD7);
      CLR_BIT(PORTB,PB1);
      
    }
    else
    {
      SET_BIT(PORTD,PD7);
      CLR_BIT(PORTB,PB1);
    }
    
  }
    else
    {
      CLR_BIT(PORTD,PD7);
      CLR_BIT(PORTB,PB1);
      
    }
  }
}
