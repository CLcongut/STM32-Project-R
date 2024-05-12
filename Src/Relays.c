#include "main.h"
#include "Relays.h"

void Relays_ON(void)
{
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
}

void Relays_OFF(void)
{
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
}