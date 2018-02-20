#define   SETBIT(reg, bit)          reg |= (1<<bit)            
#define   CLEARBIT(reg, bit)        reg &= (~(1<<bit))
#define   INVBIT(reg, bit)          reg ^= (1<<bit)
#define   BITISSET(reg, bit)      ((reg & (1<<bit)) != 0)
#define   BITISCLEAR(reg, bit)    ((reg & (1<<bit)) == 0)

// выход ШИма под двигатель 1 канал (3 вывод ардуино)
#define OUT1_L 		 3
#define OUT1_L_PORT	 PORTD
#define OUT1_L_DDR	 DDRD
#define OUT1_L_PIN 	 3

// выход ШИма под двигатель 1 канал (11 вывод ардуино)
#define OUT1_R		 11
#define OUT1_R_PORT	 PORTB
#define OUT1_R_DDR	 DDRB
#define OUT1_R_PIN 	 3



// выход ШИма под двигатель 2 канал (9 вывод ардуино)
#define OUT2_L 		 9
#define OUT2_L_PORT	 PORTB
#define OUT2_L_DDR	 DDRB
#define OUT2_L_PIN 	 1

// выход ШИма под двигатель 2 канал (10 вывод ардуино)
#define OUT2_R		 10
#define OUT2_R_PORT	 PORTB
#define OUT2_R_DDR	 DDRB
#define OUT2_R_PIN 	 2

// выход ШИма под вольтметр 1 канал (4 вывод ардуино)
#define OUT1_VM_PORT PORTD
#define OUT1_VM_DDR DDRD
#define OUT1_VM_PIN 4		

// выход ШИма под вольтметр 2 канал (2 вывод ардуино)
#define OUT2_VM_PORT PORTD
#define OUT2_VM_DDR DDRD
#define OUT2_VM_PIN 2		

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5) (19)
//      (D 0) PD0  2|    |27  PC4 (AI 4) (18)
//      (D 1) PD1  3|    |26  PC3 (AI 3) (17)
//      (D 2) PD2  4|    |25  PC2 (AI 2) (16)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1) (15)
//      (D 4) PD4  6|    |23  PC0 (AI 0) (14)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

#define POT1_1 	A6
#define POT2_2 	A7

#define BUT1_L	17
#define BUT1_R	16
#define BUT1_ML	15
#define CHAN_1	14

#define BUT2_L	8
#define BUT2_R	7
#define BUT2_ML	6
#define CHAN_2	5

#define LED2_R	13
#define LED2_L	12
#define LED1_L	19
#define LED1_R	18

#define BUT_DELAY 	50
#define ACTIV 		LOW

#define TRUE		0xFF
#define FALSE		0x00

#define DELAY 			1000

#define LED_DEB			13  	// arduino number
#define LED_DEB_PIN		5
#define LED_DEB_DDR		DDRB
#define LED_DEB_PORT 	PORTB

#define SET_LED_DEB 	(SETBIT(LED_DEB_PORT,LED_DEB_PIN))
#define RESET_LED_DEB 	(CLEARBIT(LED_DEB_PORT,LED_DEB_PIN))


#define DEB
volatile unsigned char Pot1=0;		// Хранение значения потенциометров для ШИМа
volatile unsigned char Pot2=0;

volatile unsigned char Cntr = 0;	// Счетная переменная для ШИМа

/*** Режим работы выходного ШИМа для каждого канала ***/
/*** L - Влево 
	 R - Вправо
	 M - Влево макс
	 S - Вправо 
 ***/
volatile  char Pwm1_EN=0;			
volatile  char Pwm2_EN=0;

/*****************************************************/
volatile unsigned char Chan1_EN=FALSE;		// Флаг включенности канала
volatile unsigned char Chan2_EN=FALSE;

#define DEB

void check_1(void);
void check_2(void);

