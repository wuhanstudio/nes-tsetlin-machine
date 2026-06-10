/*
main.c is the entrypoint of your game. Everything starts from here.
This has the main loop for the game, which is then used to call out to other code.
*/

#include "source/c/neslib.h"
#include "source/c/library/bank_helpers.h"
#include "source/c/configuration/game_states.h"
// #include "source/c/menus/title.h"
#include "source/c/globals.h"
#include "source/c/menus/error.h"
// #include "source/c/menus/credits.h"
// #include "source/c/map/load_map.h"
// #include "source/c/map/map.h"
// #include "source/c/graphics/game_text.h"
// #include "source/c/graphics/hud.h"
#include "source/c/graphics/fade_animation.h"
// #include "source/c/sprites/player.h"
// #include "source/c/menus/pause.h"
// #include "source/c/sprites/map_sprites.h"
// #include "source/c/sprites/sprite_definitions.h"
#include "source/c/menus/input_helpers.h"
// #include "source/c/menus/game_over.h"

#include "source/c/menus/text_helpers.h"

#include "graphics/palettes/palettes.config.h"
#include "source/c/library/itoa.h"
#include "tsetlin_model.h"
#include "mnist_model.h"
#include "tsetlin.h"

#define CHR_BANK_MENU 0
#define PRG_BANK_TITLE 2

// Step 0: Load Tsetlin model
Tsetlin* model = &tsetlin_model;

char tm_buffer[10];

// Outputs for image dimensions
const int rows = 28, cols = 28;

// Outputs for model evaluation
char predicted_class = 0;
int votes[10];

