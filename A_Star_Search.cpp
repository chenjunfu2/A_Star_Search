#include "A_Star_Search.h"
#include <queue>
#include <vector>
#include <new>

//树结构
struct A_StarTree
{
	A_StarNode Node = {0};
	A_StarTree *pParent = nullptr;
};

//排序用的比较函数
struct cmp
{
	bool operator()(const A_StarTree *pLeft, const A_StarTree *pRight)
	{
		return pLeft->Node.f > pRight->Node.f;
	}
};

long A_Star_Search(//找到返回1，否则返回0，返回-1为内存不足，返回-2为参数错误
	const bool *pMap,//地图 0是路 1是障碍
	long lMapWide,//地图宽
	long lMapHigh,//地图高

	const A_StarNode &stSource,//原点（出发点）
	const A_StarNode &stTarget,//目标点（终点）

	std::vector<A_StarNode> *pPath,//返回寻路路径
	bool bAllowDiagonal,//是否允许斜线寻路

	long lStraightLineCost,//直线代价
	long lDiagonalCost,//斜线代价

	CalculatH pfuncCalculatH,//计算当前点到目标点距离的预估函数
	SearchCallBack pfuncCurrentSearch,//获取当前走的点
	void *pCurrentSearchValue)//用户自定义参数（CurrentSearch函数使用）
{
	//检查
	if (pMap == nullptr ||
		lMapHigh == 0 ||
		lMapWide == 0 ||
		pfuncCalculatH == nullptr)
	{
		return -2;//参数错误
	}

	if (stSource.x < 0 || stSource.x >= lMapWide ||
		stSource.y < 0 || stSource.y >= lMapHigh ||
		stTarget.x < 0 || stTarget.x >= lMapWide ||
		stTarget.y < 0 || stTarget.y >= lMapHigh)
	{
		return -2;//参数错误
	}

	//创建和拷贝地图
	bool *bPathMap = new(std::nothrow) bool[(size_t)lMapHigh * (size_t)lMapWide];
	if (bPathMap == nullptr)
	{
		return -1;//内存不足
	}
	//拷贝（将障碍作为已走过的点）
	memcpy(bPathMap, pMap, (size_t)lMapHigh * (size_t)lMapWide);//bPathMap[lMapWide * y + x];

	//创建起点树
	A_StarTree *pRootNode = new(std::nothrow) A_StarTree{stSource};
	if (pRootNode == nullptr)
	{
		delete[] bPathMap;
		bPathMap = nullptr;
		return -1;//内存不足
	}

	//两个列表用于记录
	std::priority_queue<A_StarTree *, std::vector<A_StarTree *>, cmp> OpenList;
	std::queue<A_StarTree *> CloseList;

	//创建当前点指针指向起点
	A_StarTree *pCurrentNode = pRootNode;
	CloseList.push(pRootNode);//起点放入走过的列表
	//设置起点走过
	bPathMap[lMapWide * pCurrentNode->Node.y + pCurrentNode->Node.x] = true;

	//调用节点信息回调函数
	if (pfuncCurrentSearch != nullptr)
	{
		pfuncCurrentSearch(pCurrentNode->Node, pCurrentSearchValue);
	}

	//上下左右变换
	const struct
	{
		long lXconver;//x变换
		long lYconver;//y变换
		long lCost;//代价
	}stConver[8] =
	{
		{ 0,-1,lStraightLineCost},	//上
		{ 0, 1,lStraightLineCost},	//下
		{-1, 0,lStraightLineCost},	//左
		{ 1, 0,lStraightLineCost},	//右

		{-1,-1,lDiagonalCost},	//左上
		{ 1,-1,lDiagonalCost},	//右上
		{-1, 1,lDiagonalCost},	//左下
		{ 1, 1,lDiagonalCost},	//右下
	};

	long lRetVal = 0;//返回值
	bool bArriveTarget = false;//是否找到终点
	long lLoopCount = bAllowDiagonal ? 8 : 4;//如果允许斜线循环8次否则循环4次

	while (true)
	{
		for (long i = 0; i < lLoopCount; ++i)
		{
			//初始化一个新子节点
			A_StarTree *pChildNode = new(std::nothrow) A_StarTree{pCurrentNode->Node};
			if (pChildNode == nullptr)
			{
				lRetVal = -1;
				goto RESOURCE_RECOVERY;//跳出到最外层循环外清理资源
			}

			//变换
			pChildNode->Node.x += stConver[i].lXconver;
			pChildNode->Node.y += stConver[i].lYconver;
			pChildNode->Node.g += stConver[i].lCost;

			//边界判断
			if (pChildNode->Node.x < 0 || pChildNode->Node.x >= lMapWide ||
				pChildNode->Node.y < 0 || pChildNode->Node.y >= lMapHigh)//超出边界
			{
				delete pChildNode;
				pChildNode = nullptr;
				continue;
			}

			//路径判断
			if (bPathMap[lMapWide * pChildNode->Node.y + pChildNode->Node.x] == true)//走过了
			{
				delete pChildNode;
				pChildNode = nullptr;
				continue;
			}

			//计算代价、设置父节点
			pChildNode->Node.h = pfuncCalculatH(pChildNode->Node, stTarget, lStraightLineCost, lDiagonalCost);
			pChildNode->Node.f = pChildNode->Node.g + pChildNode->Node.h;
			pChildNode->pParent = pCurrentNode;
			//加入开放列表（插入排序）
			OpenList.push(pChildNode);
		}

		//f值最小节点
		A_StarTree *pMinNode = nullptr;

		//找到一个没走过的f值最小的节点
		while (!OpenList.empty())
		{
			//获得top元素（f值最小）
			pMinNode = OpenList.top();
			
			//判断是否走过
			if (bPathMap[lMapWide * pMinNode->Node.y + pMinNode->Node.x] == true)//走过了
			{
				CloseList.push(pMinNode);//放入走过的列表
				OpenList.pop();//删除该元素
				continue;//继续下一个最小节点
			}
			else//没走过
			{
				break;//退出循环
			}
		}

		if (OpenList.empty())//判断是不是没路可走了
		{
			lRetVal = 0;//没找到
			break;//是就跳出循环
		}

		//设置当前点为最小点
		pCurrentNode = pMinNode;
		CloseList.push(pMinNode);//放入走过的列表
		OpenList.pop();//删除该元素
		//设置该点走过
		bPathMap[lMapWide * pCurrentNode->Node.y + pCurrentNode->Node.x] = true;

		//调用节点信息回调函数
		if (pfuncCurrentSearch != nullptr)
		{
			pfuncCurrentSearch(pCurrentNode->Node, pCurrentSearchValue);
		}

		//如果到终点，结束
		if (pCurrentNode->Node.x == stTarget.x && pCurrentNode->Node.y == stTarget.y)
		{
			bArriveTarget = true;//已找到
			lRetVal = 1;
			break;
		}
	}

	//如果找到了，放入路径容器
	if (bArriveTarget)
	{
		while (pCurrentNode)
		{
			pPath->push_back(pCurrentNode->Node);
			pCurrentNode = pCurrentNode->pParent;
		}
	}

	//资源回收
RESOURCE_RECOVERY:
	//回收地图拷贝
	delete[] bPathMap;
	bPathMap = nullptr;

	//回收未走过列表剩余元素
	while (!OpenList.empty())
	{
		delete OpenList.top();
		OpenList.pop();
	}

	//回收已走过列表剩余元素
	while (!CloseList.empty())
	{
		delete CloseList.front();
		CloseList.pop();
	}

	//返回寻找结果
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