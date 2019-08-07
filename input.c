#include "various.h"
#include "tools.c"



int* INPUT_MAIN()
{
    set_value = (int*)calloc(3, sizeof(int));
    void_value = (int*)calloc(3, sizeof(int));

    INPUT_WINDOW = SDL_CreateWindow(
        "请输入", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INPUTSCREEN_WIDTH, INPUTSCREEN_HEIGHT, SDL_WINDOW_SHOWN| SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_MOUSE_FOCUS);
    input_render = SDL_CreateRenderer(INPUT_WINDOW, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC| SDL_RENDERER_TARGETTEXTURE);
    SDL_SetWindowOpacity(INPUT_WINDOW ,0.9);
    resetInputWindow();
    // SDL_RaiseWindow(INPUT_WINDOW);
    input_btns = initInputBtns();
    initButton(input_btns[INPUT_BACKGROUND]);
    initButton(input_btns[INPUT_BG_W]);
    initButton(input_btns[INPUT_BG_H]);
    initButton(input_btns[INPUT_BG_N]);

    initButton(input_btns[INPUT_TEXT_W]);
    initButton(input_btns[INPUT_TEXT_H]);
    initButton(input_btns[INPUT_TEXT_N]);
    initButton(input_btns[INPUT_ALERT_W]);
    initButton(input_btns[INPUT_ALERT_H]);
    initButton(input_btns[INPUT_CONFIRM]);
    initButton(input_btns[INPUT_LEFT_C]);
    initButton(input_btns[INPUT_RIGHT_C]);

    SDL_RenderPresent(input_render);

    SDL_Point point;
    int input_time = 0;
    int del_time = 0;
    int mouse_time = 0;
    Buttons* fcs_btn = input_btns[INPUT_BG_W];
    setMouse(fcs_btn);
    SDL_SetWindowInputFocus(INPUT_WINDOW);
    Buttons* option = input_btns[INPUT_BG_W];//设置两个鼠标描点是为了让输入的时候焦点保持不变
    SDL_WarpMouseInWindow(INPUT_WINDOW, INPUTSCREEN_WIDTH / 4, INPUTSCREEN_HEIGHT / 2);
    Uint32 windowID = SDL_GetWindowID(INPUT_WINDOW);
    Uint32 windowID2 = SDL_GetWindowID(MAIN_WINDOW);
    bool quit = false;

    bool confirm_mark = false;
    for (SDL_Event event; quit == false; SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
            if (event.motion.windowID == windowID) {
                point.x = event.motion.x;
                point.y = event.motion.y;
                option = findButton(&point, input_btns[3]);
                setMouse(option);
            } else if (event.motion.windowID == windowID2) {
                SDL_SetCursor(cursor_w);
            }
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                if (event.window.windowID == windowID) {
                    SDL_RaiseWindow(MAIN_WINDOW);
                    set_value = void_value;
                    quit = true;
                    continue;
                } else if (event.window.windowID == windowID2) {
                    resetInputWindow();
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.windowID == windowID && mouse_time != event.button.timestamp) {
                if (confirm_mark) {
                    pushStack(updateQueen, input_btns[INPUT_LEFT_C]);
                    pushStack(updateQueen, input_btns[INPUT_RIGHT_C]);
                    confirm_mark = false;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (mouse_time != event.button.timestamp) {
                if (event.button.windowID == windowID) {
                    if (fcs_btn->id == INPUT_CONFIRM && !confirm_flag) {
                        pushStack(updateQueen, input_btns[INPUT_LEFT_W]);
                        pushStack(updateQueen, input_btns[INPUT_RIGHT_W]);
                        confirm_mark = true;
                    }
                    point.x = event.button.x;
                    point.y = event.button.y;
                    fcs_btn = findButton(&point, input_btns[3]);
                    inputMouse(fcs_btn);
                    mouse_time = event.button.timestamp;
                } else if (event.button.windowID == windowID2) {
                    resetInputWindow();
                }
            }
            break;
        case SDL_TEXTINPUT:
            if (event.key.windowID == windowID)
                inputKey(&input_time, &event, fcs_btn);
            break;
        case SDL_KEYDOWN:
            if (event.key.windowID == windowID)
                inputKey(&del_time, &event, fcs_btn);
            break;
        }
        checkInput(&event, fcs_btn);

        if (updateQueen->next) {
            drawQueen(updateQueen, &initButton);
            SDL_RenderPresent(input_render);
        }

        if (confirm) {
            if (confirm_flag) {
                SDL_RaiseWindow(MAIN_WINDOW);
                quit = true;
                continue;
            } else {
                confirm = false;
            }
        }
        SDL_Delay(20);
    }
    SDL_DestroyRenderer(input_render);
    SDL_DestroyWindow(INPUT_WINDOW);
    return set_value;
}

inline void inputMouse(Buttons* btn)
{
    switch (btn->id) {
    case INPUT_BG_W:
    case INPUT_BG_H:
    case INPUT_BG_N:
        SDL_StartTextInput();
        break;
    case INPUT_CONFIRM:
        confirm = true;
        SDL_StopTextInput();
        break;
    default:
        SDL_StopTextInput();
    }
}

inline void inputKey(int* time, SDL_Event* event, Buttons* btn)
{
    if (btn) {
        int type = btn->id;
        if (*time != event->key.timestamp) {
            int key = event->key.keysym.sym;
            char p[2];
            p[1] = '\0';
            if (type >= INPUT_BG_W && type <= INPUT_BG_N) {
                // Buttons *btn = input_btns[type];
                Buttons* btn_txt = input_btns[type - 3];
                char* text = btn_txt->text;
                int len = strlen(text);
                switch (key) {
                case SDLK_BACKSPACE:
                    if (len) {
                        text[len - 1] = '\0';
                        btn_txt->rect = inputRect(btn_txt, 8 * (len - 1), 18, (type - 3 + 1) * 0.25, 0.5);
                        btn_txt->content = initpTexture(btn_txt);
                        pushStack(updateQueen, btn_txt);
                        pushStack(updateQueen, btn);
                    }
                    break;
                case SDLK_RETURN:
                    SDL_StopTextInput();
                    break;
                case SDLK_0 ... SDLK_9:
                    p[0] = key;
                    if (len < (((type - 3) >> 1 << 1) + 2)) {
                        sprintf(btn_txt->text,"%.*s%c",len,btn_txt->text,key);
                        btn_txt->rect = inputRect(btn_txt, 8 * (len + 1), 18, (type - 3 + 1) * 0.25, 0.5);
                        btn_txt->content = initpTexture(btn_txt);
                        pushStack(updateQueen, btn_txt);
                        pushStack(updateQueen, btn);
                    }
                default:
                    break;
                }
            }
            *time = event->key.timestamp;
        }
    }
}

inline void checkInput(SDL_Event* event, Buttons* btn)
{

    Buttons* btn_w = input_btns[INPUT_EDIT_W];
    Buttons* btn_h = input_btns[INPUT_EDIT_H];
    Buttons* btn_n = input_btns[INPUT_EDIT_N];

    if (confirm_flag == false) {
        if (btn->id == INPUT_CONFIRM && event->type == SDL_MOUSEBUTTONDOWN) {
            pushStack(updateQueen, input_btns[INPUT_WG_I]);
            pushStack(updateQueen, input_btns[INPUT_WG_CLC]);
        }
    }
    int w = atof(btn_w->text);
    int h = atof(btn_h->text);
    int n = atof(btn_n->text);
    if (btn_w->text[0] && w < 9) {
        pushStack(updateQueen, input_btns[INPUT_WG_W]);
        pushStack(updateQueen, input_btns[INPUT_WG_CLC]);
        pushStack(updateQueen, input_btns[INPUT_LOG_WG]);
        confirm_flag = false;
    } else if (btn_h->text[0] && h < 9) {
        pushStack(updateQueen, input_btns[INPUT_WG_H]);
        pushStack(updateQueen, input_btns[INPUT_WG_CLC]);
        pushStack(updateQueen, input_btns[INPUT_LOG_WG]);
        confirm_flag = false;
    } else if (btn_w->text[0] && btn_h->text[0] && btn_n->text[0]) {
        if (n >= 1 && n <= (w - 1) * (h - 1)) {
            pushStack(updateQueen, input_btns[INPUT_WG_CLC]);
            pushStack(updateQueen, input_btns[INPUT_LOG_RT]);
            confirm_flag = true;

        } else {
            pushStack(updateQueen, input_btns[INPUT_WG_N]);
            pushStack(updateQueen, input_btns[INPUT_WG_CLC]);
            pushStack(updateQueen, input_btns[INPUT_LOG_WG]);
            confirm_flag = false;
        }
    } else {
        pushStack(updateQueen, input_btns[INPUT_WG_CLC]);
        pushStack(updateQueen, input_btns[INPUT_LOG_NM]);
        confirm_flag = false;
    }

    if (confirm_flag) {
        set_value[0] = w;
        set_value[1] = h;
        set_value[2] = n;
    }
}
void resetInputWindow()
{
    SDL_RaiseWindow(INPUT_WINDOW);
    SDL_SetWindowInputFocus(INPUT_WINDOW);
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    SDL_GetWindowPosition(MAIN_WINDOW, &x, &y);
    SDL_SetWindowPosition(INPUT_WINDOW, x + MANU_WIDTH / 2 - INPUTSCREEN_WIDTH / 2, y + MANU_HEIGHT / 2 - INPUTSCREEN_HEIGHT / 2);
    SDL_WarpMouseInWindow(INPUT_WINDOW, INPUTSCREEN_WIDTH / 4, INPUTSCREEN_HEIGHT / 2);
}