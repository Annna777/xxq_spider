/********************************************************************************
* ##########  谈谈封装 ##########
* “封装”是程序不会乱掉的一个重要技能。
* 我们要写出结构化、易于阅读、理解和维护的代码。采用封装是比较容易掌握的方法。
* 
* 具体而言：
* 
* 1、先考虑好这个程序的逻辑和状态需要多少数据来表示，
* 比如这个贪吃蛇游戏，我们使用这些数据就可以表示全部游戏状态。
* // 食物
* GAME_COORD food;
* // 蛇（链表）
* PLIST snake_list;
* // 蛇移动方向
* dirction snake_dir;
* // 游戏边界（这是右下，左上为0,0）
* GAME_COORD boundary;
* 
* 2、写出一系列接口函数来操作这些数据。所谓接口函数，满足一下几个特征：
* (1) 这些数据除了接口函数以外，其他代码都不会直接访问。
* (2) 所有其他代码都只能通过调用这些接口函数来改变程序的状态，而不要直接去访问数据。
* 如本文件的若干函数都是对上述数据的操作。

* 这样实现了：对于上层代码，并不需要知道数据的操作逻辑，也不需要知道数据结构，而只需要调用函数即可。
* 对上次代码而言，逻辑简单了很多。
* 同样，如果数据结构和内部逻辑有了修改，那么可以做到接口函数的原型不变，
* 上层代码不变，而只需要修改接口函数的内部实现过程即可。
* 
* 这就称之为“封装”，或者称之为 “数据的封装和接口化”
*
*******************************************************************************/



/************** 头文件 ****************/
#include "snake.h"
#include <time.h>
/************* 全局变量 ****************/
// 食物
GAME_COORD food;
// 蛇（链表）
PLIST snake_list;
//墙（链表)
PLIST wall_list;
// 蛇移动方向
dirction snake_dir;
// 游戏边界（这是右下，左上为0,0）
GAME_COORD boundary;
//初始化障碍物列表
PLIST list_blocks;

/*********** 内部函数申明 **************/
/// 判断两个坐标是否相等。
int CoordEqual(PGAME_COORD one, PGAME_COORD two);
/// 判断蛇是否已死
int IsSnakeDead();
///判断蛇是否撞到边界
int IsSnakeKnockBorder();
///穿过边界
void ThroughBorder();
/**************函数定义****************/


// 判断两个坐标 GAME_COORD 是否重合
int CoordEqual(PGAME_COORD one, PGAME_COORD two)
{
	if (one->x == two->x && one->y == two->y)
		return 1;
	return 0;
}

// 设置边界坐标
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}

// 获得边界坐标
PGAME_COORD GetBoundary()
{
	return &boundary;
}

// 生成新的食物。
int CreateFood()
{
	PGAME_COORD posbody;
	int i;
	int size = ListSize(snake_list);
	PGAME_COORD pWallBody;
new_food:

	// 随机生成食物的坐标。
	food.x = rand() % boundary.x;
	food.y = rand() % boundary.y;

	// 判断是否和蛇重叠了，否则重新生成食物坐标，知道不重叠。

	for (i = 0; i < size; i++)
	{
		posbody = (PGAME_COORD)ListGetAt(snake_list, i);
		if (CoordEqual(posbody, &(food)))
		{
			goto new_food;
		}
	}
	for (i = 0; i < 56; i++)
	{
		pWallBody = (PGAME_COORD)GetWallAt(i);
		if (CoordEqual(pWallBody, &(food)))
		{
			goto new_food;
		}
	}
	return 1;
}

// 获得食物的坐标。
PGAME_COORD GetFood()
{
	return &food;
}