SIGNAL(TIMER2_COMPA_vect)
{

	Cntr++;

	// Если вольтметр 1 включен
	
if (Chan1_EN>0)
	{
		// если счет достиг значения ПОТ1 и есть разрешение тогда делаем выход VM1 = 0
		if(Cntr<Pot1) {SETBIT(OUT1_VM_PORT,OUT1_VM_PIN);}
		else		  {CLEARBIT(OUT1_VM_PORT,OUT1_VM_PIN);}
	}
else {CLEARBIT(OUT1_VM_PORT,OUT1_VM_PIN);}
// Если вольтметр 2 включен
if (Chan2_EN>0)
	{
		// если счет достиг значения ПОТ2 и есть разрешение тогда делаем выход VM2 = 0
		if(Cntr<Pot2)	{SETBIT(OUT2_VM_PORT,OUT2_VM_PIN);}
		else			{CLEARBIT(OUT2_VM_PORT,OUT2_VM_PIN);}
	}
else {CLEARBIT(OUT2_VM_PORT,OUT2_VM_PIN);}

/****
// РАБОТА ШИМ ДЛЯ 1 КАНАЛА ДВИГАТЕЛЯ
*****/

switch (Pwm1_EN)
	{	
		case 'L':	// если направление влево
			{
				CLEARBIT(OUT1_R_PORT,OUT1_R_PIN);	// отключаем 1 правый канал	
				
				/*непосредственно ШИМ регулирование левый канал 1*/
				if(Cntr<Pot1) {SETBIT(OUT1_L_PORT,OUT1_L_PIN);}
				else		  {CLEARBIT(OUT1_L_PORT,OUT1_L_PIN);}
			};break;
		case 'R':	// если направление вправо
			{
				CLEARBIT(OUT1_L_PORT,OUT1_L_PIN);	// отключаем 1 левый канал	
				
				/*непосредственно ШИМ регулирование правый канал 1*/
				if(Cntr<Pot1) {SETBIT(OUT1_R_PORT,OUT1_R_PIN);}
				else		  {CLEARBIT(OUT1_R_PORT,OUT1_R_PIN);}
			}; break;
		case 'M':	// если направление влево MAX
			{
				CLEARBIT(OUT1_R_PORT,OUT1_R_PIN);	// отключаем 1 правый канал	
				SETBIT(OUT1_L_PORT,OUT1_L_PIN);		// включаем 1 левый канал на постоянку
			};break;
		case 'S':	// если направление вправо MAX
			{
				CLEARBIT(OUT1_L_PORT,OUT1_L_PIN);	// отключаем 1 левый канал	
				SETBIT(OUT1_R_PORT,OUT1_R_PIN);		// включаем 1 правый канал на постоянку
			};break;
			
		default: 
			{
				CLEARBIT(OUT1_L_PORT,OUT1_L_PIN);
				CLEARBIT(OUT1_R_PORT,OUT1_R_PIN);
			}
	}

/****
// РАБОТА ШИМ ДЛЯ 2 КАНАЛА ДВИГАТЕЛЯ
*****/

switch (Pwm2_EN)
	{	
		case 'L':	// если направление влево
			{
				CLEARBIT(OUT2_R_PORT,OUT2_R_PIN);	// отключаем 2 правый канал	
				
				/*непосредственно ШИМ регулирование левый канал 2*/
				if(Cntr<Pot2) {SETBIT(OUT2_L_PORT,OUT2_L_PIN);}
				else		  {CLEARBIT(OUT2_L_PORT,OUT2_L_PIN);}
			};break;
		case 'R':	// если направление вправо
			{
				CLEARBIT(OUT2_L_PORT,OUT2_L_PIN);	// отключаем 2 левый канал	
				
				/*непосредственно ШИМ регулирование правый канал 2*/
				if(Cntr<Pot2) {SETBIT(OUT2_R_PORT,OUT2_R_PIN);}
				else		  {CLEARBIT(OUT2_R_PORT,OUT2_R_PIN);}
			}; break;
		case 'M':	// если направление влево MAX
			{
				CLEARBIT(OUT2_R_PORT,OUT2_R_PIN);	// отключаем 2 правый канал	
				SETBIT(OUT2_L_PORT,OUT2_L_PIN);		// включаем 2 левый канал на постоянку
			};break;
		case 'S':	// если направление вправо MAX
			{
				CLEARBIT(OUT2_L_PORT,OUT2_L_PIN);	// отключаем 2 левый канал	
				SETBIT(OUT2_R_PORT,OUT2_R_PIN);		// включаем 2 правый канал на постоянку
			};break;
			
		default: 
			{
				CLEARBIT(OUT2_L_PORT,OUT2_L_PIN);
				CLEARBIT(OUT2_R_PORT,OUT2_R_PIN);
			}
	}

	
}

void setup() {

SETBIT(OUT1_VM_DDR,OUT1_VM_PIN);
SETBIT(OUT2_VM_DDR,OUT2_VM_PIN);
SETBIT(LED_DEB_DDR,LED_DEB_PIN);
	
pinMode(OUT1_L,OUTPUT);
pinMode(OUT1_R,OUTPUT);
pinMode(OUT2_L,OUTPUT);
pinMode(OUT2_R,OUTPUT);

pinMode(LED_DEB,OUTPUT);

pinMode(LED1_L,OUTPUT);
pinMode(LED1_R,OUTPUT);
pinMode(LED2_L,OUTPUT);
pinMode(LED2_R,OUTPUT);

digitalWrite(LED1_L,HIGH);
digitalWrite(LED1_R,HIGH);
digitalWrite(LED2_L,HIGH);
digitalWrite(LED2_R,HIGH);

delay(400);

digitalWrite(LED1_L,LOW);
digitalWrite(LED1_R,LOW);
digitalWrite(LED2_L,LOW);
digitalWrite(LED2_R,LOW);

//TCCR1B = TCCR1B & 0b11111000 |0x02; // 9,10 - 4KhZ PWM
//TCCR2B = TCCR2B & 0b11111000 |0x02; // 3,11 - 4KhZ	PWM

// Timer 2 int config 1MHZ
TCCR2B=0;
TCCR2A =0;
TCCR2B |= (1<<CS20);  	// devider 1
TCCR2A |= (1<<WGM21);  	// CTC
OCR2A = 16;
TIMSK2 |= (1<<OCIE2A);	// COMP int enable

Serial.begin(115200);
analogReference(DEFAULT);

Serial.print("DC motor controll device ");
Serial.println("V1.1");

}


