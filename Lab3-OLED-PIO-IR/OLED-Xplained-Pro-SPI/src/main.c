/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// LED
#define LED_PIO      PIOC
#define LED_PIO_ID   ID_PIOC
#define LED_IDX      8
#define LED_IDX_MASK (1 << LED_IDX)

// Botão
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

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/
volatile char but_flag = 0;
volatile char but1_flag = 0;
volatile char but2_flag = 0;
volatile char but3_flag = 0;
volatile char led_blink_on;
volatile char led_blink_off;

/************************************************************************/
/* prototype                                                            */
/************************************************************************/
void io_init(void);
void pisca_led(int n, int t);
void BUT1_callback(void);
void BUT2_callback(void);
void BUT3_callback(void);

/************************************************************************/
/* handler / callbacks                                                  */
/************************************************************************/
void BUT_callBack(void){
	but_flag = 1;
}

void BUT1_callback(void) {
	if (pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		but1_flag = 0;
		} else {
		but1_flag = 1;
		// PINO == 0 --> Borda de descida
	}
}

void BUT2_callback(void) {
	if (pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		but2_flag = 1;
		} else {
		but2_flag = 0;
		// PINO == 0 --> Borda de descida
	}
}

void BUT3_callback(void) {
	if (pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		but3_flag = 1;
		} else {
		but3_flag = 0;
		// PINO == 0 --> Borda de descida
	}
}


/************************************************************************/
/* funções                                                              */
/************************************************************************/

void pisca_led(int n, int t) {
	
	for (int i=0;i<n;i++){
		pio_clear(LED_PIO, LED_IDX_MASK);
		delay_ms(t);
		gfx_mono_draw_filled_rect(64, 20, 1+i*2, 8, GFX_PIXEL_SET);
		pio_set(LED_PIO, LED_IDX_MASK);
		delay_ms(t);
		
		if (but2_flag) {
			but2_flag = 0;
			break;
		}
	}
}

void atualiza_display(int delay) {
	char str[128];
	sprintf(str, "delay: %d", delay);
	gfx_mono_draw_string(str, 0, 0, &sysfont);
	gfx_mono_draw_filled_rect(64, 20, 60, 8, GFX_PIXEL_CLR);
	gfx_mono_draw_rect(64, 20, 60, 8, GFX_PIXEL_SET);
}


void io_init(void) {

	// Ativação dos periféricos
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	
	// Configuração dos botões 
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_IDX_MASK, PIO_DEFAULT);
	
	pio_configure(BUT_PIO, PIO_INPUT, BUT_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT_IDX_MASK, 60);

	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, 60);
	
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, 60);
	
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT3_PIO, BUT3_PIO_IDX_MASK, 60);

	//Handlers
	pio_handler_set(BUT_PIO,
	BUT_PIO_ID,
	BUT_IDX_MASK,
	PIO_IT_RISE_EDGE,
	BUT_callBack);
	  
	pio_handler_set(BUT1_PIO,
	BUT1_PIO_ID,
	BUT1_PIO_IDX_MASK,
	PIO_IT_EDGE,
	BUT1_callback);
	
	pio_handler_set(BUT2_PIO,
	BUT2_PIO_ID,
	BUT2_PIO_IDX_MASK,
	PIO_IT_EDGE,
	BUT2_callback);
	
	pio_handler_set(BUT3_PIO,
	BUT3_PIO_ID,
	BUT3_PIO_IDX_MASK,
	PIO_IT_EDGE,
	BUT3_callback);
	
	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT_PIO, BUT_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO);
	
	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT1_PIO);
	
	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT2_PIO);
	
	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT3_PIO);
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID, 4); // Prioridade 4
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, 4); // Prioridade 4
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 4); // Prioridade 4
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 4); // Prioridade 4
}

int main (void) {
	board_init();
	sysclk_init();
	delay_init();
	io_init();
	
	// Desativa watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;

  // Init OLED
	gfx_mono_ssd1306_init();
	
	int delay = 100;
	atualiza_display(delay);

  /* Insert application code here, after the board has been initialized. */
  
	while(1) {
		
		
		if (but1_flag) {
			delay -= 100;
			atualiza_display(delay);
			but1_flag = 1;
		}
		
		int count = 0;
		while (but1_flag) {
			if (count > 25000000) {
				delay += 100;
				atualiza_display(delay);
				count = 0;
			}
			count++;
		}
		
		if (but2_flag) {
			but2_flag = 0;
			pisca_led(30, delay);
		}
		
		if (but3_flag) {
			delay += 100;
			atualiza_display(delay);
			but3_flag = 0;
		}
		
		
		// Entra em sleep mode
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);	
		
	}
	
}
