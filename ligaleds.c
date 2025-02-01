#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED1 11  // LED vermelho
#define LED2 12  // LED amarelo
#define LED3 13  // LED verde
#define BUTTON 5 // Botão

#define DELAY_MS 3000  // 3 segundos entre desligamentos

volatile bool leds_ativos = false;  // Indica se os LEDs estão ativos
volatile int estado_led = 0;  // Controle do estado dos LEDs

// Protótipo das funções
int desligar_leds_callback(alarm_id_t id, void *user_data);
void iniciar_sequencia_leds();
void debounce_button(uint gpio, uint32_t events);

// Callback para desligar LEDs um por um
int desligar_leds_callback(alarm_id_t id, void *user_data) {
    if (estado_led == 2) {
        gpio_put(LED1, 0); // Desliga LED1
        estado_led--;
        add_alarm_in_ms(DELAY_MS, desligar_leds_callback, NULL, false);
    } else if (estado_led == 1) {
        gpio_put(LED2, 0); // Desliga LED2
        estado_led--;
        add_alarm_in_ms(DELAY_MS, desligar_leds_callback, NULL, false);
    } else if (estado_led == 0) {
        gpio_put(LED3, 0); // Desliga LED3
        leds_ativos = false;  // Permite novo acionamento pelo botão
    }
    return 0;
}

// Função para iniciar a sequência dos LEDs
void iniciar_sequencia_leds() {
    if (leds_ativos) return;  // Impede nova ativação durante a sequência

    leds_ativos = true;
    estado_led = 2;  // Inicia com todos os LEDs ligados

    gpio_put(LED1, 1);
    gpio_put(LED2, 1);
    gpio_put(LED3, 1);

    // Inicia o desligamento progressivo após 3s
    add_alarm_in_ms(DELAY_MS, desligar_leds_callback, NULL, false);
}

// Tratamento do botão com debounce por software
void debounce_button(uint gpio, uint32_t events) {
    static uint64_t ultimo_tempo = 0;
    uint64_t tempo_atual = time_us_64();

    if (tempo_atual - ultimo_tempo > 50000) { // 50ms de debounce
        iniciar_sequencia_leds();
    }
    ultimo_tempo = tempo_atual;
}

int main() {
    stdio_init_all();

    // Configuração dos LEDs como saída
    gpio_init(LED1);
    gpio_init(LED2);
    gpio_init(LED3);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_set_dir(LED3, GPIO_OUT);

    // Configuração do botão como entrada com pull-up interno
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    // Configuração da interrupção do botão (falling edge)
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, &debounce_button);

    while (1) {
        tight_loop_contents(); // Mantém o processador ativo sem consumir energia extra
    }
}