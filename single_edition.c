#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_ttf.h>
#include <Windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _node {
    void* value;
    struct _node* pre;
    struct _node* next;
} Node;

Node* initQueue()
{
    Node* head = (Node*)calloc(1, sizeof(Node));
    head->value = NULL;
    head->pre = head;
    head->next = NULL;
    return head;
}

void pushQueue(Node* head, void* node)
{
    Node* new = (Node*)calloc(1, sizeof(Node));
    new->value = node;
    new->next = NULL;
    new->pre = NULL;
    Node* temp = head->pre;
    temp->next = new;
    head->pre = new;
}
void pushStack(Node* head, void* node)
{
    Node* new = (Node*)calloc(1, sizeof(Node));
    new->value = node;
    Node* temp = head->next;
    head->next = new;
    new->next = temp;
}

void* popQueue(Node* head)
{
    void* value = NULL;
    Node* get = head->next;
    if (get) {
        value = (void*)get->value;
        head->next = get->next;
        free(get);
    } else {
        head->pre = head;
    }
    return value;
}

void drawQueen(Node* queen, void (*func)())
{
    while (queen->next) {
        if (queen->next->value == NULL) {
            popQueue(queen);
            continue;
        }
        func(popQueue(queen));
    }
}

#define INITVAR(name) SDL_Texture* name = NULL;
#define INITIMAGE(name) name = initText("res/" #name ".png");
#define FREEIMAG(name) SDL_DestroyTexture(name);
//图片资源的宏列表
#define IMAGE_LISTS(_) \
    _(num_0)           \
    _(num_1)           \
    _(num_2)           \
    _(num_3)           \
    _(num_4)           \
    _(num_5)           \
    _(num_6)           \
    _(num_7)           \
    _(num_8)           \
    _(time_0)          \
    _(time_1)          \
    _(time_2)          \
    _(time_3)          \
    _(time_4)          \
    _(time_5)          \
    _(time_6)          \
    _(time_7)          \
    _(time_8)          \
    _(time_9)          \
    _(blank)           \
    _(ask)             \
    _(flag)            \
    _(flag_wrong)      \
    _(mine)            \
    _(mine_blood)      \
    _(mine_wrong)      \
    _(st_happy)        \
    _(st_neutral)      \
    _(st_sad)
// INITVAR(IMAGE_LISTS);

//单位状态变量
typedef enum _status {
    NONE,
    FLAG,
    WONDER,
    WINOVER,
    TYPE_I_ERROR,
    TYPE_II_ERROR,
    MOUSEDOWN
} Status;
typedef enum _Res {
    ING,
    WIN,
    LOSE
} GAME_RESULT;
//键盘操作变量
enum _mousetype { LEFTDOWN,
    RIGHTDOWN } MOUSETYPE;
typedef enum {
    IMG,
    TXT,
    BG
} ATTRIB;
typedef enum {
    UI_BLACK,
    UI_BLACK_GREY,
    UI_GREY,
    UI_BG_WHITE,
    UI_TXT_WHITE,
    UI_WHITE,
    UI_TOTAL
} UI_COLOR;

typedef enum {
    INPUT_EDIT_W, //宽字符
    INPUT_EDIT_H, //高字符
    INPUT_EDIT_N, //雷字符
    INPUT_BG_W, // 宽输入框
    INPUT_BG_H, // 高输入框
    INPUT_BG_N, // 雷数输入框
    INPUT_CONFIRM, //确认按钮
    INPUT_LEFT_C,
    INPUT_RIGHT_C,
    INPUT_LEFT_W,
    INPUT_RIGHT_W,
    INPUT_TEXT_W, // 宽输入框标识
    INPUT_TEXT_H, //高输入框标识
    INPUT_TEXT_N, // 数字输入框标识
    INPUT_ALERT_W, // 宽提示
    INPUT_ALERT_H, // 高提示
    INPUT_WG_W, // 宽警告
    INPUT_WG_H, // 高警告
    INPUT_WG_N, //雷数警告
    INPUT_WG_I, //错误输入警告
    INPUT_LOG_RT, //通过标志
    INPUT_LOG_WG, // 拒绝标志
    INPUT_LOG_NM, //一般标记
    INPUT_WG_CLC, //清除警告
    INPUT_BACKGROUND, //背景
    INPUT_TOTAL,
} INPUT_UIBTN;

