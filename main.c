
#include "stm32f10x.h"
#include <stdio.h>		
#include <stdint.h>		
#include <string.h>

#define GPIO_PIN_0                 ((uint16_t)0x0001)  
#define GPIO_PIN_1                 ((uint16_t)0x0002)  
#define GPIO_PIN_2                 ((uint16_t)0x0004)  
#define GPIO_PIN_3                 ((uint16_t)0x0008)  
#define GPIO_PIN_4                 ((uint16_t)0x0010)  
#define GPIO_PIN_5                 ((uint16_t)0x0020)  
#define GPIO_PIN_6                 ((uint16_t)0x0040) 
#define GPIO_PIN_7                 ((uint16_t)0x0080) 
#define GPIO_PIN_8                 ((uint16_t)0x0100)  
#define GPIO_PIN_9                 ((uint16_t)0x0200)  
#define GPIO_PIN_10                ((uint16_t)0x0400) 
#define GPIO_PIN_11                ((uint16_t)0x0800)  
#define GPIO_PIN_12                ((uint16_t)0x1000)  
#define GPIO_PIN_13                ((uint16_t)0x2000)  
#define GPIO_PIN_14                ((uint16_t)0x4000)  
#define GPIO_PIN_15                ((uint16_t)0x8000)  
#define GPIO_PIN_All               ((uint16_t)0xFFFF)

#define PIN_CODE_0                 (GPIOA->IDR & GPIO_PIN_0)       
#define PIN_CODE_1                 ((GPIOA->IDR & GPIO_PIN_4)>>3)  
#define PIN_CODE_2                 ((GPIOA->IDR & GPIO_PIN_6)>>4)  
#define PIN_CODE_3                 ((GPIOA->IDR & GPIO_PIN_7)>>4)  
#define PIN_CODE_4                 ((GPIOA->IDR & GPIO_PIN_8)>>4)  
#define PIN_CODE_5                 ((GPIOA->IDR & GPIO_PIN_9)>>4) 
#define PIN_CODE_6                 ((GPIOA->IDR & GPIO_PIN_10)>>4) 
#define PIN_CODE_7                 ((GPIOA->IDR & GPIO_PIN_12)>>5)

uint8_t  value = 0;                                     // переменная хранящая значение счетчика 
int32_t code=0;                                        //Переменная, принимающая значения с тумблеера
uint32_t button = 0;                                  //Количество нажатий кнопки
uint8_t time = 50;                                  // частота счетчика
uint32_t operation =0;
// функцция вывода значения на светодиоды 
void vivod_value (uint8_t time_out);
//  функция обработки значения с тумблера
void load_value (void);  
//функция задержки 
void delay (uint32_t ticks);
// функция включения прерываний
void enter_button(void);
void interrupt_USAT2(void);
void Send_frequency (int32_t* array_ptr, size_t array_size);
int32_t load_value_USART [8] = {0}; //массив, принимающий значение 
uint32_t record_in = 0;

	
int main(void)
{

	RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPCEN; // Включение тактирования для порта C
	   /* Установка регистра CRL битов MODE(0-7)[1:1] на тип выхода скоростью 50 MHz */
    	GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1);
	 	GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);
 		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1);
		GPIOC->CRL = (GPIOC->CRL) | ( GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1);
     

	  /* Установка регистра CRL битов CNF5[0:0] на тип выхода push-pull */
        GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF2_0 | GPIO_CRL_CNF2_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF3_0 | GPIO_CRL_CNF3_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF4_0 | GPIO_CRL_CNF4_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1));
		GPIOC->CRL = (GPIOC->CRL) & ( ~( GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1));
     
RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPAEN;  // Включение тактирования для порта A
    /* Установка регистра CRL битов CNF [1:0] на тип входа  pull-up */
	 GPIOA->CRL &= (~GPIO_CRL_CNF0_0);
  GPIOA->CRL |= GPIO_CRL_CNF0_1;
	GPIOA->CRL &= (~GPIO_CRL_CNF4_0);
  GPIOA->CRL |= GPIO_CRL_CNF4_1;
	GPIOA->CRL &= (~GPIO_CRL_CNF6_0);
  GPIOA->CRL |= GPIO_CRL_CNF6_1;
	GPIOA->CRL &= (~GPIO_CRL_CNF7_0);
  GPIOA->CRL |= GPIO_CRL_CNF7_1;
	GPIOA->CRH &= (~GPIO_CRH_CNF8_0);
  GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->CRH &= (~GPIO_CRH_CNF9_0);
  GPIOA->CRH |= GPIO_CRH_CNF9_1;
	GPIOA->CRH &= (~GPIO_CRH_CNF10_0);
  GPIOA->CRH |= GPIO_CRH_CNF10_1;
	GPIOA->CRH &= (~GPIO_CRH_CNF12_0);
  GPIOA->CRH |= GPIO_CRH_CNF12_1;
	
    RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPBEN;
    	 GPIOB->CRL &= (~GPIO_CRL_CNF4_0);
  GPIOB->CRL |= GPIO_CRL_CNF4_1;
  
  
  // конфигурация UART1
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Включение тактирование USART2
	USART2->CR1 = USART_CR1_UE;                  // разрешаем USART2, сбрасываем остальные биты
	USART2->BRR = 940;                          // скорость 38400 бод
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE ; // разрешаем приемник и передатчик
    USART2->CR2 = 0;
    USART2->CR3 = 0;
	
    interrupt_USAT2();//Включение прерывания по USART
  
	    
    enter_button(); //включение прерываний
			
	while (1)
	{
        
       if (operation==0)
       {
        switch(button)
          {  
        /****кнопка нажата 0 раз ****/
            case 0:  
                vivod_value (time);
              break;
        /****кнопка нажата 1 раз ****/
            case 1: 
                vivod_value (time);
              break;
        /****кнопка нажата 3 раза ****/
            case 3: 
                GPIOC->BRR = 0xFF; value = 0; 
              break;
        /****кнопка нажата 4 раз ****/
            case 4: 
                button = 0; time = code;
              break;
            default:
                break;
        }
           } 
          else{}       
	}						
	}
    /************Функция обрабатывает значение счетчика и выводит его значение на светодиоды************/
