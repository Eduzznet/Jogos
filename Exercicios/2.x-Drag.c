#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>

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

int main(int argc, char ** argv)
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    int wh = 600;
    int ww = 600;
    int es = 100;
    SDL_Window* win = SDL_CreateWindow("Drag e Drop",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         ww, wh, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    /* EXECUÇÃO */
    int Mouse1Apertado = 0;
    int colisao = 0;
    int arrastando = 0;
    int printouDrag = 0;
    SDL_Point mousePos;
    SDL_Point clickAjuste;
    SDL_Rect r = { 300, 200, 100, 100 };
    char status[15];
    int xoriginal = r.x;
    int yoriginal = r.y;
    Uint32 espera = es;
    
    
    while (1)    {
        SDL_Event event;
        int aux = AUX_WaitEventTimeoutCount(&event, &espera);
        if (aux) {
            switch (event.type){
                case SDL_MOUSEBUTTONUP:
                    if (Mouse1Apertado && event.button.button == SDL_BUTTON_LEFT){
                        if(arrastando){
                            strcpy(status, "Solto");
                            printf("%s\n", status);
                        }
                        Mouse1Apertado = 0;
                        colisao = 0;
                        arrastando = 0;
                        printouDrag = 0;
                        xoriginal = r.x;
                        yoriginal = r.y;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (!Mouse1Apertado && event.button.button == SDL_BUTTON_LEFT){
                        Mouse1Apertado = 1;
                        if (SDL_PointInRect(&mousePos, &r)){
                            clickAjuste.x = mousePos.x - r.x;
                            clickAjuste.y = mousePos.y - r.y;
                            colisao = 1;
                            strcpy(status, "Clicado!");
                            printf("%s\n", status);
                            break;
                        }
                    }
                    break;
                case SDL_MOUSEMOTION:
                    mousePos.x = event.motion.x;
                    mousePos.y = event.motion.y;
                    if (Mouse1Apertado && colisao){   
                        arrastando=1;
                        if (!printouDrag){
                            strcpy(status, "Arrastando...");
                            printf("%s\n", status);
                            printouDrag=1;
                        }
                        r.x = mousePos.x - clickAjuste.x;
                        r.y = mousePos.y - clickAjuste.y;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        if (arrastando){
                            r.x = xoriginal;
                            r.y = yoriginal;
                            arrastando = 0;
                            colisao = 0;
                            printouDrag = 0;
                            strcpy(status, "Cancelado!");
                            printf("%s\n", status);
                            break;
                        }
                    }
            }
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
            SDL_RenderFillRect(ren, &r);
            SDL_RenderPresent(ren);
        }
        
        //Trata os eventos do AltF4 e de fechar a janela no 'X'
        if (event.type == SDL_WINDOWEVENT){
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    break;
        }    
    }
    
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
