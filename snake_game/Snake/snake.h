
#include <Windows.h>
#include "LinkList.h"

// 蛇的状态
#define SNAKE_MOVED			0
#define SNAKE_DEAD			1
#define SNAKE_COMPLETE		2
#define SNAKE_EATEN_FOOD	3
#define SNAKE_GROWUP		4
#define SNAKE_ERROR			5
#define SNAKE_KNOCKBORDER    6
#define BLOCK_DEAD			7
#define HIT_BLOCK_DEAD      8

// 蛇的方向
typedef enum _dirction
{
	SNAKE_UP,
	SNAKE_DOWN,
	SNAKE_LEFT,
	SNAKE_RIGHT,
}dirction;

//墙的砖块增加的方向
typedef enum _walldirction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
}walldirction;

// 游戏坐标
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

// 函数声明

/// snake_and_food.c中的接口函数

// 设置边界坐标
void SetBoundary(int x, int y);

// 获得边界坐标
PGAME_COORD GetBoundary();

// 生成新的食物。
int CreateFood();

// 获得食物的坐标。
PGAME_COORD GetFood();

// 创建蛇
void CreateSnake(dirction dir, int head_x, int head_y, int init_len);

//创建墙
void CreateWall();

//墙增加
int WallGorwup(walldirction dir);

// 销毁蛇，释放内存资源。
void DistroySnake();

// 用来移动一步，
// 移动以后，如果吃到了食物，则生长。
// 如果碰到了墙或者自己，则死亡，并返回是否死亡的状态。
int SnakeMove();

// 蛇生长
int SnakeGorwup();

// 获得蛇的长度
int GetSnakeSize();

//穿过边界
void ThroughBorder();

// 获得蛇的第一个节点的坐标
PGAME_COORD GetSnakeHead();

// 获得蛇的最后一个节点的坐标
PGAME_COORD GetSnakeTail();

// 按照序号获得蛇的节点的坐标，不能超过蛇的长度，否则返回NULL
PGAME_COORD GetSnakeAt(int n);

//按照序号获得墙的节点的坐标
PGAME_COORD GetWallAt(int n);
// 改变蛇移动的方向
void SetDirction(dirction dir);

// 获得当前蛇的方向
dirction GetDirction();




/// game_opration.c 中的接口函数

int GetScore();
int GetLevel();
int GetResetTimes();
void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse,
	unsigned int one_level_scores,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y,
	int init_x, int init_y,
	int init_len,
	dirction init_dir);
void OnTimer(HWND hwnd);
void OnKeyDown(DWORD vk);

//障碍物状态
typedef enum _BLOCK_STATE
{
	BLOCK_OUT,
	HIT_DEAD,
	BLOCK_MOVED,
}BLOCK_STATE;

//定义障碍结构体
typedef struct _AUTO_BLOCK
{
	int x;
	int y;
	double dir;				// 移动时，x相对于y
	int y_step;				// y方向每个时间周期移动的距离
	BLOCK_STATE state;
	unsigned int power;
	int ratio_x;		// 在这个范围内击中都有效。
	int ratio_y;		// 在这个范围内击中都有效。
}AUTO_BLOCK, *LPAUTO_BLOCK;

//LPAUTO_BLOCK create_block(
void create_block(
	int x,
	int y,
	double dir,		// 移动时，x相对于y
	int y_step,	// y方向每个时间周期移动的距离
	BLOCK_STATE state,
	unsigned int power,
	int ratio_x,	// 在这个范围内击中都有效。
	int ratio_y
);

#define MAX_X		50// 游戏界面大小
#define MAX_Y		25//戏界面大小

//#define gen_block()			create_block(rand()%MAX_X, rand()%MAX_Y, 0,0.2,BLOCK_MOVED,1)
void gen_block();
int blocks_init();
void destory_block_at(unsigned int i);

unsigned int get_blocks_num();
LPAUTO_BLOCK get_block_at(unsigned int i);
void block_move(LPAUTO_BLOCK auto_block);
unsigned int get_block_x(LPAUTO_BLOCK auto_block);
unsigned int get_block_y(LPAUTO_BLOCK auto_block);
void block_out(LPAUTO_BLOCK auto_block);
void blocks_move_step();
int destory_block_by_state();