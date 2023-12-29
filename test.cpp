#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

class MyPoint
{
public:
	double x;
	double y;
	double z;
	int ptNum;
	MyPoint(double x, double y, double z, int ptNum)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->ptNum = ptNum;
	}
};

struct Pixel    //脚点数据
{
	double x, y, z;
	int Num;
	bool flag;
};
struct List //数据链表
{
	Pixel* pixel;
	List* next;
};
struct Line //三角形边
{
	Pixel* pixel1;  //三角形边一端点
	Pixel* pixel2;  //三角形边另一端点
	Pixel* pixel3;  //三角形边所对顶点
	bool flag;
};
struct Linelist //三角形边表
{
	Line* line;
	Linelist* next;
	Linelist() { line = new Line; }
};
struct Triangle //三角形表
{
	Line* line1;
	Line* line2;
	Line* line3;
	Triangle* next;
};

bool JudgeDis(Pixel* pt1, Pixel* pt2, Pixel* node) {//比较链表中node与pt2到pt1的距离
	return (pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y) < (pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y);
}

bool JudgePoint(Pixel* pt1, Pixel* pt2, Pixel* pt3, Pixel* node) {//比较链表中node pt3与pt1 pt2所成角的大小
	double dist11 = sqrt((pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y));
	double dist12 = sqrt((pt2->x - node->x) * (pt2->x - node->x) + (pt2->y - node->y) * (pt2->y - node->y));
	double dist12_3 = sqrt((pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y));
	double dist21 = sqrt((pt1->x - pt3->x) * (pt1->x - pt3->x) + (pt1->y - pt3->y) * (pt1->y - pt3->y));
	double dist22 = sqrt((pt3->x - pt2->x) * (pt3->x - pt2->x) + (pt3->y - pt2->y) * (pt3->y - pt2->y));
	return ((pow(dist11, 2) + pow(dist12, 2) - pow(dist12_3, 2)) / (2 * dist11 * dist12) < (pow(dist21, 2) + pow(dist22, 2) - pow(dist12_3, 2)) / (2 * dist21 * dist22)); //余弦判断角度
}

bool JudgeSameSide(double x1, double x2, double y1, double y2, double x3, double y3, Pixel* pixeltmp) {//判断该结点是否与该边所属三角形另一结点同侧
	//return ((pt2->y - pt1->y)*pixeltmp->x + (pt1->x - pt2->x)*pixeltmp->y + (pt2->x*pt1->y - pt1->x * pt2->y))*((pt2->y - pt1->y)*pt3->x + (pt1->x - pt2->x)*pt3->y + (pt2->x*pt1->y - pt1->x * pt2->y)) >= 0;
	return 	((y2 - y1) * pixeltmp->x + (x1 - x2) * pixeltmp->y + (x2 * y1 - x1 * y2)) * ((y2 - y1) * x3 + (x1 - x2) * y3 + (x2 * y1 - x1 * y2)) >= 0;
}

bool isInLineList(Pixel* pt1, Pixel* pt2, Line* linetemp) {//判断边pt1 pt2是否与linetemp相同
	return (pt1 == linetemp->pixel1 && pt2 == linetemp->pixel2) || (pt2 == linetemp->pixel1 && pt1 == linetemp->pixel2);
}

void BuildLine(Pixel* pt1, Pixel* pt2, Pixel* pt3, Linelist*& linenode) {//将当前边加入linenode边链表中
	linenode->line->pixel1 = pt1;
	linenode->line->pixel2 = pt2;
	linenode->line->pixel3 = pt3;
	linenode->line->flag = false;
	linenode->next = NULL;
}

void AddLine(Linelist*& linelast, Linelist* linenode) {//边链表中添加边
	linelast->next = linenode;
	linelast = linenode;
}

void getPoint3(List*& node, Pixel*& pt3, Pixel* pt1, Pixel* pt2) {//根据已知pt1 pt2 从node链表中寻找最佳pt3
	pt3 = NULL;
	while (node != NULL)
	{
		if (node->pixel == pt1 || node->pixel == pt2)
		{
			node = node->next;
			continue;
		}

		if (pt3 == NULL)
		{
			pt3 = node->pixel;
		}
		else
		{
			//比较当前结点pixel与pt3谁更合适--选取夹角最大的点
			if (JudgePoint(pt1, pt2, pt3, node->pixel))
				pt3 = node->pixel;
		}
		node = node->next;
	}
}

