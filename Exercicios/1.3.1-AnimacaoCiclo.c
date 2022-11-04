#include <SDL2/SDL.h>
#include <math.h>

#define PI 3.1416

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 400, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    int i=0;
    int x,y,z;
    double j;
    int raio=100;
    int circ=2*PI*raio;
    int cx=200;
    int cy=200;
    int dy=cy;
    SDL_Rect r = { cx+raio,cy, 10,10 };
    while (i < 2) {
        for(j=0;j<=(2*PI);j+=(PI/circ)){    
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
            SDL_RenderFillRect(ren, &r);
            SDL_RenderPresent(ren);
            x=raio*cos(j);
            y=raio*sin(j);
            r.x=x+cx;
            r.y=y+cy;
            SDL_Delay(50);
        }
        i++;
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
