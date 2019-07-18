/********************************************************************************
* ##########  ̸̸��װ ##########
* ����װ���ǳ��򲻻��ҵ���һ����Ҫ���ܡ�
* ����Ҫд���ṹ���������Ķ�������ά���Ĵ��롣���÷�װ�ǱȽ��������յķ�����
* 
* ������ԣ�
* 
* 1���ȿ��Ǻ����������߼���״̬��Ҫ������������ʾ��
* �������̰������Ϸ������ʹ����Щ���ݾͿ��Ա�ʾȫ����Ϸ״̬��
* // ʳ��
* GAME_COORD food;
* // �ߣ�����
* PLIST snake_list;
* // ���ƶ�����
* dirction snake_dir;
* // ��Ϸ�߽磨�������£�����Ϊ0,0��
* GAME_COORD boundary;
* 
* 2��д��һϵ�нӿں�����������Щ���ݡ���ν�ӿں���������һ�¼���������
* (1) ��Щ���ݳ��˽ӿں������⣬�������붼����ֱ�ӷ��ʡ�
* (2) �����������붼ֻ��ͨ��������Щ�ӿں������ı�����״̬������Ҫֱ��ȥ�������ݡ�
* �籾�ļ������ɺ������Ƕ��������ݵĲ�����

* ����ʵ���ˣ������ϲ���룬������Ҫ֪�����ݵĲ����߼���Ҳ����Ҫ֪�����ݽṹ����ֻ��Ҫ���ú������ɡ�
* ���ϴδ�����ԣ��߼����˺ܶࡣ
* ͬ����������ݽṹ���ڲ��߼������޸ģ���ô���������ӿں�����ԭ�Ͳ��䣬
* �ϲ���벻�䣬��ֻ��Ҫ�޸Ľӿں������ڲ�ʵ�ֹ��̼��ɡ�
* 
* ��ͳ�֮Ϊ����װ�������߳�֮Ϊ �����ݵķ�װ�ͽӿڻ���
*
*******************************************************************************/



/************** ͷ�ļ� ****************/
#include "snake.h"
#include <time.h>
/************* ȫ�ֱ��� ****************/
// ʳ��
GAME_COORD food;
// �ߣ�����
PLIST snake_list;
//ǽ������)
PLIST wall_list;
// ���ƶ�����
dirction snake_dir;
// ��Ϸ�߽磨�������£�����Ϊ0,0��
GAME_COORD boundary;
//��ʼ���ϰ����б�
PLIST list_blocks;

/*********** �ڲ��������� **************/
/// �ж����������Ƿ���ȡ�
int CoordEqual(PGAME_COORD one, PGAME_COORD two);
/// �ж����Ƿ�����
int IsSnakeDead();
///�ж����Ƿ�ײ���߽�
int IsSnakeKnockBorder();
///�����߽�
void ThroughBorder();
/**************��������****************/


// �ж��������� GAME_COORD �Ƿ��غ�
int CoordEqual(PGAME_COORD one, PGAME_COORD two)
{
	if (one->x == two->x && one->y == two->y)
		return 1;
	return 0;
}

// ���ñ߽�����
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}

// ��ñ߽�����
PGAME_COORD GetBoundary()
{
	return &boundary;
}

