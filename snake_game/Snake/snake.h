
#include <Windows.h>
#include "LinkList.h"

// �ߵ�״̬
#define SNAKE_MOVED			0
#define SNAKE_DEAD			1
#define SNAKE_COMPLETE		2
#define SNAKE_EATEN_FOOD	3
#define SNAKE_GROWUP		4
#define SNAKE_ERROR			5
#define SNAKE_KNOCKBORDER    6
#define BLOCK_DEAD			7
#define HIT_BLOCK_DEAD      8

// �ߵķ���
typedef enum _dirction
{
	SNAKE_UP,
	SNAKE_DOWN,
	SNAKE_LEFT,
	SNAKE_RIGHT,
}dirction;

//ǽ��ש�����ӵķ���
typedef enum _walldirction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
}walldirction;

// ��Ϸ����
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

// ��������

/// snake_and_food.c�еĽӿں���

// ���ñ߽�����
void SetBoundary(int x, int y);

// ��ñ߽�����
PGAME_COORD GetBoundary();

// �����µ�ʳ�
int CreateFood();

// ���ʳ������ꡣ
PGAME_COORD GetFood();

// ������
void CreateSnake(dirction dir, int head_x, int head_y, int init_len);

//����ǽ
void CreateWall();

//ǽ����
int WallGorwup(walldirction dir);

// �����ߣ��ͷ��ڴ���Դ��
void DistroySnake();

// �����ƶ�һ����
// �ƶ��Ժ�����Ե���ʳ���������
// ���������ǽ�����Լ������������������Ƿ�������״̬��
int SnakeMove();

// ������
int SnakeGorwup();

// ����ߵĳ���
int GetSnakeSize();

//�����߽�
void ThroughBorder();

// ����ߵĵ�һ���ڵ������
PGAME_COORD GetSnakeHead();

// ����ߵ����һ���ڵ������
PGAME_COORD GetSnakeTail();

// ������Ż���ߵĽڵ�����꣬���ܳ����ߵĳ��ȣ����򷵻�NULL
PGAME_COORD GetSnakeAt(int n);

//������Ż��ǽ�Ľڵ������
PGAME_COORD GetWallAt(int n);
// �ı����ƶ��ķ���
void SetDirction(dirction dir);

// ��õ�ǰ�ߵķ���
dirction GetDirction();




/// game_opration.c �еĽӿں���

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

//�ϰ���״̬
typedef enum _BLOCK_STATE
{
	BLOCK_OUT,
	HIT_DEAD,
	BLOCK_MOVED,
}BLOCK_STATE;

//�����ϰ��ṹ��
typedef struct _AUTO_BLOCK
{
	int x;
	int y;
	double dir;				// �ƶ�ʱ��x�����y
	int y_step;				// y����ÿ��ʱ�������ƶ��ľ���
	BLOCK_STATE state;
	unsigned int power;
	int ratio_x;		// �������Χ�ڻ��ж���Ч��
	int ratio_y;		// �������Χ�ڻ��ж���Ч��
}AUTO_BLOCK, *LPAUTO_BLOCK;

//LPAUTO_BLOCK create_block(
void create_block(
	int x,
	int y,
	double dir,		// �ƶ�ʱ��x�����y
	int y_step,	// y����ÿ��ʱ�������ƶ��ľ���
	BLOCK_STATE state,
	unsigned int power,
	int ratio_x,	// �������Χ�ڻ��ж���Ч��
	int ratio_y
);

#define MAX_X		50// ��Ϸ�����С
#define MAX_Y		25//Ϸ�����С

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