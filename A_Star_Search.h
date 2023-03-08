#pragma once

struct A_StarNode
{
	long x = 0, y = 0;
	long f = 0, g = 0, h = 0;
};

//计算h值函数
typedef long (*CalculatH)
(
	const A_StarNode &stCurrent,
	const A_StarNode &stTarget,
	long lStraightLineCost,
	long lDiagonalCost
);

//获取当前点信息函数
typedef void (*SearchCallBack)
(
	const A_StarNode &stCurrent,
	void *UserDefineValue
);

#include <vector>

long DefaultCalculatH//默认计算函数
(
	const A_StarNode &stCurrent,
	const A_StarNode &stTarget,
	long lStraightLineCost,
	long lDiagonalCost
);

long A_Star_Search//找到返回1，否则返回0，返回-1为内存不足，返回-2为参数错误
(
	const bool *pMap,//地图 0是路 1是障碍
	long lMapWide,//地图宽
	long lMapHigh,//地图高

	const A_StarNode &stSource,//原点（出发点）
	const A_StarNode &stTarget,//目标点（终点）

	std::vector<A_StarNode> *pPath,//返回寻路路径
	bool bAllowDiagonal = true, //是否允许斜线寻路

	long lStraightLineCost = 5, //直线代价
	long lDiagonalCost = 7,//斜线代价

	CalculatH pfuncCalculatH = DefaultCalculatH, //计算当前点到目标点距离的预估函数
	SearchCallBack pfuncCurrentSearch = nullptr,//获取当前走的点
	void *pCurrentSearchValue = nullptr//用户自定义参数（CurrentSearch函数使用）
);