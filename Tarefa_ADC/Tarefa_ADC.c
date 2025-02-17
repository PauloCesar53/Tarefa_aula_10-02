#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"//biblioteca para funções  adc
#include "hardware/pwm.h"//biblioteca para funções  pwm
#include "hardware/clocks.h"

#define JOY_Y 26//eixo  y segundo diagrama BitDogLab
#define JOY_X 27//eixo  x segundo diagrama BitDogLab
#define JOY_botton 22//pino botão Joy 
#define Botao_A 5//pino botão A
#define LED_B 12//para LED azul 
#define LED_R 13//para LED vermelho  

static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento para Bot A(em microssegundos)
static volatile uint32_t last_time_JOY = 0; // Armazena o tempo do último evento para Bot Joy(em microssegundos)
static volatile bool aux_LED_PWM=1;//variável auxiliar para botão A (se verdadeiro lEDs são alterados com JOY)
void pwm_init_gpio(uint gpio, uint wrap);//protótipo de função para configurar pwm
void LED_Control(uint JOY_Y_value, uint JOY_X_value, int ajuste, float pwm_wrap);//protótipo de função para fazer tratamento PWM nos leds 
void gpio_irq_handler(uint gpio, uint32_t events);//protótipo de função para interrupção 

int main()
{
    stdio_init_all();
    adc_init();//inicializando adc
    adc_gpio_init(JOY_Y);//inicializando pino direção y 
    adc_gpio_init(JOY_X);//inicializando pino direção x
    //configurnado botão joy com ativação do pull up interno 
    gpio_init(JOY_botton);
    gpio_set_dir(JOY_botton, GPIO_IN);
    gpio_pull_up(JOY_botton);
    //configurnado botão A com ativação do pull up interno 
    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);
    //configurando LED azul e LED vermelho
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    //configurando PWM
    uint pwm_wrap = 4096;// definindo valor de wrap referente a 12 bits do ADC
    pwm_init_gpio(LED_B, pwm_wrap); 
    pwm_init_gpio(LED_R, pwm_wrap);
    uint8_t ajuste=55;// variável para ajustar o centro do Joystick e permanecer apagado proxímo ao centro 
    // configurando a interrupção com botão na descida para o botão A
    gpio_set_irq_enabled_with_callback(Botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // configurando a interrupção com botão na descida para o botão JOY
    gpio_set_irq_enabled_with_callback(JOY_botton, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    while (true) {
        adc_select_input(0);//canal adc JOY para eixo y
        uint16_t JOY_Y_value = adc_read(); // Lê o valor do eixo y, de 0 a 4095.
        adc_select_input(1);//canal adc JOY para eixo x
        uint16_t JOY_X_value = adc_read();// Lê o valor do eixo x, de 0 a 4095.
        uint  JOY_botton_value= gpio_get(JOY_botton) == 0; // 0 indica que o botão está pressionado.
        if(aux_LED_PWM){
            LED_Control(JOY_Y_value, JOY_X_value, ajuste, pwm_wrap);
        }
        sleep_ms(100);
    }
    return 0;
}
void pwm_init_gpio(uint gpio, uint wrap) {//função para configuração do PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    
    pwm_set_enabled(slice_num, true);  
}
void LED_Control(uint JOY_Y_value, uint JOY_X_value, int ajuste, float pwm_wrap){//função para fazer tratamento PWM nos leds 
    if(JOY_Y_value >= pwm_wrap/2+ajuste){//se valor lido for maior ou igual a metade considerando erro 
            pwm_set_gpio_level(LED_B, (-pwm_wrap + JOY_Y_value*2)); //considerando intensidade zero em 2048 e máxima em 4096
        }else if((JOY_Y_value <= pwm_wrap/2-ajuste)){
            pwm_set_gpio_level(LED_B, (pwm_wrap - JOY_Y_value*2)); //considerando intendidade zero em 2048 e máxima em 0
        }else{
            pwm_set_gpio_level(LED_B, 0.0);
        }
        if(JOY_X_value >= pwm_wrap/2+ajuste){//se valor lido for maior ou igual a metade 
            pwm_set_gpio_level(LED_R, (-pwm_wrap + JOY_X_value*2)); //considerando intensidade zero em 2048 e máxima em 4096
        }else if(JOY_X_value <= pwm_wrap/2-ajuste){
            pwm_set_gpio_level(LED_R, (pwm_wrap - JOY_X_value*2)); //considerando intendidade zero em 2048 e máxima em 0
        }else{
            pwm_set_gpio_level(LED_R, 0.0);
        }
};
void gpio_irq_handler(uint gpio, uint32_t events)//função de interrupção com tratamento de deboucing 
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());//// Obtém o tempo atual em microssegundos
    if (gpio_get(Botao_A) == 0  && (current_time - last_time_A) > 200000)//200ms de boucing adiconado como condição 
    { 
        last_time_A = current_time; // Atualiza o tempo do último evento  
        aux_LED_PWM=!aux_LED_PWM;//muda cada vez que for acionado alterando fução do botão A
        pwm_set_gpio_level(LED_R, 0.0);//garantir que lEDs apaguem 
        pwm_set_gpio_level(LED_B, 0.0);
    }
    if (gpio_get(JOY_botton) == 0  && (current_time - last_time_JOY) > 200000)//200ms de boucing adiconado como condição 
    { // 
        last_time_JOY = current_time; // Atualiza o tempo do último evento

    }
}