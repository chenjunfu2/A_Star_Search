#include <iostream>
#include <vector>
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

	//���ع��
	CONSOLE_CURSOR_INFO cursor_info = {1, false};
	SetConsoleCursorInfo(ConsoleHAND, &cursor_info);

	//������յ�
	MyPoint begPos = {1, 1};
	MyPoint endPos = {9, 9};

	//��ά�����¼�Ƿ��߹�
	// 0 false ��ʾû���߹�  1 true �߹�
	bool pathMap[ROWS][COLS] = {0};//���еĵ㶼��0 ��û���߹�
	//�������߹�
	pathMap[begPos.row][begPos.col] = true;

	//��ά����������ͼ
	int map[ROWS][COLS] = {//0��ʾ·  1��ʾ�ϰ�
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
	SetConsoleCursorPosition(ConsoleHAND, COORD{0,0});
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			cout << (map[y][x] ? "��" : "��");
		}
		cout << endl;
	}
	

	//���·�� ��һ��
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			if (pathMap[y][x])
			{
				SetConsoleCursorPosition(ConsoleHAND, COORD{(short)x * 2,(short)y});
				cout << "��";
			}

		}
		cout << endl;
	}

	system("pause");

	//����һ����
	treeNode *pRoot = new treeNode(begPos);//�������

	//׼��һ������  ��������С��f
	vector<treeNode *> buff;
	//��ǰ��
	treeNode *pCurrent = pRoot;
	treeNode *pChild = NULL;

	bool isFindEnd = false;
	while (true)
	{
		//1 �Ѱ˵㶼������
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
			buff.push_back(pChild);
			//����
			pCurrent->child.push_back(pChild);
			pChild->pParent = pCurrent;
		}


		//û·���ߣ��˳�
		if (buff.empty())
		{
			break;
		}

		//3 �ҳ�buff�� f��С�ĵ�  ��  ɾ��
		vector<treeNode *>::iterator itMin;
		do
		{
			itMin = buff.begin();//�����һ����С
			for (vector<treeNode *>::iterator it = ++buff.begin(); it != buff.end(); ++it)
			{//��������buff����
				if ((*it)->pos.f < (*itMin)->pos.f)
				{
					itMin = it;
				}
			}

			if (pathMap[(*itMin)->pos.row][(*itMin)->pos.col] == false)//����õ�δ�߹�������
			{
				break;
			}

			//����õ��ѱ��߹�������һ��δ�ҹ��ĵ�
			buff.erase(itMin);//ɾ��
		} while (!buff.empty());
		
		//��
		pCurrent = *itMin;
		pathMap[pCurrent->pos.row][pCurrent->pos.col] = true;

		//���·�� ��һ��
		for (int y = 0; y < ROWS; ++y)
		{
			for (int x = 0; x < COLS; ++x)
			{
				if (pathMap[y][x])
				{
					SetConsoleCursorPosition(ConsoleHAND, COORD{(short)x * 2,(short)y});
					cout << "��";
				}

			}
			cout << endl;
		}
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
		SetConsoleCursorPosition(ConsoleHAND, COORD{(short)pCurrent->pos.col * 2,(short)pCurrent->pos.row});
		cout << "��";
		pCurrent = pCurrent->pParent;
	}

	SetConsoleCursorPosition(ConsoleHAND, COORD{(short)0,(short)ROWS});

	if (isFindEnd)
	{
		cout << "�ҵ��յ���!                 " << endl;
	}
	else
	{
		cout << "�޷��ҵ��յ�!               " << endl;
	}

	system("pause");

	return 0;
}