// 创建蛇
void CreateSnake(dirction dir, int head_x, int head_y, int init_len)
{
	PGAME_COORD p;
	int i;

	SetDirction(dir);

	snake_list = ListCreate(NULL);

	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	// 蛇头部的初始位置；
	p->x = head_x;
	p->y = head_y;
	ListPushFront(snake_list, p);

	for (i = 1; i < init_len; i++)
	{
		//p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
		SnakeGorwup();
	}
	return;
}
//创建墙
void CreateWall()
{
	PGAME_COORD p;
	int i;
	wall_list = ListCreate(NULL);

	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	// 墙的初始位置；
	p->x = 8;
	p->y = 10;
	ListPushFront(wall_list, p);
	for (i = 1; i < 5; i++)
		WallGorwup(UP);
	for (i = 1; i < 10; i++)
		WallGorwup(RIGHT);
	PGAME_COORD pNewTail;
	PGAME_COORD pTail;
	pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	pTail = (PGAME_COORD)ListGetAt(wall_list, ListSize(wall_list) - 1);
	pNewTail->x = pTail->x+15;
	pNewTail->y = pTail->y;
	ListPushBack(wall_list, pNewTail);
	for (i = 0; i < 9; i++)
		WallGorwup(RIGHT);
	for (i = 1; i < 5; i++)
		WallGorwup(DOWN);
	pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	pTail = (PGAME_COORD)ListGetAt(wall_list, ListSize(wall_list) - 1);
	pNewTail->x = pTail->x;
	pNewTail->y =pTail->y + 5;
	ListPushBack(wall_list, pNewTail);
	for (i = 1; i < 5; i++)
		WallGorwup(DOWN);
	for (i = 0; i < 9; i++)
		WallGorwup(LEFT);
	pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	pTail = (PGAME_COORD)ListGetAt(wall_list, ListSize(wall_list) - 1);
	pNewTail->x = pTail->x-15;
	pNewTail->y = pTail->y;
	ListPushBack(wall_list, pNewTail);
	for (i = 1; i < 10; i++)
		WallGorwup(LEFT);
	for (i = 1; i < 5; i++)
		WallGorwup(UP);
	return;
}
//增加墙的长度
int WallGorwup(walldirction dir)
{
	// 给新的节点分配内存
	PGAME_COORD pNewTail;
	PGAME_COORD pTail;		
	pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
		pTail = (PGAME_COORD)ListGetAt(wall_list, ListSize(wall_list) - 1);
		switch (dir)
		{
		case RIGHT:
			pNewTail->x = pTail->x + 1;
			pNewTail->y = pTail->y;
			break;
		case LEFT:
			pNewTail->x = pTail->x - 1;
			pNewTail->y = pTail->y;
			break;
		case DOWN:
			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y + 1;
			break;
		case UP:
			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y - 1;
			break;
		}

	// 新节点放入到链表尾的位置
	ListPushBack(wall_list, pNewTail);
	return 1;
}

//穿过边界
void ThroughBorder() {
	PGAME_COORD posHead;
	int size = ListSize(snake_list);
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);

	if (posHead->x < 0)
		posHead->x = boundary.x;
	else if (posHead->x > boundary.x)
		posHead->x = 0;
	else if (posHead->y < 0)
		posHead->y = boundary.y;
	else if (posHead->y > boundary.y)
		posHead->y = 0;
}

//判断蛇是否碰到边界,未碰到则判断是否碰到自己
int IsSnakeKnockBorder()
{
	PGAME_COORD posHead;
	int size = ListSize(snake_list);

	/// 判断是否碰到边界
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);

	if (posHead->x < 0 || posHead->x > boundary.x ||
		posHead->y < 0 || posHead->y > boundary.y)
	{
		return SNAKE_KNOCKBORDER;
	}
	else
		return IsSnakeDead();
}
// 判断蛇是否死了。
// 返回0 蛇没有死 else 蛇死了。
int IsSnakeDead()
{
	PGAME_COORD posBody;
	PGAME_COORD posHead;
	PGAME_COORD pWallBody;
	int i;
	int size = ListSize(snake_list);
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);
	/// 判断是否碰到自己死亡
	//// 从第二个节点开始，逐一和头节点比较。
	size = ListSize(snake_list);

	for (i = 1; i < size; i++)
	{
		posBody = (PGAME_COORD)ListGetAt(snake_list, i);
		if (CoordEqual(posHead, posBody))
		{
			return SNAKE_DEAD;
		}
	}
	///判断是否碰到墙死亡
	for (i = 0; i < 56; i++)
	{
		pWallBody = (PGAME_COORD)GetWallAt(i);
		if (CoordEqual(pWallBody, posHead))
		{
			return SNAKE_DEAD;
		}
	}

	return SNAKE_MOVED;
}

// 销毁蛇，释放内存资源。
void DistroySnake()
{
	ListDistoryAndFree(snake_list);
	return;
}

