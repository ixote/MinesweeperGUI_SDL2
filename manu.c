#include "input.c"
#include "various.h"

// int* manuWindow();
// int* markButton(SDL_Event* event, Buttons* btn, int* arry);
// char* catTexts(int* temp);
// bool quit;
// Buttons** manu_btns;
int* manuWindow()
{
    int* result_arry = (int*)calloc(3, sizeof(int));
    int* temp_array = (int*)calloc(3, sizeof(int));
    result_arry[0] = 9;
    result_arry[1] = 9;
    result_arry[2] = 10;

    SDL_SetWindowSize(MAIN_WINDOW, MANU_WIDTH, MANU_HEIGHT);
    SDL_SetWindowPosition(MAIN_WINDOW,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    manu_btns = initManuBtns();
    initButton(manu_btns[MANU_BACKGROUND]);
    initButton(manu_btns[MANU_TITLE]);
    initButton(manu_btns[MANU_VERSION]);
    initButton(manu_btns[MANU_EASY]);
    initButton(manu_btns[MANU_MIDDLE]);
    initButton(manu_btns[MANU_HARD]);
    initButton(manu_btns[MANU_SELF]);
    initButton(manu_btns[MANU_START]);
    initButton(manu_btns[MANU_QUIT]);
    initButton(manu_btns[MANU_MARK_EASY]);
    initButton(manu_btns[MANU_ALERT_EASY]);
    SDL_RenderPresent(MAIN_RENDER);
    Buttons* fcs_manu = manu_btns[MANU_BACKGROUND];
    SDL_Point point;
    int click_time = 0;
    Uint32 windowID = SDL_GetWindowID(MAIN_WINDOW);
    quit = false;
    for (SDL_Event event; quit == false; SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
            if (event.motion.windowID == windowID) {
                point.x = event.motion.x;
                point.y = event.motion.y;
                fcs_manu = findButton(&point, manu_btns[0]);
                setMouse(fcs_manu);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.windowID == windowID && click_time != event.button.timestamp) {
                temp_array = markButton(&event, fcs_manu, result_arry);
                if (temp_array[0]) {
                    result_arry = temp_array;
                }
                click_time = event.button.timestamp;
            }
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                if (event.window.windowID == windowID) {
                    result_arry[0] = 0;
                    quit = true;
                    continue;
                }
        }

        if (updateQueen->next) {
            drawQueen(updateQueen, &initButton);
            SDL_RenderPresent(MAIN_RENDER);
        }
        SDL_Delay(20);
    }
    return result_arry;
}

int* markButton(SDL_Event* event, Buttons* btn, int* arry)
{
    int* result = (int*)calloc(3, sizeof(int));
    int* temp = (int*)calloc(3, sizeof(int));
    int type = btn->id;
    int mark_index = type + MANU_MARK_EASY - MANU_EASY;
    int alert_index = type + MANU_ALERT_EASY - MANU_EASY;
    switch (type) {
    case MANU_EASY ... MANU_HARD:
        pushStack(updateQueen, manu_btns[mark_index]);
        pushStack(updateQueen, manu_btns[MANU_MARK_CLC]);
        pushStack(updateQueen, manu_btns[alert_index]);
        pushStack(updateQueen, manu_btns[MANU_ALERT_CLC]);
        break;
    case MANU_SELF:
        SDL_SetCursor(cursor_w);
        temp = INPUT_MAIN();
        if (temp[0] != 0) {
            result = temp;
            pushStack(updateQueen, manu_btns[mark_index]);
            pushStack(updateQueen, manu_btns[MANU_MARK_CLC]);
            manu_btns[alert_index]->text = catTexts(temp);
            manu_btns[alert_index]->rect = inputRect(manu_btns[alert_index], 8 * strlen(manu_btns[alert_index]->text), 20, 0.5, 0.32);
            manu_btns[alert_index]->content = initpTexture(manu_btns[alert_index]);
            pushStack(updateQueen, manu_btns[alert_index]);
            pushStack(updateQueen, manu_btns[MANU_ALERT_CLC]);
        }
        break;
    case MANU_START:

        quit = true;
        break;
    case MANU_QUIT:
        arry[0] = 0;
        quit = true;
        break;
    default:
        break;
    }

    switch (type) {
    case MANU_EASY:
        result[0] = 9;
        result[1] = 9;
        result[2] = 10;
        break;
    case MANU_MIDDLE:
        result[0] = 16;
        result[1] = 16;
        result[2] = 40;
        break;
    case MANU_HARD:
        result[0] = 30;
        result[1] = 16;
        result[2] = 99;
        break;
    case MANU_SELF:
    default:
        break;
    }
    return result;
}
char* catTexts(int* temp)
{
    char* string = (char*)calloc(20, sizeof(char));
    sprintf(string, "宽:%d 高:%d 雷:%d", temp[0], temp[1], temp[2]);
    return string;
}