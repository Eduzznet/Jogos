#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Event evt;
    SDL_Rect r[11];
    int cor[11][3]={0,0,255, 255,0,0, 0,255,0, 255,255,0, 0,255,255, 255,0,255, 
                    249,92,0, 0,90,0, 102,51,51, 255,153,204, 0,0,0};
    r[0].x=40;
    r[0].y=20;
    r[0].h=10;
    r[0].w=10;
    int i=0;
    int mx,my;
    while (1) {
        SDL_SetRenderDrawColor(ren, 255,255,255,0);
        SDL_RenderClear(ren);
        if(evt.type != SDL_MOUSEMOTION){
            for (int j=0;j<=i;j++){
                SDL_SetRenderDrawColor(ren, cor[j][0],cor[j][1],cor[j][2],0);
                SDL_RenderFillRect(ren, &r[j]);
                SDL_RenderPresent(ren);
            }
        }
        SDL_WaitEvent(&evt);
        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    if(r[0].y>0){
                        r[0].y -= 5;
                    }
                    break;
                case SDLK_DOWN:
                    if(r[0].y+r[0].h<100){
                        r[0].y += 5;
                    }
                    break;
                case SDLK_LEFT:
                    if(r[0].x>0){
                        r[0].x -= 5;
                    }
                    break;
                case SDLK_RIGHT:
                    if(r[0].x+r[0].w<200){
                        r[0].x += 5;
                    }
                    break;
            }
        }
        if (evt.type == SDL_MOUSEBUTTONDOWN){
            if (evt.button.button == SDL_BUTTON_LEFT){
                SDL_GetMouseState(&mx, &my); 
                if (i<10){
                    i++;
                    r[i].x=mx;
                    r[i].y=my;
                    r[i].h=10;
                    r[i].w=10;
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
