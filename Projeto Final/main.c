#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int MAXENT = 60;
int wh = 600;
int ww = 600;

typedef enum {
    FALLING,
    CRASHED,
    DESTROYED,
} Target_State;

typedef enum {
    AST,
    SAT,
} Target_Type;

struct Target
{
    int vx,vy,vr;
    struct SDL_Rect Rect;
    int value;
    Target_Type type;
    int animCount; 
    Target_State state;
    int crashcount; 
    float angle;
};

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
    Uint32 antes = SDL_GetTicks();
    Uint32 tempassado = 0;
    SDL_FlushEvent(SDL_MOUSEMOTION);
    int isevt = SDL_WaitEventTimeout(evt, *ms);
    if (isevt) {
        tempassado = SDL_GetTicks()-antes;
        if (tempassado > *ms) tempassado = *ms;
        *ms -= tempassado;
    }
    return isevt;
}

void remover_elemento(struct Target *array, int index)
{
   int i;
   for(i = index; i < MAXENT; i++) array[i] = array[i + 1];
}

void spawn (struct Target *array, int aux){
    int gen;
    int sx = rand() % 2;
    struct Target RGrande = {2-(4*sx),3,0, 10,rand() % 300, 30,30, 10, AST, 0, FALLING, 0};
    struct Target RPequeno = {2-(4*sx),3,0, 10,rand() % 300, 15,15, 5, AST, 0, FALLING, 0};
    struct Target Satelite = {2,0,0, 10,rand() % 400, 25,25, -10, SAT, 0, FALLING, 0};
    if (RGrande.vx<0){
        RGrande.Rect.x=570;
    }
    if (RPequeno.vx<0){
        RPequeno.Rect.x=590;
    }
    gen = rand() % 100;
    if (gen<50){
        array[aux] = RGrande;
    } else if (gen<95)
    {
        array[aux] = RPequeno;
    } else {
        array [aux] = Satelite;
    }            
}

int min(int num1, int num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}

void split (struct Target *a, int i, int count){
    int j;
    struct Target RPeq = {a[i].vx, 
                          a[i].vy,
                          a[i].vr, 
                          a[i].Rect.x, 
                          a[i].Rect.y , 
                         (a[i].Rect.w)/2,
                         (a[i].Rect.h)/2, 
                         (a[i].value)/2, 
                         AST, 
                         a[i].animCount, 
                         a[i].state, 
                         a[i].crashcount};
    remover_elemento(a, i);
    count-=1;
    for(j=0; j<3; j++){
        RPeq.Rect.x -= rand()% 20-19 - j*10;
        RPeq.Rect.y += rand()% 20-10 + j*5;
        a[count+j] = RPeq;
    }
}

int PointInRect (SDL_Point* p, SDL_Rect* r){
    return!(p->x < r->x ||
    p->y < r->y ||
    p->x > r->x+r->w ||
    p->y > r->y+r->h );
}

void saveRecord (char *rec){
    FILE *fp;
    fp = fopen ("record", "w+");
    fputs(rec, fp);
    fclose(fp);
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
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}

