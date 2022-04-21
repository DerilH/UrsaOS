#include "keyboard.h"
#include "../../Terminal.h"

    bool isLeftShiftPressed;
    bool isRightShiftPressed;

void HandleKeyboard(uint8_t scancode, Terminal *terminal){

    switch (scancode){
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
        case Enter:
            terminal->EndL();
            return;
        case Spacebar:
            terminal->Print(" ");
            return;
        case BackSpace:
           terminal->Backspace();
           return;
    }

    char chr = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);
    if (chr != 0){
        terminal->PrintChar(chr);
    }
}