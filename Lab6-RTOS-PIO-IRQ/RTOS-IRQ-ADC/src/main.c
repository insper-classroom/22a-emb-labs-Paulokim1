/************************************************************************/
/* INCLUDES                                                             */
/************************************************************************/
#include "asf.h"


/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_IDX_MASK (1 << LED_PIO_IDX)

#define LED1_PIO           PIOA
#define LED1_PIO_ID        ID_PIOA
#define LED1_PIO_IDX       0
#define LED1_PIO_IDX_MASK  (1 << LED1_PIO_IDX)

#define LED2_PIO           PIOC
#define LED2_PIO_ID        ID_PIOC
#define LED2_PIO_IDX       30
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)

#define LED3_PIO           PIOB
#define LED3_PIO_ID        ID_PIOB
#define LED3_PIO_IDX       2
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)

#define BUT_PIO      PIOA
#define BUT_PIO_ID   ID_PIOA
#define BUT_IDX		 11
#define BUT_IDX_MASK (1 << BUT_IDX)

#define BUT1_PIO          PIOD
#define BUT1_PIO_ID       ID_PIOD
#define BUT1_PIO_IDX      28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)

#define BUT2_PIO          PIOC
#define BUT2_PIO_ID       ID_PIOC
#define BUT2_PIO_IDX      31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)

#define BUT3_PIO          PIOA
#define BUT3_PIO_ID       ID_PIOA
#define BUT3_PIO_IDX      19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

// PAD
#define KEY_LIN_1				PIOD
#define KEY_LIN_1_ID			ID_PIOD
#define KEY_LIN_1_IDX			30
#define KEY_LIN_1_IDX_MASK (1 << KEY_LIN_1_IDX)

#define KEY_LIN_2				PIOA
#define KEY_LIN_2_ID			ID_PIOA
#define KEY_LIN_2_IDX			6
#define KEY_LIN_2_IDX_MASK (1 << KEY_LIN_2_IDX)

#define KEY_LIN_3				PIOC
#define KEY_LIN_3_ID			ID_PIOC
#define KEY_LIN_3_IDX			19
#define KEY_LIN_3_IDX_MASK (1 << KEY_LIN_3_IDX)

#define KEY_LIN_4				PIOA
#define KEY_LIN_4_ID			ID_PIOA
#define KEY_LIN_4_IDX			2
#define KEY_LIN_4_IDX_MASK (1 << KEY_LIN_4_IDX)

#define KEY_COL_1				PIOA
#define KEY_COL_1_ID			ID_PIOA
#define KEY_COL_1_IDX			3
#define KEY_COL_1_IDX_MASK (1 << KEY_COL_1_IDX)

#define KEY_COL_2				PIOA
#define KEY_COL_2_ID			ID_PIOA
#define KEY_COL_2_IDX			21
#define KEY_COL_2_IDX_MASK (1 << KEY_COL_2_IDX)

#define KEY_COL_3				PIOD
#define KEY_COL_3_ID			ID_PIOD
#define KEY_COL_3_IDX			27
#define KEY_COL_3_IDX_MASK (1 << KEY_COL_3_IDX)

#define KEY_COL_4				PIOD
#define KEY_COL_4_ID			ID_PIOD
#define KEY_COL_4_IDX			20
#define KEY_COL_4_IDX_MASK (1 << KEY_COL_4_IDX)

/************************************************************************/
/* GLOBAL VARS                                                          */
/************************************************************************/

char lido = '\0';


/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void keypad_init(void);
char le_keypad();

/************************************************************************/
/* CALLBACKS                                                            */
/************************************************************************/


/************************************************************************/
/* INITS                                                                */
/************************************************************************/
void LED_init() {
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_IDX_MASK, 0, 0, 0);
	
	/**
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 1, 0, 0);
	
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 1, 0, 0);
	
	pmc_enable_periph_clk(LED3_PIO_ID);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 1, 0, 0);
	**/
};

