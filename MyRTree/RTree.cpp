#include "stdafx.h"

RTree::RTree(int maxBranchs)
{
	root=NewNode();
	(*this).maxBranchs = maxBranchs;
	minBranchs = maxBranchs / 2;
}

void RTree::Insert(Rect rect)
{
	Node* p=root;
	while (p->level != 0)
	{
		Rect r = AddRect(rect, p->branchs[0].rect);
		long dif = AreaOfRect(r) - AreaOfRect(p->branchs[0].rect);
		int pos = 0;
		for (int i = 1; i < p->count; i++)
		{
			r = AddRect(rect, p->branchs[i].rect);
			long dif2= AreaOfRect(r) - AreaOfRect(p->branchs[i].rect);
			if (dif > dif2)
			{
				pos = i;
				dif = dif2;
			}
		}
		p = p->branchs[pos].nextNode;		
	}
	//这时，p是指向叶节点的指针
	Branch b = { rect,NULL };
	Insert(p, b);
	/*
	if (p->count < maxBranchs)
	{
		Branch b = { rect,NULL };
		p->branchs.push_back(b);
		p->count++;
		//更新最小界限矩形
		if (p->parent != NULL)
		{
			UpdateMBR(p, rect);
		}
		
	}
	else
	{//分裂

	}*/
}

void RTree::Insert(Node* p, Branch b)
{
	if (p->count < maxBranchs)
	{
		p->branchs.push_back(b);
		p->count++;
		if (p->parent != NULL)
		{
			UpdateMBR(p, b.rect);
		}
	}
	else
	{

	}
}

Node* RTree::NewNode()
{
	Node* newNode;
	newNode = new Node;
	newNode->parent = NULL;
	newNode->pos = -1;
	newNode->count = 0;
	newNode->level = 0;
	return newNode;
}

Rect RTree::AddRect(Rect r1, Rect r2)
{
	int x0 = r1.x0 < r2.x0 ? r1.x0 : r2.x0;
	int y0 = r1.y0 < r2.y0 ? r1.y0 : r2.y0;
	int x1 = r1.x1 > r2.x1 ? r1.x1 : r2.x1;
	int y1 = r1.y0 > r2.y0 ? r1.y0 : r2.y0;
	Rect r = { x0,y0,x1,y1 };
	return r;
}

long RTree::AreaOfRect(Rect r)
{
	long area = (r.x1 - r.x0)*(r.y1 - r.y0);
	return area;
}

void RTree::UpdateMBR(Node* p, Rect r)
{
	p->parent->branchs[p->pos].rect = AddRect(p->parent->branchs[p->pos].rect, r);
	if (p->parent->parent != NULL)
	{
		r = p->parent->branchs[p->pos].rect;
		p = p->parent;
		UpdateMBR(p, r);
	}
}