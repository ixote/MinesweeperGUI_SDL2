
#include "various.h"
#ifndef _TOOL
#define _TOOL

void setUiColor(SDL_Color* color, int r, int g, int b)
{
    color->r = r;
    color->g = g;
    color->b = b;
}

SDL_Color* initUiColor()
{
    SDL_Color* colors = (SDL_Color*)calloc(UI_TOTAL, sizeof(SDL_Color));
    pushStack(freeGameMalloc, colors);
    setUiColor(&colors[UI_BLACK], 0, 0, 0);
    setUiColor(&colors[UI_BLACK_GREY], 50, 50, 50);
    setUiColor(&colors[UI_GREY], 100, 100, 100);
    setUiColor(&colors[UI_BG_WHITE], 200, 200, 200);
    setUiColor(&colors[UI_TXT_WHITE], 210, 210, 210);
    setUiColor(&colors[UI_WHITE], 255, 255, 255);
    return colors;
}

void initRe()
{

    search_number = 0;
    flag_num = 0;
    UIColor = initUiColor();
    //初始化字体
    font = TTF_OpenFont("source/STXINWEI.TTF", 96);
}

Buttons** initInputBtns()
{

    Buttons** button = (Buttons**)calloc(INPUT_TOTAL, sizeof(Buttons*));
    pushStack(freeGameMalloc, button);
    Buttons* head = (Buttons*)calloc(1, sizeof(Buttons));
    Buttons* temp = head;
    for (INPUT_UIBTN type = INPUT_EDIT_W; type < INPUT_TOTAL; type++) {
        button[type] = (Buttons*)calloc(1, sizeof(Buttons));
        pushStack(freeGameMalloc, button[type]);
        temp->next = button[type];
        temp = button[type];
        button[type]->text = (char*)calloc(50, sizeof(char));
        pushStack(freeGameMalloc, button[type]->text);

        button[type]->id = type;

        button[type]->cas = (CANVAS*)calloc(1, sizeof(CANVAS));
        pushStack(freeGameMalloc, button[type]->cas);
        button[type]->cas->width = INPUTSCREEN_WIDTH;
        button[type]->cas->height = INPUTSCREEN_HEIGHT;
        button[type]->cas->window = INPUT_WINDOW;
        button[type]->cas->render = input_render;
    }
    button[INPUT_BACKGROUND]->next = NULL;

    //INPUT_EDIT_W
    button[INPUT_EDIT_W]->color = UIColor[UI_BLACK];
    button[INPUT_EDIT_W]->mouse = A;
    // button[INPUT_EDIT_W]->text = NULL;
    button[INPUT_EDIT_W]->rect = NULL;
    button[INPUT_EDIT_W]->type = TXT;
    button[INPUT_EDIT_W]->content = NULL;

    //INPUT_EDIT_H
    button[INPUT_EDIT_H]->color = UIColor[UI_BLACK];
    button[INPUT_EDIT_H]->mouse = A;
    // button[INPUT_EDIT_H]->text = NULL;
    button[INPUT_EDIT_H]->rect = NULL;
    button[INPUT_EDIT_H]->type = TXT;
    button[INPUT_EDIT_H]->content = NULL;

    //INPUT_EDIT_N
    button[INPUT_EDIT_N]->color = UIColor[UI_BLACK];
    button[INPUT_EDIT_N]->mouse = A;
    // button[INPUT_EDIT_N]->text = NULL;
    button[INPUT_EDIT_N]->rect = NULL;
    button[INPUT_EDIT_N]->type = TXT;
    button[INPUT_EDIT_N]->content = NULL;

    //INPUT_BG_W
    button[INPUT_BG_W]->color = UIColor[UI_TXT_WHITE];
    button[INPUT_BG_W]->mouse = I;
    // button[INPUT_BG_W]->text = NULL;
    button[INPUT_BG_W]->rect = inputRect(button[INPUT_BG_W], 40, 20, 0.25, 0.5);
    button[INPUT_BG_W]->type = BG;
    button[INPUT_BG_W]->content = initpTexture(button[INPUT_BG_W]);

    //INPUT_BG_H
    button[INPUT_BG_H]->color = UIColor[UI_TXT_WHITE];
    button[INPUT_BG_H]->mouse = I;
    // button[INPUT_BG_H]->text = NULL;
    button[INPUT_BG_H]->rect = inputRect(button[INPUT_BG_H], 40, 20, 0.5, 0.5);
    button[INPUT_BG_H]->type = BG;
    button[INPUT_BG_H]->content = initpTexture(button[INPUT_BG_H]);

    //INPUT_BG_N
    button[INPUT_BG_N]->color = UIColor[UI_TXT_WHITE];
    button[INPUT_BG_N]->mouse = I;
    // button[INPUT_BG_N]->text = NULL;
    button[INPUT_BG_N]->rect = inputRect(button[INPUT_BG_N], 40, 20, 0.75, 0.5);
    button[INPUT_BG_N]->type = BG;
    button[INPUT_BG_N]->content = initpTexture(button[INPUT_BG_N]);

    //INPUT_CONFIRM
    button[INPUT_CONFIRM]->color = UIColor[UI_BLACK];
    button[INPUT_CONFIRM]->mouse = H;
    strcpy(button[INPUT_CONFIRM]->text, "确认");
    button[INPUT_CONFIRM]->rect = inputRect(button[INPUT_CONFIRM], 8 * strlen(button[INPUT_CONFIRM]->text), 25, 0.5, 0.8);
    button[INPUT_CONFIRM]->type = TXT;
    button[INPUT_CONFIRM]->content = initpTexture(button[INPUT_CONFIRM]);

    //INPUT_CANCEL
    // button[INPUT_LEFT_C]->color = UIColor[UI_BLACK];
    button[INPUT_LEFT_C]->mouse = A;
    strcpy(button[INPUT_LEFT_C]->text, "res/mine.png");
    button[INPUT_LEFT_C]->rect = inputRect(button[INPUT_LEFT_C], 25, 25, 0.33, 0.8);
    button[INPUT_LEFT_C]->type = IMG;
    button[INPUT_LEFT_C]->content = initpTexture(button[INPUT_LEFT_C]);
    //INPUT_CANCEL
    // button[INPUT_CANCEL]->color = UIColor[UI_BLACK];
    button[INPUT_RIGHT_C]->mouse = A;
    strcpy(button[INPUT_RIGHT_C]->text, "res/mine.png");
    button[INPUT_RIGHT_C]->rect = inputRect(button[INPUT_RIGHT_C], 25, 25, 0.66, 0.8);
    button[INPUT_RIGHT_C]->type = IMG;
    button[INPUT_RIGHT_C]->content = initpTexture(button[INPUT_RIGHT_C]);

    //INPUT_CANCEL
    // button[INPUT_LEFT_C]->color = UIColor[UI_BLACK];
    button[INPUT_LEFT_W]->mouse = A;
    strcpy(button[INPUT_LEFT_W]->text, "res/mine_wrong.png");
    button[INPUT_LEFT_W]->rect = inputRect(button[INPUT_LEFT_W], 25, 25, 0.33, 0.8);
    button[INPUT_LEFT_W]->type = IMG;
    button[INPUT_LEFT_W]->content = initpTexture(button[INPUT_LEFT_W]);
    //INPUT_CANCEL
    // button[INPUT_CANCEL]->color = UIColor[UI_BLACK];
    button[INPUT_RIGHT_W]->mouse = A;
    strcpy(button[INPUT_RIGHT_W]->text, "res/mine_wrong.png");
    button[INPUT_RIGHT_W]->rect = inputRect(button[INPUT_RIGHT_W], 25, 25, 0.66, 0.8);
    button[INPUT_RIGHT_W]->type = IMG;
    button[INPUT_RIGHT_W]->content = initpTexture(button[INPUT_RIGHT_W]);

    //INPUT_TEXT_W
    button[INPUT_TEXT_W]->color = UIColor[UI_BLACK];
    button[INPUT_TEXT_W]->mouse = A;
    strcpy(button[INPUT_TEXT_W]->text, "宽:");
    button[INPUT_TEXT_W]->rect = inputRect(button[INPUT_TEXT_W], 25, 20, 0.12, 0.5);
    button[INPUT_TEXT_W]->type = TXT;
    button[INPUT_TEXT_W]->content = initpTexture(button[INPUT_TEXT_W]);

    //INPUT_TEXT_H
    button[INPUT_TEXT_H]->color = UIColor[UI_BLACK];
    button[INPUT_TEXT_H]->mouse = A;
    strcpy(button[INPUT_TEXT_H]->text, "高:");
    button[INPUT_TEXT_H]->rect = inputRect(button[INPUT_TEXT_H], 25, 20, 0.37, 0.5);
    button[INPUT_TEXT_H]->type = TXT;
    button[INPUT_TEXT_H]->content = initpTexture(button[INPUT_TEXT_H]);

    //INPUT_TEXT_N
    button[INPUT_TEXT_N]->color = UIColor[UI_BLACK];
    button[INPUT_TEXT_N]->mouse = A;
    strcpy(button[INPUT_TEXT_N]->text, "雷:");
    button[INPUT_TEXT_N]->rect = inputRect(button[INPUT_TEXT_N], 25, 20, 0.62, 0.5);
    button[INPUT_TEXT_N]->type = TXT;
    button[INPUT_TEXT_N]->content = initpTexture(button[INPUT_TEXT_N]);

    //INPUT_ALERT_W
    button[INPUT_ALERT_W]->color = UIColor[UI_BLACK_GREY];
    button[INPUT_ALERT_W]->mouse = A;
    strcpy(button[INPUT_ALERT_W]->text, "(9-99)");
    button[INPUT_ALERT_W]->rect = inputRect(button[INPUT_ALERT_W], 6 * strlen(button[INPUT_ALERT_W]->text), 18, 0.12, 0.62);
    button[INPUT_ALERT_W]->type = TXT;
    button[INPUT_ALERT_W]->content = initpTexture(button[INPUT_ALERT_W]);

    //INPUT_ALERT_H
    button[INPUT_ALERT_H]->color = UIColor[UI_BLACK_GREY];
    button[INPUT_ALERT_H]->mouse = A;
    strcpy(button[INPUT_ALERT_H]->text, "(9-99)");
    button[INPUT_ALERT_H]->rect = inputRect(button[INPUT_ALERT_H], 6 * strlen(button[INPUT_ALERT_H]->text), 18, 0.37, 0.62);
    button[INPUT_ALERT_H]->type = TXT;
    button[INPUT_ALERT_H]->content = initpTexture(button[INPUT_ALERT_H]);

    //INPUT_WG_W
    button[INPUT_WG_W]->color = UIColor[UI_BLACK_GREY];
    button[INPUT_WG_W]->mouse = A;
    strcpy(button[INPUT_WG_W]->text, "宽度至少为9");
    button[INPUT_WG_W]->rect = inputRect(button[INPUT_WG_W], 6 * strlen(button[INPUT_WG_W]->text), 20, 0.5, 0.25);
    button[INPUT_WG_W]->type = TXT;
    button[INPUT_WG_W]->content = initpTexture(button[INPUT_WG_W]);

    //INPUT_WG_H
    button[INPUT_WG_H]->color = UIColor[UI_BLACK_GREY];
    button[INPUT_WG_H]->mouse = A;
    strcpy(button[INPUT_WG_H]->text, "高度至少为9");
    button[INPUT_WG_H]->rect = inputRect(button[INPUT_WG_H], 6 * strlen(button[INPUT_WG_H]->text), 20, 0.5, 0.25);
    button[INPUT_WG_H]->type = TXT;
    button[INPUT_WG_H]->content = initpTexture(button[INPUT_WG_H]);

    //INPUT_WG_N
    button[INPUT_WG_N]->color = UIColor[UI_BLACK_GREY];
    button[INPUT_WG_N]->mouse = A;
    strcpy(button[INPUT_WG_N]->text, "雷的范围为1~(宽-1)*(高-1)");
    button[INPUT_WG_N]->rect = inputRect(button[INPUT_WG_N], 6 * strlen(button[INPUT_WG_N]->text), 20, 0.5, 0.25);
    button[INPUT_WG_N]->type = TXT;
    button[INPUT_WG_N]->content = initpTexture(button[INPUT_WG_N]);

    //INPUT_WG_I
    button[INPUT_WG_I]->color = UIColor[UI_BLACK_GREY];
    button[INPUT_WG_I]->mouse = A;
    strcpy(button[INPUT_WG_I]->text, "请正确填写数据!!!");
    button[INPUT_WG_I]->rect = inputRect(button[INPUT_WG_I], 6 * strlen(button[INPUT_WG_I]->text), 20, 0.5, 0.25);
    button[INPUT_WG_I]->type = TXT;
    button[INPUT_WG_I]->content = initpTexture(button[INPUT_WG_I]);

    //INPUT_LOG_RT
    // button[INPUT_LOG_RT]->color = {};
    button[INPUT_LOG_RT]->mouse = A;
    strcpy(button[INPUT_LOG_RT]->text, "res/st_happy.png");
    button[INPUT_LOG_RT]->rect = inputRect(button[INPUT_LOG_RT], 30, 30, 0.9, 0.5);
    button[INPUT_LOG_RT]->type = IMG;
    button[INPUT_LOG_RT]->content = initpTexture(button[INPUT_LOG_RT]);

    //INPUT_LOG_WG
    // button[INPUT_LOG_RT]->color = {};
    button[INPUT_LOG_WG]->mouse = A;
    strcpy(button[INPUT_LOG_WG]->text, "res/st_sad.png");
    button[INPUT_LOG_WG]->rect = inputRect(button[INPUT_LOG_WG], 30, 30, 0.9, 0.5);
    button[INPUT_LOG_WG]->type = IMG;
    button[INPUT_LOG_WG]->content = initpTexture(button[INPUT_LOG_WG]);

    //INPUT_LOG_WG
    // button[INPUT_LOG_RT]->color = {};
    button[INPUT_LOG_NM]->mouse = A;
    strcpy(button[INPUT_LOG_NM]->text, "res/st_neutral.png");
    button[INPUT_LOG_NM]->rect = inputRect(button[INPUT_LOG_NM], 30, 30, 0.9, 0.5);
    button[INPUT_LOG_NM]->type = IMG;
    button[INPUT_LOG_NM]->content = initpTexture(button[INPUT_LOG_NM]);

    //INPUT_WG_CLC
    button[INPUT_WG_CLC]->color = UIColor[UI_BG_WHITE];
    button[INPUT_WG_CLC]->mouse = A;
    // button[INPUT_WG_CLC]->text = NULL;
    button[INPUT_WG_CLC]->rect = inputRect(button[INPUT_WG_CLC], INPUTSCREEN_WIDTH, 20, 0.5, 0.25);
    button[INPUT_WG_CLC]->type = BG;
    button[INPUT_WG_CLC]->content = initpTexture(button[INPUT_WG_CLC]);

    //INPUT_BACKGROUND
    button[INPUT_BACKGROUND]->color = UIColor[UI_BG_WHITE];
    button[INPUT_BACKGROUND]->mouse = A;
    // button[INPUT_BACKGROUND]->text = NULL;
    button[INPUT_BACKGROUND]->rect = inputRect(button[INPUT_BACKGROUND], INPUTSCREEN_WIDTH, INPUTSCREEN_HEIGHT, 0.5, 0.5);
    button[INPUT_BACKGROUND]->type = BG;
    button[INPUT_BACKGROUND]->content = initpTexture(button[INPUT_BACKGROUND]);
    return button;
}

