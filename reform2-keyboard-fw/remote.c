/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#include <stdlib.h>
#include "remote.h"
#include "oled.h"
#include "powersave.h"
#include "constants.h"
#include "LUFA/Drivers/Peripheral/Serial.h"

char response[64];
int term_x = 0;
int term_y = 0;
float voltages[8];
int alert_low_battery = 0;
int alert_blink = 0;
uint8_t remote_som_power_expected_state = 0;

void empty_serial(void) {
  int clock = 0;
  while (Serial_ReceiveByte()>=0 && clock<100) {
    // flush serial
    clock++;
  }
}

int remote_receive_string(int print) {
  char done = 0;
  int32_t clock = 0;
  int res_x = 0;
  response[0] = 0;

  while (!done) {
    int16_t chr = -1;
    clock = 0;
    while (chr==-1 || chr==0) {
      chr=Serial_ReceiveByte();
      clock++;
      if (clock>500000) goto timeout;
    }
    int poke_chr = chr;
    if (chr=='\n') poke_chr=' ';
    if (chr!='\r') {
      if (print) {
        gfx_poke(term_x,term_y,poke_chr);
        gfx_poke(term_x+1,term_y,' ');
        term_x++;
        if (term_x>=20) {
          term_x=0;
          term_y++;
          if (term_y>=3) {
            term_y=0;
          }
        }
      }
      if (res_x<63) {
        response[res_x++] = chr;
        response[res_x] = 0;
      }
    }
    if (chr=='\r') done = 1;
  }
timeout:
  if (!done && print) gfx_poke(20,0,'T');
  empty_serial();
  if (print) {
    gfx_flush();
  }
  return done;
}

int remote_try_wakeup(void) {
  int ok = 0;
  char buf[64];

#ifdef KBD_VARIANT_STANDALONE
  // there's no remote
  return 0;
#endif

  for (int i=0; i<1000; i++) {
    if (i%10 == 0) {
      gfx_clear();
      sprintf(buf, "Waking up LPC... %d%%", i/4);
      gfx_poke_str(0, 0, buf);
      gfx_flush();
    }

    Serial_SendByte('a');
    Serial_SendByte('\r');

    if (Serial_ReceiveByte()>0) {
      remote_receive_string(0);
      ok = 1;
      break;
    }

    Delay_MS(25);
  }
  Serial_SendByte('\r');
  Delay_MS(10);
  while (remote_receive_string(0)) {
    Delay_MS(25);
  }

  return ok;
}

int remote_try_command(char* cmd, int print_response) {
  int ok = 0;

#ifdef KBD_VARIANT_STANDALONE
  // there's no remote
  return 0;
#endif

  empty_serial();
  for (int tries=0; tries<2; tries++) {
    for (int i=0; i<strlen(cmd); i++) {
      Serial_SendByte(cmd[i]);
    }
    Serial_SendByte('\r');
    Delay_MS(1);

    if (print_response) {
      term_x = 0;
      term_y = 0;
    }
    ok = remote_receive_string(print_response);

    if (!ok && tries == 0) {
      remote_try_wakeup();
      empty_serial();
    }
    if (ok) break;
  }
  if (!ok) {
    gfx_clear();
    gfx_poke_str(0, 0, "No response from LPC.");
    gfx_flush();
  }

  empty_serial();
  return ok;
}

void insert_bat_icon(char* str, int x, float v) {
  char icon = 0;
  if (v>=3.3) {
    icon = 8;
  } else if (v>=3.1) {
    icon = 6;
  } else if (v>=3.0) {
    icon = 4;
  } else if (v>=2.9) {
    icon = 2;
  } else {
    icon = 0;
  }
  str[x]   = 4*32+icon;
  str[x+1] = 4*32+icon+1;
}

