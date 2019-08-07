#include "mines.c"
#include "tools.c"

Message* Game(int* data)
{
    int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
    int CORE_1 = (desktopHeight - 70) / data[1];
    int CORE_0 = desktopWidth / data[0];
    CORE = CORE_1 < CORE_0 ? CORE_1 : CORE_0;
    CORE = CORE < 30 ? CORE : 30;

    SCREEN_WIDTH = CORE * data[0];
    SCREEN_HEIGHT = CORE * data[1] + 50; //CORE * 3;

    GAME_MANU_WIDTH = SCREEN_WIDTH;
    manuQueue = initQueue();
    SDL_RenderClear(MAIN_RENDER);
    
    SDL_SetWindowSize(MAIN_WINDOW, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowPosition(MAIN_WINDOW,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // initRe();
    game_btn = initGameBtns();
    initButton(game_btn[GAME_BACKGROUND]);
    initButton(game_btn[GAME_TIME_1]);
    initButton(game_btn[GAME_TIME_2]);
    initButton(game_btn[GAME_TIME_3]);
    initButton(game_btn[GAME_TIME_4]);
    initButton(game_btn[GAME_STATE]);
    initButton(game_btn[GAME_RETURN]);
    initButton(game_btn[GAME_NUMBER_1]);
    initButton(game_btn[GAME_NUMBER_2]);
    initButton(game_btn[GAME_NUMBER_3]);
    initButton(game_btn[GAME_NUMBER_4]);

    initGame(data);
    SDL_RenderPresent(MAIN_RENDER);

    int change_type = -1;
    int mouse_time = 0;
    Uint32 windowID = SDL_GetWindowID(MAIN_WINDOW);

    Buttons* fcs_gm = NULL;
    Buttons* mouse_option = game_btn[GAME_BACKGROUND];
    SDL_Point point;
    int time_count = 0;
    Uint32 start_tm = SDL_GetTicks();
    int result_num = 0;
    bool quit = false;
    for (SDL_Event game_event; quit == false; SDL_PollEvent(&game_event)) {

        point.x = game_event.motion.x;
        point.y = game_event.motion.y;
        mouse_option = findButton(&point, game_btn[0]);
        setMouse(mouse_option);

        switch (game_event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (game_event.button.windowID == windowID && mouse_time != game_event.button.timestamp) {
                point.x = game_event.button.x;
                point.y = game_event.button.y;
                fcs_gm = findButton(&point, game_btn[0]);
                if (point.y < 50) {
                    if (fcs_gm->id == GAME_STATE) {
                        result_num = ING;
                        changeState(GAME_STATE, "res/st_neutral.png");
                        initButton(game_btn[GAME_STATE]);
                        time_count = 0;
                        start_tm = SDL_GetTicks();
                        initGame(data);
                    } else if (fcs_gm->id == GAME_RETURN) {
                        new_game->next = true;
                        quit = true;
                        continue;
                    }
                } else {
                    if (game_event.button.button == SDL_BUTTON_LEFT)
                        change_type = LEFTDOWN;
                    else if (game_event.button.button == SDL_BUTTON_RIGHT) {
                        change_type = RIGHTDOWN;
                    }
                }
                mouse_time = game_event.button.timestamp;
            }
            break;
        case SDL_WINDOWEVENT:
            if (game_event.window.event == SDL_WINDOWEVENT_CLOSE)
                if (game_event.window.windowID == windowID) {
                    quit = true;
                    continue;
                }
        }
        if (change_type != -1) {
            result_num = changeMap(&map[point.x / CORE + 1][(point.y - 50) / CORE + 1], new_game, change_type);
            drawMap();
            changeNum(GAME_NUMBER_1, new_game->mines - flag_num);
            SDL_RenderPresent(MAIN_RENDER);
            change_type = -1;
        }
        if (result_num) {
            char* state_str;
            switch (result_num) {
            case WIN:
                flag_num = 0;
                state_str = "res/st_happy.png";
                getScore(new_game, time_count);
                break;
            case LOSE:
                state_str = "res/st_sad.png";
                break;
            }
            changeState(GAME_STATE, state_str);
        } else {
            if (search_number) {
                time_count = (SDL_GetTicks() - start_tm) / 1000;
                changeNum(GAME_TIME_1, time_count);
            } else {
                start_tm = SDL_GetTicks();
            }
        }

        if (manuQueue->next) {
            drawQueen(manuQueue, &initButton);
            SDL_RenderPresent(MAIN_RENDER);
        }
        SDL_Delay(20);
    }
    return new_game;
}

void changeNum(GAME_UIBTN UIbtn, int ticks)
{
    ticks = ticks<0?0:ticks;
    int time_s[4];
    for (int i = 3; i >= 0; i--) {
        time_s[i] = ticks % 10;
        ticks /= 10;
    }
    for (int i = 0; i <= 3; i++) {
        int btn = UIbtn + i;
        game_btn[btn]->text[9] = '0' + time_s[i];
        game_btn[btn]->content = initpTexture(game_btn[btn]);
        pushStack(manuQueue, game_btn[btn]);
    }
}
void initGame(int* data)
{
    new_game = initMessage(data[0], data[1], data[2]);
    map = initMineMap(new_game, 1);
    drawMap();
}

void changeState(GAME_UIBTN UIbtn, char* state_str)
{
    strcpy(game_btn[UIbtn]->text, state_str);
    game_btn[UIbtn]->content = initpTexture(game_btn[UIbtn]);
    pushStack(manuQueue, game_btn[UIbtn]);
}

void getScore(Message* msg, int time)
{
    msg->time = time;
    msg->score = msg->mines * msg->mines * 1024 / msg->length / msg->time;
}