#include <gb/gb.h>
#include "snake.h"
#include <stdlib.h>

// Direction: | 1:left | 2: right | 3:up | 4:down

void move(int direction);
void update_score();
void spawn_fruit();

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

void move(int direction) {
    UINT8 old_tail_x = snake[snake_length - 1].x;
    UINT8 old_tail_y = snake[snake_length - 1].y;

    for (UINT8 i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    switch(direction) {
        case 1: headX--; break;
        case 2: headX++; break;
        case 3: headY--; break;
        case 4: headY++; break;
    }

    snake[0].x = headX;
    snake[0].y = headY;

    set_bkg_tile_xy(old_tail_x, old_tail_y, 0);

    if (fruit.active && headX == fruit.x && headY == fruit.y) {
        score++;

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



void main() {
    UINT8 key, prevA = 0;
    UINT8 frame = 0;
    UINT8 direction = 0;
    int frames_per_move = 60 / speed;

    snake[0].x = headX;
    snake[0].y = headY;

    fruit.active = 0;

    UINT16 seed = 0;
    while (seed < 1000) seed++;

    move_win(7, 136);  // 7: Correct Material Shift,   136: 144-8 -> to get a window 8bit tall
    SHOW_WIN;

    set_bkg_data(0, snake_tilesLen, SnakeTile);
    set_win_data(0, snake_tilesLen, SnakeTile);

    fill_bkg_rect(0, 0, 20, 18, 0);

    set_bkg_tile_xy(5, 5, 3);

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

        if (key & J_LEFT)  direction = 1;
        if (key & J_RIGHT) direction = 2;
        if (key & J_UP)    direction = 3;
        if (key & J_DOWN)  direction = 4;
        // if ((key & J_A) && !prevA) {
        //     snake_length++;
        //     score++;
        //     spawn_fruit();
        // }
        // prevA = key & J_A;

        frame++;
        if (frame >= frames_per_move) {
            frame = 0;        // reset
            if (direction != 0)
                move(direction);
        }

        update_score();
        wait_vbl_done();
    }
}