int remote_get_voltages(void) {
  term_x = 0;
  term_y = 0;

  float bat_volts = 0;
  float bat_amps = 0;
  char bat_gauge[5] = {0,0,0,0,0};

  int ok = remote_try_command("c", 0);
  if (!ok) return ok;

  // lpc format: 32 32 32 32 32 32 32 32 mA 0256mV26143 ???% P1
  //             |  |  |  |  |  |  |  |  | |      |     |    |
  //             0  3  6  9  12 15 18 21 24|      |     |    |
  //                                       26     33    39   44
  //                                       |
  //                                       `- can be a minus
  float sum_volts = 0;

  for (int i=0; i<8; i++) {
    voltages[i] = ((float)((response[i*3]-'0')*10 + (response[i*3+1]-'0')))/10.0;
    if (voltages[i]<0) voltages[i]=0;
    if (voltages[i]>=10) voltages[i]=9.9;
    sum_volts += voltages[i];
  }

  int amps_offset = 3*8+2;
  // cut off string
  response[amps_offset+5]=0;
  bat_amps = ((float)atoi(&response[amps_offset]))/1000.0;
  int volts_offset = amps_offset+5+2;
  response[volts_offset+5]=0;
  bat_volts = ((float)atoi(&response[volts_offset]))/1000.0;
  int gauge_offset = volts_offset+5+1;
  strncpy(bat_gauge, &response[gauge_offset], 4);

  char* power_str = "   ";
  int syspower_offset = gauge_offset+5;
  char power_digit = response[syspower_offset+1];
  if (power_digit == '1') {
    power_str = " On";
  } else if (power_digit == '0') {
    power_str = "Off";
  }

  // plot
  gfx_clear();
  char str[32];

  sprintf(str,"[] %.1f  [] %.1f   %s",voltages[0],voltages[4],bat_gauge);
  insert_bat_icon(str,0,voltages[0]);
  insert_bat_icon(str,8,voltages[4]);
  gfx_poke_str(0,0,str);

  sprintf(str,"[] %.1f  [] %.1f    %s",voltages[1],voltages[5],power_str);
  insert_bat_icon(str,0,voltages[1]);
  insert_bat_icon(str,8,voltages[5]);
  gfx_poke_str(0,1,str);

  if (bat_amps>=0) {
    sprintf(str,"[] %.1f  [] %.1f %2.3fA",voltages[2],voltages[6],bat_amps);
  } else {
    sprintf(str,"[] %.1f  [] %.1f %2.2fA",voltages[2],voltages[6],bat_amps);
  }
  insert_bat_icon(str,0,voltages[2]);
  insert_bat_icon(str,8,voltages[6]);
  gfx_poke_str(0,2,str);

  sprintf(str,"[] %.1f  [] %.1f %2.2fV",voltages[3],voltages[7],bat_volts);
  insert_bat_icon(str,0,voltages[3]);
  insert_bat_icon(str,8,voltages[7]);
  gfx_poke_str(0,3,str);
  gfx_flush();

  return ok;
}

int remote_check_for_low_battery(void) {
  char bat_gauge[5] = {0,0,0,0,0};

  alert_low_battery = 0;
  empty_serial();

  Serial_SendByte('c');
  Serial_SendByte('\r');
  Delay_MS(1);
  int ok = remote_receive_string(0);
  if (!ok) return ok;

  for (int i=0; i<8; i++) {
    // TODO: only accept digits
    voltages[i] = ((float)((response[i*3]-'0')*10 + (response[i*3+1]-'0')))/10.0;
    if (voltages[i]<0) voltages[i]=0;
    if (voltages[i]>=10) voltages[i]=9.9;
    if (voltages[i]<3.0) {
      alert_low_battery = 1;
    }
  }

  int gauge_offset = 3*8+2+5+2+5+1;
  strncpy(bat_gauge, &response[gauge_offset], 3);

  if (bat_gauge[0] == '?') {
    // battery charge level unknown
  } else {
    int percent = atoi(bat_gauge);
    if (percent<10) {
      alert_low_battery = 1;
    }
  }

  int syspower_offset = gauge_offset+5;
  if (response[syspower_offset] == 'P') {
    char digit = response[syspower_offset+1];
    if (digit == '0' || digit == '1') {
      int is_computer_on = (digit == '1');
      if (!is_computer_on && remote_som_power_expected_state == 1) {
        // LPC says the computer is off, but we didn't expect it to be.
        // the only way this happens is if LPC turned off the system
        // due to a low battery condition.
        //
        // The keyboard will then go to sleep accordingly.

        keyboard_power_off();
      }
      remote_som_power_expected_state = is_computer_on;
    }
  }

  return ok;
}

int remote_get_status(void) {
  gfx_clear();
  empty_serial();

  gfx_poke_str(0, 2, "MNT Reform Keyboard");
  gfx_poke_str(0, 3, KBD_FW_REV);
  gfx_on();
  gfx_flush();

#ifndef KBD_VARIANT_STANDALONE
  int ok = remote_try_command("s", 1);
  return ok;
#endif
  return 1;
}

int remote_turn_on_som(void) {
  gfx_clear();

  int ok = remote_try_command("1p", 0);
  if (!ok) {
    // FIXME what is remote_som_power_expected_state?
    return ok;
  }

  remote_som_power_expected_state = 1;
  return ok;
}

int remote_turn_off_som(void) {
  int ok = remote_try_command("0p", 0);
  if (!ok) return ok;

  remote_som_power_expected_state = 0;
  return ok;
}

int remote_reset_som(void) {
  return remote_try_command("2p", 0);
}

int remote_wake_som(void) {
  int ok = remote_try_command("1w", 0);
  if (!ok) return ok;
  ok = remote_try_command("0w", 0);
  return ok;
}

int remote_turn_off_aux(void) {
  return remote_try_command("3p", 0);
}

int remote_turn_on_aux(void) {
  return remote_try_command("4p", 0);
}

int remote_report_voltages(void) {
  return remote_try_command("0c", 0);
}

int remote_enable_som_uart(void) {
  return remote_try_command("1u", 0);
}

int remote_disable_som_uart(void) {
  return remote_try_command("0u", 0);
}

void remote_process_alerts(void) {
  if (alert_low_battery) {
    gfx_on();
    for (int x=8;x<=11;x++) {
      gfx_poke( x,0,' ');
    }
    if (alert_blink) {
      gfx_poke( 9,0,4*32+2);
      gfx_poke(10,0,4*32+3);
    }
    gfx_flush();
  }
  alert_blink = 1-alert_blink;
}

void remote_init(void) {
  Serial_Init(57600, false);
}
