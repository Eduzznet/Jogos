#include <assert.h>
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
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect r2 = { 60,20, 10,10 };
    SDL_Rect r3 = { 80,20, 10,10 };
    int mx,my;
    Uint32 espera = es;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderFillRect(ren, &r2);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (aux) {
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        if(r.y>0){
                            r.y -= 5;
                        }
                        break;
                    case SDLK_DOWN:
                        if(r.y+r.h<wh){
                            r.y += 5;
                        }
                        break;
                    case SDLK_LEFT:
                        if(r.x>0){
                            r.x -= 5;
                        }
                        break;
                    case SDLK_RIGHT:
                        if(r.x+r.w<ww){
                            r.x += 5;
                        }
                        break;
                }    
            }
            if (evt.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mx, &my);
                r3.x=mx;
                r3.y=my;
            }         
        }
        else {
            espera = es;            
            r2.x += 2;
            r2.y += 2;
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
