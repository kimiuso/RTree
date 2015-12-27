#pragma once
#include <vector>
using namespace std;//ку╥ф3

struct Rect;
struct Node;
struct Branch;

class RTree 
{
protected:
	int maxBranchs;
	int minBranchs;
public:
	Node* root;

public:
	RTree(int);
	void Insert(Rect);
	Rect AddRect(Rect, Rect);
	long AreaOfRect(Rect);
	void UpdateMBR(Node*, Rect);

private:
	Node* NewNode();
};

struct Rect
{
	int x0, y0, x1, y1;
};

struct Node
{
	vector<Branch> branchs;
	Node* parent;
	int pos;
	int count;
	int level;
};

struct Branch
{
	Rect rect;
	Node* nextNode;
};