typedef enum {
    MENU,
    RUNNING,
    PAUSED,
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
    Uint32 es = 100;
    SDL_Window* win = SDL_CreateWindow("IMPACT",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         ww, wh, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "img/aim.png");
    SDL_Texture* bg = IMG_LoadTexture(ren, "img/space.png");
    SDL_Texture* mn = IMG_LoadTexture(ren, "img/menu.png");
    SDL_Texture* bp = IMG_LoadTexture(ren, "img/play.png");
    SDL_Texture* bq = IMG_LoadTexture(ren, "img/quit.png");
    SDL_Texture* go = IMG_LoadTexture(ren, "img/gameover.png");
    SDL_Texture* pa = IMG_LoadTexture(ren, "img/playAgain.png");
    SDL_Texture* mM = IMG_LoadTexture(ren, "img/mainMenu.png");
    SDL_Texture* ct = IMG_LoadTexture(ren, "img/continue.png");
    SDL_Texture* st = IMG_LoadTexture(ren, "img/satelite.png");
    SDL_Texture* astanim = IMG_LoadTexture(ren, "img/asteroidsAnim.png");
    assert(img != NULL);
    assert(bg != NULL);
    assert(mn != NULL);
    assert(bp != NULL);
    assert(bq != NULL);
    assert(go != NULL);
    assert(pa != NULL);
    assert(mM != NULL);
    assert(ct != NULL);
    assert(st !=NULL);
    assert(astanim != NULL);

    TTF_Font* fnt = TTF_OpenFont("img/LCDM2N__.ttf", 40);
    TTF_Font* fntUltra = TTF_OpenFont("img/LCDMono2 Ultra.ttf", 40);
    assert(fnt != NULL);
    SDL_Color clr = {0x00,0xFF,0x00,0xFF};
    SDL_Color clrPop = {30, 0, 0, 255};
    SDL_Color clrblue = {2, 255, 216, 255};

    /* EXECUÇÃO */
    time_t t;
    srand((unsigned) time(&t));
    struct Target Rects [MAXENT];
    SDL_Rect Recortes [7]= {19,89,84,76, 100,45,135,145, 240,13,164,164, 400,13,167,161, 570,13,166,163, 735,47,124,120, 860,25,165,147};

    int Points=0;
    int entcout = 0;
    int gen;
    int i = 0;
    int running = 1;
    int cooldown = 0;
    int entrespawn = 0 ;
    int combocount = 0;
    float popCount = 8016235520;
    char record[10]="0";
    char PontosString[10];
    char comboString[10];
    char PopString [10];
    int hit = 0;
    int Level = 0;
    SDL_Rect PopRect = {440,530, 150,30};
    SDL_Rect comboRect = {27,40, 20,20};
    SDL_Rect fundoM = {0,0, ww, wh}; 
    SDL_Rect fundo = {0,0, ww, wh};
    SDL_Rect Mira = { 250,100, 0,0 };
    SDL_Rect recorteMira;
    SDL_Rect BotaoStart = {150, 250, 300, 50};
    SDL_Rect BotaoQuit = {150, 350, 300, 50};
    SDL_Rect BotaoQuit2 = {175, 355, 250, 80};
    SDL_Rect BotaoPA = {150,150, 300, 80};
    SDL_Rect BotaoMM = {150,245, 300, 80};
    SDL_Rect Terra = {0,wh-75, ww, 200};
    SDL_Rect ScoreRect = {5,5, 30,30};
    SDL_Rect RecordRect = {315,145, 30,40};
    SDL_Rect ResultRect = {230,550, 30,40};
    SDL_Rect ResultTextRect = {30,550, 200,40};
    SDL_Rect xRect ={10,45, 15,15};
    SDL_Rect newRecordRect = {390,5, 200,50};
    SDL_Point mousePos;
    int mx,my;
    Uint32 espera = es;
    Uint32 antes = 0;

    FILE *fp;
    fp = fopen ("record", "r+");
    fgets(record, 10, fp);
    fclose(fp);

    while (running) {
        SDL_Event evt;
        int aux = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (Game == MENU){
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
            SDL_RenderCopy(ren, mn, NULL, &fundoM);
            RecordRect.w = 15*strlen(record);
            render_text(ren ,record ,fnt ,&RecordRect, &clrblue);
            SDL_RenderCopy(ren, bp, NULL, &BotaoStart);
            SDL_RenderCopy(ren, bq, NULL, &BotaoQuit);
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
                            running = 0;
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
                        running=0;
            }
        }
        if (Game == GAMEOVER){
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
            SDL_RenderCopy(ren, go, NULL, &fundoM);
            SDL_RenderCopy(ren, pa, NULL, &BotaoPA);
            SDL_RenderCopy(ren, mM, NULL, &BotaoMM);
            SDL_RenderCopy(ren, bq, NULL, &BotaoQuit2);
            ResultRect.w = 30*strlen(PontosString);
            render_text(ren, PontosString, fnt, &ResultRect, &clrblue);
            if (atoi(PontosString)>=atoi(record)){
                render_text(ren, "NEW RECORD:", fnt, &ResultTextRect, &clrblue);
                strcpy(record, PontosString);
                saveRecord(record);
            } else {
                render_text(ren, "YOUR SCORE:", fnt, &ResultTextRect, &clrblue);
            }
            SDL_RenderPresent(ren);
            combocount = 0;
            popCount = 8016235520;
            Points = 0;
            for (i=0; i<entcout; i++){
                remover_elemento(Rects, i);
            }
            entcout=0;
            switch (evt.type){
                case SDL_MOUSEBUTTONUP:
                    if (evt.button.button == SDL_BUTTON_LEFT){
                        if (PointInRect(&mousePos, &BotaoPA)){
                            Game = RUNNING;
                            Gun=READY;
                        }
                        if (PointInRect(&mousePos, &BotaoMM)){
                            Game = MENU;
                        }
                        if (PointInRect(&mousePos, &BotaoQuit2)){
                            /* FINALIZACAO */
                            running = 0;
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
                        running=0;
            }
        }
        if (Game == PAUSED){
            SDL_ShowCursor(SDL_ENABLE);
            SDL_RenderCopy(ren, ct, NULL, &BotaoPA);
            SDL_RenderCopy(ren, mM, NULL, &BotaoMM);
            SDL_RenderCopy(ren, bq, NULL, &BotaoQuit2);
            SDL_RenderPresent(ren);
            if (atoi(PontosString)>atoi(record)){
                strcpy(record, PontosString);
                saveRecord(record);
            }

            switch (evt.type){
                case SDL_MOUSEBUTTONUP:
                    if (evt.button.button == SDL_BUTTON_LEFT){
                        if (PointInRect(&mousePos, &BotaoPA)){
                            Game = RUNNING;
                            Gun = READY;
                        }
                        if (PointInRect(&mousePos, &BotaoMM)){
                            combocount = 0;
                            popCount = 8016235520;
                            Points = 0;
                            for (i=0; i<entcout; i++){
                                remover_elemento(Rects, i);
                            }
                            entcout=0;
                            Game = MENU;
                        }
                        if (PointInRect(&mousePos, &BotaoQuit2)){
                            /* FINALIZACAO */
                            running = 0;
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
                        running = 0;
            }
        }
        if (Game == RUNNING){
            SDL_ShowCursor(SDL_DISABLE);
            Mira.w=40;
            Mira.h=40;
            Mira.x=mousePos.x;
            Mira.y=mousePos.y;
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
            SDL_RenderCopy(ren, bg, NULL, &fundo);
            sprintf(PontosString, "%d", Points);
            ScoreRect.w = 30*strlen(PontosString);
            render_text(ren, PontosString, fnt, &ScoreRect, &clr);
            sprintf(comboString, "%d", combocount);
            render_text(ren, comboString, fnt, &comboRect, &clr);
            render_text(ren, "x", fnt, &xRect, &clr);

            sprintf(PopString, "%.0f", popCount);
            render_text(ren, PopString, fntUltra, &PopRect, &clrPop);

            if (atoi(PontosString)>atoi(record)){
                render_text(ren, "NEW RECORD!!!", fnt, &newRecordRect, &clr);
            }

            for (i=0; i<entcout; i++){
                if (Rects[i].type == SAT){
                    SDL_RenderCopy(ren, st, NULL, &Rects[i].Rect);  
                } else {
                    if(Rects[i].Rect.y>350 && Rects[i].state == FALLING){
                        Rects[i].animCount=min(Rects[i].animCount + (int)((Rects[i].Rect.y - 300)/100), 5);
                    }
                    if(Rects[i].state==CRASHED){
                        Rects[i].animCount=6;
                    }
                    if (Rects[i].vx<0){
                        SDL_RenderCopy(ren, astanim, &Recortes[Rects[i].animCount], &Rects[i].Rect);
                    } else{
                        SDL_RenderCopyEx(ren, astanim, &Recortes[Rects[i].animCount], &Rects[i].Rect, 0, NULL, SDL_FLIP_HORIZONTAL);
                    }
                }
            } 
            if (Gun==READY || Gun==SHOOTING){
                recorteMira = (SDL_Rect) {0,0, 512,512};
            } else {
                recorteMira = (SDL_Rect) {512,0, 512,512};
            }
            SDL_RenderCopy(ren, img, &recorteMira, &Mira);
            //SDL_RenderFillRect(ren, &Terra);
            SDL_RenderPresent(ren);
       
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
                            if (SDL_HasIntersection(&Mira, &Rects[i].Rect) && Gun == SHOOTING && Rects[i].state == FALLING){
                                Gun = RELOADING;
                                combocount+=1;
                                hit = 1;
                                if(Rects[i].type == AST){
                                    if (Rects[i].Rect.h>15){
                                        Points+=Rects[i].value*combocount;
                                        split (Rects, i, entcout);
                                        entcout+=2;        
                                    }
                                    else{
                                        Points+=Rects[i].value*combocount;
                                        remover_elemento(Rects, i);
                                        entcout-=1;
                                    }
                                } else {
                                    Points+=Rects[i].value;
                                    remover_elemento(Rects, i);
                                    entcout-=1;
                                    combocount = 0;
                                }          
                            }       
                        }
                        if (hit == 0){
                            combocount = 0;
                        }
                        Level = (int)(log10(Points)/4);
                        Gun = RELOADING;
                    }
                }
                if (evt.type == SDL_KEYDOWN){
                    if (evt.key.keysym.sym == SDLK_ESCAPE){
                        Game = PAUSED;
                    }
                    if (evt.key.keysym.sym == SDLK_F1){
                        Game = GAMEOVER;
                    }
                }
            }
            else {
                SDL_FlushEvent(SDL_MOUSEMOTION);
                espera = es;        
                if (Gun == RELOADING){
                    cooldown++;
                    if (cooldown==5){
                        Gun = READY;
                        cooldown=0;
                    }
                }

                entrespawn= rand() % 100;
                if(entrespawn>90-Level && entcout<10+Level){
                    spawn(Rects, entcout);
                    entcout+=1;
                }
                for (i=0; i<entcout; i++){
                    Rects[i].Rect.x += Rects[i].vx;
                    Rects[i].Rect.y += Rects[i].vy;
                    if (Rects[i].state == CRASHED){
                        Rects[i].crashcount++;
                        if (Rects[i].crashcount>=5){
                            Rects[i].state = DESTROYED;
                        }
                    }
                    if (SDL_HasIntersection(&Terra, &Rects[i].Rect)){ 
                        if(Rects[i].state == FALLING){
                            combocount = 0;
                            if (Rects[i].Rect.h>15){
                                popCount-= 1e9;
                            }
                            if (Rects[i].Rect.h==15){
                                popCount-= 5e7;
                            }
                            if (popCount<=0){
                                Game = GAMEOVER;
                            }
                            Rects[i].state = CRASHED;
                            Rects[i].vx = 0;
                            Rects[i].vy = 0;
                        }
                        if (Rects[i].state == DESTROYED){
                            remover_elemento(Rects, i);
                            entcout-=1;
                        }
                    }
                    if (Rects[i].Rect.x + Rects[i].Rect.w > ww && Rects[i].type == SAT) {
                        remover_elemento(Rects, i);
                        entcout-=1;   
                    }
                }
            }
            //Trata os eventos do AltF4 e de fechar a janela no 'X'
            if (evt.type == SDL_WINDOWEVENT){
                if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
                        running=0;
            }
        }
    }

    /* FINALIZACAO */
    if (atoi(PontosString)>atoi(record)){
        strcpy(record, PontosString);
        saveRecord(record);
    }
    TTF_CloseFont(fnt);
    SDL_DestroyTexture(img);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(mn);
    SDL_DestroyTexture(bp);
    SDL_DestroyTexture(bq);
    SDL_DestroyTexture(go);
    SDL_DestroyTexture(mM);
    SDL_DestroyTexture(pa);
    SDL_DestroyTexture(st);
    SDL_DestroyTexture(astanim);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    fclose (fp);
    return 0;
}
