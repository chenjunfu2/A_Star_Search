#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

//行数 Y轴
#define ROWS  10
//列数 X轴
#define COLS  10

//直线代价
#define ZXDJ  10
//斜线代价
#define XXDJ  14//.14213562373095048801688724209

//点结构  点类
struct MyPoint
{
	int row = 0, col = 0;//row是y col是x
	int f = 0, g = 0;//f=g+h f是最终成本 g是沉没成本、已支出成本(从起点到当前点的成本） h是预估成本(从当前点到目标点的成本)
};

//上下左右
enum dir
{
	p_up,
	p_down,
	p_left,
	p_right,
	p_lup,
	p_ldown,
	p_rup,
	p_rdown
};


//树节点类型
struct treeNode
{
	MyPoint	pos;
	treeNode *pParent = NULL;
	vector<treeNode *>	child;

	treeNode(void) = default;
	treeNode(const MyPoint &p) :pos(p) {}
	treeNode(int r, int c) :pos{r,c} {}
};

//计算H值并返回
inline int getH(const MyPoint &pos, const MyPoint &end)
{
	//long temp=(end.col - pos.col) *(end.col - pos.col) + (end.row - pos.row) * (end.row - pos.row);
	//return sqrt((double)temp) * ZXDJ;

	return (abs(end.col - pos.col) + abs(end.row - pos.row)) * ZXDJ;
}

int main(void)
{
	//控制台句柄
	HANDLE ConsoleHAND = GetStdHandle(STD_OUTPUT_HANDLE);

	//隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = {1, false};
	SetConsoleCursorInfo(ConsoleHAND, &cursor_info);

	//起点与终点
	MyPoint begPos = {1, 1};
	MyPoint endPos = {9, 9};

	//二维数组记录是否走过
	// 0 false 表示没有走过  1 true 走过
	bool pathMap[ROWS][COLS] = {0};//所有的点都是0 都没有走过
	//标记起点走过
	pathMap[begPos.row][begPos.col] = true;

	//二维数组描述地图
	int map[ROWS][COLS] = {//0表示路  1表示障碍
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 1, 0, 1, 1, 1, 1, 0, 0, 0 },
		{ 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 0, 1, 0, 1, 0, 0, 0 },
		{ 0, 1, 0, 0, 1, 0, 1, 0, 0, 0 },
		{ 0, 1, 0, 1, 1, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 0, 1, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	};
	
	//输出地图
	SetConsoleCursorPosition(ConsoleHAND, COORD{0,0});
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			cout << (map[y][x] ? "■" : "□");
		}
		cout << endl;
	}
	

	//输出路径 看一看
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			if (pathMap[y][x])
			{
				SetConsoleCursorPosition(ConsoleHAND, COORD{(short)x * 2,(short)y});
				cout << "○";
			}

		}
		cout << endl;
	}

	system("pause");

	//创建一颗树
	treeNode *pRoot = new treeNode(begPos);//起点入树

	//准备一个数组  用来找最小的f
	vector<treeNode *> buff;
	//当前点
	treeNode *pCurrent = pRoot;
	treeNode *pChild = NULL;

	bool isFindEnd = false;
	while (true)
	{
		//1 把八点都做出来
		for (int i = p_up; i < p_rdown; ++i)
		{
			pChild = new treeNode(pCurrent->pos);
			switch (i)
			{
			case p_up:
				pChild->pos.row--;
				pChild->pos.g += ZXDJ;
				break;
			case p_down:
				pChild->pos.row++;
				pChild->pos.g += ZXDJ;
				break;
			case p_left:
				pChild->pos.col--;
				pChild->pos.g += ZXDJ;
				break;
			case p_right:
				pChild->pos.col++;
				pChild->pos.g += ZXDJ;
				break;
			case p_lup:
				pChild->pos.col--;
				pChild->pos.row--;
				pChild->pos.g += XXDJ;
				break;
			case p_rup:
				pChild->pos.col++;
				pChild->pos.row--;
				pChild->pos.g += XXDJ;
				break;
			case p_ldown:
				pChild->pos.col--;
				pChild->pos.row++;
				pChild->pos.g += XXDJ;
				break;
			case p_rdown:
				pChild->pos.col++;
				pChild->pos.row++;
				pChild->pos.g += XXDJ;
				break;
			}

			//边界判断
			if (pChild->pos.row < 0 || pChild->pos.row >= ROWS ||
				pChild->pos.col < 0 || pChild->pos.col >= COLS)
			{
				//不能走
				delete pChild;
				pChild = NULL;
				continue;
			}

			//判断能不能走
			if (pathMap[pChild->pos.row][pChild->pos.col] == true ||//走过
				map[pChild->pos.row][pChild->pos.col] == 1)			//是障碍
			{
				//不能走
				delete pChild;
				pChild = NULL;
				continue;
			}

			//能走
			//2 算出来h 和f值 放到树里 放到buff里
			pChild->pos.f = pChild->pos.g + getH(pChild->pos, endPos);
			//放到buff里
			buff.push_back(pChild);
			//入树
			pCurrent->child.push_back(pChild);
			pChild->pParent = pCurrent;
		}


		//没路可走，退出
		if (buff.empty())
		{
			break;
		}

		//3 找出buff中 f最小的点  走  删掉
		vector<treeNode *>::iterator itMin;
		do
		{
			itMin = buff.begin();//假设第一个最小
			for (vector<treeNode *>::iterator it = ++buff.begin(); it != buff.end(); ++it)
			{//遍历整个buff数组
				if ((*it)->pos.f < (*itMin)->pos.f)
				{
					itMin = it;
				}
			}

			if (pathMap[(*itMin)->pos.row][(*itMin)->pos.col] == false)//如果该点未走过，则走
			{
				break;
			}

			//如果该点已被走过则找下一个未找过的点
			buff.erase(itMin);//删掉
		} while (!buff.empty());
		
		//走
		pCurrent = *itMin;
		pathMap[pCurrent->pos.row][pCurrent->pos.col] = true;

		//输出路径 看一看
		for (int y = 0; y < ROWS; ++y)
		{
			for (int x = 0; x < COLS; ++x)
			{
				if (pathMap[y][x])
				{
					SetConsoleCursorPosition(ConsoleHAND, COORD{(short)x * 2,(short)y});
					cout << "○";
				}

			}
			cout << endl;
		}
		Sleep(20);

		//4 如果找到了终点 循环结束
		if (pCurrent->pos.row == endPos.row && pCurrent->pos.col == endPos.col)
		{
			isFindEnd = true;
			break;
		}
	}

	//输出路径 
	while (pCurrent)
	{
		SetConsoleCursorPosition(ConsoleHAND, COORD{(short)pCurrent->pos.col * 2,(short)pCurrent->pos.row});
		cout << "●";
		pCurrent = pCurrent->pParent;
	}

	SetConsoleCursorPosition(ConsoleHAND, COORD{(short)0,(short)ROWS});

	if (isFindEnd)
	{
		cout << "找到终点了!                 " << endl;
	}
	else
	{
		cout << "无法找到终点!               " << endl;
	}

	system("pause");

	return 0;
}

