#pragma once

#include "general.h"

tByte hInitIO(void);
void  readInputs(inpVector* inVec);
void  writeOutputs(outpVector* outVec);

// Byte-level port access (for future expansion)
tByte readByte(const gpio_num_t pinGroup[8]);
void  writeByte(const gpio_num_t pinGroup[8], tByte value);