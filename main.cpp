#include <iostream>
#include "definitions.h"

using std::cerr;
using std::endl;

struct Snake {
    vec2 head = { (int)VERTICAL_CHUNCK_COUNT / 2, (int)HORIZONTAL_CHUNCK_COUNT / 2 };
    Direction move_direction = IDLE;
    vec2 body[VERTICAL_CHUNCK_COUNT * HORIZONTAL_CHUNCK_COUNT] = {};
    int body_length = START_LENGTH;
};

struct Chunck {
    SDL_Rect rect = { NULL };
    int colors[3] = { 0, 0, 0 };
    Uint8 alpha = 0;
};

Chunck buffer[VERTICAL_CHUNCK_COUNT][HORIZONTAL_CHUNCK_COUNT];
Snake snake;

int main(int argc, char** argv)
{
    init_game();
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Video Mode not initialized!" << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }
    if (TTF_Init() < 0) {
        cerr << "SDL_ttf could not initialize! TTF_Error:" << TTF_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (!(window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, NULL))) {
        cerr << "Window is not created" << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }
    if (!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))) {
        cerr << "Renderer is not created" << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }
    //this font should be installed. (it's in Release folder)
    font = TTF_OpenFont("C:\\Windows\\Fonts\\unispace bd.ttf", 32);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return EXIT_FAILURE;
    }

    sprintf_s(score_displayer, "Score: %d", snake.body_length);
    textSurface = TTF_RenderText_Solid(font, score_displayer, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Event event;

    while (!gameOver) {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                gameOver = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    if (snake.move_direction == DOWN) break;
                    snake.move_direction = UP;
                    break;
                case SDLK_s:
                    if (snake.move_direction == UP) break;
                    snake.move_direction = DOWN;
                    break;
                case SDLK_d:
                    if (snake.move_direction == LEFT) break;
                    snake.move_direction = RIGHT;
                    break;
                case SDLK_a:
                    if (snake.move_direction == RIGHT) break;
                    snake.move_direction = LEFT;
                    break;
                case SDLK_ESCAPE:
                    gameOver = true;
                    break;
                }
            }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 1);
        SDL_RenderClear(renderer);

        draw_buffer(renderer);
        logic();

        sprintf_s(score_displayer, "Score: %d", snake.body_length - START_LENGTH);
        textSurface = TTF_RenderText_Solid(font, score_displayer, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h }; // rectangle where the text is drawn 
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_Delay(1000 / 30); //30 fps
        SDL_RenderPresent(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);
    RenderGameOverText();
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    SDL_FreeSurface(textSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
void RenderGameOverText() {
    sprintf_s(score_displayer, "Game Over! Score: %d", snake.body_length - START_LENGTH);
    textSurface = TTF_RenderText_Solid(font, score_displayer, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2, textSurface->w, textSurface->h }; // rectangle where the text is drawn 
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

}
void init_buffer() {
    for (int y = 0; y < HORIZONTAL_CHUNCK_COUNT; y++)
    {
        for (int x = 0; x < VERTICAL_CHUNCK_COUNT; x++) {
            buffer[x][y].rect = { x * chunck_size, y * chunck_size, chunck_size, chunck_size };
        }
    }
}
void draw_buffer(SDL_Renderer* renderer)
{
    for (int y = 0; y < HORIZONTAL_CHUNCK_COUNT; y++)
    {
        for (int x = 0; x < VERTICAL_CHUNCK_COUNT; x++)
        {
            if (x == (int)snake.head.x && y == (int)snake.head.y) setColor(x, y, 0xff, 0, 0);
            else if (x == food_pos.x && y == food_pos.y) setColor(x, y, 0, 0xff, 0);
            else setColor(x, y, 0, 0, 0);

            for (int i = 0; i < snake.body_length; i++)
            {
                if (x == snake.body[i].x && y == snake.body[i].y) setColor(x, y, 0xff, 0, 0);
            }

            SDL_SetRenderDrawColor(renderer, buffer[x][y].colors[0], buffer[x][y].colors[1], buffer[x][y].colors[2], buffer[x][y].alpha);
            SDL_RenderFillRect(renderer, &buffer[x][y].rect);
        }
    }
}
void update_body() {
    for (int i = 0; i < snake.body_length; i++) {
        snake.body[snake.body_length - i].x = snake.body[snake.body_length - i - 1].x;
        snake.body[snake.body_length - i].y = snake.body[snake.body_length - i - 1].y;
    }
    snake.body[0].x = snake.head.x;
    snake.body[0].y = snake.head.y;
}
void logic()
{
    if (snake.head.x >= VERTICAL_CHUNCK_COUNT) snake.head.x = 0;
    if (snake.head.x < 0) snake.head.x = VERTICAL_CHUNCK_COUNT;
    if (snake.head.y >= HORIZONTAL_CHUNCK_COUNT) snake.head.y = 0;
    if (snake.head.y < 0) snake.head.y = HORIZONTAL_CHUNCK_COUNT;
    if (snake.head.x == food_pos.x && snake.head.y == food_pos.y)
    {
        generateFoodPostion(food_pos.x, food_pos.y);
        snake.body_length++;
    }
    if (snake.move_direction == IDLE)
    {
        for (int i = 0; i < START_LENGTH; i++) {
            snake.body[i].x = -1;
            snake.body[i].y = -1;
        }
    }
    for (int i = 1; i < snake.body_length; i++)
    {
        if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y && snake.move_direction != IDLE) {
            gameOver = true;
        }
    }
    update_body();

    switch (snake.move_direction) {
    case UP:
        snake.head.y--;
        break;
    case DOWN:
        snake.head.y++;
        break;
    case LEFT:
        snake.head.x--;
        break;
    case RIGHT:
        snake.head.x++;
        break;
    }
}

void generateFoodPostion(int& x, int& y) {
    do {
        x = generate_food_x(random_device);
        y = generate_food_y(random_device);
    } while (x == snake.head.x && y == snake.head.y);
}
void init_game() {
    snake.body[0].x = snake.head.x;
    snake.body[0].y = snake.head.y;
    generateFoodPostion(food_pos.x, food_pos.y);
    init_buffer();
}
void setColor(int posX, int posY, int color1, int color2, int color3) {
    buffer[posX][posY].colors[0] = color1;
    buffer[posX][posY].colors[1] = color2;
    buffer[posX][posY].colors[2] = color3;
}