typedef enum {
    MANU_TITLE, //菜单标题
    MANU_VERSION, // 版本号
    MANU_EASY, // 简单选项
    MANU_MIDDLE, // 中等选项
    MANU_HARD, // 困难选项
    MANU_SELF, // 自定义选项
    MANU_START, // 开始
    MANU_QUIT, // 退出

    MANU_ALERT_EASY, // 简单的标签
    MANU_ALERT_MIDDLE, //中等的标签
    MANU_ALERT_HARD, // 困难的标签
    MANU_ALERT_SELF, // 自定义的标签
    MANU_ALERT_CLC, //清理标签
    MANU_MARK_EASY, // 简单选择标记
    MANU_MARK_MIDDLE, // 中等选择标记
    MANU_MARK_HARD, // 困难选择标记
    MANU_MARK_SELF, //自定义选择标记
    MANU_MARK_CLC, // 选择标记清除
    MANU_BACKGROUND, // 菜单背景

    // MANU_AVATAR, // 头像 //TODO：连接数据库，储存本地玩家数据
    // MANU_NAME, // 姓名
    // MANU_LOAD, //读档
    // MANU_LOGIN, //登陆
    // MANU_RANK, //历史分数排名
    // MANU_LOGON, //注册

    MANU_TOTAL
} MANU_UIBTN;
typedef enum {
    GAME_TIME_1, //一号时间位置
    GAME_TIME_2, // 二号时间位置
    GAME_TIME_3, //三号时间位置
    GAME_TIME_4, // 四号时间位置

    GAME_STATE, // 中间的状态脸
    GAME_RETURN,
    GAME_NUMBER_1, //剩余数量-个
    GAME_NUMBER_2, //剩余数量-十
    GAME_NUMBER_3, //剩余数量-百
    GAME_NUMBER_4, //剩余数量-千

    GAME_MANU_RESTART, //重新开始
    GAME_MANU_EXIT, //退出
    GAME_MANU_STOP, //暂停
    // GAME_MANU_ANALYSIS,  //分析
    //GAME_MANU_AI, 自动扫雷
    GAME_MANU_RETURN, //返回主菜单
    // GAME_STORE, //保存

    GAME_BACKGROUND, // 菜单背景
    GAME_TOTAL
} GAME_UIBTN;
//panel.c
typedef enum {
    PANEL_WIDTH,
    PANEL_HEIGHT,
    PANEL_NUMBER,
    PANEL_TIME,
    PANEL_SCORE,
    PANEL_BACKGROUND, // 菜单背景
    PANEL_TOTAL
} PANEL_UIBTN;

typedef enum _mouse {
    A,
    H,
    I
} MOUSE;

//地图单位的结构
typedef struct _pot {
    bool init;
    bool search;
    char mark;
    int click;
    int size;
    Status status;
    struct _pot** around;
    SDL_Rect rect;
} Minepot;
typedef struct {
    bool state;
    bool next;
    int width;
    int hight;
    int length;
    int mines;
    int time;
    int score;
    SDL_Rect* rect;
} Message;
typedef struct _canvas {
    unsigned int width;
    unsigned int height;
    SDL_Renderer* render;
    SDL_Window* window;
} CANVAS;
typedef struct _button {
    int id;
    ATTRIB type;
    MOUSE mouse;
    SDL_Color color;
    char* text;
    SDL_Rect* rect;
    CANVAS* cas;
    SDL_Texture* content;
    struct _button* next;
} Buttons;

//预定义值:
int CORE;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
//窗口列表
SDL_Window* GAME_WINDOW; //游戏主窗口
SDL_Renderer* GAME_RENDER; // 游戏画布
Node* eventQueue;
Node* mineQueue;
Node* expandQueue;
const int INPUTSCREEN_WIDTH = 320;
const int INPUTSCREEN_HEIGHT = 120;
//初始value
SDL_Window* INPUT_WINDOW = NULL;
SDL_Renderer* input_render = NULL;
TTF_Font* font = NULL;
Buttons** button = NULL;
Node* updateQueen = NULL;
// Node* freeGameMalloc = NULL;
// Node* freeGameTexture = NULL;
SDL_Cursor* cursor_i = NULL;
SDL_Cursor* cursor_a = NULL;
SDL_Cursor* cursor_h = NULL;
SDL_Cursor* cursor_w = NULL;
SDL_Color* UIColor;
const int MANU_WIDTH = 360;
const int MANU_HEIGHT = 600;
SDL_Window* MAIN_WINDOW;
SDL_Renderer* MAIN_RENDER;
int* set_value = NULL;
int* void_value = NULL;
int GAME_MANU_WIDTH;
int GAME_MANU_HEIGHT = 50;
Buttons** manu_btns;
int search_number = 0;
int flag_num = 0;
Node* freeGameMalloc = NULL;
Node* freeGameTexture = NULL;
bool confirm_flag = false;
bool confirm = false;
Buttons** input_btns;
bool quit;
Node* manuQueue;
Message* new_game;
Minepot** map;
Buttons** game_btn;