void setLineFlag(Linelist*& linetemp, Pixel*& pt1, Pixel*& pt2, Pixel*& pt3, Line*& ln, Linelist*& linelast, bool& flag)
{//遍历linetemp所有边，将pt1 pt3所在边ln的flag置为true，并更新三角网中的边链表linelast
	while (linetemp != NULL)
	{
		if (isInLineList(pt1, pt3, linetemp->line)) {
			linetemp->line->flag = true;
			flag = true;
			ln = linetemp->line;//初始化三角形边ln2
			break;
		}
		linetemp = linetemp->next;
	}
	if (!flag)//该边不在三角网的边链表中
	{

		Linelist* linenode = new Linelist();

		linenode->line->pixel1 = pt3;
		linenode->line->pixel2 = pt1;
		linenode->line->pixel3 = pt2;
		linenode->line->flag = false;
		linenode->next = NULL;

		//更新三角网的边链表
		linelast->next = linenode;
		linelast = linenode;

		ln = linenode->line;//初始化三角形边ln2

	}
}

void BuildFirstTri(const List* list, Triangle*& tglnode, Linelist*& linehead, Linelist*& linelast, Line*& ln1, Line*& ln2, Line*& ln3, Pixel*& pt1, Pixel*& pt2, Pixel*& pt3)
{
	pt1 = list->pixel;//数据链中的首点
	//找离pt1最近点pt2
	pt2 = list->next->pixel;//数据链表中的第二点

	List* node;
	node = list->next->next;
	while (node != NULL)
	{
		if (JudgeDis(pt1, pt2, node->pixel))
			pt2 = node->pixel;
		node = node->next;
	}

	//找第三个点
	//重新初始化node为链表第一点的next
	node = list->next;
	getPoint3(node, pt3, pt1, pt2);

	//根据三点构建三角形
	Linelist* linenode;
	//构建边ln1
	linenode = new Linelist();
	BuildLine(pt1, pt2, pt3, linenode);
	ln1 = linenode->line;

	//设置边链表的头尾指针
	linehead = linelast = linenode;

	//构建边ln2
	linenode = new Linelist();
	BuildLine(pt2, pt3, pt1, linenode);
	ln2 = linenode->line;
	//设置边链表的尾指针
	AddLine(linelast, linenode);

	//构建边ln3
	linenode = new Linelist();
	BuildLine(pt3, pt1, pt2, linenode);
	ln3 = linenode->line;
	//设置边链表的尾指针
	AddLine(linelast, linenode);

	//first Triangle

	//构建三角形
	tglnode = new Triangle;
	tglnode->line1 = ln1;
	tglnode->line2 = ln2;
	tglnode->line3 = ln3;
	tglnode->next = NULL;
}

void BuildTIN(List*& list, Linelist*& linehead, Linelist*& linelast, Line*& ln1, Line*& ln2, Line*& ln3, Pixel*& pt1, Pixel*& pt2, Pixel*& pt3, bool& flag, Triangle*& tglnode, Triangle*& tgllast)
{
	Linelist* linetmp, * linetemp;
	List* pixeltmp;
	double x1, y1, x2, y2, x3, y3;
	linetmp = linehead;//三角网边链表的头指针
	while (linetmp != NULL)
	{
		if (linetmp->line->flag == true)//该边已经在三角网中
		{
			linetmp = linetmp->next;
			continue;
		}
		//获取当前边、当前边的结点pt1 pt2 及该边所属三角形中的第三个点(x3,y3)
		ln1 = linetmp->line;//初始化三角形边pt1 pt2 --ln1

		pt1 = linetmp->line->pixel1;
		pt2 = linetmp->line->pixel2;
		//当前三角形的三个结点坐标
		x1 = linetmp->line->pixel1->x;
		y1 = linetmp->line->pixel1->y;
		x2 = linetmp->line->pixel2->x;
		y2 = linetmp->line->pixel2->y;
		x3 = linetmp->line->pixel3->x;
		y3 = linetmp->line->pixel3->y;

		pixeltmp = list;//获取结点链表头指针
		//寻找下一结点pt3
		//getPoint3(pixeltmp, pt3, pt1, pt2);//有新增条件，不可复用
		pt3 = NULL;

		while (pixeltmp != NULL)
		{
			if (pixeltmp->pixel == pt1 || pixeltmp->pixel == pt2)
			{
				pixeltmp = pixeltmp->next;
				continue;
			}
			//判断该结点是否与该边所属三角形另一结点同侧--新增条件
			if (JudgeSameSide(x1, x2, y1, y2, x3, y3, pixeltmp->pixel))
			{
				pixeltmp = pixeltmp->next;
				continue;
			}

			//为pt3赋值
			if (pt3 == NULL)pt3 = pixeltmp->pixel;
			else//遍历所有结点，寻找最佳pt3
			{
				if (JudgePoint(pt1, pt2, pt3, pixeltmp->pixel))
					pt3 = pixeltmp->pixel;
			}
			pixeltmp = pixeltmp->next;
		}

		//初始化三角形边ln2
		if (pt3 != NULL)
		{
			linetemp = linehead;
			flag = false;
			//遍历所有边，将pt1 pt3所在边ln2 flag置为true
			setLineFlag(linetemp, pt1, pt2, pt3, ln2, linelast, flag);


			//初始化三角形边ln3
			linetemp = linehead;
			flag = false;
			//遍历所有边，将pt2 pt3所在边ln3 flag置为true
			setLineFlag(linetemp, pt2, pt1, pt3, ln3, linelast, flag);

			//构建三角形
			tglnode = new Triangle;
			tglnode->line1 = ln1;
			tglnode->line2 = ln2;
			tglnode->line3 = ln3;
			tglnode->next = NULL;
			//更新三角形链表
			tgllast->next = tglnode;
			tgllast = tglnode;
		}
		//当前边的flag置为true（遍历构建三角网完成）
		linetmp->line->flag = true;
		linetmp = linetmp->next;
	}
}