Buttons** initManuBtns()
{
    Buttons** button = (Buttons**)calloc(MANU_TOTAL, sizeof(Buttons*));
    pushStack(freeGameMalloc, button);
    //manu
    Buttons* head = (Buttons*)calloc(1, sizeof(Buttons));
    Buttons* temp = head;
    for (MANU_UIBTN type = MANU_TITLE; type < MANU_TOTAL; type++) {
        button[type] = (Buttons*)calloc(1, sizeof(Buttons));
        pushStack(freeGameMalloc, button[type]);
        temp->next = button[type];
        temp = button[type];
        button[type]->text = (char*)calloc(50, sizeof(char));
        pushStack(freeGameMalloc, button[type]->text);

        button[type]->id = type;
        button[type]->cas = (CANVAS*)calloc(1, sizeof(CANVAS));
        pushStack(freeGameMalloc, button[type]->cas);
        button[type]->cas->width = MANU_WIDTH;
        button[type]->cas->height = MANU_HEIGHT;
        button[type]->cas->window = MAIN_WINDOW;
        button[type]->cas->render = MAIN_RENDER;
    }
    button[MANU_BACKGROUND]->next = NULL;

    //MANU_TITLE
    button[MANU_TITLE]->color = UIColor[UI_BLACK];
    button[MANU_TITLE]->mouse = A;
    strcpy(button[MANU_TITLE]->text, "扫 雷");
    button[MANU_TITLE]->rect = inputRect(button[MANU_TITLE], 250, 100, 0.5, 0.2);
    button[MANU_TITLE]->type = TXT;
    button[MANU_TITLE]->content = initpTexture(button[MANU_TITLE]);

    //MANU_VERSION
    button[MANU_VERSION]->color = UIColor[UI_BLACK];
    button[MANU_VERSION]->mouse = A;
    strcpy(button[MANU_VERSION]->text, "v0.1");
    button[MANU_VERSION]->rect = inputRect(button[MANU_VERSION], 60, 30, 0.75, 0.1);
    button[MANU_VERSION]->type = TXT;
    button[MANU_VERSION]->content = initpTexture(button[MANU_VERSION]);

    //MANU_EASY
    button[MANU_EASY]->color = UIColor[UI_BLACK];
    button[MANU_EASY]->mouse = H;
    strcpy(button[MANU_EASY]->text, "简单");
    button[MANU_EASY]->rect = inputRect(button[MANU_EASY], 80, 40, 0.5, 0.4);
    button[MANU_EASY]->type = TXT;
    button[MANU_EASY]->content = initpTexture(button[MANU_EASY]);

    //MANU_MIDDLE
    button[MANU_MIDDLE]->color = UIColor[UI_BLACK];
    button[MANU_MIDDLE]->mouse = H;
    strcpy(button[MANU_MIDDLE]->text, "中等");
    button[MANU_MIDDLE]->rect = inputRect(button[MANU_MIDDLE], 80, 40, 0.5, 0.5);
    button[MANU_MIDDLE]->type = TXT;
    button[MANU_MIDDLE]->content = initpTexture(button[MANU_MIDDLE]);

    //MANU_HARD
    button[MANU_HARD]->color = UIColor[UI_BLACK];
    button[MANU_HARD]->mouse = H;
    strcpy(button[MANU_HARD]->text, "困难");
    button[MANU_HARD]->rect = inputRect(button[MANU_HARD], 80, 40, 0.5, 0.6);
    button[MANU_HARD]->type = TXT;
    button[MANU_HARD]->content = initpTexture(button[MANU_HARD]);

    //MANU_SELF
    button[MANU_SELF]->color = UIColor[UI_BLACK];
    button[MANU_SELF]->mouse = H;
    strcpy(button[MANU_SELF]->text, "自定义");
    button[MANU_SELF]->rect = inputRect(button[MANU_SELF], 120, 40, 0.5, 0.7);
    button[MANU_SELF]->type = TXT;
    button[MANU_SELF]->content = initpTexture(button[MANU_SELF]);

    //MANU_QUIT
    button[MANU_QUIT]->color = UIColor[UI_BLACK];
    button[MANU_QUIT]->mouse = H;
    strcpy(button[MANU_QUIT]->text, "退出");
    button[MANU_QUIT]->rect = inputRect(button[MANU_QUIT], 80, 40, 0.66, 0.8);
    button[MANU_QUIT]->type = TXT;
    button[MANU_QUIT]->content = initpTexture(button[MANU_QUIT]);

    //MANU_START
    button[MANU_START]->color = UIColor[UI_BLACK];
    button[MANU_START]->mouse = H;
    strcpy(button[MANU_START]->text, "开始");
    button[MANU_START]->rect = inputRect(button[MANU_START], 80, 40, 0.33, 0.8);
    button[MANU_START]->type = TXT;
    button[MANU_START]->content = initpTexture(button[MANU_START]);

    //MANU_ALERT_EASY
    button[MANU_ALERT_EASY]->color = UIColor[UI_BLACK_GREY];
    button[MANU_ALERT_EASY]->mouse = A;
    strcpy(button[MANU_ALERT_EASY]->text, "宽:9 高:9 雷:10");
    button[MANU_ALERT_EASY]->rect = inputRect(button[MANU_ALERT_EASY], 8 * strlen(button[MANU_ALERT_EASY]->text), 20, 0.5, 0.32);
    button[MANU_ALERT_EASY]->type = TXT;
    button[MANU_ALERT_EASY]->content = initpTexture(button[MANU_ALERT_EASY]);

    //MANU_ALERT_MIDDLE
    button[MANU_ALERT_MIDDLE]->color = UIColor[UI_BLACK_GREY];
    button[MANU_ALERT_MIDDLE]->mouse = A;
    strcpy(button[MANU_ALERT_MIDDLE]->text, "宽:16 高:16 雷:40");
    button[MANU_ALERT_MIDDLE]->rect = inputRect(button[MANU_ALERT_MIDDLE], 8 * strlen(button[MANU_ALERT_MIDDLE]->text), 20, 0.5, 0.32);
    button[MANU_ALERT_MIDDLE]->type = TXT;
    button[MANU_ALERT_MIDDLE]->content = initpTexture(button[MANU_ALERT_MIDDLE]);

    //MANU_ALERT_HARD
    button[MANU_ALERT_HARD]->color = UIColor[UI_BLACK_GREY];
    button[MANU_ALERT_HARD]->mouse = A;
    strcpy(button[MANU_ALERT_HARD]->text, "宽:30 高:16 雷:99");
    button[MANU_ALERT_HARD]->rect = inputRect(button[MANU_ALERT_HARD], 8 * strlen(button[MANU_ALERT_HARD]->text), 20, 0.5, 0.32);
    button[MANU_ALERT_HARD]->type = TXT;
    button[MANU_ALERT_HARD]->content = initpTexture(button[MANU_ALERT_HARD]);

    //MANU_ALERT_SELF
    button[MANU_ALERT_SELF]->color = UIColor[UI_BLACK_GREY];
    button[MANU_ALERT_SELF]->mouse = A;
    // button[MANU_ALERT_SELF]->text = "宽:30 高:16 雷:99";
    // button[MANU_ALERT_SELF]->rect = inputRect(button[MANU_ALERT_SELF], 8*strlen(button[MANU_ALERT_SELF]->text), 20, 0.5, 0.3);
    button[MANU_ALERT_SELF]->type = TXT;
    // button[MANU_ALERT_SELF]->content = initpTexture(button[MANU_ALERT_SELF]);

    //MANU_ALERT_CLC
    button[MANU_ALERT_CLC]->color = UIColor[UI_BG_WHITE];
    button[MANU_ALERT_CLC]->mouse = A;
    // button[MANU_ALERT_CLC]->text = "宽:16 长:16 雷:40";
    button[MANU_ALERT_CLC]->rect = inputRect(button[MANU_ALERT_CLC], MANU_WIDTH, 20, 0.5, 0.32);
    button[MANU_ALERT_CLC]->type = BG;
    button[MANU_ALERT_CLC]->content = initpTexture(button[MANU_ALERT_CLC]);

    //MANU_MARK_EASY
    // button[MANU_MARK_EASY]->color = black;
    button[MANU_MARK_EASY]->mouse = A;
    strcpy(button[MANU_MARK_EASY]->text, "res/flag.png");
    button[MANU_MARK_EASY]->rect = inputRect(button[MANU_MARK_EASY], 30, 30, 0.75, 0.4);
    button[MANU_MARK_EASY]->type = IMG;
    button[MANU_MARK_EASY]->content = initpTexture(button[MANU_MARK_EASY]);

    //MANU_MARK_MIDDLE
    // button[MANU_MARK_MIDDLE]->color = black;
    button[MANU_MARK_MIDDLE]->mouse = A;
    strcpy(button[MANU_MARK_MIDDLE]->text, "res/flag.png");
    button[MANU_MARK_MIDDLE]->rect = inputRect(button[MANU_MARK_MIDDLE], 30, 30, 0.75, 0.5);
    button[MANU_MARK_MIDDLE]->type = IMG;
    button[MANU_MARK_MIDDLE]->content = initpTexture(button[MANU_MARK_MIDDLE]);

    //MANU_MARK_HARD
    // button[MANU_MARK_MIDDLE]->color = black;
    button[MANU_MARK_HARD]->mouse = A;
    strcpy(button[MANU_MARK_HARD]->text, "res/flag.png");
    button[MANU_MARK_HARD]->rect = inputRect(button[MANU_MARK_HARD], 30, 30, 0.75, 0.6);
    button[MANU_MARK_HARD]->type = IMG;
    button[MANU_MARK_HARD]->content = initpTexture(button[MANU_MARK_HARD]);

    //MANU_MARK_SELF
    // button[MANU_MARK_MIDDLE]->color = black;
    button[MANU_MARK_SELF]->mouse = A;
    strcpy(button[MANU_MARK_SELF]->text, "res/flag.png");
    button[MANU_MARK_SELF]->rect = inputRect(button[MANU_MARK_SELF], 30, 30, 0.75, 0.7);
    button[MANU_MARK_SELF]->type = IMG;
    button[MANU_MARK_SELF]->content = initpTexture(button[MANU_MARK_SELF]);

    //MANU_MARK_CLC
    button[MANU_MARK_CLC]->color = UIColor[UI_BG_WHITE];
    button[MANU_MARK_CLC]->mouse = A;
    // button[MANU_MARK_CLC]->text = "res/flag.png";
    button[MANU_MARK_CLC]->rect = inputRect(button[MANU_MARK_CLC], 30, 0.4 * MANU_HEIGHT, 0.75, 0.55);
    button[MANU_MARK_CLC]->type = BG;
    button[MANU_MARK_CLC]->content = initpTexture(button[MANU_MARK_CLC]);

    //MANU_BACKGROUND
    button[MANU_BACKGROUND]->color = UIColor[UI_BG_WHITE];
    button[MANU_BACKGROUND]->mouse = A;
    // button[MANU_MARK_CLC]->text = "res/flag.png";
    button[MANU_BACKGROUND]->rect = inputRect(button[MANU_BACKGROUND], MANU_WIDTH, MANU_HEIGHT, 0.5, 0.5);
    button[MANU_BACKGROUND]->type = BG;
    button[MANU_BACKGROUND]->content = initpTexture(button[MANU_BACKGROUND]);
    return button;
}
Buttons** initGameBtns()
{
    Buttons** button = (Buttons**)calloc(GAME_TOTAL, sizeof(Buttons*));
    pushStack(freeGameMalloc, button);
    //manu
    Buttons* head = (Buttons*)calloc(1, sizeof(Buttons));
    Buttons* temp = head;

    for (MANU_UIBTN type = MANU_TITLE; type < GAME_TOTAL; type++) {
        button[type] = (Buttons*)calloc(1, sizeof(Buttons));
        pushStack(freeGameMalloc, button[type]);
        temp->next = button[type];
        temp = button[type];
        button[type]->text = (char*)calloc(100, sizeof(char));
        pushStack(freeGameMalloc, button[type]->text);

        button[type]->id = type;
        button[type]->cas = (CANVAS*)calloc(1, sizeof(CANVAS));
        pushStack(freeGameMalloc, button[type]->cas);
        button[type]->cas->width = GAME_MANU_WIDTH;
        button[type]->cas->height = GAME_MANU_HEIGHT;
        button[type]->cas->window = MAIN_WINDOW;
        button[type]->cas->render = MAIN_RENDER;
    }
    button[GAME_BACKGROUND]->next = NULL;

    //MANU_TITLE
    // button[GAME_TIME_1]->color = UIColor[UI_BLACK];
    button[GAME_TIME_1]->mouse = A;
    strcpy(button[GAME_TIME_1]->text, "res/time_0.png");
    button[GAME_TIME_1]->rect = inputRect(button[GAME_TIME_1], 20, 40, 0.7, 0.5);
    button[GAME_TIME_1]->type = IMG;
    button[GAME_TIME_1]->content = initpTexture(button[GAME_TIME_1]);

    // button[GAME_TIME_2]->color = UIColor[UI_BLACK];
    button[GAME_TIME_2]->mouse = A;
    strcpy(button[GAME_TIME_2]->text, "res/time_0.png");
    button[GAME_TIME_2]->rect = inputRect(button[GAME_TIME_2], 20, 40, 0.7 + 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_TIME_2]->type = IMG;
    button[GAME_TIME_2]->content = initpTexture(button[GAME_TIME_2]);

    // button[GAME_TIME_3]->color = UIColor[UI_BLACK];
    button[GAME_TIME_3]->mouse = A;
    strcpy(button[GAME_TIME_3]->text, "res/time_0.png");
    button[GAME_TIME_3]->rect = inputRect(button[GAME_TIME_3], 20, 40, 0.7 + 40.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_TIME_3]->type = IMG;
    button[GAME_TIME_3]->content = initpTexture(button[GAME_TIME_3]);

    // button[GAME_TIME_4]->color = UIColor[UI_BLACK];
    button[GAME_TIME_4]->mouse = A;
    strcpy(button[GAME_TIME_4]->text, "res/time_0.png");
    button[GAME_TIME_4]->rect = inputRect(button[GAME_TIME_4], 20, 40, 0.7 + 60.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_TIME_4]->type = IMG;
    button[GAME_TIME_4]->content = initpTexture(button[GAME_TIME_4]);

    // button[GAME_TIME_4]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_1]->mouse = A;
    strcpy(button[GAME_NUMBER_1]->text, "res/time_0.png");
    button[GAME_NUMBER_1]->rect = inputRect(button[GAME_NUMBER_1], 20, 40, 0.3 - 60.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_NUMBER_1]->type = IMG;
    button[GAME_NUMBER_1]->content = initpTexture(button[GAME_NUMBER_1]);
    // button[GAME_TIME_3]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_2]->mouse = A;
    strcpy(button[GAME_NUMBER_2]->text, "res/time_0.png");
    button[GAME_NUMBER_2]->rect = inputRect(button[GAME_NUMBER_2], 20, 40, 0.3 - 40.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_NUMBER_2]->type = IMG;
    button[GAME_NUMBER_2]->content = initpTexture(button[GAME_NUMBER_2]);
    // button[GAME_TIME_2]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_3]->mouse = A;
    strcpy(button[GAME_NUMBER_3]->text, "res/time_0.png");
    button[GAME_NUMBER_3]->rect = inputRect(button[GAME_NUMBER_3], 20, 40, 0.3 - 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_NUMBER_3]->type = IMG;
    button[GAME_NUMBER_3]->content = initpTexture(button[GAME_NUMBER_3]);

    // button[GAME_NUMBER_1]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_4]->mouse = A;
    strcpy(button[GAME_NUMBER_4]->text, "res/time_0.png");
    button[GAME_NUMBER_4]->rect = inputRect(button[GAME_NUMBER_4], 20, 40, 0.3, 0.5);
    button[GAME_NUMBER_4]->type = IMG;
    button[GAME_NUMBER_4]->content = initpTexture(button[GAME_NUMBER_4]);
    // button[GAME_STATE_NORMAL]
    button[GAME_STATE]->mouse = H;
    strcpy(button[GAME_STATE]->text, "res/st_neutral.png");
    button[GAME_STATE]->rect = inputRect(button[GAME_STATE], 40, 40, 0.5+ 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_STATE]->type = IMG;
    button[GAME_STATE]->content = initpTexture(button[GAME_STATE]);
    // button[GAME_RETURN_NORMAL]
    button[GAME_RETURN]->mouse = H;
    strcpy(button[GAME_RETURN]->text, "res/ret.png");
    button[GAME_RETURN]->rect = inputRect(button[GAME_RETURN], 40, 40, 0.5 - 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_RETURN]->type = TXT;
    button[GAME_RETURN]->content = initpTexture(button[GAME_RETURN]);

    //GAME_BACKGROUND
    button[GAME_BACKGROUND]->color = UIColor[UI_BG_WHITE];
    button[GAME_BACKGROUND]->mouse = A;
    // button[GAME_BACKGROUND]->text = "res/white.png";
    button[GAME_BACKGROUND]->rect = inputRect(button[GAME_BACKGROUND], GAME_MANU_WIDTH, GAME_MANU_HEIGHT, 0.5, 0.5);
    button[GAME_BACKGROUND]->type = BG;
    button[GAME_BACKGROUND]->content = initpTexture(button[GAME_BACKGROUND]);

    return button;
}
void initButton(Buttons* btn)
{
    SDL_RenderCopy(btn->cas->render, btn->content, NULL, btn->rect);
}

