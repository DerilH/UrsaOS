#include "Terminal.h"
#include "cstr.h"

Terminal::Terminal(unsigned int SpaceWidth, unsigned int SpaceHeight, Point SpaceCenter, PSF1_FONT *stdFont, unsigned int stdColor, BasicRenderer *renderer)
{
    this->WindowWidth = SpaceWidth;
    this->WindowHeight = SpaceHeight;
    this->WindowCenter = SpaceCenter;
    this->stdFont = stdFont;
    this->stdColor = stdColor;

    this->startRow = SpaceCenter.y - SpaceHeight / 2;
    this->startCol = SpaceCenter.x - SpaceWidth / 2;
    this->currentRow = this->startRow;
    this->currentCol = this->startCol;

    this->renderer = renderer;
    this->renderer->DrawUnderLine(8, this->currentCol, this->currentRow);
}

void Terminal::PrintChar(char chr)
{
    if (chr == '\n')
    {
        this->EndL();
        return;
    }
    else if (this->currentCol >= this->startCol + WindowWidth)
    {
        this->EndL();
    }
    if (this->currentRow >= this->startRow + this->WindowHeight)
    {
        return;
    }
    this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);
    this->renderer->PrintChar(chr, this->currentCol, this->currentRow);
    this->currentCol += this->charSize;
    this->renderer->DrawUnderLine(this->charSize, this->currentCol, this->currentRow);
}

void Terminal::Print(const char *line)
{
    char *temp = (char *)line;
    while (*temp != '\0')
    {
        if (*temp == '\n')
        {
            this->EndL();
            temp++;
            continue;
        }
        if (this->currentCol >= this->startCol + WindowWidth)
        {
            this->EndL();
        }
        if (this->currentRow >= this->startRow + this->WindowHeight)
            return;
        this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);
        this->renderer->PrintChar(*temp, this->currentCol, this->currentRow);
        this->currentCol += this->charSize;
        temp++;
    }
    this->renderer->DrawUnderLine(this->charSize, this->currentCol, this->currentRow);
}

void Terminal::Print(const char *line, unsigned int color)
{
    char *temp = (char *)line;
    while (*temp != '\0')
    {
        if (*temp == '\n')
        {
            this->EndL();
            temp++;
            continue;
        }
        if (this->currentCol >= this->startCol + WindowWidth)
        {
            this->EndL();
        }
        if (this->currentRow >= this->startRow + this->WindowHeight)
            return;
        this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);
        this->renderer->PrintChar(color, *temp, this->currentCol, this->currentRow);
        this->currentCol += this->charSize;
        temp++;
    }
    this->renderer->DrawUnderLine(this->charSize, this->stdColor, this->currentCol, this->currentRow);
}

void Terminal::EndL()
{
    this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);

    this->currentRow += this->charSize * 2;
    this->currentCol = this->startCol;

    this->renderer->DrawUnderLine(this->charSize, this->currentCol, this->currentRow);
}

void Terminal::Back()
{
    if (this->currentCol <= 0 & !(this->currentRow - 8 <= 0))
    {
        this->currentRow -= 16;
        this->currentCol = this->WindowCenter.x + this->WindowWidth;
    }
    else
        this->currentCol -= 8;
}

void Terminal::Backspace()
{
    this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);
    Back();
    this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);
    this->renderer->DrawUnderLine(this->charSize, this->currentCol, this->currentRow);
}

void Terminal::Error(const char *message)
{
    this->EndL();

    char *temp = (char *)message;
    while (*temp != '\0')
    {
        if (*temp == '\n')
        {
            this->EndL();
            temp++;
            continue;
        }
        if (this->currentCol >= this->startCol + WindowWidth)
        {
            this->EndL();
        }
        if (this->currentRow >= this->startRow + this->WindowHeight)
            return;
        this->renderer->Clear(this->charSize, this->charSize, this->currentCol, this->currentRow);
        this->renderer->PrintChar(this->stdFont, 0xff0000, *temp, this->currentCol, this->currentRow);
        this->currentCol += this->charSize;
        temp++;
    }
    this->EndL();
};

void Terminal::PutLine(const char *line, unsigned int xOff, unsigned int yOff)
{
    this->renderer->Print(this->stdFont, this->stdColor, line, xOff, yOff);
}

void Terminal::BufferSize(unsigned int xOff, unsigned int yOff)
{
    this->PutLine(to_string(strlen(this->textBuffer)), xOff, yOff);
}

void Terminal::DrawBuffer()
{
    char *buffer = this->textBuffer;

    this->renderer->Clear(this->WindowWidth, this->WindowHeight, this->startCol, this->startRow);
    this->renderer->DrawUnderLine(8, 0xffffff, this->startCol, this->startRow);
    while (*buffer != '\0')
    {

        if (*this->textBuffer == '\n')
        {
            this->currentRow += this->charSize * 2;
            this->currentCol = this->startCol;
            continue;
        }
        else if (this->currentCol >= WindowCenter.x + WindowWidth)
        {
            this->currentRow += this->charSize * 2;
            this->currentCol = this->startCol;
        }
        this->renderer->PrintChar(this->stdFont, this->stdColor, *buffer, this->currentCol, this->currentRow);
        this->currentCol += this->charSize;
        buffer++;
    }
}
