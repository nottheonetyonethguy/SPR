#include <stdio.h>
#include <stdbool.h>

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
    font = TTF_OpenFont("font.ttf", 33);
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
    // SDL_DestroyTexture(gTextTexture);
    SDL_FreeSurface(gImage);
    SDL_FreeSurface(gText);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void entry(char *text)

{
    // SDL_Delay(100);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gTexture, NULL, NULL);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);

    if (textSurface == NULL)
    {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }
    // gTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // if (gTextTexture == NULL)
    // {
    //     printf("Failed to create texture: %s\n", SDL_GetError());
    //     return;
    // }
    SDL_Rect textRect = {
        WIDTH / 2 - 5,
        HEIGHT / 2 - 30,
        textSurface->w,
        textSurface->h};
    SDL_RenderCopy(renderer, gTexture, NULL, &textRect);
    SDL_UpdateWindowSurface(gWindow);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(gTexture);
}

void loadMedia(char *text)
{
    printf("2");
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

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
    // SDL_DestroyTexture(gTexture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
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
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                loop = false;
                break;
            case SDL_TEXTINPUT:
                entry(e.text.text);
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    loop = false;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    entry(" ");
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
                    printf("\ns is pressed %d", sPressed);
                }
                if (e.key.keysym.sym == SDLK_p)
                {
                    pPressed = true;
                    printf("\np is pressed %d", pPressed);
                }
                if (e.key.keysym.sym == SDLK_r)
                {
                    if (yPressed)
                    {
                        rPressed = true;
                        printf("\nr is pressed %d", rPressed);
                        gameLogic(sPressed, pPressed, rPressed);
                    }
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    return1 = true;
                }
                if (e.key.keysym.sym == SDLK_RETURN2)
                {
                    return2 = true;
                }

                break;
            }
        }

        if (yPressed && (return1 || return2))
        {
            printf("\n1");
            loadMedia("gameBG.png");
            entry(" ");
            printf("\n0");
            // if (state[SDL_SCANCODE_R])
            // {
            //     printf("r is pressed %d", rPressed);
            //     rPressed = true;
            // }
            // else if (state[SDL_SCANCODE_P])
            // {
            //     pPressed = true;
            // }
            // else if (state[SDL_SCANCODE_S])
            // {
            //     sPressed = true;
            // // }
            // printf("\n%d r ", rPressed);
        }
        else if (nPressed && (return1 || return2))
        {
            loop = false;
        }
    }
    close();

    return 0;
}

void gameLogic(bool s, bool p, bool r)
{
    printf("rgl %d", rPressed);
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
    rPressed = false;
    pPressed = false;
    sPressed = false;
    yPressed = false;
    nPressed = false;
    return1 = false;
    return2 = false;
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
            loadMedia("rock_RockBeatsRock.png");
    }
}

int randomNumber()
{
    return 1;
}
