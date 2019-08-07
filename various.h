#ifndef _VAR
#define _VAR

#include "main.h"
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
bool MakeWindowTransparent(SDL_Window* window, COLORREF colorKey);

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
#endif