void loop() {

check_1();
check_2(); 

}

void check_1 (void)
{
	
	if (digitalRead(CHAN_1)==ACTIV)
	{
		Chan1_EN = TRUE; 		
		
		Pot1=((analogRead(POT1_1))>>2);	// Считываем значение с ПОТ.	
		
		/* Проверка кнопок*/
		if	(digitalRead(BUT1_L)==ACTIV)	
		{
			Pwm1_EN = 'L';
			#ifdef DEB
				Serial.println ("BUT1_L");
			#endif
			
			digitalWrite(LED1_L,HIGH);	// индикация режима.
			digitalWrite(LED1_R,LOW);	
		
		}	// Проверка кнопок
		else if	(digitalRead(BUT1_R)==ACTIV)	
		{
		
			Pwm1_EN = 'R';			
			#ifdef DEB
				Serial.println ("BUT1_R");
			#endif
			
			digitalWrite(LED1_L,LOW);// индикация режима.
			digitalWrite(LED1_R,HIGH);
		} 
		else if	(digitalRead(BUT1_ML)==ACTIV)	// Если зажали МАХ, то гоним на макс скорости в одну либо вдругую сторону 
		{
			//если было направление влево, то выставляем МАХ лево
			if 		(Pwm1_EN == 'L') {Pwm1_EN = 'M';}	
			//если было направление вправо, то выставляем МАХ право
			else if (Pwm1_EN == 'R') {Pwm1_EN = 'S';}
			
			#ifdef DEB
			Serial.println ("MAX1");
			#endif
		}
		// до того момента когда кнопка МАХ зажата, иначе остановка полная.
		else if (Pwm1_EN=='M'){Pwm1_EN = 'L';}
		else if (Pwm1_EN=='S'){Pwm1_EN = 'R';}
	}
	else {
			Chan1_EN = FALSE; 
			digitalWrite(LED1_L,LOW);
			digitalWrite(LED1_R,LOW);
			Pwm1_EN = 0;
		 }
}
void check_2 (void)
{
	
	if (digitalRead(CHAN_2)==ACTIV)
	{
		Chan2_EN = TRUE; 	
				
		Pot2=((analogRead(POT2_2))>>2);	// Считываем значение с ПОТ.		
		/* Проверка кнопок*/
		if	(digitalRead(BUT2_L)==ACTIV)	
		{
			Pwm2_EN = 'L';
			#ifdef DEB
			Serial.println ("BUT2_L");
			#endif
			
			digitalWrite(LED2_L,HIGH);	// индикация режима.
			digitalWrite(LED2_R,LOW);	
		
		}	// Проверка кнопок
		else if	(digitalRead(BUT2_R)==ACTIV)	
		{
			Pwm2_EN = 'R';			
			#ifdef DEB
			Serial.println ("BUT2_R");
			#endif
			
			digitalWrite(LED2_L,LOW);// индикация режима.
			digitalWrite(LED2_R,HIGH);
		} 
		else if	(digitalRead(BUT2_ML)==ACTIV)	// Если зажали МАХ, то гоним на макс скорости в одну либо вдругую сторону
		{
			//если было направление влево, то выставляем МАХ лево
			if 		(Pwm2_EN == 'L') {Pwm2_EN = 'M';}	
			//если было направление вправо, то выставляем МАХ право
			else if (Pwm2_EN == 'R') {Pwm2_EN = 'S';}
			
			#ifdef DEB
			Serial.println ("MAX2");
			#endif
		}
		// до того момента когда кнопка МАХ зажата, иначе возврат в скоростной режим.
		else if (Pwm2_EN=='M'){Pwm2_EN = 'L';}
		else if (Pwm2_EN=='S'){Pwm2_EN = 'R';}
	}
	else {
			Chan2_EN = FALSE; 
			digitalWrite(LED2_L,LOW);
			digitalWrite(LED2_R,LOW);
			Pwm2_EN = 0;
		 }
}
