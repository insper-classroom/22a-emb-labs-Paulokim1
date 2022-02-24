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
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT); //pino 11 como entrada do botao
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT); 
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT); 
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT); 
	
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0); //pino 8 como saida do LED
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 1, 0, 0);

	
	//Habilita o pull-up (o valor padrao do pino é energizado)
	//pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);

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
		if(!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				pio_set(LED1_PIO, LED1_PIO_IDX_MASK);      // Coloca 1 no pino LED
				delay_ms(100);                        // Delay por software de 100 ms
				pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				delay_ms(100);                        // Delay por software de 100 ms
			}	
		}
		
		else if(!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				pio_set(LED2_PIO, LED2_PIO_IDX_MASK);      // Coloca 1 no pino LED
				delay_ms(100);                        // Delay por software de 100 ms
				pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				delay_ms(100);                        // Delay por software de 100 ms
			}
		}
		
		else if(!pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)){
			for(int i=0; i < 5; i++){
				pio_set(LED3_PIO, LED3_PIO_IDX_MASK);      // Coloca 1 no pino LED
				delay_ms(100);                        // Delay por software de 100 ms
				pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);    // Coloca 0 no pino do LED
				delay_ms(100);                        // Delay por software de 100 ms
			}
		}
		else {
				pio_set(LED1_PIO, LED1_PIO_IDX_MASK);      // Coloca 1 no pino LED
				pio_set(LED2_PIO, LED2_PIO_IDX_MASK);      // Coloca 2 no pino LED
				pio_set(LED3_PIO, LED3_PIO_IDX_MASK);      // Coloca 3 no pino LED


		}
	}
	return 0;
}

