/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

//-----------SAME70-----------//
#define LED_PIO           PIOC                 // periferico que controla o 
#define LED_PIO_ID        ID_PIOC                  // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do botao da placa
#define BUT_PIO           PIOA // periférico que controla o botão
#define BUT_PIO_ID		  ID_PIOA
#define BUT_PIO_IDX		  11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX) // esse já está pronto.


//-----------OLED 1-----------//
// Configuracoes do botao 1
#define BUT1_PIO         PIOD // periférico que controla o botão
#define BUT1_PIO_ID		 ID_PIOD
#define BUT1_PIO_IDX	 28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX) // esse já está pronto.

// Configuracoes do botao 2
#define BUT2_PIO          PIOC // periférico que controla o botão
#define BUT2_PIO_ID		  ID_PIOC
#define BUT2_PIO_IDX	  31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX) // esse já está pronto.

// Configuracoes do botao 3
#define BUT3_PIO          PIOA // periférico que controla o botão
#define BUT3_PIO_ID		  ID_PIOA
#define BUT3_PIO_IDX	  19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX) // esse já está pronto.

// Configuracoes do LED 1
#define LED1_PIO           PIOA                 // periferico que controla o LED
#define LED1_PIO_ID        ID_PIOA              // ID do periférico PIOC (controla LED)
#define LED1_PIO_IDX       0                    // ID do LED no PIO
#define LED1_PIO_IDX_MASK  (1 << LED1_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do LED 2
#define LED2_PIO           PIOC                 // periferico que controla o LED
#define LED2_PIO_ID        ID_PIOC                  // ID do periférico PIOC (controla LED)
#define LED2_PIO_IDX       30                    // ID do LED no PIO
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do LED 3
#define LED3_PIO           PIOB                 // periferico que controla o LED
#define LED3_PIO_ID        ID_PIOB                  // ID do periférico PIOC (controla LED)
#define LED3_PIO_IDX       2                    // ID do LED no PIO
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)   // Mascara para CONTROLARMOS o LED

/*  Default pin configuration (no attribute). */
#define _PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define _PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define _PIO_DEGLITCH            (1u << 1)
/*  The internal debouncing filter is active. */
#define _PIO_DEBOUNCE            (1u << 3)



/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void _pio_set(Pio *p_pio, const uint32_t ul_mask)
{
	p_pio->PIO_SODR = ul_mask;
}

void _pio_clear(Pio *p_pio, const uint32_t ul_mask)
{
	p_pio->PIO_CODR = ul_mask;
}

void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_pull_up_enable){
	if (ul_pull_up_enable){
		p_pio->PIO_PUER = ul_mask;
	}
	else{
		p_pio->PIO_PUDR = ul_mask;
	}
}

void _pio_set_input(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_attribute){
	pio_pull_up(p_pio, ul_mask, ul_attribute & PIO_PULLUP);
	p_pio->PIO_ODR = ul_mask;
	p_pio->PIO_PER = ul_mask;
	
	if (ul_attribute & (PIO_DEGLITCH | PIO_DEBOUNCE)) {
		p_pio->PIO_IFER = ul_mask;
	} else {
		p_pio->PIO_IFDR = ul_mask;
	}
	
	if (ul_attribute & PIO_DEGLITCH) {
		p_pio->PIO_IFSCDR = ul_mask;
	} else {
		p_pio->PIO_IFSCER = ul_mask;
	}
}

void _pio_set_output(Pio *p_pio, const uint32_t ul_mask,
const uint32_t ul_default_level,
const uint32_t ul_multidrive_enable,
const uint32_t ul_pull_up_enable)
{
	p_pio->PIO_OER = ul_mask;
	p_pio->PIO_PER = ul_mask;
	
	if (ul_multidrive_enable) {
		p_pio->PIO_MDER = ul_mask;
		} else {
		p_pio->PIO_MDDR = ul_mask;
	}
	
	if (ul_default_level) {
		p_pio->PIO_SODR = ul_mask;
		} else {
		p_pio->PIO_CODR = ul_mask;
	}
	
	pio_pull_up(p_pio, ul_mask, ul_pull_up_enable & PIO_PULLUP);
}

uint32_t _pio_get(Pio *p_pio, const pio_type_t ul_type, const uint32_t ul_mask){
	uint32_t ul_reg;

	if ((ul_type == PIO_OUTPUT_0) || (ul_type == PIO_OUTPUT_1)) {
		ul_reg = p_pio->PIO_ODSR;
		} else {
		ul_reg = p_pio->PIO_PDSR;
	}

	if ((ul_reg & ul_mask) == 0) {
		return 0;
		} else {
		return 1;
	}
}

void _delay_ms(int ms) {
	for (int i = 0; i < (300000/2)*ms; i++){
		asm("NOP");
	}
}

// Função de inicialização do uC
void init(void){
	// Initialize the board clock
	sysclk_init();

	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativação dos PIO'
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(LED3_PIO_ID);
	
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	
	//Inicializa entradas e saidas
	_pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE); //pino 11 como entrada do botao
	_pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT); 
	_pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT); 
	_pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT); 
	
	_pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0); //pino 8 como saida do LED
	_pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 1, 0, 0);
	_pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 1, 0, 0);
	_pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 1, 0, 0);

	
	//Habilita o pull-up (o valor padrao do pino é energizado)
	_pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	_pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	_pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	_pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);

}


/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void) {
	// inicializa sistema e IOs
	init();

	// super loop
	// aplicacoes embarcadas não devem sair do while(1).
	while (1)
	{
		
		if(!_pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				_pio_set(LED_PIO, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED
				_delay_ms(100);                        // Delay por software de 100 ms
				_pio_clear(LED_PIO, LED_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				_delay_ms(100);                        // Delay por software de 100 ms
			}
		}
		
		if(!_pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				_pio_set(LED1_PIO, LED1_PIO_IDX_MASK);      // Coloca 1 no pino LED
				_delay_ms(100);                        // Delay por software de 100 ms
				_pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				_delay_ms(100);                        // Delay por software de 100 ms
			}	
		}
		
		else if(!_pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				_pio_set(LED2_PIO, LED2_PIO_IDX_MASK);      // Coloca 1 no pino LED
				_delay_ms(100);                        // Delay por software de 100 ms
				_pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				_delay_ms(100);                        // Delay por software de 100 ms
			}
		}
		
		else if(!_pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				_pio_set(LED3_PIO, LED3_PIO_IDX_MASK);      // Coloca 1 no pino LED
				_delay_ms(100);                        // Delay por software de 100 ms
				_pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				_delay_ms(100);                        // Delay por software de 100 ms
			}
		}
		else {
			_pio_set(LED_PIO, LED_PIO_IDX_MASK);
			_pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			_pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			_pio_set(LED3_PIO, LED3_PIO_IDX_MASK);      


		}
	}
	return 0;
}

