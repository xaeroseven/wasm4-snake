#include "wasm4.h"
#include "snake.h"
#include <stdlib.h>

struct snake snake;
uint32_t frame_count = 0;
uint8_t prev_state = 0;
struct point fruit;
const uint8_t peach[] = {
    0x00,0xa0,0x02,0x00,0x0e,0xf0,0x36,0x5c,0xd6,0x57,0xd5,0x57,0x35,0x5c,0x0f,0xf0
};
const uint8_t strawberry[16] = {
    0x00,0x10,0x0a,0x10,0x04,0x88,0x16,0x82,0x35,0x58,0x15,0x54,0x1d,0x50,0x05,0x40
};
const uint8_t watermelon[16] = {
    0x00,0x00,0x00,0x00,0x00,0x2b,0x02,0xa7,0xea,0x9b,0xe6,0xab,0x39,0x9c,0x0f,0xf0
};
const uint8_t* fruits[] = { peach, strawberry, watermelon };

const uint8_t smiley[] = {
    0b11000011,
    0b10000001,
    0b00100100,
    0b00100100,
    0b00000000,
    0b00100100,
    0b10011001,
    0b11000011,
};

void start() {
    PALETTE[0] = 0xfbf7f3;
    PALETTE[1] = 0xe5b083;
    PALETTE[2] = 0x426e5d;
    PALETTE[3] = 0x20283d;

    snake_create(&snake);
    snake_push(&snake,(struct point){2,0});
    snake_push(&snake,(struct point){1,0});
    snake_push(&snake,(struct point){0,0});
   
    snake.direction = (struct point){1,0};
}

void input() {
    const uint8_t just_pressed = *GAMEPAD1 & (*GAMEPAD1 ^ prev_state);

    srand(frame_count);

    if (just_pressed & BUTTON_UP) {
        snake_up(&snake);
    } 
    if (just_pressed & BUTTON_DOWN) {
        snake_down(&snake);
    }
    if (just_pressed & BUTTON_LEFT) {
        snake_left(&snake);
    }
    if (just_pressed & BUTTON_RIGHT) {
        snake_right(&snake);
    }

    prev_state = *GAMEPAD1;
}

void update () {
    frame_count++;

    input();

    if (frame_count % 5 == 0) {
        snake_update(&snake);

        if (snake_isdead(&snake)) {
            snake_create(&snake);
           
            snake_push(&snake,(struct point){2,0});
            snake_push(&snake,(struct point){1,0});
            snake_push(&snake,(struct point){0,0});

            snake.direction = (struct point){1,0};
        }

        if (snake.body[0].x == fruit.x && snake.body[0].y == fruit.y) {
            struct point p = snake.body[snake.length-1];
            snake_push(&snake,p);
            fruit.x = rand() % 20;
            fruit.y = rand() % 20;
        }
    }

    snake_draw(&snake);

    *DRAW_COLORS = 0x4320;
    blit(fruits[snake.length % 3],fruit.x * 8, fruit.y * 8, 8, 8, BLIT_2BPP);
}
