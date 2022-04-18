#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 1280
#define HEIGHT 720

// window
SDL_Window *gWindow = NULL;

// renderer
SDL_Renderer *renderer = NULL;

// image surface
SDL_Surface *gImage = NULL;

SDL_Color color = {255, 255, 255};
SDL_Surface *gText = NULL;
SDL_Texture *gTextTexture = NULL;

// texture
SDL_Texture *gTexture = NULL;

TTF_Font *font = NULL;

bool return1 = false, return2 = false;
bool yPressed = false, nPressed = false;
bool sPressed = false, pPressed = false, rPressed = false;

// function protoypes
bool init();
void close();
void entry(char *text);
void loadMedia(char *text);
void gameLogic(bool s, bool p, bool r);
void logic(char *text);
void checker();
int randomNumber();

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gWindow = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff) < 0)
    {
        printf("Failed to set renderer drawing color: %s\n", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    gImage = IMG_Load("loading.png");
    if (gImage == NULL)
    {
        printf("Failed to load image: %s\n", SDL_GetError());
        return false;
    }
    gTexture = SDL_CreateTextureFromSurface(renderer, gImage);
    if (gTexture == NULL)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return false;
    }
    // free surface
    SDL_FreeSurface(gImage);

    // font
    if (TTF_Init() < 0)
    {
        printf("Failed to initialize TTF: %s\n", TTF_GetError());
        return false;
    }
    font = TTF_OpenFont("font.ttf", 30);
    if (font == NULL)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        return false;
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_StartTextInput();
    return true;
}

void close()
{
    SDL_StopTextInput();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gTexture);
    SDL_DestroyTexture(gTextTexture);
    SDL_FreeSurface(gImage);
    SDL_FreeSurface(gText);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void entry(char *text)

{
    gText = TTF_RenderText_Solid(font, text, color);
    if (gText == NULL)
    {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }
    gTextTexture = SDL_CreateTextureFromSurface(renderer, gText);
    if (gTextTexture == NULL)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return;
    }
    SDL_FreeSurface(gText);

    SDL_Rect textRect = {
        WIDTH / 2 - 5,
        HEIGHT / 2 - 17,
        gText->w,
        gText->h};
    SDL_RenderCopy(renderer, gTextTexture, NULL, &textRect);
    SDL_UpdateWindowSurface(gWindow);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(gTexture);
}

void loadMedia(char *text)
{
    gImage = NULL;
    gTexture = NULL;
    gImage = IMG_Load(text);
    if (gImage == NULL)
    {
        printf("Failed to load image: %s\n", SDL_GetError());
    }
    gTexture = SDL_CreateTextureFromSurface(renderer, gImage);
    if (gTexture == NULL)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
    }
    // free surface
    SDL_FreeSurface(gImage);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(gTexture);
}

void checker()
{
    if (yPressed && (return1 || return2))
    {
        loadMedia("gameBG.png");
        entry(" ");
        yPressed = false;
        return1 = false;
        return2 = false;
    }
    else if (nPressed && (return1 || return2))
    {
        close();
    }
    else if (rPressed || sPressed || pPressed)
    {
        if (return1 || return2)
        {
            gameLogic(sPressed, pPressed, rPressed);
        }
    }
}

void gameLogic(bool s, bool p, bool r)
{
    if (s)
    {
        logic("scissors");
    }
    else if (p)
    {
        logic("paper");
    }
    else if (r)
    {
        logic("rock");
    }
    else
    {
        return;
    }
    rPressed = false;
    sPressed = false;
    pPressed = false;
}

void logic(char *text)
{
    int num = randomNumber();
    if (text == "scissors")
    {
        if (num < 33)
            loadMedia("scissor_RockBeatsScissor.png");
        else if (num < 66)
            loadMedia("scissor_scissorBeatsPaper.png");
        else
            loadMedia("scissor_scissorEqualsScissor.png");
    }
    else if (text == "paper")
    {
        if (num < 33)
            loadMedia("paper_PaperBeatsRock.png");
        else if (num < 66)
            loadMedia("paper_ScissorBeatsPaper.png");
        else
            loadMedia("paper_PaperEqualsPaper.png");
    }
    else if (text == "rock")
    {
        if (num < 33)
            loadMedia("rock_PaperBeatsRock.png");
        else if (num < 66)
            loadMedia("rock_RockBeatsScissors.png");
        else
            loadMedia("rock_RockEqualsRock.png");
    }
}

int randomNumber()
{
    srand(time(NULL));

    int n = rand() % 100;
    return n;
}

int main(int argc, char *argv[])
{
    if (!init())
    {
        printf("Failed to initialize!\n");
        return -1;
    }

    bool loop = true;

    while (loop)
    {

        SDL_Event e;
        while (SDL_WaitEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                loop = false;
                close();
                break;
            case SDL_TEXTINPUT:
                entry(e.text.text);
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    loop = false;
                    close();
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (yPressed || nPressed)
                    {
                        loadMedia("loading.png");
                    }
                    if (rPressed || sPressed || pPressed)
                    {
                        loadMedia("gameBG.png");
                    }
                }
                if (e.key.keysym.sym == SDLK_y)
                {

                    yPressed = true;
                }
                if (e.key.keysym.sym == SDLK_n)
                {
                    nPressed = true;
                }
                if (e.key.keysym.sym == SDLK_s)
                {
                    sPressed = true;
                }
                if (e.key.keysym.sym == SDLK_p)
                {
                    pPressed = true;
                }
                if (e.key.keysym.sym == SDLK_r)
                {
                    rPressed = true;
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    return1 = true;
                    checker();
                }
                if (e.key.keysym.sym == SDLK_RETURN2)
                {
                    return2 = true;
                }
                break;
            }
            if (loop == false)
            {
                SDL_PumpEvents();
            }
        }
    }

    close();

    return 0;
}
