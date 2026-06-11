/*
main.c is the entrypoint of your game. Everything starts from here.
This has the main loop for the game, which is then used to call out to other code.
*/

#include "graphics/palettes/palettes.config.h"

#include "source/c/neslib.h"
#include "source/c/globals.h"

#include "source/c/library/bank_helpers.h"
#include "source/c/library/itoa.h"

#include "source/c/configuration/game_states.h"
#include "source/c/graphics/fade_animation.h"

#include "source/c/menus/input_helpers.h"
#include "source/c/menus/error.h"
#include "source/c/menus/text_helpers.h"

#include "source/c/lime-tm/tsetlin_model.h"
#include "source/c/lime-tm/mnist_model.h"
#include "source/c/lime-tm/tsetlin.h"

#include "source/c/mnist/mnist.h"

#define CHR_BANK_MENU 0
#define PRG_BANK_TITLE 2

// Step 0: Load Tsetlin model
Tsetlin* model = &tsetlin_model;

// Outputs for image dimensions
int mnist_index = 0;
const int rows = 28, cols = 28;
char img[784];

// Outputs for model evaluation
int predicted_class = 0;
int votes[2];

void draw_my_screen(void) {
    int i;
    char tm_buffer[10];

    ppu_off();
    pal_bg(titlePalette);
    pal_spr(titlePalette);

    set_chr_bank_0(CHR_BANK_MENU);
    set_chr_bank_1(CHR_BANK_MENU);

    clear_screen();
    oam_clear();

    // Copy from const mist_booleanize_img to an array
    for (i = 0; i < rows * cols; i++) {
        img[i] = mnist_test_images[mnist_index][i];
    }

    // Booleanize image using a threshold
    mnist_booleanize_img(img, rows * cols, 75);

    // mnist_print_img(mnist_test_images[mnist_index], 75);
    mnist_print_img(img, 0);

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
    predicted_class =tsetlin_evaluate(model, img, votes);

    // Print predicted class
    put_str(NTADR_A(0, 28), "Prediction:");

    itoa(predicted_class, tm_buffer);
    put_str(NTADR_A(12, 28), tm_buffer);

    // Print votes for each class
    put_str(NTADR_A(14, 28), "Votes:");

    itoa(votes[0], tm_buffer);
    put_str(NTADR_A(21, 28), tm_buffer);

    itoa(votes[1], tm_buffer);
    put_str(NTADR_A(25, 28), tm_buffer);

    ppu_on_all();

    gameState = GAME_STATE_TITLE_INPUT;
}

void handle_my_input(void) {
    if (pad_trigger(0) & PAD_START) {
        mnist_index = (mnist_index + 1) % 2;
        gameState = GAME_STATE_TITLE_DRAW;
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
                banked_call(PRG_BANK_MNIST, draw_my_screen);
                fade_in();
                break;
            case GAME_STATE_TITLE_INPUT:
                banked_call(PRG_BANK_TITLE, handle_my_input);
                break;

            case GAME_STATE_CREDITS:
                fade_out();
                reset();
                break;
            default:
                crash_error_use_banked_details(ERR_UNKNOWN_GAME_STATE, ERR_UNKNOWN_GAME_STATE_EXPLANATION, "gameState value", gameState);
        }
        ppu_wait_nmi();
    }
}
