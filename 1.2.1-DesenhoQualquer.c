#include <SDL2/SDL.h>

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
    SDL_RenderDrawPoint(ren, 100, 100);
    SDL_RenderDrawPoint(ren, 500, 120);
    SDL_RenderDrawPoint(ren, 600, 90);
    SDL_RenderDrawPoint(ren, 200, 220);
    SDL_RenderDrawPoint(ren, 350, 110);
    SDL_RenderDrawPoint(ren, 370, 300);
    SDL_RenderDrawPoint(ren, 400, 100);
    SDL_RenderDrawPoint(ren, 900, 50);
    SDL_RenderDrawPoint(ren, 830, 150);
    SDL_RenderDrawPoint(ren, 770, 50);
    SDL_RenderDrawPoint(ren, 700, 270);
    SDL_SetRenderDrawColor(ren, 0xFA,0x81,0x28,0x00);
    SDL_Rect r = { 350,270, 300,120 };
    SDL_RenderFillRect(ren, &r);
    SDL_Rect r2 = { 400,190, 200,80 };
    SDL_RenderFillRect(ren, &r2);
    SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
    SDL_Rect r3 = { 400,370, 80,80 };
    SDL_RenderFillRect(ren, &r3);
    SDL_Rect r4 = { 520,370, 80,80 };
    SDL_RenderFillRect(ren, &r4);
    SDL_RenderDrawLine(ren, 0, 430, 1000, 430);
    SDL_RenderDrawLine(ren, 10, 460, 90, 460);
    SDL_RenderDrawLine(ren, 180, 460, 270, 460);
    SDL_RenderDrawLine(ren, 360, 460, 450, 460);
    SDL_RenderDrawLine(ren, 550, 460, 640, 460);
    SDL_RenderDrawLine(ren, 730, 460, 820, 460);
    SDL_RenderDrawLine(ren, 910, 460, 1000, 460);
    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