GAME_RESULT result; // = ING;
//初始化图片命名表 - 这里是一个宏展开
IMAGE_LISTS(INITVAR)
//mines.c
//游戏信息
Message* initMessage(int width, int hight, int mines);
//初始化地图
Minepot** initMineMap(Message* init_message, int makr);
// 更新块的状态
void updateBlank(Minepot* pot);
//初始化雷区
void initMines(Minepot* pot, Message* game);
//点击与展开程序
void expandDFS(Minepot* pot);
//根据操作更新地图状态
int changeMap(Minepot* pot, Message* game, int type);
//绘制地图
void drawMap();
SDL_Texture* initText(char* path);
void showMines(GAME_RESULT result);
//input.c
void initValues();
Buttons** initManuBtns();
Buttons** initInputBtns();
void initRe();
void initButton(Buttons* btn);
int* INPUT_MAIN();
void setMouse(Buttons* btn);
SDL_Texture* initpTexture(Buttons* btn);
SDL_Rect* inputRect(Buttons* btn, double w, double h, double x_ray, double y_ray);
Buttons* findButton(SDL_Point* point, Buttons* btn);
//tools.c
void setUiColor(SDL_Color* color, int r, int g, int b);
SDL_Color* initUiColor();
void initRe();
Buttons** initInputBtns();
Buttons** initManuBtns();
Buttons** initGameBtns();
void initButton(Buttons* btn);
SDL_Texture* initpTexture(Buttons* btn);
SDL_Rect* inputRect(Buttons* btn, double w, double h, double x_ray, double y_ray);
void setMouse(Buttons* btn);
Buttons* findButton(SDL_Point* point, Buttons* start);
void initFreeAdCur();
void freeGame();
//input.c
void inputMouse(Buttons* btn);
void inputKey(int* time, SDL_Event* event, Buttons* btn);
void checkInput(SDL_Event* event, Buttons* btn);
void resetInputWindow();
void initGame(int* data);
void changeNum(GAME_UIBTN UIbtn, int ticks);
void changeState(GAME_UIBTN UIbtn, char* state_str);
void getScore(Message* msg, int time);
Message* Game(int* data);
void initFreeAdCur()
{
    //初始化更新链表
    updateQueen = initQueue();
    freeGameMalloc = initQueue();
    freeGameTexture = initQueue();
    //初始化鼠标手势
    cursor_i = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    cursor_a = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    cursor_h = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    cursor_w = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
}
void freeGame()
{
    drawQueen(freeGameTexture, &SDL_DestroyTexture);
    drawQueen(freeGameMalloc, &free);
    TTF_CloseFont(font);
    SDL_FreeCursor(cursor_i);
    SDL_FreeCursor(cursor_a);
    SDL_FreeCursor(cursor_h);
    SDL_FreeCursor(cursor_w);
}
int* manuWindow();
int* markButton(SDL_Event* event, Buttons* btn, int* arry);
char* catTexts(int* temp);

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    MAIN_WINDOW = SDL_CreateWindow("扫雷v0.1",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        MANU_WIDTH, MANU_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS);
    SDL_SetWindowOpacity(MAIN_WINDOW, 0.9);
    MAIN_RENDER = SDL_CreateRenderer(MAIN_WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    initFreeAdCur();
    initRe();
    int* init = manuWindow();
    Message* result;
    while (1) {
        if (init[0] == 0)
            break;
        else {
            SDL_Delay(100);
            result = Game(init);
        }
        if (result->next == false)
            break;
        else {
            SDL_Delay(100);
            init = manuWindow();
        }
    }
    SDL_DestroyRenderer(MAIN_RENDER);
    SDL_DestroyWindow(MAIN_WINDOW); //销毁window
    SDL_Quit(); // 卸载SDL
    freeGame();
    return 0;
}
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

