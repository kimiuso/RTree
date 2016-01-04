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
	//��ʱ��p��ָ��Ҷ�ڵ��ָ��
	Branch b = { rect,NULL };
	Insert(p, b);
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
		//�ڵ����
		vector<Branch> tempB(p->branchs);
		tempB.push_back(b);
		if (b.nextNode != NULL) { b.nextNode->parent = p; }
		//�ҳ������Խ�Զ���������Σ���Ϊ����
		vector<Branch>::iterator it=tempB.begin();
		int top=0;
		int right=0;
		int bottom=0;
		int left=0;
		int topPoint[2];
		int rightPoint[2];
		int bottomPoint[2];
		int leftPoint[2];
		int seed1 = 0;
		int seed2 = 1;
		topPoint[0] = (*it).rect.x0 + (*it).rect.x1;
		topPoint[1] = (*it).rect.y0 + (*it).rect.y1;
		rightPoint[0] = topPoint[0];
		rightPoint[1] = topPoint[1];
		bottomPoint[0] = topPoint[0];
		bottomPoint[1] = topPoint[1];
		leftPoint[0] = topPoint[0];
		leftPoint[1] = topPoint[1];		

		for (int i = 1; it + i != tempB.end(); i++)
		{
			int center[2];
			center[0] = (*(it + i)).rect.x0 + (*(it + i)).rect.x1;
			center[1] = (*(it + i)).rect.y0 + (*(it + i)).rect.y1;
			if (center[0] < leftPoint[0]) { left = i; leftPoint[0] = center[0]; leftPoint[1] = center[1]; }
			if (center[0] > rightPoint[0]) { right = i; rightPoint[0] = center[0]; rightPoint[1] = center[1]; }
			if (center[1] < bottomPoint[1]) { bottom = i; bottomPoint[0] = center[0]; bottomPoint[1] = center[1]; }
			if (center[1] > topPoint[1]) { top = i; topPoint[0] = center[0]; topPoint[1] = center[1]; }
		}
		if ((rightPoint[0] - leftPoint[0]) > (topPoint[1] - bottomPoint[1])) { seed1 = left; seed2 = right; }
		else { seed1 = bottom; seed2 = top; }
		//�Ѿ��ҵ�������

		////�����������µĽڵ�
        
		p->branchs.clear();
		p->branchs.push_back(*(it+seed1));
		p->count = 1;
		Node* newChildNode = NewNode();
		newChildNode->branchs.push_back(*(it + seed2));
		newChildNode->count = 1;
		for (int i = 0; it + i != tempB.end(); i++)
		{
			if (i == seed1 || i == seed2) { continue; }
			if (p->count > minBranchs) 
			{
				newChildNode->branchs.push_back(*(it + i));
				newChildNode->count++;
			}
			else if (newChildNode->count > minBranchs)
			{
				p->branchs.push_back(*(it + i));
				p->count++;
			}
			else
			{
				Rect r = AddRect((*(it + i)).rect, GetMBR(p).rect);
				long dif = AreaOfRect(r) - AreaOfRect(GetMBR(p).rect);
				r = AddRect((*(it + i)).rect, GetMBR(newChildNode).rect);
				long dif2 = AreaOfRect(r) - AreaOfRect(GetMBR(newChildNode).rect);
				if (dif > dif2)
				{
					newChildNode->branchs.push_back(*(it + i));
					newChildNode->count++;
				}
				else
				{
					p->branchs.push_back(*(it + i));
					p->count++;
				}
			}
			
		}

		if (newChildNode->branchs.at(0).nextNode != NULL)
		{//����½ڵ㲻��Ҷ�ڵ�
			for (int i = 0; i<newChildNode->branchs.size(); i++)
			{//�������ӽڵ㵽�½ڵ��ָ��
				newChildNode->branchs.at(i).nextNode->parent = newChildNode;
				newChildNode->branchs.at(i).nextNode->pos = i;
			}
			for (int i = 0; i<p->branchs.size(); i++)
			{//���´��ӽڵ㵽�ɽڵ��ָ��
				p->branchs.at(i).nextNode->pos = i;
			}
		}
		
		if (p->parent == NULL)
		{//������ڵ���ѣ�����������һ���µĸ��ڵ㣬�ٲ�����Ѻ�ڵ��Ӧ�ķ�֧
			root = NewNode();
			Insert(root, GetMBR(p));
			Insert(root, GetMBR(newChildNode));
		}
		else
		{//ˢ�¸��ڵ��оɽڵ��Ӧ�ķ�֧���ٲ����½ڵ��Ӧ�ķ�֧
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
	printf_s("���ڵ㣺");
	/*if (root->count == 0)
	{
		printf_s("����");
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
			printf_s("����p,");
		}
		printf_s("pos=%d,",p->pos);
		printf_s("\n");
	}
}

vector<Rect> RTree::Search(Rect r)
{
	if (root->branchs.size() == 0) { return vector<Rect>();}
	return Search(root, r);
	
}

vector<Rect> RTree::Search(Node *p, Rect r)
{
	vector<Rect> result;
	for (int i = 0; i < p->branchs.size(); i++) 
	{
		if (Match(r, p->branchs.at(i).rect))
		{
			/*printf_s("Match rect:%d,%d,%d,%d.\n", p->branchs.at(i).rect.x0, p->branchs.at(i).rect.y0,
				p->branchs.at(i).rect.x1, p->branchs.at(i).rect.y1);*/
			if (p->branchs.at(i).nextNode != NULL)
			{//�������Ҷ�ڵ㣬���������
				vector<Rect> subResult = Search(p->branchs.at(i).nextNode, r);
				if (subResult.size() != 0)
				{
					for (int i = 0; i < subResult.size(); i++)
					{
						result.push_back(subResult.at(i));
					}
				}
			}
			else
			{//Ҷ�ڵ㣬����Ҫ�ҵľ���
				result.push_back(p->branchs.at(i).rect);
			}
		}
	}
	return result;
}

bool RTree::Match(Rect r1, Rect r2)
{
	printf_s("Match rect:%d,%d,%d,%d.  ", r2.x0, r2.y0,
		r2.x1, r2.y1);
	if (r1.x0 < r2.x0)
	{//r1�����
		if (r1.x1 <= r2.x0) { return false; }
		if (r1.y0 >= r2.y1|| r1.y1 <= r2.y0) { return false; }
	}
	else
	{//r2����߻����������ε���߹���
		if (r2.x1 <= r1.x0) { return false; }
		if (r2.y0 >= r1.y1 || r2.y1 <= r1.y0) { return false; }
	}
	printf_s("true\n");
	return true;
}

