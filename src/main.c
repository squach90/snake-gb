#include <gb/gb.h>
#include <stdlib.h>
#include <stdio.h>
#include "snake.h"
#include "border.h"
#include "letter.h"

// Direction: | 1:left | 2: right | 3:up | 4:down

#define LEFT  1
#define RIGHT 2
#define UP    3
#define DOWN  4

UINT8 current_direction = RIGHT;

// console_init();

void move(int direction);
void update_score();
void spawn_fruit();
void game_over();
void draw_Main_screen();

typedef struct {
    UINT8 x, y;
} Segment;

typedef struct {
    UINT8 x, y, active;
} Fruit;

Segment snake[200];
UINT8 snake_length = 1;

Fruit fruit;

UINT8 headX = 3, headY = 3;
UINT8 score = 0, prevScore = 255;
int speed = 7; // tiles/sec
int die = 0;   // 0 -> FALSE | 1 -> TRUE
int isMainScreen = 1;  // 0 -> FALSE | 1 -> TRUE

int ppf = 1; // Point per fruit

const UINT8 digitTiles[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

UINT16 rng_seed = 1;

UINT8 gb_rand(UINT8 max) {
    rng_seed = (rng_seed * 1103515245 + 12345) & 0xFFFF;
    return rng_seed % max;
}

void spawn_fruit() {
    UINT8 x, y;
    UINT8 collision;

    do {
        collision = 0;
        x = gb_rand(20);
        y = gb_rand(17);

        for(UINT8 i=0; i<snake_length; i++) {
            if(snake[i].x == x && snake[i].y == y) {
                collision = 1;
                break;
            }
        }
    } while(collision);

    fruit.x = x;
    fruit.y = y;
    fruit.active = 1;

    set_bkg_tile_xy(fruit.x, fruit.y, 4);
}

void update_score() {
    if(score == prevScore) return;
    set_win_tile_xy(2,0,digitTiles[score/10]); // tens
    set_win_tile_xy(3,0,digitTiles[score%10]); // ones
    prevScore = score;
}

void play_eat_sound() {
    NR10_REG = 0x07;
    NR11_REG = 0x40;
    NR12_REG = 0xF3;
    NR13_REG = 0x10;
    NR14_REG = 0xC3;
}


void move(int direction) {

    if ((current_direction == LEFT  && direction == RIGHT) ||
        (current_direction == RIGHT && direction == LEFT)  ||
        (current_direction == UP    && direction == DOWN)  ||
        (current_direction == DOWN  && direction == UP)) {
        direction = current_direction;
    } else {
        current_direction = direction;
    }

    UINT8 old_tail_x = snake[snake_length - 1].x;
    UINT8 old_tail_y = snake[snake_length - 1].y;

    // Move body
    for (UINT8 i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Move head
    if (direction == LEFT)  headX--;
    if (direction == RIGHT) headX++;
    if (direction == UP)    headY--;
    if (direction == DOWN)  headY++;

    UINT8 new_headX = headX;
    UINT8 new_headY = headY;

    // Collide Border
    if (new_headX >= 20 || new_headY >= 17) {
        game_over();
        die = 1;
        return;
    }

    // Collide Body
    for (UINT8 i = 0; i < snake_length; i++) {
        if (snake[i].x == new_headX && snake[i].y == new_headY) {
            game_over();
            die = 1;
            return;
        }
    }

    snake[0].x = new_headX;
    snake[0].y = new_headY;

    headX = new_headX;
    headY = new_headY;

    set_bkg_tile_xy(old_tail_x, old_tail_y, 0);

    if (fruit.active && headX == fruit.x && headY == fruit.y) {
        score += ppf;

        play_eat_sound();

        snake[snake_length].x = snake[snake_length - 1].x;
        snake[snake_length].y = snake[snake_length - 1].y;

        snake_length++;
        fruit.active = 0;
        spawn_fruit();
    }

    // body
    for (UINT8 i = 1; i < snake_length; i++) {
        set_bkg_tile_xy(
            snake[i].x,
            snake[i].y,
            2
        );
    }

    // head
    set_bkg_tile_xy(headX, headY, 3);
}

void game_over() {
    DISPLAY_OFF;
    VBK_REG = 0;
    for (UINT8 y = 0; y < 18; y++) {
        for (UINT8 x = 0; x < 20; x++) {
            set_bkg_tile_xy(x, y, 0);
        }
    }

    HIDE_WIN;

    for (UINT8 i = 0; i < 40; i++) {
        move_sprite(i, 0, 0);
    }

    draw_GO_screen();

    DISPLAY_ON;
}

void draw_GO_screen() {
    // Load tileset
    set_bkg_data(16, border_tilesLen, BorderTiles);
    set_bkg_data(24, letters_tilesLen, LettersTiles);

    // TOP
    set_bkg_tile_xy(3, 3, 18);
    for(UINT8 x = 4; x <= 14; x++) set_bkg_tile_xy(x, 3, 20);
    set_bkg_tile_xy(15, 3, 19);

    // BOTTOM
    set_bkg_tile_xy(3, 13, 16);
    for(UINT8 x = 4; x <= 14; x++) set_bkg_tile_xy(x, 13, 21);
    set_bkg_tile_xy(15, 13, 17);

    // LEFT
    for(UINT8 y = 4; y <= 12; y++) set_bkg_tile_xy(3, y, 22);

    // RIGHT
    for(UINT8 y = 4; y <= 12; y++) set_bkg_tile_xy(15, y, 23);

    set_bkg_tile_xy(5, 6, 24);  // G
    set_bkg_tile_xy(6, 6, 25);  // A
    set_bkg_tile_xy(7, 6, 26);  // M
    set_bkg_tile_xy(8, 6, 27);  // E
    set_bkg_tile_xy(9, 6, 0);   // SPACE
    set_bkg_tile_xy(10, 6, 28); // O
    set_bkg_tile_xy(11, 6, 29); // V
    set_bkg_tile_xy(12, 6, 27); // E
    set_bkg_tile_xy(13, 6, 30); // R

    set_bkg_tile_xy(5, 9, 31);  // S
    set_bkg_tile_xy(6, 9, 32);  // C
    set_bkg_tile_xy(7, 9, 28);  // O
    set_bkg_tile_xy(8, 9, 30);  // R
    set_bkg_tile_xy(9, 9, 27);  // E
    set_bkg_tile_xy(10, 9, 0);  // SPACE

    set_bkg_tile_xy(11, 9, digitTiles[score / 10]); // tens
    set_bkg_tile_xy(12, 9, digitTiles[score % 10]); // ones
}

void draw_Main_screen() {
    set_bkg_data(16, border_tilesLen, BorderTiles);
    set_bkg_data(24, letters_tilesLen, LettersTiles);

    set_bkg_tile_xy(3, 3, 18);
    for(UINT8 x = 4; x <= 14; x++) set_bkg_tile_xy(x, 3, 20);
    set_bkg_tile_xy(15, 3, 19);

    set_bkg_tile_xy(5, 4, 31);  // S
    set_bkg_tile_xy(6, 4, 33);  // N
    set_bkg_tile_xy(7, 4, 25);  // A
    set_bkg_tile_xy(8, 4, 34);  // K
    set_bkg_tile_xy(9, 4, 27);  // E

    set_bkg_tile_xy(11, 4, 2);  // Body
    set_bkg_tile_xy(12, 4, 2);  // Body
    set_bkg_tile_xy(13, 4, 3);  // Head

    // BOTTOM
    set_bkg_tile_xy(3, 5, 16);
    for(UINT8 x = 4; x <= 14; x++) set_bkg_tile_xy(x, 5, 21);
    set_bkg_tile_xy(15, 5, 17);


    // LEFT
    set_bkg_tile_xy(3, 4, 22);

    // RIGHT (Fruit)
    set_bkg_tile_xy(15, 4, 4);


    set_bkg_tile_xy(4, 10, 38);   // P
    set_bkg_tile_xy(5, 10, 30);   // R
    set_bkg_tile_xy(6, 10, 27);   // E
    set_bkg_tile_xy(7, 10, 31);   // S
    set_bkg_tile_xy(8, 10, 31);   // S

    set_bkg_tile_xy(10, 10, 31);  // S
    set_bkg_tile_xy(11, 10, 39);  // T
    set_bkg_tile_xy(12, 10, 25);  // A
    set_bkg_tile_xy(13, 10, 30);  // R
    set_bkg_tile_xy(14, 10, 39);  // T

    set_bkg_tile_xy(3, 17, 4);    // Fruit

    set_bkg_tile_xy(5, 17, 31);   // S
    set_bkg_tile_xy(6, 17, 35);   // Q
    set_bkg_tile_xy(7, 17, 36);   // U
    set_bkg_tile_xy(8, 17, 25);   // A
    set_bkg_tile_xy(9, 17, 32);   // C
    set_bkg_tile_xy(10, 17, 37);  // H
    set_bkg_tile_xy(11, 17, 14);  // 9
    set_bkg_tile_xy(12, 17, 5);   // 0

    set_bkg_tile_xy(14, 17, 4);   // Fruit
}

void restart_game() {
    // Reset variables
    snake_length = 1;
    headX = 3;
    headY = 3;
    score = 0;
    prevScore = 255;
    die = 0;
    current_direction = RIGHT;

    // Clear background
    VBK_REG = 0;
    fill_bkg_rect(0, 0, 20, 18, 0);
    SHOW_WIN;

    // Reset snake
    snake[0].x = headX;
    snake[0].y = headY;

    // Reset fruit
    fruit.active = 0;
    spawn_fruit();

    // Update score display
    update_score();
}


void main() {
    // --- INIT SOUND ---
    NR52_REG = 0x80;  // Sound ON
    NR50_REG = 0x77;  // Volume
    NR51_REG = 0xFF;  // Enable all channels

    UINT8 key, prevA = 0;
    UINT8 frame = 0;
    UINT8 direction = 0;
    int frames_per_move = 60 / speed;

    snake[0].x = headX;
    snake[0].y = headY;

    fruit.active = 0;

    UINT16 seed = 0;
    while (seed < 1000) seed++;

    fill_bkg_rect(0, 0, 20, 18, 0);
    set_bkg_data(0, snake_tilesLen, SnakeTile);
    set_win_data(0, snake_tilesLen, SnakeTile);

    SHOW_BKG;
    DISPLAY_ON;

    draw_Main_screen();

    while(1) {
        key = joypad();
        if (key & J_START) break;  // START press => exit the loop
        wait_vbl_done();
    }

    move_win(7, 136);  // 7: Correct Material Shift,   136: 144-8 -> to get a window 8bit tall
    SHOW_WIN;

    fill_bkg_rect(0, 0, 20, 18, 0);

    set_bkg_tile_xy(3, 3, 3);

    set_win_tile_xy(0, 0, 4);
    set_win_tile_xy(1, 0, 15);

    set_win_tile_xy(2, 0, digitTiles[0]);
    set_win_tile_xy(3, 0, digitTiles[0]);
    prevScore = 0;

    SHOW_BKG;
    DISPLAY_ON;

    update_score();
    spawn_fruit();

    while(1) {
        key = joypad();

        if (die == 0) {
            if (key & J_LEFT)  direction = 1;
            if (key & J_RIGHT) direction = 2;
            if (key & J_UP)    direction = 3;
            if (key & J_DOWN)  direction = 4;

            frame++;
            if (frame >= frames_per_move) {
                frame = 0;        // reset
                if (direction != 0)
                    move(direction);
            }

            update_score();
        } else {
            if (key & J_START) {
                restart_game();
            }
        }

        wait_vbl_done();
    }
}