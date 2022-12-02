#include <assert.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

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
    TTF_Init();
    int wh = 600;
    int ww = 600;
    int es = 100;
    char podio[3][10] = {"Primeiro", "Segundo", "Terceiro"};
    SDL_Window* win = SDL_CreateWindow("Animando e Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         ww, wh, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    TTF_Font* fnt = TTF_OpenFont("arial.ttf", 40);
    assert(fnt != NULL);
    SDL_Color clr = {0x00,0x00,0x00,0xFF};
    SDL_Surface* sfc;
    SDL_Texture* txt;

    /* EXECUÇÃO */
    start: ;
    SDL_Rect r = { 0,20, 10,10 };
    SDL_Rect r2 = { 0,300, 10,10 };
    SDL_Rect r3 = { 0,580, 10,10 };
    SDL_Rect cheg = { 580,0, 20,600 };
    int vermchegou = 0;
    int verdchegou = 0;
    int azulchegou = 0;
    int chegou = 0;
    int i;
    int mx,my;
    Uint32 espera = es;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xD3,0xD3,0xD3,0x00);
        SDL_RenderFillRect(ren, &cheg);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_SetRenderDrawColor(ren, 0x00,0xFF,0x00,0x00);
        SDL_RenderFillRect(ren, &r2);
        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r3);
       
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (aux) {
            if (evt.type == SDL_KEYDOWN) {
                if (!SDL_HasIntersection(&r, &cheg)){
                    switch (evt.key.keysym.sym) {
                        case SDLK_LEFT:
                            if(r.x>0){
                                r.x -= 5;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(r.x+r.w<cheg.x+1){
                                r.x += 2;
                            }
                            break;
                    }
                }
                else{
                    if (azulchegou==0){
                        strcpy(podio[chegou],"Azul");
                        chegou+=1;
                        azulchegou=1;
                    }
                }  
            }
            if (!SDL_HasIntersection(&r3, &cheg)){
                if (evt.type == SDL_MOUSEMOTION && r3.x+r.w<cheg.x+1){
                    SDL_GetMouseState(&mx, &my);
                    r3.x=mx;
                }
            }
            else{
                if (vermchegou==0){
                    strcpy(podio[chegou],"Vermelho");
                    chegou+=1;
                    vermchegou=1;
                }            
            }          
        }
        else {
            espera = es;
            if (!SDL_HasIntersection(&r2, &cheg) && r2.x+r.w<cheg.x+1){            
                r2.x += 5;
            }
            else{
                if (verdchegou==0){
                    strcpy(podio[chegou],"Verde");
                    chegou+=1;
                    verdchegou=1;
                }
            }
        }
        if (chegou == 3){
                //printf("%s ganhou!!!\n", podio[0]);
                SDL_Delay(500);
                SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
                SDL_RenderClear(ren);
                SDL_Surface* sfc = TTF_RenderText_Blended(fnt, strcat (podio[0], " venceu!!!"), clr);
                assert(sfc != NULL);
                SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sfc);
                assert(txt != NULL);
                SDL_FreeSurface(sfc);
                SDL_Rect t = { 200,270, 200,30 };
                SDL_RenderCopy(ren, txt, NULL, &t);
                SDL_RenderPresent(ren);
                SDL_Delay(5000);
                goto start;
        }
        //Trata os eventos do AltF4 e de fechar a janela no 'X'
        if (evt.type == SDL_WINDOWEVENT){
            if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
                    break;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(txt);
    TTF_CloseFont(fnt);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
