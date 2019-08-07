#include "structure.h"
#include "various.h"
//状态数据

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
    if (pot->click == 0 && pot->search != true){
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
        printf("search_number(%d) vs game->length(%d) - game->mines(%d) = %d\n", search_number, game->length, game->mines, game->length - game->mines);
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
