// Lucas Luige Costa Miranda
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 12

// frequência de 50Hz -> (125 * 10^6) / (25000 * 100) ≈ 50Hz
#define WRAP 25000
#define DIV 100

const float dgs[] = {12, 7.35, 2.5}; // 180°, 90° e 0°
int step = (0.025 * WRAP) / 100.0; // variavel para modificação de nivel

uint slice;

void setup(){
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);//-----------------------|
    slice = pwm_gpio_to_slice_num(SERVO_PIN);//                          |
    pwm_set_clkdiv(slice, DIV);//                                        |  Configuração do PWM
    pwm_set_wrap(slice, WRAP);//                                         |
    pwm_set_enabled(slice, true);//--------------------------------------|
}

int main()
{
    stdio_init_all();
    setup(); // Configura e inicializa o PWM

    uint16_t mvs[3];
    
    for(int i = 0; i < 3; i++){
        mvs[i] = (uint16_t)(WRAP * dgs[i] / 100.0); // Calcula o valor de movimento para cada angulo
        pwm_set_gpio_level(SERVO_PIN, mvs[i]);
        sleep_ms(5000);
    }

    uint16_t dc = mvs[2];

    while (true) {
        pwm_set_gpio_level(SERVO_PIN, dc);
        dc += step;

        if(dc <= mvs[2] || dc >= mvs[0])    // Veririfica se o angulo máximo/minimo foi atingido, respectivamente 180° e 0°
            step *= -1;                     // Inverte o sentido do movimento
        
        sleep_ms(10);
    }
}
