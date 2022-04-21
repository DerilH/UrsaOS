#pragma once 
#include "SimpleFonts.h"
#include "Graphics/BasicRenderer.h"
#include "Utility/Point.h"

class Terminal {
public:
    PSF1_FONT *stdFont;
    unsigned int stdColor;
    unsigned int startRow, startCol, currentRow, currentCol;
    unsigned int WindowWidth, WindowHeight;
    char *textBuffer = (char*)"";
    unsigned int charSize = 8;
    Point WindowCenter = Point(0,0);
    BasicRenderer *renderer;

    Terminal(){};
    Terminal(unsigned int SpaceWidth, unsigned int SpaceHeight, Point SpaceCenter, PSF1_FONT *stdFont, unsigned int stdColor, BasicRenderer *renderer);

    void Print(const char *chr);
    void Print(const char *chr, unsigned int color);

    void PrintChar(char chr);

    void EndL();
    void Backspace();
    void Back();

    void PutLine(const char* line, unsigned int xOff, unsigned int yOff);
    void Error(const char *message);
    
    void BufferSize(unsigned int xOff, unsigned int yOff);
private:
    void DrawBuffer();

};