SDL_Texture* initpTexture(Buttons* btn)
{
    SDL_Window* window = btn->cas->window;
    SDL_Renderer* render = btn->cas->render;
    char* text = btn->text;
    SDL_Color color = btn->color;
    switch (btn->type) {
    case TXT: {
        SDL_Surface* pTextSurface = TTF_RenderUTF8_Blended(font, text, color);
        SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(render, pTextSurface);
        SDL_FreeSurface(pTextSurface);
        pushStack(freeGameTexture, pTextSurface);

        return pTextTexture;
    }
    case IMG: {
        SDL_Surface* image = IMG_Load(text);
        SDL_Surface* opt_image = SDL_ConvertSurfaceFormat(image, SDL_GetWindowPixelFormat(window), 0);
        SDL_Texture* pImgTexture = SDL_CreateTextureFromSurface(render, opt_image);
        SDL_FreeSurface(opt_image);
        SDL_FreeSurface(image);
        pushStack(freeGameTexture, pImgTexture);
        return pImgTexture;
    }
    case BG: {
        SDL_Surface* image = IMG_Load("res/white.png");
        SDL_Surface* opt_image = SDL_ConvertSurfaceFormat(image, SDL_GetWindowPixelFormat(window), 0);
        SDL_Texture* pImgTexture = SDL_CreateTextureFromSurface(render, opt_image);
        SDL_FreeSurface(opt_image);
        SDL_FreeSurface(image);
        SDL_SetTextureColorMod(pImgTexture, color.r, color.g, color.b);
        pushStack(freeGameTexture, pImgTexture);
        return pImgTexture;
    }
    default:
        return NULL;
    }
}
SDL_Rect* inputRect(Buttons* btn, double w, double h, double x_ray, double y_ray)
{
    SDL_Rect* rect = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    pushStack(freeGameMalloc, rect);
    rect->w = w;
    rect->h = h;
    rect->x = btn->cas->width * x_ray - rect->w / 2;
    rect->y = btn->cas->height * y_ray - rect->h / 2;
    return rect;
}
void setMouse(Buttons* btn)
{
    if (btn->mouse) {
        MOUSE mouse_type = btn->mouse;
        switch (mouse_type) {
        case I:
            SDL_SetCursor(cursor_i);
            break;
        case H:
            SDL_SetCursor(cursor_h);
            break;
        default:
            SDL_SetCursor(cursor_a);
        }
    } else {
        SDL_SetCursor(cursor_a);
    }
}

Buttons* findButton(SDL_Point* point, Buttons* start)
{
    for (Buttons* i = start; i != NULL; i = i->next) {
        if (i->rect) {
            if (SDL_PointInRect(point, i->rect)) {
                return i;
            }
        }
    }
    return start;
}

#endif