// 用来移动一步，
// 移动以后，如果吃到了食物，则生长。
// 如果碰到了墙或者自己，则死亡，并返回是否死亡的状态。
int SnakeMove()
{
	// 头和尾的坐标
	PGAME_COORD posHead;
	PGAME_COORD posTail;

	// 把尾从链表中取出，按照当前方向放置到头的位置。
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);
	posTail = (PGAME_COORD)ListPopBack(snake_list);

	// 根据当前方向来设定新的头坐标。
	switch (snake_dir)
	{
	case SNAKE_UP:
		posTail->y = posHead->y - 1;
		posTail->x = posHead->x;
		break;
	case SNAKE_DOWN:
		posTail->y = posHead->y + 1;
		posTail->x = posHead->x;
		break;
	case SNAKE_LEFT:
		posTail->x = posHead->x - 1;
		posTail->y = posHead->y;
		break;
	case SNAKE_RIGHT:
		posTail->x = posHead->x + 1;
		posTail->y = posHead->y;
		break;
	}
	ListPushFront(snake_list, posTail);

	// 判断是否吃到了食物
	if (CoordEqual(posHead, &food))
	{
		return SNAKE_EATEN_FOOD;
	}
	if (destory_block_by_state() == HIT_BLOCK_DEAD)
	{
		return SNAKE_DEAD;
	}
	//障碍物也移动一步
	blocks_move_step();
	// 如果没有吃到食物判断是否撞到边界或者自己，然后返回状态。
	return IsSnakeKnockBorder();
}

// 蛇生长
int SnakeGorwup()
{
	// 给新的节点分配内存
	PGAME_COORD pNewTail;
	PGAME_COORD pTail;		// 倒数第一节点
	PGAME_COORD pLastButOne;	// 倒数第二节点
	int size = GetSnakeSize();

	if (size >= boundary.x*boundary.y-1) //长到最长了，游戏结束！~
		return SNAKE_COMPLETE;

	if (size == 0) // 没有头，不知从何生长，返回错误。
		return SNAKE_ERROR;

	pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	if (size == 1) // 只有一个节点，按照当前的移动方向生长。
	{
		
		pTail = (PGAME_COORD)GetSnakeTail();
		switch (snake_dir)
		{
		case SNAKE_LEFT:
			pNewTail->x = pTail->x + 1;
			pNewTail->y = pTail->y;
			break;
		case SNAKE_RIGHT:
			pNewTail->x = pTail->x - 1;
			pNewTail->y = pTail->y;
			break;
		case SNAKE_UP:
			pNewTail->x = pTail->x ;
			pNewTail->y = pTail->y + 1;
			break;
		case SNAKE_DOWN:
			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y - 1;
			break;
		}

	}
	else // 有两个以上节点，取倒数第一和倒数第二计算生长方向。
	{
		pTail = (PGAME_COORD)GetSnakeTail();
		pLastButOne = (PGAME_COORD)GetSnakeAt(size - 2);
		// 沿着倒数第二->倒数第一的方向，添加一个新的节点。
		pNewTail->x = pTail->x + (pTail->x - pLastButOne->x);
		pNewTail->y = pTail->y + (pTail->y - pLastButOne->y);

	}

	// 新节点放入到蛇尾的位置
	ListPushBack(snake_list, pNewTail);
	return SNAKE_GROWUP;

}

// 获得蛇的长度
int GetSnakeSize()
{
	return ListSize(snake_list);
}

// 获得蛇的第一个节点的坐标
PGAME_COORD GetSnakeHead()
{
	return (PGAME_COORD)ListGetAt(snake_list, 0);
}

// 获得蛇的最后一个节点的坐标
PGAME_COORD GetSnakeTail()
{
	return (PGAME_COORD)ListGetAt(snake_list, GetSnakeSize() - 1);
}

// 按照序号获得蛇的节点的坐标，不能超过蛇的长度，否则返回NULL
PGAME_COORD GetSnakeAt(int n)
{
	if (n < ListSize(snake_list))
		return (PGAME_COORD)ListGetAt(snake_list, n);
	else
		return NULL;

}

//获得墙的节点坐标
PGAME_COORD GetWallAt(int n)
{
		return (PGAME_COORD)ListGetAt(wall_list, n);
}
// 改变蛇移动的方向
void SetDirction(dirction dir)
{

	switch (snake_dir)
	{
	case SNAKE_UP:
		if (dir == SNAKE_DOWN)
			return;
		break;
	case SNAKE_DOWN:
		if (dir == SNAKE_UP)
			return;
		break;
	case SNAKE_LEFT:
		if (dir == SNAKE_RIGHT)
			return;
		break;
	case SNAKE_RIGHT:
		if (dir == SNAKE_LEFT)
			return;
		break;
	}
	snake_dir = dir;
}


// 获得当前蛇的方向
dirction GetDirction()
{
	return snake_dir;
}

//创建障碍物链表
int blocks_init()
{
	time_t t;
	list_blocks = ListCreate(0);
	srand((unsigned int)(time(&t)*time(&t)));
	return 0;
}
//创建障碍物
LPAUTO_BLOCK create_block(
	int x,
	int y,
	double dir,		// 移动时，x相对于y
	int y_step,	// y方向每个时间周期移动的距离
	BLOCK_STATE state,
	unsigned int power
)