char mnist_test_images[][784] = {
    // Number 0
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,132,255,152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,224,253,251,153,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,205,253,253,253,248,76,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,84,247,253,253,253,253,253,118,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,204,253,253,253,253,253,253,220,117,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,130,253,253,253,253,253,253,253,253,220,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,194,253,253,253,253,168,197,253,253,253,54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,192,253,253,253,253,146,2,10,253,253,253,215,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,253,253,253,253,143,7,0,7,253,253,253,253,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,72,253,253,253,69,9,0,0,7,253,253,253,253,75,0,0,0,0,0,0,0,0,0,0,0,0,0,41,231,253,253,232,22,0,0,0,7,253,253,253,253,75,0,0,0,0,0,0,0,0,0,0,0,0,0,145,253,253,239,53,0,0,0,0,7,253,253,253,253,75,0,0,0,0,0,0,0,0,0,0,0,0,0,192,253,253,123,0,0,0,0,5,89,253,253,253,251,73,0,0,0,0,0,0,0,0,0,0,0,0,38,222,253,253,123,0,0,0,24,208,253,253,253,253,125,0,0,0,0,0,0,0,0,0,0,0,0,0,206,253,253,240,7,0,0,29,206,253,253,253,253,163,32,0,0,0,0,0,0,0,0,0,0,0,0,0,213,253,253,239,0,0,123,229,253,253,253,253,119,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,213,253,253,250,128,196,248,253,253,253,214,54,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,191,253,253,253,253,253,253,253,223,123,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,237,253,253,253,253,246,195,26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,134,253,253,119,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    // Number 1
    // {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,167,252,118,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,19,200,254,254,180,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,165,254,254,254,254,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,238,254,254,254,232,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,163,254,254,254,254,151,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,236,254,254,254,254,69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,228,254,254,254,254,254,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,49,240,254,254,254,251,77,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,159,254,254,254,254,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,108,253,254,254,254,247,106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,222,254,254,254,254,126,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,116,254,254,254,255,254,89,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,254,254,254,254,254,227,39,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,254,254,254,254,254,138,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,92,254,254,254,254,226,41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,173,254,254,254,254,185,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,173,254,254,254,254,185,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,173,254,254,254,254,163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,161,254,254,254,113,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,254,254,194,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// const char mnist_test_images_bitpacked[] = {
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
//     0x80, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xF8, 0x00,       
//     0x01, 0xFF, 0x80, 0x00, 0x1F, 0xF8, 0x00, 0x03, 0xF3, 0xC0, 0x00, 0x3E, 0x3E, 0x00, 0x03, 0x83,       
//     0xE0, 0x00, 0x78, 0x3E, 0x00, 0x0F, 0x03, 0xE0, 0x00, 0xF0, 0x7C, 0x00, 0x0F, 0x0F, 0xC0, 0x01,       
//     0xE1, 0xF8, 0x00, 0x1E, 0x7F, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0xFE, 0x00,       
//     0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       
//     0x00, 0x00 
// };

void mnist_print_img(const char* buf)
{
    int y, x;
    for(y = 0; y < 28; y++) 
    {
        for (x = 0; x < 28; x++) 
        {
            if (buf[y*28+x] > 0) {
                put_str(NTADR_A(y, x), "@");
            } else {
                put_str(NTADR_A(y, x), " ");
            }
        }

    }
}

void mnist_print_img_bitpacked(const unsigned char *buf)
{
    int y, x;
    for (y = 0; y < 28; y++)
    {
        for (x = 0; x < 28; x++)
        {
            int pixel_idx = y * 28 + x;
            int byte_idx  = pixel_idx >> 3;      // /8
            int bit_idx   = 7 - (pixel_idx & 7); // MSB first

            int pixel = (buf[byte_idx] >> bit_idx) & 1;

            if (pixel) {
                put_str(NTADR_A(y, x), "@");
            } else {
                put_str(NTADR_A(y, x), " ");
            }
        }
    }
}

void mnist_booleanize_img(char* img, uint32_t size, int threshold) {
    int i;
    for (i = 0; i < size; i++) {
        img[i] = (img[i] > threshold) ? 1 : 0;
    }
}

void draw_my_screen(void) {
    ppu_off();
    pal_bg(titlePalette);
    pal_spr(titlePalette);

    set_chr_bank_0(CHR_BANK_MENU);
    set_chr_bank_1(CHR_BANK_MENU);
    clear_screen();
    oam_clear();

    // Booleanize image using a threshold
    mnist_booleanize_img(mnist_test_images[0], rows * cols, 75);

    mnist_print_img(mnist_test_images[0]);
    // mnist_print_img_bitpacked(mnist_test_images_bitpacked);

    put_str(NTADR_A(0, 27), "Feature:");
    itoa(model->n_feature, tm_buffer);
    put_str(NTADR_A(8, 27), tm_buffer);

    put_str(NTADR_A(12, 27), "Clause:");
    itoa(model->n_clause, tm_buffer);
    put_str(NTADR_A(19, 27), tm_buffer);

    put_str(NTADR_A(23, 27), "State:");
    itoa(model->n_state, tm_buffer);
    put_str(NTADR_A(29, 27), tm_buffer);

    // Evaluate
    tsetlin_evaluate(model, mnist_test_images[0], votes, &predicted_class);

    put_str(NTADR_A(0, 28), "Prediction:");
    itoa(predicted_class, tm_buffer);
    put_str(NTADR_A(12, 28), tm_buffer);

    put_str(NTADR_A(14, 28), "Votes:");

    itoa(votes[0], tm_buffer);
    put_str(NTADR_A(21, 28), tm_buffer);
    itoa(votes[1], tm_buffer);
    put_str(NTADR_A(23, 28), tm_buffer);

    ppu_on_all();

    gameState = GAME_STATE_TITLE_INPUT;
}

void handle_my_input(void) {
    if (pad_trigger(0) & PAD_START) {
        gameState = GAME_STATE_CREDITS;
    }
}
void main(void) {
    fade_out_instant();
    gameState = GAME_STATE_TITLE_DRAW;
    bank_spr(1);

    while (1) {
        everyOtherCycle = !everyOtherCycle;
        switch (gameState) {
            case GAME_STATE_TITLE_DRAW:
                banked_call(PRG_BANK_TITLE, draw_my_screen);
                // music_play(SONG_TITLE);
                fade_in();
                break;
            case GAME_STATE_TITLE_INPUT:
                banked_call(PRG_BANK_TITLE, handle_my_input);
                break;

            case GAME_STATE_CREDITS:
                // music_stop();

                fade_out();
                // Draw the "you won" screen
                // banked_call(PRG_BANK_CREDITS_MENU, draw_win_screen);
                fade_in();
                // banked_call(PRG_BANK_MENU_INPUT_HELPERS, wait_for_start);
                fade_out();

                // Folow it up with the credits.
                // banked_call(PRG_BANK_CREDITS_MENU, draw_credits_screen);
                fade_in();
                // banked_call(PRG_BANK_MENU_INPUT_HELPERS, wait_for_start);
                fade_out();
                reset();
                break;
            default:
                crash_error_use_banked_details(ERR_UNKNOWN_GAME_STATE, ERR_UNKNOWN_GAME_STATE_EXPLANATION, "gameState value", gameState);
        }
        ppu_wait_nmi();
    }
}
