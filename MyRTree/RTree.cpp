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
	while (p->branchs.size() > 0 && p->branchs[0].nextNode != NULL)
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
		if (b.nextNode != NULL)
		{
			b.nextNode->parent = p;
			b.nextNode->pos = p->branchs.size() - 1;
		}
	}
	else
	{
		vector<Branch> tempB(p->branchs);
		tempB.push_back(b);
		Node* newChildNode = NewNode();
		p->branchs.assign(tempB.begin(), tempB.begin() + minBranchs + 1);
		p->count = minBranchs + 1;

		newChildNode->branchs.assign(tempB.begin() + minBranchs + 1, tempB.end());
		newChildNode->count = maxBranchs - minBranchs;
		if (newChildNode->branchs.at(0).nextNode != NULL)
		{
			for (int i = 0; i<newChildNode->branchs.size(); i++)
			{
				newChildNode->branchs.at(i).nextNode->parent = newChildNode;
				newChildNode->branchs.at(i).nextNode->pos = i;
			}
		}
		
		if (p->parent == NULL)
		{
			root = NewNode();
			/*
			Node* childNode1 = NewNode();
			Node* childNode2 = NewNode();
			//简单分裂
			childNode1->branchs.assign(tempB.begin(), tempB.begin()+minBranchs+1);
			childNode1->count = minBranchs + 1;
			childNode2->branchs.assign(tempB.begin() + minBranchs + 1, tempB.end());			
			childNode2->branchs.push_back(tempB.back());
			childNode2->count = maxBranchs - minBranchs;
			Insert(root, GetMBR(childNode1));
			Insert(root, GetMBR(childNode2));
			*/
			Insert(root, GetMBR(p));
			Insert(root, GetMBR(newChildNode));
		}
		else
		{
			p->parent->branchs[p->pos] = GetMBR(p);
			Insert(p->parent, GetMBR(newChildNode));
		}
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
	int y1 = r1.y1 > r2.y1 ? r1.y1 : r2.y1;
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
	//printf_s("UpdateMBR");
	p->parent->branchs[p->pos].rect = AddRect(p->parent->branchs[p->pos].rect, r);
	if (p->parent->parent != NULL)
	{
		r = p->parent->branchs[p->pos].rect;
		p = p->parent;
		UpdateMBR(p, r);
	}
}

Branch RTree::GetMBR(Node* p)
{
	vector<Branch>::iterator it=p->branchs.begin();
	Rect r = (*it).rect;
	for (it++; it!=p->branchs.end(); it++)
	{
		r = AddRect(r, (*it).rect);
	}
	return Branch{ r, p };
}

void RTree::PrintRTree()
{
	printf_s("根节点：");
	/*if (root->count == 0)
	{
		printf_s("空树");
	}
	else
	{
		queue<Node*> q;

	}*/
	queue<Node*> q;
	q.push(root);
	while (!q.empty())
	{
		Node* p = q.front();
		q.pop();
		vector<Branch>::iterator it;
		for (it = p->branchs.begin(); it != p->branchs.end(); it++)
		{
			if ((*it).nextNode != NULL)
			{
				q.push((*it).nextNode);
			}
			printf_s("rect:%d,%d,%d,%d.", (*it).rect.x0, (*it).rect.y0, 
				(*it).rect.x1, (*it).rect.y1);

		}
		if(p->parent!=NULL)
		{
			printf_s("存在p,");
		}
		printf_s("pos=%d,",p->pos);
		printf_s("\n");
	}
}

