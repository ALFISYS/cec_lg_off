/*
 * This file is part of cec_lg_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/


/**
 * CONTENTS OF THIS FILE ARE GENERATED WITH 'gen_constants.h'
*/
#ifndef F_CPU
  #error No F_CPU configured.
#elif F_CPU == 16500000
  #define T0CS             ((uint8_t)5) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MIN    ((uint8_t)6) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D1_MAX ((uint8_t)13) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D0_MIN ((uint8_t)20) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MAX    ((uint8_t)27) // generated using 'helper_constants.ps1'
  #define CEC_DB_HL_MAX    ((uint8_t)33) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MIN    ((uint8_t)56) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MAX    ((uint8_t)63) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MIN    ((uint8_t)69) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MAX    ((uint8_t)76) // generated using 'helper_constants.ps1'
#elif F_CPU == 16000000
  #define T0CS             ((uint8_t)5) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MIN    ((uint8_t)6) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D1_MAX ((uint8_t)12) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D0_MIN ((uint8_t)20) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MAX    ((uint8_t)27) // generated using 'helper_constants.ps1'
  #define CEC_DB_HL_MAX    ((uint8_t)32) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MIN    ((uint8_t)55) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MAX    ((uint8_t)61) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MIN    ((uint8_t)67) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MAX    ((uint8_t)73) // generated using 'helper_constants.ps1'
#elif F_CPU == 8000000
  #define T0CS             ((uint8_t)4) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MIN    ((uint8_t)12) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D1_MAX ((uint8_t)25) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D0_MIN ((uint8_t)39) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MAX    ((uint8_t)53) // generated using 'helper_constants.ps1'
  #define CEC_DB_HL_MAX    ((uint8_t)64) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MIN    ((uint8_t)109) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MAX    ((uint8_t)122) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MIN    ((uint8_t)134) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MAX    ((uint8_t)147) // generated using 'helper_constants.ps1'
#elif F_CPU == 4000000
  #define T0CS             ((uint8_t)4) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MIN    ((uint8_t)6) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D1_MAX ((uint8_t)12) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D0_MIN ((uint8_t)20) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MAX    ((uint8_t)27) // generated using 'helper_constants.ps1'
  #define CEC_DB_HL_MAX    ((uint8_t)32) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MIN    ((uint8_t)55) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MAX    ((uint8_t)61) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MIN    ((uint8_t)67) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MAX    ((uint8_t)73) // generated using 'helper_constants.ps1'
#elif F_CPU == 2000000
  #define T0CS             ((uint8_t)3) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MIN    ((uint8_t)12) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D1_MAX ((uint8_t)25) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D0_MIN ((uint8_t)39) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MAX    ((uint8_t)53) // generated using 'helper_constants.ps1'
  #define CEC_DB_HL_MAX    ((uint8_t)64) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MIN    ((uint8_t)109) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MAX    ((uint8_t)122) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MIN    ((uint8_t)134) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MAX    ((uint8_t)147) // generated using 'helper_constants.ps1'
#elif F_CPU == 1000000
  #define T0CS             ((uint8_t)3) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MIN    ((uint8_t)6) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D1_MAX ((uint8_t)12) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_D0_MIN ((uint8_t)20) // generated using 'helper_constants.ps1'
  #define CEC_DB_LH_MAX    ((uint8_t)27) // generated using 'helper_constants.ps1'
  #define CEC_DB_HL_MAX    ((uint8_t)32) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MIN    ((uint8_t)55) // generated using 'helper_constants.ps1'
  #define CEC_SB_LH_MAX    ((uint8_t)61) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MIN    ((uint8_t)67) // generated using 'helper_constants.ps1'
  #define CEC_SB_HL_MAX    ((uint8_t)73) // generated using 'helper_constants.ps1'
#else
    #error No preset for your F_CPU (cpu frequency) is pre-calculated. Use 'helper_constants.ps1' and calculate preset for your F_CPU, then add to this place in code.
#endif