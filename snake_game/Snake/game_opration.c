
/************** ͷ�ļ� ****************/
#include <Windows.h>
#include "snake.h"

/************** �궨�� ****************/
// ��ʱ��ID��
#define TIMER_ID 12340
// �ж���Ϸ�Ƿ�ﵽ��������
#define IS_SPEEDUP(s) (s % dwOneLevelScores) == 0

/************** ȫ�ֱ��� ****************/
// ��ʱ����ʱ���
DWORD dwTimerElapse;
// ÿ���һ������ʱ��ʱ���������̵ı�����
DOUBLE dbLevelSpeedupRatio;
// �˱�����ʾ���ٸ���������һ������
DWORD dwOneLevelScores;

// ����Ʒ֣���ʼΪ0
int score = 0;
// ���漶�𣬳�ʼΪ0
int level = 0;
//��ʮ��δ�Ե�ʳ�������ʳ��
int step = 0;
//ʳ��������������ϣ���ȼ��½�
int ResetTimes = 0;
//����ʱ�õ���ʱ����
int flag = 0;


/************** �������� ****************/
// ���ػ���
int GetScore(){return score;}

// ����level
int GetLevel(){return level;}

//����ResetTimes
int GetResetTimes() { return ResetTimes; }
// ����ٶȡ�
BOOL SpeedUp(HWND hwnd)
{
	// ������ʱ����ʱʱ��
	dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return TRUE;
}
BOOL SpeedDown(HWND hwnd)
{
	// ������ʱ����ʱʱ��
	dwTimerElapse = (DWORD)(dwTimerElapse / dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return TRUE;
}


// ��Ϸ���Ƶ�һ����Ҫ���̡�
// ����ʱ������ʱ���еĴ����߼���
void OnTimer(HWND hwnd)
{
	// ��ʱ����ʱ�Ժ����ƶ�һ����
	// �����Ժ��Ժ��״̬�����к�������
	switch (SnakeMove())

	{
	// ������Ѿ�����
	case SNAKE_KNOCKBORDER:
		ThroughBorder();
		//���ײ���߽磬�򴩹�
		break;
	case SNAKE_DEAD:
		// ���ȹرռ�ʱ���������ڼ�ʱ�����������ٴε��ñ�����
		KillTimer(hwnd, TIMER_ID);
		// Ȼ��֪ͨ��ң�Game Over�ˣ��˳����̡�
		MessageBox(0, "Game Over", "Game Over", 0);
		ExitProcess(0);
		break;
	// ����߳Ե���ʳ��
	case SNAKE_EATEN_FOOD:
		// �Ʒ֣�
		score++;
		// �����µ�ʳ�ʳ����һ��ȫ��Ψһ������
		CreateFood();
		// �߽���������Ȼ���ж��������Ժ��״̬�����к�������
		step = 0;
		//����߳Ե���ʳ�step��Ϊ0
		switch (SnakeGorwup())
		{
			// ���Ӯ�ˣ�ֹͣ��ʱ����
		case SNAKE_COMPLETE:		
			KillTimer(hwnd, TIMER_ID);
			// ֪ͨ��ң��˳����̣�������Ϸ��
			MessageBox(0, "You Win!", "You Win", 0);
			ExitProcess(0);
			break;
			// ��������ֹͣ��ʱ�������˳���
		case SNAKE_ERROR:
			KillTimer(hwnd, TIMER_ID);
			MessageBox(hwnd, "Error!!", "Error!1", MB_OK);
			ExitProcess(0);
			break;
			// û�н�����Ҳû�д��������Ե�ʳ�
		case SNAKE_GROWUP:
			// �жϼƷ֣��Ƿ�������
			if (IS_SPEEDUP(score))
			{
				level++;
				SpeedUp(hwnd);
			}
			break;
		} //switch (SnakeGorwup()) ����
		break;
	case SNAKE_MOVED:
		break;
	}
	if (step >= 50)
	{
		CreateFood();
		step = 0;
		ResetTimes++;
		flag = 0;
	}//�����������50������ʳ�������������
	if ((ResetTimes+5)%5==0&&flag==0&& ResetTimes!=0)
	{
		level--;
		SpeedDown(hwnd);
		flag = 1;
	}//ÿ�����ô����������򽵼�����
	step++;
	//������״̬�ٵ��ϰ����߽�����Ϸ
	/*
	if (destory_block_by_state() == HIT_BLOCK_DEAD)
	{
		KillTimer(hwnd, TIMER_ID);
		// Ȼ��֪ͨ��ң�Game Over�ˣ��˳����̡�
		MessageBox(0, "Game Over", "Game Over", 0);
		ExitProcess(0);
	}
	//�ϰ���Ҳ�ƶ�һ��
	blocks_move_step();
	*/
	
	if (rand() % 1000 < 500) // 7%�ĸ��ʣ���������ϰ���
	{
		gen_block();
	}
	return;
}


// �����ڲ���ʱ���е��߼�����
// ����Ϸֻʹ�õ��˼����������Ҽ����ơ�
void OnKeyDown(DWORD vk)
{
	switch (vk) // virtual key value
	{
	case VK_LEFT:
		SetDirction(SNAKE_LEFT);
		break;
	case VK_RIGHT:
		SetDirction(SNAKE_RIGHT);
		break;
	case VK_UP:
		SetDirction(SNAKE_UP);
		break;
	case VK_DOWN:
		SetDirction(SNAKE_DOWN);
		break;
	}
	return;
}


// ��Ϸ�ĳ�ʼ����
// ������Ϸ���ڲ����ݽṹ��ϵͳ����
void CreateGame(HWND hwnd, // �����ھ��
	DWORD dwInitTimerElapse, //
	unsigned int one_level_scores,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y,
	int init_x, int init_y, 
	int init_len,
	dirction init_dir)//ö������
{
	// �������������
	// ��Ҫʹ�����������ʳ���λ�õȡ�
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);

	dbLevelSpeedupRatio = level_speedup_ratio;
	dwTimerElapse = dwInitTimerElapse;
	dwOneLevelScores = one_level_scores;

	// ������Ϸ�ı߽�
	SetBoundary(boundary_x, boundary_y);

	// ������ʾ̰���ߵ����ݽṹ
	CreateSnake(init_dir, init_x, init_y, init_len);

	//������ʾǽ�����ݽṹ
	CreateWall();

	// ������ʾʳ������ݽṹ
	CreateFood();

	//������ʾ�ϰ������ݽṹ
	blocks_init();

	// ����һ����ʱ��
	// ÿ���� dwTimerElapse ���룬hwnd���ڣ������ڣ��ͻ��յ�һ��WM_TIMER��Ϣ��
	// ��ʱ������������Ϸ���е���Ҫʱ���ߡ�
	// dwTimerElapse����Ӱ����Ϸ���еĿ����仯��
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);

}
