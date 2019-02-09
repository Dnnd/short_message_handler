#include <msp430.h>

#define OVF_LED_PORT P2OUT
#define OVF_LED_PORT_DIR P2DIR

#define EOF_LED_PORT P1OUT
#define EOF_LED_PORT_DIR P2DIR

#define OVF_LED_BIT BIT1
#define EOF_LED_BIT BIT2

void ShowOverflowIndicator() {
    OVF_LED_PORT |= OVF_LED_BIT;
}
void HideOverflowIndicator() {
    OVF_LED_PORT &= ~OVF_LED_BIT;
}
void ShowEndOfMessageIndicator() {
    EOF_LED_PORT |= EOF_LED_BIT;
}
void HideEndOfMessageIndicator() {
    EOF_LED_PORT &= EOF_LED_BIT;
}

void ConfigureIndicators() {
    OVF_LED_PORT_DIR |= OVF_LED_BIT;
    EOF_LED_PORT_DIR |= EOF_LED_BIT;
    EOF_LED_PORT &= ~EOF_LED_BIT;
    OVF_LED_PORT &= ~OVF_LED_BIT;
}
