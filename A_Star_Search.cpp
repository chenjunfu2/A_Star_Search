#include "A_Star_Search.h"
#include <queue>
#include <vector>
#include <new>

//���ṹ
struct A_StarTree
{
	A_StarNode Node = {0};
	A_StarTree *pParent = nullptr;
};

//�����õıȽϺ���
struct cmp
{
	bool operator()(const A_StarTree *pLeft, const A_StarTree *pRight)
	{
		return pLeft->Node.f > pRight->Node.f;
	}
};

long A_Star_Search(//�ҵ�����1�����򷵻�0������-1Ϊ�ڴ治�㣬����-2Ϊ��������
	const bool *pMap,//��ͼ 0��· 1���ϰ�
	long lMapWide,//��ͼ��
	long lMapHigh,//��ͼ��

	const A_StarNode &stSource,//ԭ�㣨�����㣩
	const A_StarNode &stTarget,//Ŀ��㣨�յ㣩

	std::vector<A_StarNode> *pPath,//����Ѱ··��
	bool bAllowDiagonal,//�Ƿ�����б��Ѱ·

	long lStraightLineCost,//ֱ�ߴ���
	long lDiagonalCost,//б�ߴ���

	CalculatH pfuncCalculatH,//���㵱ǰ�㵽Ŀ�������Ԥ������
	SearchCallBack pfuncCurrentSearch,//��ȡ��ǰ�ߵĵ�
	void *pCurrentSearchValue)//�û��Զ��������CurrentSearch����ʹ�ã�
{
	//���
	if (pMap == nullptr ||
		lMapHigh == 0 ||
		lMapWide == 0 ||
		pfuncCalculatH == nullptr)
	{
		return -2;//��������
	}

	if (stSource.x < 0 || stSource.x >= lMapWide ||
		stSource.y < 0 || stSource.y >= lMapHigh ||
		stTarget.x < 0 || stTarget.x >= lMapWide ||
		stTarget.y < 0 || stTarget.y >= lMapHigh)
	{
		return -2;//��������
	}

	//�����Ϳ�����ͼ
	bool *bPathMap = new(std::nothrow) bool[(size_t)lMapHigh * (size_t)lMapWide];
	if (bPathMap == nullptr)
	{
		return -1;//�ڴ治��
	}
	//���������ϰ���Ϊ���߹��ĵ㣩
	memcpy(bPathMap, pMap, (size_t)lMapHigh * (size_t)lMapWide);//bPathMap[lMapWide * y + x];

	//���������
	A_StarTree *pRootNode = new(std::nothrow) A_StarTree{stSource};
	if (pRootNode == nullptr)
	{
		delete[] bPathMap;
		bPathMap = nullptr;
		return -1;//�ڴ治��
	}

	//�����б����ڼ�¼
	std::priority_queue<A_StarTree *, std::vector<A_StarTree *>, cmp> OpenList;
	std::queue<A_StarTree *> CloseList;

	//������ǰ��ָ��ָ�����
	A_StarTree *pCurrentNode = pRootNode;
	CloseList.push(pRootNode);//�������߹����б�
	//��������߹�
	bPathMap[lMapWide * pCurrentNode->Node.y + pCurrentNode->Node.x] = true;

	//���ýڵ���Ϣ�ص�����
	if (pfuncCurrentSearch != nullptr)
	{
		pfuncCurrentSearch(pCurrentNode->Node, pCurrentSearchValue);
	}

	//�������ұ任
	const struct
	{
		long lXconver;//x�任
		long lYconver;//y�任
		long lCost;//����
	}stConver[8] =
	{
		{ 0,-1,lStraightLineCost},	//��
		{ 0, 1,lStraightLineCost},	//��
		{-1, 0,lStraightLineCost},	//��
		{ 1, 0,lStraightLineCost},	//��

		{-1,-1,lDiagonalCost},	//����
		{ 1,-1,lDiagonalCost},	//����
		{-1, 1,lDiagonalCost},	//����
		{ 1, 1,lDiagonalCost},	//����
	};

	long lRetVal = 0;//����ֵ
	bool bArriveTarget = false;//�Ƿ��ҵ��յ�
	long lLoopCount = bAllowDiagonal ? 8 : 4;//�������б��ѭ��8�η���ѭ��4��

	while (true)
	{
		for (long i = 0; i < lLoopCount; ++i)
		{
			//��ʼ��һ�����ӽڵ�
			A_StarTree *pChildNode = new(std::nothrow) A_StarTree{pCurrentNode->Node};
			if (pChildNode == nullptr)
			{
				lRetVal = -1;
				goto RESOURCE_RECOVERY;//�����������ѭ����������Դ
			}

			//�任
			pChildNode->Node.x += stConver[i].lXconver;
			pChildNode->Node.y += stConver[i].lYconver;
			pChildNode->Node.g += stConver[i].lCost;

			//�߽��ж�
			if (pChildNode->Node.x < 0 || pChildNode->Node.x >= lMapWide ||
				pChildNode->Node.y < 0 || pChildNode->Node.y >= lMapHigh)//�����߽�
			{
				delete pChildNode;
				pChildNode = nullptr;
				continue;
			}

			//·���ж�
			if (bPathMap[lMapWide * pChildNode->Node.y + pChildNode->Node.x] == true)//�߹���
			{
				delete pChildNode;
				pChildNode = nullptr;
				continue;
			}

			//������ۡ����ø��ڵ�
			pChildNode->Node.h = pfuncCalculatH(pChildNode->Node, stTarget, lStraightLineCost, lDiagonalCost);
			pChildNode->Node.f = pChildNode->Node.g + pChildNode->Node.h;
			pChildNode->pParent = pCurrentNode;
			//���뿪���б���������
			OpenList.push(pChildNode);
		}

		//fֵ��С�ڵ�
		A_StarTree *pMinNode = nullptr;

		//�ҵ�һ��û�߹���fֵ��С�Ľڵ�
		while (!OpenList.empty())
		{
			//���topԪ�أ�fֵ��С��
			pMinNode = OpenList.top();
			
			//�ж��Ƿ��߹�
			if (bPathMap[lMapWide * pMinNode->Node.y + pMinNode->Node.x] == true)//�߹���
			{
				CloseList.push(pMinNode);//�����߹����б�
				OpenList.pop();//ɾ����Ԫ��
				continue;//������һ����С�ڵ�
			}
			else//û�߹�
			{
				break;//�˳�ѭ��
			}
		}

		if (OpenList.empty())//�ж��ǲ���û·������
		{
			lRetVal = 0;//û�ҵ�
			break;//�Ǿ�����ѭ��
		}

		//���õ�ǰ��Ϊ��С��
		pCurrentNode = pMinNode;
		CloseList.push(pMinNode);//�����߹����б�
		OpenList.pop();//ɾ����Ԫ��
		//���øõ��߹�
		bPathMap[lMapWide * pCurrentNode->Node.y + pCurrentNode->Node.x] = true;

		//���ýڵ���Ϣ�ص�����
		if (pfuncCurrentSearch != nullptr)
		{
			pfuncCurrentSearch(pCurrentNode->Node, pCurrentSearchValue);
		}

		//������յ㣬����
		if (pCurrentNode->Node.x == stTarget.x && pCurrentNode->Node.y == stTarget.y)
		{
			bArriveTarget = true;//���ҵ�
			lRetVal = 1;
			break;
		}
	}

	//����ҵ��ˣ�����·������
	if (bArriveTarget)
	{
		while (pCurrentNode)
		{
			pPath->push_back(pCurrentNode->Node);
			pCurrentNode = pCurrentNode->pParent;
		}
	}

	//��Դ����
RESOURCE_RECOVERY:
	//���յ�ͼ����
	delete[] bPathMap;
	bPathMap = nullptr;

	//����δ�߹��б�ʣ��Ԫ��
	while (!OpenList.empty())
	{
		delete OpenList.top();
		OpenList.pop();
	}

	//�������߹��б�ʣ��Ԫ��
	while (!CloseList.empty())
	{
		delete CloseList.front();
		CloseList.pop();
	}

	//����Ѱ�ҽ��
	return lRetVal;
}

long DefaultCalculatH(
	const A_StarNode &stCurrent,
	const A_StarNode &stTarget,
	long lStraightLineCost,
	long lDiagonalCost)
{
	return (abs(stTarget.x - stCurrent.x) + abs(stTarget.y - stCurrent.y)) * lStraightLineCost;
}