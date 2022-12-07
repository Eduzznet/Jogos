#include <math.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PI 3.1416

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
    Uint32 antes = SDL_GetTicks();
    Uint32 tempassado = 0; 
    int isevt = SDL_WaitEventTimeout(evt, *ms);
    if (isevt) {
        tempassado = SDL_GetTicks()-antes;
        if (tempassado > *ms) tempassado = *ms;
  	    *ms -= tempassado;
    }
    return isevt;	
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 400, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "boo.png");
    SDL_Texture* bg = IMG_LoadTexture(ren, "fundo.png");
    assert(img != NULL);

    /* EXECUÇÃO */
    double j=0;
    int raio=100;
    int circ=2*PI*raio;
    int cx=200;
    int cy=200;
    int es=50;
    int passo=-5;
    int gira=1;
    int continua=1;
    int isleft=1;
    Uint32 espera = es;
    SDL_Rect r = { cx+raio,cy, 50,50 };
    SDL_Rect c;
    c = (SDL_Rect) { 100,0, 100,80 };
    SDL_Rect fundo = {0,0,400,400};
    while (continua) {
        SDL_Event evt;
        int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (aux) {
            switch(evt.type){
                case SDL_KEYDOWN: 
                    gira= !gira;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    passo = -passo;
                    isleft= !isleft;
                    break;
                //Trata os eventos do AltF4 e de fechar a janela no 'X'
                case SDL_WINDOWEVENT: 
                    if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
                        continua=0;
                    break;
            }       
        }
        else {
            if(gira){
                espera = es;               
                SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
                SDL_RenderClear(ren);
                SDL_RenderCopy(ren, bg, NULL, &fundo);
                if (isleft) {
                    c = (SDL_Rect) {   0,0, 100,80 };
                } else {
                    c = (SDL_Rect) { 100,0, 100,80 };
                }
                SDL_RenderCopy(ren, img, &c, &r);
                SDL_RenderPresent(ren);
                r.x=raio*cos(j)+cx;
                r.y=raio*sin(j)+cy;
                j+=(PI/circ)*passo;
                if (j>(2*PI)) j=0;
            }
        }
    }
    /* FINALIZACAO */
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
