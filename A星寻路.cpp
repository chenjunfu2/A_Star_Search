#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
using namespace std;

//���� Y��
#define ROWS  10
//���� X��
#define COLS  10

//ֱ�ߴ���
#define ZXDJ  10
//б�ߴ���
#define XXDJ  14//.14213562373095048801688724209

//��ṹ  ����
struct MyPoint
{
	int row = 0, col = 0;//row��y col��x
	int f = 0, g = 0;//f=g+h f�����ճɱ� g�ǳ�û�ɱ�����֧���ɱ�(����㵽��ǰ��ĳɱ��� h��Ԥ���ɱ�(�ӵ�ǰ�㵽Ŀ���ĳɱ�)
};

//��������
struct
{
	int rowM;//y
	int colM;//x
	int DJ;//����
}
PosMove[8] =
{
	{-1,0,ZXDJ},//��
	{1,0,ZXDJ},//��
	{0,-1,ZXDJ},//��
	{0,1,ZXDJ},//��

	{-1,-1,XXDJ},//����
	{-1,1,XXDJ},//����
	{1,-1,XXDJ},//����
	{1,1,XXDJ},//����
};

//���ڵ�����
struct treeNode
{
	MyPoint	pos;
	treeNode *pParent = NULL;
	vector<treeNode *>	child;

	treeNode(void) = default;
	treeNode(const MyPoint &p) :pos(p) {}
	treeNode(int r, int c) :pos{r,c} {}
};

//����Hֵ������
inline int getH(const MyPoint &pos, const MyPoint &end)
{
	//long temp=(end.col - pos.col) *(end.col - pos.col) + (end.row - pos.row) * (end.row - pos.row);
	//return sqrt((double)temp) * ZXDJ;

	return (abs(end.col - pos.col) + abs(end.row - pos.row)) * ZXDJ;
}

int main(void)
{
	//����̨���
	HANDLE ConsoleHAND = GetStdHandle(STD_OUTPUT_HANDLE);
#define GOTO_XY(X,Y) (SetConsoleCursorPosition(ConsoleHAND, COORD{(short)(X),(short)(Y)}))

	//���ع��
	CONSOLE_CURSOR_INFO cursor_info = {1, false};
	SetConsoleCursorInfo(ConsoleHAND, &cursor_info);

	//������յ�
	MyPoint begPos = {1, 1};
	MyPoint endPos = {9, 9};


	//��ά����������ͼ
	int map[ROWS][COLS] = //0��ʾ·  1��ʾ�ϰ�
	{
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
	
	//�����ͼ
	GOTO_XY(0, 0);
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			cout << (map[y][x] ? "��" : "��");
		}
		cout << endl;
	}

	//��ά�����¼�Ƿ��߹�
	// 0 false ��ʾû���߹�  1 true �߹�
	bool pathMap[ROWS][COLS] = {0};//���еĵ㶼��0 ��û���߹�

	//�������߹�
	pathMap[begPos.row][begPos.col] = true;
	//���·��
	GOTO_XY(begPos.col * 2, begPos.col);
	cout << "��";

	GOTO_XY(0, ROWS);
	system("pause");

	//����һ����
	treeNode *pRoot = new treeNode(begPos);//�������
	//׼��һ������  ��������С��f
	vector<treeNode *> open;
	vector<treeNode *> close;
	//��ǰ��
	treeNode *pCurrent = pRoot;
	//�Ƿ��ҵ�
	bool isFindEnd = false;

	while (true)
	{
		//1 �Ѱ˵㶼������
		for (int i = 0; i < 8; ++i)
		{
			treeNode *pChild = new treeNode(pCurrent->pos);

			pChild->pos.row += PosMove[i].rowM;
			pChild->pos.col += PosMove[i].colM;
			pChild->pos.g += PosMove[i].DJ;

			//�߽��ж�
			if (pChild->pos.row < 0 || pChild->pos.row >= ROWS ||
				pChild->pos.col < 0 || pChild->pos.col >= COLS)
			{
				//������
				delete pChild;
				pChild = NULL;
				continue;
			}

			//�ж��ܲ�����
			if (pathMap[pChild->pos.row][pChild->pos.col] == true ||//�߹�
				map[pChild->pos.row][pChild->pos.col] == 1)			//���ϰ�
			{
				//������
				delete pChild;
				pChild = NULL;
				continue;
			}

			//����
			//2 �����h ��fֵ �ŵ����� �ŵ�buff��
			pChild->pos.f = pChild->pos.g + getH(pChild->pos, endPos);
			//�ŵ�buff��
			open.push_back(pChild);
			//����
			pCurrent->child.push_back(pChild);
			pChild->pParent = pCurrent;
		}

		//û·���ߣ��˳�
		if (open.empty())
		{
			break;
		}

		//3 �ҳ�buff�� f��С�ĵ�  ��  ɾ��
		vector<treeNode *>::iterator itMin;
		do
		{
			itMin = open.begin();//�����һ����С
			for (vector<treeNode *>::iterator it = ++open.begin(); it != open.end(); ++it)
			{//��������buff����
				if ((*it)->pos.f < (*itMin)->pos.f)
				{
					itMin = it;
				}
			}
		
			if (pathMap[(*itMin)->pos.row][(*itMin)->pos.col] == true)
			{
				//����õ��ѱ��߹�������һ��δ�ҹ��ĵ�
				close.push_back(*itMin);
				open.erase(itMin);//ɾ��
			}
			else
			{
				//����õ�δ�߹������˳�ѭ����
				break;
			}
			
		} while (!open.empty());
		
		//��
		pCurrent = *itMin;//����

		close.push_back(*itMin);//����
		open.erase(itMin);//ɾ��
		pathMap[pCurrent->pos.row][pCurrent->pos.col] = true;//�߹�
		
		//���·��
		GOTO_XY(pCurrent->pos.col * 2, pCurrent->pos.row);
		cout << "��";
		Sleep(20);

		//4 ����ҵ����յ� ѭ������
		if (pCurrent->pos.row == endPos.row && pCurrent->pos.col == endPos.col)
		{
			isFindEnd = true;
			break;
		}
	}

	//���·�� 
	while (pCurrent)
	{
		GOTO_XY(pCurrent->pos.col * 2,pCurrent->pos.row);
		cout << "��";
		pCurrent = pCurrent->pParent;
	}

	GOTO_XY(0,ROWS);

	if (isFindEnd)
	{
		cout << "�ҵ��յ���!                 " << endl;
	}
	else
	{
		cout << "�޷��ҵ��յ�!               " << endl;
	}


	for (vector<treeNode *>::iterator it = ++open.begin(); it != open.end(); ++it)
	{
		delete *it;//�ͷ��ڴ�
	}
	for (vector<treeNode *>::iterator it = ++close.begin(); it != close.end(); ++it)
	{
		delete *it;//�ͷ��ڴ�
	}

	system("pause");

	return 0;
}

