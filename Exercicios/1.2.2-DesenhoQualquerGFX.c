#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Hello World!",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         1000, 600, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0x23,0x39,0x5d,0x00);
    SDL_Rect rc = { 0,0, 1000,430 };
    SDL_RenderFillRect(ren, &rc);
    SDL_SetRenderDrawColor(ren, 0xFC,0xEE,0xA7,0x00);
    filledTrigonRGBA(ren, 10, 100, 30, 100, 20, 50, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 10, 70, 30, 70, 20, 120, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 210, 220, 230, 220, 220, 170, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 210, 190, 230, 190, 220, 240, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 500, 120, 520, 120, 510, 70, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 500, 90, 520, 90, 510, 140, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 900, 50, 920, 50, 910, 0, 253, 253, 150, 255);
    filledTrigonRGBA(ren, 900, 20, 920, 20, 910, 70, 253, 253, 150, 255);    
    SDL_RenderDrawPoint(ren, 600, 90);
    SDL_RenderDrawPoint(ren, 350, 110);
    SDL_RenderDrawPoint(ren, 370, 300);
    SDL_RenderDrawPoint(ren, 400, 100);
    SDL_RenderDrawPoint(ren, 830, 150);
    SDL_RenderDrawPoint(ren, 770, 50);
    SDL_RenderDrawPoint(ren, 700, 270);
    boxRGBA(ren, 0, 430, 1000, 600, 128, 128, 128, 255);
    SDL_RenderDrawLine(ren, 0, 430, 1000, 430);
    SDL_SetRenderDrawColor(ren, 0xFA,0x81,0x28,0x00);
    SDL_Rect r = { 350,270, 300,120 };
    SDL_RenderFillRect(ren, &r);
    SDL_Rect r2 = { 400,190, 200,80 };
    SDL_RenderFillRect(ren, &r2);
    filledCircleRGBA(ren, 420, 390, 50, 0, 0, 0, 255);
    filledCircleRGBA(ren, 570, 390, 50, 0, 0, 0, 255);
    thickLineRGBA(ren, 10, 460, 90, 460, 5, 255, 255, 0, 255);
    thickLineRGBA(ren, 180, 460, 270, 460, 5, 255, 255, 0, 255);
    thickLineRGBA(ren, 360, 460, 450, 460, 5, 255, 255, 0, 255);
    thickLineRGBA(ren, 550, 460, 640, 460, 5, 255, 255, 0, 255);
    thickLineRGBA(ren, 730, 460, 820, 460, 5, 255, 255, 0, 255);
    thickLineRGBA(ren, 910, 460, 1000, 460, 5, 255, 255, 0, 255);
    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
