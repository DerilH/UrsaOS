#pragma once
#include <stdint.h>
#include "ScancodeTranslation.h"
#include "../../Terminal.h"

void HandleKeyboard(uint8_t scancode, Terminal *terminal);