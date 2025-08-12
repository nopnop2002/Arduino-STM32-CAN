//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
#if 0
#ifdef ARDUINO_NUCLEO_H743ZI2
  #include "ACANFD-STM32-programmable-ram-sections.h"
#elif defined (ARDUINO_NUCLEO_G431KB)
  #include "ACANFD-STM32-fixed-ram-sections.h"
#elif defined (ARDUINO_NUCLEO_G474RE)
  #include "ACANFD-STM32-fixed-ram-sections.h"
#elif defined (ARDUINO_WEACT_G474CE)
  #include "ACANFD-STM32-fixed-ram-sections.h"
#elif defined (ARDUINO_NUCLEO_G0B1RE)
  #include "ACANFD-STM32-fixed-ram-sections.h"
#elif defined (ARDUINO_NUCLEO_H723ZG)
  #include "ACANFD-STM32-programmable-ram-sections.h"
#else
  #error "Unhandled Nucleo Board"
#endif
#endif

#include "ACANFD-STM32-fixed-ram-sections.h"

//------------------------------------------------------------------------------