//以下是程序中关于建网的部分：
//功能：   用给定的数据链表数据，组建Delaunay不规则三角网
//输入参数：数据链表list;
//输出参数：不规则三角网首三角形地址

Triangle* CreateDelaunayTIN(List* list)
{
	//组建第一个三角形

	//List *node;
	Pixel* pt1, * pt2, * pt3;//三角形三个顶点
	bool flag;
	Line* ln1, * ln2, * ln3;//三角形的三条边
	Linelist* linehead, * linelast;// *linenode ;//三角网的边链表
	Triangle* tglhead, * tglnode, * tgllast;//三角网的三角形链表

	BuildFirstTri(list, tglnode, linehead, linelast, ln1, ln2, ln3, pt1, pt2, pt3);

	//第一个三角形构建完毕！更新三角形链表
	tglhead = tgllast = tglnode;

	BuildTIN(list, linehead, linelast, ln1, ln2, ln3, pt1, pt2, pt3, flag, tglnode, tgllast);

	Triangle* TIN;
	//创建三角网
	TIN = tglhead;

	return TIN;
}

void printList(List* list) {
	cout << "List:\n";
	while (list != NULL) {
		cout << list->pixel->Num << ": " << list->pixel->x << "," << list->pixel->y << endl;
		list = list->next;
	}
}

int main()
{
	//此处已删除读入点集数据文件
	//pts信息:vector<MyPoint>
	//MyPoint数据结构如下
	//ptNum:点的序号 x:点的投影坐标X y:点的投影坐标Y z:点的高程Z
	vector<MyPoint> pts;
	pts.push_back(MyPoint(1.0, 2.0, 0.0, 0));
	pts.push_back(MyPoint(1.2, -1, 0.0, 1));
	pts.push_back(MyPoint(-0.8, -0.9, 0.0, 2));
	pts.push_back(MyPoint(-1.2, 1.0, 0.0, 3));

	List* ListHead, * ListNode, * ListLast;
	ListHead = ListNode = ListLast = NULL;
	for (int i = 0; i < pts.size(); i++) {
		Pixel* p = new Pixel();
		p->x = pts.at(i).x;
		p->y = pts.at(i).y;
		p->z = pts.at(i).z;
		p->Num = pts.at(i).ptNum;
		List* Node = new List();
		Node->pixel = p;
		Node->next = NULL;
		if (i == 0) {
			ListHead = Node;
			ListNode = ListHead;
		}
		else {
			ListNode->next = Node;
			ListNode = ListNode->next;
			ListLast = ListNode;
		}

	}

	printList(ListHead);
	Triangle* tri = new Triangle();

	tri = CreateDelaunayTIN(ListHead);
	int i = 0;
	while (tri != NULL) {
		cout << i << ": " << " e1:  p1: " << tri->line1->pixel1->Num << " p2: " << tri->line1->pixel2->Num << " e2:  p1: " << tri->line2->pixel1->Num << " p2: " << tri->line2->pixel2->Num
			<< " e3:  p1: " << tri->line3->pixel1->Num << " p2: " << tri->line3->pixel2->Num << "\t" << endl;
		tri = tri->next;
		i++;
	}

	return 0;
}