void vivod_value (uint8_t time_out)
	 {	 
         GPIOC->BRR = value;
         value= value +1;
         GPIOC->BSRR = value;
		 // GPIOC->BRR = value;
         delay (time_out);
        // GPIOC->BRR = value;
		 if (value == 255)
		 {value = 0; GPIOC->BRR = 0xFF;   }
	}
    /************Функция задержки************/
void delay (uint32_t ticks)
{
	for (uint32_t i = 1; i < (10000000); i=i+1+ticks)
	{
	}
}
void enter_button(void)
{
	/****************************************************************/
	/*Включение прерываний от пина 13*/
	/****************************************************************/
	//Включение тактирования на блок альтернативных функций
RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_AFIOEN;
//Разрешить прерывание 13 пина порта С
AFIO->EXTICR[3] = 0x0020;
AFIO->EXTICR[1] = 0x0001;  
//Разрешить прерывание 13 линии
EXTI-> IMR|=(EXTI_IMR_MR13);
EXTI-> EMR|=(EXTI_EMR_MR13);
    
EXTI-> IMR|=(EXTI_IMR_MR4);
EXTI-> EMR|=(EXTI_EMR_MR4);    

//Прерывание 13 линии по спадающему фронту 
EXTI->RTSR|=(EXTI_RTSR_TR13);
    
EXTI->RTSR|=(EXTI_RTSR_TR4);    
/* Разрешение прерываний */
NVIC->ISER[1] = (uint32_t)(1UL << (((uint32_t)EXTI15_10_IRQn) & 0x1FUL));
NVIC_EnableIRQ(EXTI4_IRQn);
   
}
/*Обработчик прерывания*/
void EXTI15_10_IRQHandler (void)
{

EXTI->PR |= GPIO_PIN_13;
	
button=button+1;
    if (button ==1)
    {
    Send_frequency (load_value_USART , sizeof(load_value_USART ));
    }
}
void EXTI4_IRQHandler (void)
{
EXTI->PR |= GPIO_PIN_4; 
operation=operation+1;  
if (operation%2==0)
{
operation=0;
}  


    
}    
/************Функция считывает значение с тумблера************/
void load_value (void)
	{
        code = 0;		
	    code |= PIN_CODE_0;  
		code |= PIN_CODE_1;
		code |= PIN_CODE_2;
		code |= PIN_CODE_3;
		code |= PIN_CODE_4;
		code |= PIN_CODE_5;
		code |= PIN_CODE_6;
		code |= PIN_CODE_7;
	}
    
    
    
void USART2_IRQHandler (void)
{
	if ((USART2->DR) != 32) //Если значение не равно SPACE то записываем значение в массив 
	{
	if (*(load_value_USART+record_in)!=0)
	{
	record_in++;
	}
	*(load_value_USART+record_in) = USART2->DR;
  }
  else if ((USART2->DR) == 32) //Если значение равно SPACE то обнуляем действующее значение и счетчик значений
	{
		record_in++;
	*(load_value_USART+record_in)= 0;
		record_in=0;
	}
 	while ((USART2->SR & USART_SR_TXE) == 0) {}
	//while ((USART2->SR & USART_SR_TXE) == 0) {}
}
void interrupt_USAT2(void)
{
/* разрешаем прерывание по приему */
USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; 
/*разрешение прерывания USART2 в контроллере прерываний*/
NVIC_EnableIRQ (USART2_IRQn);
}
void Send_frequency (int32_t* array_ptr, size_t array_size)
{
	int32_t v = 0;
	uint32_t i = 0;
	v = (load_value_USART[2]-48)+(load_value_USART[1]-48)*10+(load_value_USART[0]-48)*100;
	if(0xFF>v )
	{
	v=0;
	}
    code=v;
    memset(load_value_USART,0,8);
     
		//	d=0;
}