// �����µ�ʳ�
int CreateFood()
{
	PGAME_COORD posbody;
	int i;
	int size = ListSize(snake_list);
	PGAME_COORD pWallBody;
new_food:

	// �������ʳ������ꡣ
	food.x = rand() % boundary.x;
	food.y = rand() % boundary.y;

	// �ж��Ƿ�����ص��ˣ�������������ʳ�����֪꣬�����ص���

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

// ���ʳ������ꡣ
PGAME_COORD GetFood()
{
	return &food;
}

// ������
void CreateSnake(dirction dir, int head_x, int head_y, int init_len)
{
	PGAME_COORD p;
	int i;

	SetDirction(dir);

	snake_list = ListCreate(NULL);

	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	// ��ͷ���ĳ�ʼλ�ã�
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
//����ǽ
void CreateWall()
{
	PGAME_COORD p;
	int i;
	wall_list = ListCreate(NULL);

	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	// ǽ�ĳ�ʼλ�ã�
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
//����ǽ�ĳ���
int WallGorwup(walldirction dir)
{
	// ���µĽڵ�����ڴ�
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

	// �½ڵ���뵽����β��λ��
	ListPushBack(wall_list, pNewTail);
	return 1;
}

//�����߽�
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

//�ж����Ƿ������߽�,δ�������ж��Ƿ������Լ�
int IsSnakeKnockBorder()
{
	PGAME_COORD posHead;
	int size = ListSize(snake_list);

	/// �ж��Ƿ������߽�
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);

	if (posHead->x < 0 || posHead->x > boundary.x ||
		posHead->y < 0 || posHead->y > boundary.y)
	{
		return SNAKE_KNOCKBORDER;
	}
	else
		return IsSnakeDead();
}
// �ж����Ƿ����ˡ�
// ����0 ��û���� else �����ˡ�
int IsSnakeDead()
{
	PGAME_COORD posBody;
	PGAME_COORD posHead;
	PGAME_COORD pWallBody;
	int i;
	int size = ListSize(snake_list);
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);
	/// �ж��Ƿ������Լ�����
	//// �ӵڶ����ڵ㿪ʼ����һ��ͷ�ڵ�Ƚϡ�
	size = ListSize(snake_list);

	for (i = 1; i < size; i++)
	{
		posBody = (PGAME_COORD)ListGetAt(snake_list, i);
		if (CoordEqual(posHead, posBody))
		{
			return SNAKE_DEAD;
		}
	}
	///�ж��Ƿ�����ǽ����
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

// �����ߣ��ͷ��ڴ���Դ��
void DistroySnake()
{
	ListDistoryAndFree(snake_list);
	return;
}

// �����ƶ�һ����
// �ƶ��Ժ�����Ե���ʳ���������
// ���������ǽ�����Լ������������������Ƿ�������״̬��
int SnakeMove()
{
	// ͷ��β������
	PGAME_COORD posHead;
	PGAME_COORD posTail;

	// ��β��������ȡ�������յ�ǰ������õ�ͷ��λ�á�
	posHead = (PGAME_COORD)ListGetAt(snake_list, 0);
	posTail = (PGAME_COORD)ListPopBack(snake_list);

	// ���ݵ�ǰ�������趨�µ�ͷ���ꡣ
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

	// �ж��Ƿ�Ե���ʳ��
	if (CoordEqual(posHead, &food))
	{
		return SNAKE_EATEN_FOOD;
	}
	if (destory_block_by_state() == HIT_BLOCK_DEAD)
	{
		return SNAKE_DEAD;
	}
	//�ϰ���Ҳ�ƶ�һ��
	blocks_move_step();
	// ���û�гԵ�ʳ���ж��Ƿ�ײ���߽�����Լ���Ȼ�󷵻�״̬��
	return IsSnakeKnockBorder();
}

// ������
int SnakeGorwup()
{
	// ���µĽڵ�����ڴ�
	PGAME_COORD pNewTail;
	PGAME_COORD pTail;		// ������һ�ڵ�
	PGAME_COORD pLastButOne;	// �����ڶ��ڵ�
	int size = GetSnakeSize();

	if (size >= boundary.x*boundary.y-1) //������ˣ���Ϸ������~
		return SNAKE_COMPLETE;

	if (size == 0) // û��ͷ����֪�Ӻ����������ش���
		return SNAKE_ERROR;

	pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	if (size == 1) // ֻ��һ���ڵ㣬���յ�ǰ���ƶ�����������
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
	else // ���������Ͻڵ㣬ȡ������һ�͵����ڶ�������������
	{
		pTail = (PGAME_COORD)GetSnakeTail();
		pLastButOne = (PGAME_COORD)GetSnakeAt(size - 2);
		// ���ŵ����ڶ�->������һ�ķ������һ���µĽڵ㡣
		pNewTail->x = pTail->x + (pTail->x - pLastButOne->x);
		pNewTail->y = pTail->y + (pTail->y - pLastButOne->y);

	}

	// �½ڵ���뵽��β��λ��
	ListPushBack(snake_list, pNewTail);
	return SNAKE_GROWUP;

}

// ����ߵĳ���
int GetSnakeSize()
{
	return ListSize(snake_list);
}

// ����ߵĵ�һ���ڵ������
PGAME_COORD GetSnakeHead()
{
	return (PGAME_COORD)ListGetAt(snake_list, 0);
}

// ����ߵ����һ���ڵ������
PGAME_COORD GetSnakeTail()
{
	return (PGAME_COORD)ListGetAt(snake_list, GetSnakeSize() - 1);
}

// ������Ż���ߵĽڵ�����꣬���ܳ����ߵĳ��ȣ����򷵻�NULL
PGAME_COORD GetSnakeAt(int n)
{
	if (n < ListSize(snake_list))
		return (PGAME_COORD)ListGetAt(snake_list, n);
	else
		return NULL;

}

//���ǽ�Ľڵ�����
PGAME_COORD GetWallAt(int n)
{
		return (PGAME_COORD)ListGetAt(wall_list, n);
}
// �ı����ƶ��ķ���
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


// ��õ�ǰ�ߵķ���
dirction GetDirction()
{
	return snake_dir;
}

//�����ϰ�������
int blocks_init()
{
	time_t t;
	list_blocks = ListCreate(0);
	srand((unsigned int)(time(&t)*time(&t)));
	return 0;
}
//�����ϰ���
LPAUTO_BLOCK create_block(
	int x,
	int y,
	double dir,		// �ƶ�ʱ��x�����y
	int y_step,	// y����ÿ��ʱ�������ƶ��ľ���
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

//ɾ����i���ϰ���
void destory_block_at(unsigned int i)
{
	//LPAUTO_BLOCK block = 
	ListDeleteAt(list_blocks, i);
	//free(block);
}

//�����ϰ�����
unsigned int get_blocks_num()
{
	return ListSize(list_blocks);
}

//���صз���i���ϰ����Ӧ����
LPAUTO_BLOCK get_block_at(unsigned int i)
{
	return (LPAUTO_BLOCK)ListGetAt(list_blocks, i);
}

//�ϰ�����ƶ�
void block_move(LPAUTO_BLOCK AUTO_BLOCK)
{
	AUTO_BLOCK->y += AUTO_BLOCK->y_step;
	AUTO_BLOCK->x += (int)(AUTO_BLOCK->y_step*AUTO_BLOCK->dir);
}

//�����ϰ���ĺ�����
unsigned int get_block_x(LPAUTO_BLOCK AUTO_BLOCK)
{
	return AUTO_BLOCK->x;
}

//�����ϰ����������
unsigned int get_block_y(LPAUTO_BLOCK AUTO_BLOCK)
{
	return AUTO_BLOCK->y;
}

//���ϰ���ɳ�ȥ
void block_out(LPAUTO_BLOCK AUTO_BLOCK)
{
	AUTO_BLOCK->state = BLOCK_OUT;
}

void block_hit(LPAUTO_BLOCK AUTO_BLOCK)
{
	AUTO_BLOCK->state = HIT_DEAD;
}
// �ж��Ƿ�ײ�ϣ�
// ���ײ���򷵻�С���״̬��ײ�������������򷵻ؿ�ָ�롣
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
		// ֻ�ܴ� BLOCK_MOVED ��Ϊ HIT_DEAD, ���ܷ���仯��
		return 0;
	}
}


//ʹ�ϰ��ƶ����ж�״̬
void blocks_move_step()
{
	unsigned int num, i;
	PGAME_COORD snake;
	LPAUTO_BLOCK AUTO_BLOCK;
	num = get_blocks_num();
	// �ƶ������ж��Ƿ�ɳ���
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
	// ����ж��Ƿ�ײ�������״̬��
	for (i = 0; i < num; i++)
	{
		AUTO_BLOCK = get_block_at(i);
		be_hit(snake, AUTO_BLOCK);
	}
}


//������״̬�ٵ��ϰ����߽�����Ϸ
int destory_block_by_state()
{
	int flag = 0;
	unsigned int num, i;
	LPAUTO_BLOCK AUTO_BLOCK;

again:
	num = get_blocks_num();

	// �Ӻ���ǰɾ����Ȼһ��ѭ��ɾ������Ļ����ͻᳬ
	// ����һ��ѭ��ֻɾ��һ�������ѭ����
	// ����������ԸĽӵĵط�
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
