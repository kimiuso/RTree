// MyRTree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void test1()
{
	RTree rtree(3);
	Rect r = { 0,0,1,1 };
	rtree.Insert(r);
	rtree.PrintRTree();
	for (int i = 1; i < 10; i++)
	{
		r = { i,i,i + 1,i + 1 };
		rtree.Insert(r);
		rtree.PrintRTree();
		printf_s("\n");
	}

}

void test2()
{
	vector<int> vInt;
	vInt.push_back(1);
	vInt.push_back(2);
	vInt.push_back(3);
	vInt.push_back(4);
	vInt.push_back(5);
	int n = 6;
	vInt.push_back(n);
	n = 7;
	vInt.push_back(n);
	vector<int>::iterator it = vInt.begin();
	for (int i = 0; it + i != vInt.end(); i++)
	{
		printf("vInt[%d]=%d,", i, *(it + i));
	}
	vInt.erase(it+3);
	for (int i = 0; it + i != vInt.end(); i++)
	{
		printf("vInt[%d]=%d,", i, *(it + i));
	}

}

void test3()
{
	int i[2];
	int i2[2];
	i[0] = 0;
	i[1] = 1;
	*i2 = *i;
	printf_s("%d", i[1]);
}

void testvector()
{
	vector<int> vInt;
	vInt.push_back(1);
	vInt.push_back(3);
	try
	{
		printf_s("%d", vInt.at(2));
	}
	catch (const std::exception& e)
	{
		printf_s("%s", e.what());
	}
	
	vector<int> vInt2;
	//vInt2=vInt;
	//vInt.assign(0, 1);
	vInt2.assign(vInt.begin(), vInt.begin()+1);
	try
	{
		printf_s("%d", vInt.at(0));
		printf_s("vInt2[0]=%d\n", vInt2.at(0));
	}
	catch (const std::exception& e)
	{
		printf_s("%s", e.what());
	}
	vInt2[0] = 2;
	try
	{
		printf_s("vInt2[0]=%d\n", vInt2.at(0));
	}
	catch (const std::exception& e)
	{
		printf_s("%s", e.what());
	}
	vInt2.push_back(3);
	printf_s("vInt2.size=%d\n", vInt2.size());
	vector<int>::iterator it = vInt2.begin();
	for (int i = 0; it+i != vInt2.end(); i++)
	{
		//int n = i;
		//it=it+i;
		printf("vInt2[%d]=%d,",i,*(it+i));
	}
	//vector<int>::iterator i = vInt2.begin()+1;
	//printf_s("%d,", vInt2.begin());
	//printf_s("%d", vInt2.end());
	//printf_s("%d", *i);
}



int main()
{
	int test=0;
	//testvector();
	//test3();
	scanf_s("%d", &test);
	return 0;
}

