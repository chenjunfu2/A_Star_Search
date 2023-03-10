#include "A_Star_Search.h"

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define MAPX 41
#define MAPY 40
//起点与终点
A_StarNode stBeg = {MAPX / 2,MAPY - 1};
A_StarNode stEnd = {MAPX / 2 - 1,MAPY / 2 - 1};

//二维数组描述地图
bool map[MAPY][MAPX] = //0表示路  1表示障碍
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,},
	{1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,0,1,0,1,0,0,0,1,},
	{1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,},
	{1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,},
	{1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,},
	{1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,1,},
	{1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,},
	{1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,},
	{1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,},
	{1,0,1,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,},
	{1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,},
	{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
	{1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,},
	{1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,},
	{1,0,0,0,0,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{1,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,},
	{1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,},
	{1,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	{1,1,1,1,0,1,0,1,0,1,0,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,},
	{1,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,},
	{1,0,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,},
	{1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,},
	{1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,0,1,1,1,0,1,},
	{1,0,0,0,0,0,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,1,},
	{1,0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,1,1,1,},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,},
	{1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,},
	{1,0,0,0,1,0,1,1,0,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,0,1,1,1,1,1,0,1,1,0,1,},
	{1,1,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,1,0,0,1,},
	{1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,},
	{1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,},
	{1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
};


void SearchCbk(
	const A_StarNode &stCurrent,
	void *UserDefineValue)
{
	//输出路径
	SetConsoleCursorPosition(UserDefineValue, COORD{(short)(stCurrent.x * 2),(short)(stCurrent.y)});
	printf("○");
	//_getch();
	//Sleep(20);
}

int main(void)
{
	HANDLE ConsoleHAND = GetStdHandle(STD_OUTPUT_HANDLE);
#define GOTO_XY(X,Y) (SetConsoleCursorPosition(ConsoleHAND, COORD{(short)(X),(short)(Y)}))

	//隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = {1, false};
	SetConsoleCursorInfo(ConsoleHAND, &cursor_info);

	//输出地图
	GOTO_XY(0, 0);
	for (int y = 0; y < MAPY; ++y)
	{
		for (int x = 0; x < MAPX; ++x)
		{
			printf("%s", map[y][x] ? "■" : "□");
		}
		putchar('\n');
	}

	system("pause");

	std::vector<A_StarNode> vaPath;
	if (A_Star_Search((bool *)map, MAPX, MAPY, stBeg, stEnd, &vaPath, true, 10, 14, DefaultCalculatH, SearchCbk, ConsoleHAND) != 1)
	{
		GOTO_XY(0, MAPY);
		printf("无法找到终点!\n");
		system("pause");
		return -1;
	}

	for (auto const &it : vaPath)
	{
		GOTO_XY(it.x * 2, it.y);
		printf("●");
	}

	GOTO_XY(0, MAPY);
	printf("找到终点!                      \n");
	system("pause");
	return 0;
}