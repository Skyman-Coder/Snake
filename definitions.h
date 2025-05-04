#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>

struct vec2 {
    int x, y;
};
enum Direction {
    LEFT, RIGHT, UP, DOWN, IDLE
};

#define rows 50
#define cols 50
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define chunck_size 10
#define VERTICAL_CHUNCK_COUNT (int)(WINDOW_WIDTH / chunck_size)
#define HORIZONTAL_CHUNCK_COUNT (int)(WINDOW_HEIGHT / chunck_size)
#define START_LENGTH 5

void RenderGameOverText();
void init_buffer();
void draw_buffer(SDL_Renderer* renderer);
void update_body();
void logic();
void generateFoodPostion(int& x, int& y);
void init_game();
void setColor(int posX, int posY, int color1, int color2, int color3);

std::random_device random_device;
std::uniform_int_distribution<int> generate_food_x{ 0, VERTICAL_CHUNCK_COUNT - 1 };
std::uniform_int_distribution<int> generate_food_y{ 0, HORIZONTAL_CHUNCK_COUNT - 1 };

char score_displayer[20];
bool gameOver = false;
vec2 food_pos = { 0, 0 };

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Color textColor = { 0xff, 0xff, 0xff, 0xff }; // white color
SDL_Surface* textSurface;
SDL_Texture* textTexture;
TTF_Font* font;