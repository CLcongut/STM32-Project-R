#ifndef GUI_H
#define GUI_H

void _gui_Load(void);
void _gui_Fire_State(uint8_t FState);
void _gui_Fumes_Value(uint8_t *fume_buf);
void _gui_TAH_Value(uint8_t *tah_buf);
void _gui_Fumes_Alarm(uint8_t d_fume_L_alarm, uint8_t d_fume_H_alarm, uint8_t b_fume_state, uint8_t b_beep_state);

#endif