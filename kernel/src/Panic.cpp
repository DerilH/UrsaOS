#include "Panic.h"
#include "Graphics/BasicRenderer.h"

void Panic(const char *message){
    GlobalRenderer.ClearBuffer(0x0000FF);
    GlobalRenderer.Print("Fatal error ocurred. You PC need to restart", 0, 0);
    GlobalRenderer.Print(message, 0, 16);
}