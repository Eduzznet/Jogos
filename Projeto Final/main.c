#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

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

void remover_elemento(SDL_Rect *array, int index, int array_length)
{
   int i;
   for(i = index; i < array_length - 1; i++) array[i] = array[i + 1];
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    int wh = 600;
    int ww = 600;
    int es = 100;
    SDL_Window* win = SDL_CreateWindow("Animando e Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         ww, wh, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect Rects [3] = { 40,20, 10,10,  60,300, 10,10,  20,500, 10,10 };
    SDL_Rect Mira = { 80,20, 40,40 };
    //printf("%d", Rects[0].x);
    SDL_Point mousePos;
    int mx,my;
    Uint32 espera = es;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        for (int i=0; i<3; i++) SDL_RenderFillRect(ren, &Rects[i]);
        SDL_RenderFillRect(ren, &Mira);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (aux) {
            if (evt.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mx, &my);
                mousePos.x = mx;
                mousePos.y = my;
                Mira.x=mx - (Mira.w/2);
                Mira.y=my - (Mira.h/2);
            }
            if (evt.type == SDL_MOUSEBUTTONDOWN){ 
                if (evt.button.button == SDL_BUTTON_LEFT){
                    if (SDL_HasIntersection(&Mira, &Rects[0]))
                        remover_elemento(Rects, 0, 3);
                        //Rects[0].w+=5;        
                }
            }
        }
        //Trata os eventos do AltF4 e de fechar a janela no 'X'
        if (evt.type == SDL_WINDOWEVENT){ 
            if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
                    break;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
