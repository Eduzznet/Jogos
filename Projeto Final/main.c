#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int MAXENT = 20;

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

void remover_elemento(SDL_Rect *array, int index)
{
   int i;
   for(i = index; i < MAXENT; i++) array[i] = array[i + 1];
}

void spawn (SDL_Rect *array, int aux){
    int gen;
    SDL_Rect RGrande = {10,rand() % 500, 30,30};
    SDL_Rect RPequeno = {10,rand() % 500, 10,10};
    gen = rand() % 2;
                    switch (gen){
                        case 0:
                            array[aux] = RGrande;
                            break;
                        case 1:
                            array[aux] = RPequeno;
                            break;    
                    }
}

void split (SDL_Rect *array, int index, SDL_Rect r, int count){
    int i;
    SDL_Rect RPequeno = {r.x, r.y , 10,10};
    remover_elemento(array, index);
    count-=1;
    for(i=0; i<3; i++){
        RPequeno.x -= rand()% 20-19 - i*10;
        RPequeno.y += rand()% 20-10 + i*5;
        array[count+i] = RPequeno;
    }
}

int PointInRect (SDL_Point* p, SDL_Rect* r){
    return!(p->x < r->x ||
    p->y < r->y ||
    p->x > r->x+r->w ||
    p->y > r->y+r->h );
}

