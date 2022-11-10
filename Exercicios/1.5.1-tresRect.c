#include <SDL2/SDL.h>
#include <assert.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animando e Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect r2 = { 60,20, 10,10 };
    SDL_Rect r3 = { 80,20, 10,10 };
    int mx,my;
    int espera = 500;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderFillRect(ren, &r2);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);
        if (isevt) {
            espera -= (SDL_GetTicks() - antes);
            if (espera < 0) {
                espera = 0;
            }
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        if(r.y>0){
                            r.y -= 5;
                        }
                        break;
                    case SDLK_DOWN:
                        if(r.y+r.h<100){
                            r.y += 5;
                        }
                        break;
                    case SDLK_LEFT:
                        if(r.x>0){
                            r.x -= 5;
                        }
                        break;
                    case SDLK_RIGHT:
                        if(r.x+r.w<200){
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
            espera = 500;
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
