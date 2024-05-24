#ifndef SYS_H
#define SYS_H

// typedef struct
// {
//     short Fumes_Threshold_L;
//     short Fumes_Threshold_H;
//     uint8_t fume_state;
//     uint8_t beep_state;
// } SYS_Data_Sruct;

// extern SYS_Data_Sruct SYS_DS;

extern uint8_t fume_state;
extern uint8_t beep_state;

extern short Fumes_Threshold_L;
extern short Fumes_Threshold_H;

void _sys_Init(void);
void _sys_Loop(void);

#endif