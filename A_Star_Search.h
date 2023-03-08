#pragma once

struct A_StarNode
{
	long x = 0, y = 0;
	long f = 0, g = 0, h = 0;
};

//����hֵ����
typedef long (*CalculatH)
(
	const A_StarNode &stCurrent,
	const A_StarNode &stTarget,
	long lStraightLineCost,
	long lDiagonalCost
);

//��ȡ��ǰ����Ϣ����
typedef void (*SearchCallBack)
(
	const A_StarNode &stCurrent,
	void *UserDefineValue
);

#include <vector>

long DefaultCalculatH//Ĭ�ϼ��㺯��
(
	const A_StarNode &stCurrent,
	const A_StarNode &stTarget,
	long lStraightLineCost,
	long lDiagonalCost
);

long A_Star_Search//�ҵ�����1�����򷵻�0������-1Ϊ�ڴ治�㣬����-2Ϊ��������
(
	const bool *pMap,//��ͼ 0��· 1���ϰ�
	long lMapWide,//��ͼ��
	long lMapHigh,//��ͼ��

	const A_StarNode &stSource,//ԭ�㣨�����㣩
	const A_StarNode &stTarget,//Ŀ��㣨�յ㣩

	std::vector<A_StarNode> *pPath,//����Ѱ··��
	bool bAllowDiagonal = true, //�Ƿ�����б��Ѱ·

	long lStraightLineCost = 5, //ֱ�ߴ���
	long lDiagonalCost = 7,//б�ߴ���

	CalculatH pfuncCalculatH = DefaultCalculatH, //���㵱ǰ�㵽Ŀ�������Ԥ������
	SearchCallBack pfuncCurrentSearch = nullptr,//��ȡ��ǰ�ߵĵ�
	void *pCurrentSearchValue = nullptr//�û��Զ��������CurrentSearch����ʹ�ã�
);