void render_text(
    SDL_Renderer *renderer,
    const char *text,
    TTF_Font *font,
    SDL_Rect *rect,
    SDL_Color *color
) {
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderText_Blended(font, text, *color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    //rect->w = surface->w;
    //rect->h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}

typedef enum {
    MENU,
    RUNNING,
    GAMEOVER,
} Game_State;

Game_State Game = MENU;

typedef enum {
    READY,
    SHOOTING,
    RELOADING,
} Gun_State;

Gun_State Gun = READY;

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    TTF_Init();
    int wh = 600;
    int ww = 600;
    int es = 100;
    SDL_Window* win = SDL_CreateWindow("Projeto",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         ww, wh, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "img/aim.png");
    SDL_Texture* bg = IMG_LoadTexture(ren, "img/space.png");
    SDL_Texture* ast = IMG_LoadTexture(ren, "img/asteroid.png");
    SDL_Texture* mn = IMG_LoadTexture(ren, "img/menu.png");
    SDL_Texture* bp = IMG_LoadTexture(ren, "img/play.png");
    SDL_Texture* bq = IMG_LoadTexture(ren, "img/quit.png");
    assert(img != NULL);
    assert(bg != NULL);
    assert(ast != NULL);
    assert(mn != NULL);
    assert(bp != NULL);
    assert(bq != NULL);

    TTF_Font* fnt = TTF_OpenFont("img/LCDM2N__.ttf", 40);
    assert(fnt != NULL);
    SDL_Color clr = {0x00,0xFF,0x00,0xFF};
    //char PontosString[128];
    /*SDL_Surface* sfc1;
    SDL_Surface* sfc2;
    SDL_Surface* sfc3;
    SDL_Texture* score;
    SDL_Texture* texX;
    SDL_Texture* combo;*/
    //int Points=0;
    /*sprintf(PontosString, "%d", Points);
    sfc = TTF_RenderText_Solid(fnt, PontosString, clr);
    assert(sfc != NULL);
    SDL_Texture* score = SDL_CreateTextureFromSurface(ren, sfc);
    assert(score != NULL);
    SDL_FreeSurface(sfc);*/

    /* EXECUÇÃO */
    time_t t;
    srand((unsigned) time(&t));
    SDL_Rect Rects [MAXENT];// = { 40,20, 10,10,  60,300, 30,30,  20,500, 10,10 };
    //Rects[4] = teste;
    char PontosString[128];
    int Points=0;
    int entcout = 0;
    int gen;
    int i = 0;
    int cooldown = 0;
    int entrespawn = 0 ;
    int combocount = 0;
    char comboString[128];
    int hit = 0;
    SDL_Rect comboRect = {27,40, 20,20};
    SDL_Rect fundoM = {0,0, ww, wh}; 
    SDL_Rect fundo = {0,0, ww, wh};
    SDL_Rect Mira = { 250,100, 0,0 };
    SDL_Rect recorteMira;
    SDL_Rect BotaoStart = {150, 100, 300, 50};
    SDL_Rect BotaoQuit = {150, 200, 300, 50};
    SDL_Rect Terra = {0,wh-75, ww, 200};
    SDL_Rect ScoreRect = {5,5, 30,30};
    SDL_Rect xRect ={10,45, 15,15};
    //SDL_Rect RGrande = {10,rand() % 500, 30,30};
    //SDL_Rect RPequeno = {10,rand() % 500, 10,10};
    //printf("%d", Rects[0].x);
    SDL_Point mousePos;
    int mx,my;
    Uint32 espera = es;
    while (1) {
        SDL_Event evt;
        int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (Game == MENU){
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
            SDL_RenderCopy(ren, mn, NULL, &fundoM);
            SDL_RenderCopy(ren, bp, NULL, &BotaoStart);
            SDL_RenderCopy(ren, bq, NULL, &BotaoQuit);
            //SDL_RenderFillRect(ren, &BotaoStart);
            //SDL_RenderFillRect(ren, &BotaoQuit);
            SDL_RenderPresent(ren);
            switch (evt.type){
                case SDL_MOUSEBUTTONUP:
                    if (evt.button.button == SDL_BUTTON_LEFT){
                        if (PointInRect(&mousePos, &BotaoStart)){
                            Game = RUNNING;
                            Gun=READY;
                        }
                        if (PointInRect(&mousePos, &BotaoQuit)){
                            /* FINALIZACAO */
                            SDL_DestroyRenderer(ren);
                            SDL_DestroyWindow(win);
                            SDL_Quit();
                            return 0;
                        }
                    }
                    break;
                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&mx, &my);
                    mousePos.x = mx;
                    mousePos.y = my;
                    break;
            }
            //Trata os eventos do AltF4 e de fechar a janela no 'X'
            if (evt.type == SDL_WINDOWEVENT){
                if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
                        break;
            }
        }
        if (Game == RUNNING){
            SDL_ShowCursor(SDL_DISABLE);
            Mira.w=40;
            Mira.h=40;
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
            SDL_RenderCopy(ren, bg, NULL, &fundo);

            sprintf(PontosString, "%d", Points);
            ScoreRect.w = 30*strlen(PontosString);
            render_text(ren, PontosString, fnt, &ScoreRect, &clr);

            /*sfc1 = TTF_RenderText_Solid(fnt, PontosString, clr);
            assert(sfc1 != NULL);
            score = SDL_CreateTextureFromSurface(ren, sfc1);
            assert(score != NULL);
            SDL_FreeSurface(sfc1);*/

            sprintf(comboString, "%d", combocount);
            render_text(ren, comboString, fnt, &comboRect, &clr);
            /*sfc2 = TTF_RenderText_Solid(fnt, comboString, clr);
            assert(sfc2 != NULL);
            combo = SDL_CreateTextureFromSurface(ren, sfc2);
            assert(combo != NULL);
            SDL_FreeSurface(sfc2);*/

            render_text(ren, "x", fnt, &xRect, &clr);
            /*sfc3 = TTF_RenderText_Solid(fnt, "x", clr);
            assert(sfc3 !=NULL);
            texX = SDL_CreateTextureFromSurface(ren, sfc3);
            assert(texX !=NULL);
            SDL_FreeSurface(sfc3);*/

            //SDL_RenderCopy(ren,score,NULL,&ScoreRect);
            //SDL_RenderCopy(ren,combo, NULL,&comboRect);
            //SDL_RenderCopy(ren,texX, NULL,&xRect);
            //SDL_DestroyTexture(score);
           // SDL_DestroyTexture(texX);
            //SDL_DestroyTexture(combo);*/
            for (i=0; i<entcout; i++) SDL_RenderCopy(ren, ast, NULL, &Rects[i]);//SDL_RenderFillRect(ren, &Rects[i]);
            if (Gun==READY || Gun==SHOOTING){
                recorteMira = (SDL_Rect) {0,0, 512,512};
            }else {
                recorteMira = (SDL_Rect) {512,0, 512,512};
            }
            SDL_RenderCopy(ren, img, &recorteMira, &Mira);
            //SDL_RenderFillRect(ren, &Terra);
            SDL_RenderPresent(ren);
       
            //SDL_Event evt;
            //int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
            if (aux) {
                if (evt.type == SDL_MOUSEMOTION){
                    SDL_GetMouseState(&mx, &my);
                    mousePos.x = mx;
                    mousePos.y = my;
                    Mira.x = mx - (Mira.w/2);
                    Mira.y = my - (Mira.h/2);
                }
                if (evt.type == SDL_MOUSEBUTTONDOWN){
                    if (evt.button.button == SDL_BUTTON_LEFT && Gun == READY){
                        Gun = SHOOTING;
                        hit = 0; 
                        for (i=0; i<entcout; i++){
                            //if (!(SDL_HasIntersection(&Mira, &Rects[i])) && Gun == SHOOTING){
                            //    combocount = 0;   
                            //}
                            if (SDL_HasIntersection(&Mira, &Rects[i]) && Gun == SHOOTING){
                                Gun = RELOADING;
                                combocount+=1;
                                hit = 1;
                                if (Rects[i].h>10){
                                    Points+=10*combocount;
                                    split (Rects, i, Rects[i], entcout);
                                    entcout+=2;        
                                }
                                else{
                                    remover_elemento(Rects, i);
                                    entcout-=1;
                                    Points+=5*combocount;
                                }          
                            }       
                        }
                        if (hit == 0){
                            combocount = 0;
                        }
                        Gun = RELOADING;
                    }
                }
                if (evt.type == SDL_KEYDOWN){
                    if (evt.key.keysym.sym == SDLK_ESCAPE){
                        Game = MENU;
                    }
                }
            }
            else {       
                //espera = es;        
                //entrespawn++;
                if (Gun == RELOADING){
                    cooldown++;
                    if (cooldown==5){
                        Gun = READY;
                        cooldown=0;
                    }
                }
                //espera=es;
                //entrespawn=0;
                //for (i=0; i<3; i++){
                entrespawn= rand() % 100;
                if(entrespawn>95 && entcout<10){
                    spawn(Rects, entcout);
                    entcout++;
                }
                //}
                //if (entrespawn==5 && entcout<10){
                //while (entcout<10){
                /*    gen = rand() % 2;
                    switch (gen){
                        case 0:
                            Rects[entcout] = RGrande;
                            entcout+=1;
                            break;
                        case 1:
                            Rects[entcout] = RPequeno;
                            entcout+=1;
                            break;    
                    }
                }*/
                //}
                espera=es;
                for (i=0; i<entcout; i++){
                    if (Rects[i].h==10){          
                        Rects[i].x += 2;
                        Rects[i].y += 2;
                    }
                    if (Rects[i].h>10){
                        Rects[i].x +=2;
                        Rects[i].y +=2;
                    }
                    if (Rects[i].x + Rects[i].w > ww || SDL_HasIntersection(&Terra, &Rects[i])){//Rects[i].y + Rects[i].h > wh){
                        remover_elemento(Rects, i);
                        entcout-=1;
                        combocount = 0;
                    }
                }
            }
            //printf("%d", Gun);
            //Trata os eventos do AltF4 e de fechar a janela no 'X'
            if (evt.type == SDL_WINDOWEVENT){
                if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
                        break;
            }
        }
    }

    /* FINALIZACAO */
    //SDL_DestroyTexture(score);
    //SDL_DestroyTexture(texX);
    //SDL_DestroyTexture(combo);
    TTF_CloseFont(fnt);
    SDL_DestroyTexture(img);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(ast);
    SDL_DestroyTexture(mn);
    SDL_DestroyTexture(bp);
    SDL_DestroyTexture(bq);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
