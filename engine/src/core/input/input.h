#pragma once

#include "core/type.h"
#include "core/input/scan_code.h"
#include "core/input/key_code.h"
#include "core/input/key_mod.h"

typedef u16 Key_Mod_Flags;

typedef struct Key_Sym {
    Scan_Code scan;
    Key_Code key;
    Key_Mod_Flags mod;
} Key_Sym;

// TODO: Finish input system - see https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input#keystroke-message-flags
