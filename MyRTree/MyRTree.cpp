// MyRTree.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

void test1()
{
	RTree rtree(5);
	Rect r = { 0,0,1,1 };
	rtree.Insert(r);

}

int main()
{
	int test;
	test1();
	scanf_s("%d", &test);
	return 0;
}

