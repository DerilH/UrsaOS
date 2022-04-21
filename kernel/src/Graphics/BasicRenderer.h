#pragma once

#include "Screen.h"
#include "../SimpleFonts.h"

class BasicRenderer {
private: 
    Screen screen;
    unsigned int stdColor = 0x0000;
    PSF1_FONT *stdFont; 
public:
    BasicRenderer(){};
    BasicRenderer(Screen screen, PSF1_FONT *stdFont, unsigned int stdColor);

    void Print(const char *line, unsigned int xOff, unsigned int yOff);
    void Print(unsigned int color, const char *line, unsigned int xOff, unsigned int yOff);
    void Print(PSF1_FONT *font, unsigned int color, const char *line, unsigned int xOff, unsigned int yOff);

    bool PrintChar(char chr, unsigned int xOff, unsigned yOff);
    bool PrintChar(unsigned int color, char chr, unsigned int xOff, unsigned yOff);
    bool PrintChar(PSF1_FONT *font, unsigned int color, char chr, unsigned int xOff, unsigned yOff);

    void DrawUnderLine(unsigned int charSize, unsigned int xOff, unsigned int yOff);
    void DrawUnderLine(unsigned int charSize, unsigned int color, unsigned int xOff, unsigned int yOff);

    void Clear(unsigned int Width, unsigned int Height, unsigned int xOff, unsigned int yOff);
    void Clear(unsigned int color, unsigned int Width, unsigned int Height, unsigned int xOff, unsigned int yOff);
    
    void ClearBuffer();
    void ClearBuffer(unsigned int color);
};

extern BasicRenderer GlobalRenderer;