int* INPUT_MAIN()
{
    set_value = (int*)calloc(3, sizeof(int));
    void_value = (int*)calloc(3, sizeof(int));

    INPUT_WINDOW = SDL_CreateWindow(
        "请输入", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INPUTSCREEN_WIDTH, INPUTSCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS);
    input_render = SDL_CreateRenderer(INPUT_WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetWindowOpacity(INPUT_WINDOW, 0.9);
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
    Buttons* option = input_btns[INPUT_BG_W]; //设置两个鼠标描点是为了让输入的时候焦点保持不变
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
                        sprintf(btn_txt->text, "%.*s%c", len, btn_txt->text, key);
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
    initButton(game_btn[GAME_NUMBER_1]);
    initButton(game_btn[GAME_NUMBER_2]);
    initButton(game_btn[GAME_NUMBER_3]);
    initButton(game_btn[GAME_NUMBER_4]);
    initButton(game_btn[GAME_RETURN]);
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
    ticks = ticks < 0 ? 0 : ticks;
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

inline SDL_Texture* initText(char* path)
{
    SDL_Surface* image = IMG_Load(path);
    SDL_Surface* opt_image = (SDL_Surface*)calloc(1, sizeof(SDL_Surface));
    opt_image = SDL_ConvertSurfaceFormat(image, SDL_GetWindowPixelFormat(MAIN_WINDOW), 0);
    SDL_Texture* text = SDL_CreateTextureFromSurface(MAIN_RENDER, opt_image);
    pushStack(freeGameTexture, text);
    SDL_FreeSurface(opt_image);
    SDL_FreeSurface(image);
    return text;
}
Message* initMessage(int width, int hight, int mines)
{

    freeGameMalloc = initQueue();
    freeGameTexture = initQueue();
    Message* new = (Message*)calloc(1, sizeof(Message*));
    pushStack(freeGameMalloc, new);
    new->state = false;
    new->next = false;
    new->width = width;
    new->hight = hight;
    new->length = width* hight;
    new->mines = mines;
    new->rect = NULL;
    new->score = -1;
    return new;
}
Minepot** initMineMap(Message* init_message, int mark)
{
    eventQueue = initQueue();
    mineQueue = initQueue();
    expandQueue = initQueue();
    result = ING;
    search_number = 0;
    //初始化随机数
    srand((unsigned)(time(NULL)));
    //初始化资源 - 宏展开
    IMAGE_LISTS(INITIMAGE)
    flag_num = 0;
    //初始化地图
    int width = init_message->width;
    int hight = init_message->hight;
    Minepot** map = (Minepot**)calloc((width + 2), sizeof(Minepot*));
    pushStack(freeGameMalloc, map);
    for (int i = 0; i < width + 2; i++) {
        map[i] = (Minepot*)calloc((hight + 2), sizeof(Minepot));
        pushStack(freeGameMalloc, map[i]);
    }
    int size = width * hight;

    for (int row = 0; row < width + 2; row++) {
        for (int col = 0; col < hight + 2; col++) {
            int k = rand() % 2 ? 1 : -1;
            map[row][col].click = 0;
            map[row][col].status = NONE;
            map[row][col].mark = 0;
            map[row][col].rect.w = CORE;
            map[row][col].rect.h = CORE;
            map[row][col].rect.x = CORE * (row - 1);
            map[row][col].rect.y = 50 * mark + CORE * (col - 1);
            map[row][col].size = size;
            if (row * col == 0 || row == width + 1 || col == hight + 1) {
                map[row][col].search = true;
                map[row][col].init = true;
                map[row][col].around = (Minepot**)calloc(8, sizeof(Minepot*));
            } else {
                map[row][col].search = false;
                map[row][col].init = false;
                map[row][col].around = (Minepot**)calloc(8, sizeof(Minepot*));
                map[row][col].around[0] = &map[row - 1][col + k];
                map[row][col].around[1] = &map[row - 1][col];
                map[row][col].around[2] = &map[row - 1][col - k];
                map[row][col].around[3] = &map[row][col + k];
                map[row][col].around[4] = &map[row][col - k];
                map[row][col].around[5] = &map[row + 1][col];
                map[row][col].around[6] = &map[row + 1][col + k];
                map[row][col].around[7] = &map[row + 1][col - k];
                pushQueue(eventQueue, &map[row][col]);
            }
            pushStack(freeGameMalloc, map[row][col].around);
        }
    }
    return map;
}

inline void updateBlank(Minepot* pot)
{

    SDL_Texture* update = NULL;
    switch (pot->status) {
    case NONE:
        update = blank;
        break;
    case FLAG:
        update = flag;
        break;
    case WONDER:
        update = ask;
        break;
    case WINOVER:
        update = mine;
        break;
    case TYPE_I_ERROR:
        update = mine_blood;
        break;
    case TYPE_II_ERROR:
        update = flag_wrong;
        break;
    case MOUSEDOWN:
        switch (pot->mark) {
        case 0:
            update = num_0;
            break;
        case 1:
            update = num_1;
            break;
        case 2:
            update = num_2;
            break;
        case 3:
            update = num_3;
            break;
        case 4:
            update = num_4;
            break;
        case 5:
            update = num_5;
            break;
        case 6:
            update = num_6;
            break;
        case 7:
            update = num_7;
            break;
        case 8:
            update = num_8;
            break;
        default:
            search_number--;
            update = mine_wrong;
            break;
        }
        break;
    }
    SDL_RenderCopy(MAIN_RENDER, update, NULL, &pot->rect);
}

void drawMap() // 使用队列更新赋值
{
    while (eventQueue->next != NULL)
        updateBlank((Minepot*)popQueue(eventQueue));
}

void initMines(Minepot* pot, Message* game)
{
    //使用概率推断的方法代替洗牌算法，效率没有明显差异但是代码更加简洁可控
    //使用深度优先的方式递归初始化
    if (game->mines) {
        game->length--;
        pot->init = true;
        if (abs(game->rect->x - pot->rect.x) > CORE || abs(game->rect->y - pot->rect.y) > CORE) {
            if (rand() % (game->length + 1) < game->mines) {
                pot->mark = 9;
                pushQueue(mineQueue, pot);
                game->mines--;
            }
        }
        for (int i = 0; i < 8; i++) {
            Minepot* round = pot->around[i];
            if (pot->mark >= 9)
                round->mark++;
            if (round && round->init == false) {
                initMines(round, game);
            }
        }
    }
}

//TODO: 深度优先搜索，广度优先搜索效果据说更好
void expandDFS(Minepot* pot)
{
    if (pot->click == 0 && pot->search != true) {
        pot->status = MOUSEDOWN;
        pot->search = true;
        search_number++;
        pushStack(eventQueue, pot);
        if (pot->mark >= 9) {
            result = LOSE;
        } else if (pot->mark == 0) {
            for (int i = 0; i < 8; i++) {
                Minepot* round = pot->around[i];
                if (round->search != true) {
                    expandDFS(round);
                }
            }
        }
    }
}

int changeMap(Minepot* pot, Message* game, int type)
{
    if (result == ING) {
        if (type == LEFTDOWN && pot->click == 0) {
            if (game->state == false) {
                game->rect = &pot->rect;
                int mines = game->mines;
                int length = game->length;
                game->length -= 9;
                initMines(pot, game);
                game->mines = mines;
                game->length = length;
                game->state = true;
            }
            expandDFS(pot);
        } else if (type == RIGHTDOWN && pot->search == false) {

            pot->click = (pot->click + 1) % 3;
            pushStack(eventQueue, pot);
            switch (pot->click) {
            case 0:
                pot->status = NONE;
                break;
            case 1:
                flag_num++;
                flag_num = flag_num > game->mines ? game->mines : flag_num;
                pot->status = FLAG;
                if (pot->mark < 9)
                    pushQueue(mineQueue, pot);
                break;
            case 2:
                flag_num--;
                pot->status = WONDER;
                break;
            }
        }
        if (search_number == game->length - game->mines) {
            result = WIN;
        }
        if (result != ING) {
            showMines(result);
        }
    }
    return result;
}
void showMines(GAME_RESULT result)
{
    Minepot* mine_node = NULL;
    while (mineQueue->next != NULL) {
        mine_node = popQueue(mineQueue);
        pushStack(eventQueue, mine_node);
        if (mine_node->mark >= 9) {
            if (result == WIN || mine_node->status == FLAG) {
                mine_node->status = WINOVER;
            } else if (mine_node->search == false) {
                mine_node->status = TYPE_I_ERROR;
            }
        } else if (mine_node->mark < 9 && mine_node->status == FLAG) {
            mine_node->status = TYPE_II_ERROR;
        }
    }
}
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
    button[GAME_TIME_1]->rect = inputRect(button[GAME_TIME_1], 20, 40, 0.75, 0.5);
    button[GAME_TIME_1]->type = IMG;
    button[GAME_TIME_1]->content = initpTexture(button[GAME_TIME_1]);

    // button[GAME_TIME_2]->color = UIColor[UI_BLACK];
    button[GAME_TIME_2]->mouse = A;
    strcpy(button[GAME_TIME_2]->text, "res/time_0.png");
    button[GAME_TIME_2]->rect = inputRect(button[GAME_TIME_2], 20, 40, 0.75 + 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_TIME_2]->type = IMG;
    button[GAME_TIME_2]->content = initpTexture(button[GAME_TIME_2]);

    // button[GAME_TIME_3]->color = UIColor[UI_BLACK];
    button[GAME_TIME_3]->mouse = A;
    strcpy(button[GAME_TIME_3]->text, "res/time_0.png");
    button[GAME_TIME_3]->rect = inputRect(button[GAME_TIME_3], 20, 40, 0.75 + 40.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_TIME_3]->type = IMG;
    button[GAME_TIME_3]->content = initpTexture(button[GAME_TIME_3]);

    // button[GAME_TIME_4]->color = UIColor[UI_BLACK];
    button[GAME_TIME_4]->mouse = A;
    strcpy(button[GAME_TIME_4]->text, "res/time_0.png");
    button[GAME_TIME_4]->rect = inputRect(button[GAME_TIME_4], 20, 40, 0.75 + 60.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_TIME_4]->type = IMG;
    button[GAME_TIME_4]->content = initpTexture(button[GAME_TIME_4]);

    // button[GAME_TIME_4]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_1]->mouse = A;
    strcpy(button[GAME_NUMBER_1]->text, "res/time_0.png");
    button[GAME_NUMBER_1]->rect = inputRect(button[GAME_NUMBER_1], 20, 40, 0.25 - 60.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_NUMBER_1]->type = IMG;
    button[GAME_NUMBER_1]->content = initpTexture(button[GAME_NUMBER_1]);
    // button[GAME_TIME_3]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_2]->mouse = A;
    strcpy(button[GAME_NUMBER_2]->text, "res/time_0.png");
    button[GAME_NUMBER_2]->rect = inputRect(button[GAME_NUMBER_2], 20, 40, 0.25 - 40.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_NUMBER_2]->type = IMG;
    button[GAME_NUMBER_2]->content = initpTexture(button[GAME_NUMBER_2]);
    // button[GAME_TIME_2]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_3]->mouse = A;
    strcpy(button[GAME_NUMBER_3]->text, "res/time_0.png");
    button[GAME_NUMBER_3]->rect = inputRect(button[GAME_NUMBER_3], 20, 40, 0.25 - 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_NUMBER_3]->type = IMG;
    button[GAME_NUMBER_3]->content = initpTexture(button[GAME_NUMBER_3]);

    // button[GAME_NUMBER_1]->color = UIColor[UI_BLACK];
    button[GAME_NUMBER_4]->mouse = A;
    strcpy(button[GAME_NUMBER_4]->text, "res/time_0.png");
    button[GAME_NUMBER_4]->rect = inputRect(button[GAME_NUMBER_4], 20, 40, 0.25, 0.5);
    button[GAME_NUMBER_4]->type = IMG;
    button[GAME_NUMBER_4]->content = initpTexture(button[GAME_NUMBER_4]);
    // button[GAME_STATE_NORMAL]
    button[GAME_STATE]->mouse = H;
    strcpy(button[GAME_STATE]->text, "res/st_neutral.png");
    button[GAME_STATE]->rect = inputRect(button[GAME_STATE], 40, 40, 0.5+20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_STATE]->type = IMG;
    button[GAME_STATE]->content = initpTexture(button[GAME_STATE]);
    // button[GAME_RETURN_NORMAL]
    button[GAME_RETURN]->mouse = H;
    strcpy(button[GAME_RETURN]->text, "res/ret.png");
    button[GAME_RETURN]->rect = inputRect(button[GAME_RETURN], 40, 40, 0.5 - 20.0 / GAME_MANU_WIDTH, 0.5);
    button[GAME_RETURN]->type = IMG;
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
