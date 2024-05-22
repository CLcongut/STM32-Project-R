#ifndef SYS_H
#define SYS_H

extern uint8_t set_rail_state;
extern uint8_t cur_rail_state;

void _sys_Init(void);
void _sys_Loop(void);

#endif