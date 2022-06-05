/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#ifndef _REMOTE_H_
#define _REMOTE_H_

void empty_serial(void);
int remote_receive_string(int print);
int remote_try_wakeup(void);
int remote_try_command(char* cmd, int print_response);
int remote_get_voltages(void);
int remote_check_for_low_battery(void);
int remote_get_status(void);
int remote_turn_on_som(void);
int remote_turn_off_som(void);
int remote_reset_som(void);
int remote_wake_som(void);
int remote_turn_off_aux(void);
int remote_turn_on_aux(void);
int remote_report_voltages(void);
int remote_enable_som_uart(void);
int remote_disable_som_uart(void);
void remote_process_alerts(void);
void remote_init(void);

#endif