void keypad_init()
{
	
	pmc_enable_periph_clk(KEY_LIN_1_ID);
	pio_set_output(KEY_LIN_1, KEY_LIN_1_IDX_MASK, 1, 0, 0);
	
	pmc_enable_periph_clk(KEY_LIN_2_ID);
	pio_set_output(KEY_LIN_2, KEY_LIN_2_IDX_MASK, 1, 0, 0);
	
	pmc_enable_periph_clk(KEY_LIN_3_ID);
	pio_set_output(KEY_LIN_3, KEY_LIN_3_IDX_MASK, 1, 0, 0);
	
	pmc_enable_periph_clk(KEY_LIN_4_ID);
	pio_set_output(KEY_LIN_4, KEY_LIN_4_IDX_MASK, 1, 0, 0);

	
	pmc_enable_periph_clk(KEY_COL_1_ID);
	pio_set_input(KEY_COL_1, KEY_COL_1_IDX_MASK, PIO_PULLUP);
	
	pmc_enable_periph_clk(KEY_COL_2_ID);
	pio_set_input(KEY_COL_2, KEY_COL_2_IDX_MASK, PIO_PULLUP);
	
	pmc_enable_periph_clk(KEY_COL_3_ID);
	pio_set_input(KEY_COL_3, KEY_COL_3_IDX_MASK, PIO_PULLUP);
	
	pmc_enable_periph_clk(KEY_COL_4_ID);
	pio_set_input(KEY_COL_4, KEY_COL_4_IDX_MASK, PIO_PULLUP);/**/
	
}

char le_keypad()
{
	
	pio_clear(KEY_LIN_1, KEY_LIN_1_IDX_MASK);
	pio_set(KEY_LIN_2, KEY_LIN_2_IDX_MASK);
	pio_set(KEY_LIN_3, KEY_LIN_3_IDX_MASK);
	pio_set(KEY_LIN_4, KEY_LIN_4_IDX_MASK);

	if(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '1';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '2';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '3';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = 'A';
		delay_ms(20);
	}


	pio_set(KEY_LIN_1, KEY_LIN_1_IDX_MASK);
	pio_clear(KEY_LIN_2, KEY_LIN_2_IDX_MASK);
	pio_set(KEY_LIN_3, KEY_LIN_3_IDX_MASK);
	pio_set(KEY_LIN_4, KEY_LIN_4_IDX_MASK);

	if(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '4';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '5';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '6';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = 'B';
		delay_ms(20);
	}


	pio_set(KEY_LIN_1, KEY_LIN_1_IDX_MASK);
	pio_set(KEY_LIN_2, KEY_LIN_2_IDX_MASK);
	pio_clear(KEY_LIN_3, KEY_LIN_3_IDX_MASK);
	pio_set(KEY_LIN_4, KEY_LIN_4_IDX_MASK);

	if(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '7';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '8';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '9';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = 'C';
		delay_ms(20);
	}


	pio_set(KEY_LIN_1, KEY_LIN_1_IDX_MASK);
	pio_set(KEY_LIN_2, KEY_LIN_2_IDX_MASK);
	pio_set(KEY_LIN_3, KEY_LIN_3_IDX_MASK);
	pio_clear(KEY_LIN_4, KEY_LIN_4_IDX_MASK);

	if(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_1,PIO_INPUT,KEY_COL_1_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '*';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_2,PIO_INPUT,KEY_COL_2_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '0';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_3,PIO_INPUT,KEY_COL_3_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = '#';
		delay_ms(20);
	}

	else if(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
	{
		while(!(pio_get(KEY_COL_4,PIO_INPUT,KEY_COL_4_IDX_MASK)))
		{
			delay_ms(20);
		}
		lido = 'D';
		delay_ms(20);
	}

	return lido;

}



/************************************************************************/
/* HANDLERS                                                             */
/************************************************************************/

/************************************************************************/
/* FUNCTIONS                                                             */
/************************************************************************/


/************************************************************************/
/* Main Code	                                                        */
/************************************************************************/


int main(void){
	/* Initialize the SAM system */
	sysclk_init();

	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	

	/* Configura Leds e botões */
	LED_init();
	keypad_init();



	while (1) {
		
		char lido = le_keypad(); //Chama a função de varredura do KeyPad 
		printf(lido);
		
		
		
	}
}