{
	LPAUTO_BLOCK AUTO_BLOCK;
	AUTO_BLOCK = (LPAUTO_BLOCK)malloc(sizeof(AUTO_BLOCK));
	if (AUTO_BLOCK == 0)
		return (LPAUTO_BLOCK)0;
	AUTO_BLOCK->x = x;
	AUTO_BLOCK->y = y;
	AUTO_BLOCK->dir = dir;
	AUTO_BLOCK->power = power;
	AUTO_BLOCK->y_step = y_step;
	AUTO_BLOCK->state = BLOCK_MOVED;

	ListPushBack(list_blocks, AUTO_BLOCK);

	return AUTO_BLOCK;
}

//删掉第i个障碍物
void destory_block_at(unsigned int i)
{
	//LPAUTO_BLOCK block = 
	ListDeleteAt(list_blocks, i);
	//free(block);
}

//返回障碍物数
unsigned int get_blocks_num()
{
	return ListSize(list_blocks);
}

//返回敌方第i个障碍物对应数据
LPAUTO_BLOCK get_block_at(unsigned int i)
{
	return (LPAUTO_BLOCK)ListGetAt(list_blocks, i);
}

//障碍物的移动
void block_move(LPAUTO_BLOCK AUTO_BLOCK)
{
	AUTO_BLOCK->y += AUTO_BLOCK->y_step;
	AUTO_BLOCK->x += (int)(AUTO_BLOCK->y_step*AUTO_BLOCK->dir);
}

//返回障碍物的横坐标
unsigned int get_block_x(LPAUTO_BLOCK AUTO_BLOCK)
{
	return AUTO_BLOCK->x;
}

//返回障碍物的纵坐标
unsigned int get_block_y(LPAUTO_BLOCK AUTO_BLOCK)
{
	return AUTO_BLOCK->y;
}

//若障碍物飞出去
void block_out(LPAUTO_BLOCK AUTO_BLOCK)
{
	AUTO_BLOCK->state = BLOCK_OUT;
}

void block_hit(LPAUTO_BLOCK AUTO_BLOCK)
{
	AUTO_BLOCK->state = HIT_DEAD;
}
// 判断是否被撞上，
// 如果撞上则返回小球的状态：撞到后死亡，否则返回空指针。
int be_hit(PGAME_COORD snake, LPAUTO_BLOCK block)
{

	if (snake->x == get_block_x(block) && snake ->y == get_block_y(block))
	{
		//block->state = HIT_DEAD;
		block_hit(block);
		return 1;
	}
	else
	{
		// 只能从 BLOCK_MOVED 变为 HIT_DEAD, 不能反向变化。
		return 0;
	}
}


//使障碍移动并判断状态
void blocks_move_step()
{
	unsigned int num, i;
	PGAME_COORD snake;
	LPAUTO_BLOCK AUTO_BLOCK;
	num = get_blocks_num();
	// 移动，并判断是否飞出。
	for (i = 0; i < num; i++)
	{
		AUTO_BLOCK = get_block_at(i);
		block_move(AUTO_BLOCK);
		if (get_block_x(AUTO_BLOCK) < 0 || get_block_x(AUTO_BLOCK) > MAX_X ||
			get_block_y(AUTO_BLOCK) < 0 || get_block_y(AUTO_BLOCK) > MAX_Y)
		{
			block_out(AUTO_BLOCK);
		}
	}
	snake = GetSnakeHead();
	// 逐个判断是否撞到，标记状态。
	for (i = 0; i < num; i++)
	{
		AUTO_BLOCK = get_block_at(i);
		be_hit(snake, AUTO_BLOCK);
	}
}


//根据其状态毁掉障碍或者结束游戏
int destory_block_by_state()
{
	int flag = 0;
	unsigned int num, i;
	LPAUTO_BLOCK AUTO_BLOCK;

again:
	num = get_blocks_num();

	// 从后往前删，不然一次循环删除多个的话，就会超
	// 或者一次循环只删除一个，多次循环。
	// 这是链表可以改接的地方
	for (i = 0; i < num; i++)
	{
		AUTO_BLOCK = get_block_at(i);
		if (AUTO_BLOCK->state == HIT_DEAD)
		{
			flag = 1;
			//return HIT_BLOCK_DEAD ;
		}
		else if (AUTO_BLOCK->state == BLOCK_OUT)
		{

			destory_block_at(i);
			goto again;
		}
	}
	if (flag)
		return HIT_BLOCK_DEAD;
	else
		return 1;
}

void gen_block()
{
	create_block(rand() % MAX_X, rand() % MAX_Y, 0, 1, BLOCK_MOVED, 1);
}
