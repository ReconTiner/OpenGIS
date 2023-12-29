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

struct Pixel    //�ŵ�����
{
	double x, y, z;
	int Num;
	bool flag;
};
struct List //��������
{
	Pixel* pixel;
	List* next;
};
struct Line //�����α�
{
	Pixel* pixel1;  //�����α�һ�˵�
	Pixel* pixel2;  //�����α���һ�˵�
	Pixel* pixel3;  //�����α����Զ���
	bool flag;
};
struct Linelist //�����α߱�
{
	Line* line;
	Linelist* next;
	Linelist() { line = new Line; }
};
struct Triangle //�����α�
{
	Line* line1;
	Line* line2;
	Line* line3;
	Triangle* next;
};

bool JudgeDis(Pixel* pt1, Pixel* pt2, Pixel* node) {//�Ƚ�������node��pt2��pt1�ľ���
	return (pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y) < (pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y);
}

bool JudgePoint(Pixel* pt1, Pixel* pt2, Pixel* pt3, Pixel* node) {//�Ƚ�������node pt3��pt1 pt2���ɽǵĴ�С
	double dist11 = sqrt((pt1->x - node->x) * (pt1->x - node->x) + (pt1->y - node->y) * (pt1->y - node->y));
	double dist12 = sqrt((pt2->x - node->x) * (pt2->x - node->x) + (pt2->y - node->y) * (pt2->y - node->y));
	double dist12_3 = sqrt((pt1->x - pt2->x) * (pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y));
	double dist21 = sqrt((pt1->x - pt3->x) * (pt1->x - pt3->x) + (pt1->y - pt3->y) * (pt1->y - pt3->y));
	double dist22 = sqrt((pt3->x - pt2->x) * (pt3->x - pt2->x) + (pt3->y - pt2->y) * (pt3->y - pt2->y));
	return ((pow(dist11, 2) + pow(dist12, 2) - pow(dist12_3, 2)) / (2 * dist11 * dist12) < (pow(dist21, 2) + pow(dist22, 2) - pow(dist12_3, 2)) / (2 * dist21 * dist22)); //�����жϽǶ�
}

bool JudgeSameSide(double x1, double x2, double y1, double y2, double x3, double y3, Pixel* pixeltmp) {//�жϸý���Ƿ���ñ�������������һ���ͬ��
	//return ((pt2->y - pt1->y)*pixeltmp->x + (pt1->x - pt2->x)*pixeltmp->y + (pt2->x*pt1->y - pt1->x * pt2->y))*((pt2->y - pt1->y)*pt3->x + (pt1->x - pt2->x)*pt3->y + (pt2->x*pt1->y - pt1->x * pt2->y)) >= 0;
	return 	((y2 - y1) * pixeltmp->x + (x1 - x2) * pixeltmp->y + (x2 * y1 - x1 * y2)) * ((y2 - y1) * x3 + (x1 - x2) * y3 + (x2 * y1 - x1 * y2)) >= 0;
}

bool isInLineList(Pixel* pt1, Pixel* pt2, Line* linetemp) {//�жϱ�pt1 pt2�Ƿ���linetemp��ͬ
	return (pt1 == linetemp->pixel1 && pt2 == linetemp->pixel2) || (pt2 == linetemp->pixel1 && pt1 == linetemp->pixel2);
}

void BuildLine(Pixel* pt1, Pixel* pt2, Pixel* pt3, Linelist*& linenode) {//����ǰ�߼���linenode��������
	linenode->line->pixel1 = pt1;
	linenode->line->pixel2 = pt2;
	linenode->line->pixel3 = pt3;
	linenode->line->flag = false;
	linenode->next = NULL;
}

void AddLine(Linelist*& linelast, Linelist* linenode) {//����������ӱ�
	linelast->next = linenode;
	linelast = linenode;
}

void getPoint3(List*& node, Pixel*& pt3, Pixel* pt1, Pixel* pt2) {//������֪pt1 pt2 ��node������Ѱ�����pt3
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
			//�Ƚϵ�ǰ���pixel��pt3˭������--ѡȡ�н����ĵ�
			if (JudgePoint(pt1, pt2, pt3, node->pixel))
				pt3 = node->pixel;
		}
		node = node->next;
	}
}

void setLineFlag(Linelist*& linetemp, Pixel*& pt1, Pixel*& pt2, Pixel*& pt3, Line*& ln, Linelist*& linelast, bool& flag)
{//����linetemp���бߣ���pt1 pt3���ڱ�ln��flag��Ϊtrue���������������еı�����linelast
	while (linetemp != NULL)
	{
		if (isInLineList(pt1, pt3, linetemp->line)) {
			linetemp->line->flag = true;
			flag = true;
			ln = linetemp->line;//��ʼ�������α�ln2
			break;
		}
		linetemp = linetemp->next;
	}
	if (!flag)//�ñ߲����������ı�������
	{

		Linelist* linenode = new Linelist();

		linenode->line->pixel1 = pt3;
		linenode->line->pixel2 = pt1;
		linenode->line->pixel3 = pt2;
		linenode->line->flag = false;
		linenode->next = NULL;

		//�����������ı�����
		linelast->next = linenode;
		linelast = linenode;

		ln = linenode->line;//��ʼ�������α�ln2

	}
}

void BuildFirstTri(const List* list, Triangle*& tglnode, Linelist*& linehead, Linelist*& linelast, Line*& ln1, Line*& ln2, Line*& ln3, Pixel*& pt1, Pixel*& pt2, Pixel*& pt3)
{
	pt1 = list->pixel;//�������е��׵�
	//����pt1�����pt2
	pt2 = list->next->pixel;//���������еĵڶ���

	List* node;
	node = list->next->next;
	while (node != NULL)
	{
		if (JudgeDis(pt1, pt2, node->pixel))
			pt2 = node->pixel;
		node = node->next;
	}

	//�ҵ�������
	//���³�ʼ��nodeΪ�����һ���next
	node = list->next;
	getPoint3(node, pt3, pt1, pt2);

	//�������㹹��������
	Linelist* linenode;
	//������ln1
	linenode = new Linelist();
	BuildLine(pt1, pt2, pt3, linenode);
	ln1 = linenode->line;

	//���ñ������ͷβָ��
	linehead = linelast = linenode;

	//������ln2
	linenode = new Linelist();
	BuildLine(pt2, pt3, pt1, linenode);
	ln2 = linenode->line;
	//���ñ������βָ��
	AddLine(linelast, linenode);

	//������ln3
	linenode = new Linelist();
	BuildLine(pt3, pt1, pt2, linenode);
	ln3 = linenode->line;
	//���ñ������βָ��
	AddLine(linelast, linenode);

	//first Triangle

	//����������
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
	linetmp = linehead;//�������������ͷָ��
	while (linetmp != NULL)
	{
		if (linetmp->line->flag == true)//�ñ��Ѿ�����������
		{
			linetmp = linetmp->next;
			continue;
		}
		//��ȡ��ǰ�ߡ���ǰ�ߵĽ��pt1 pt2 ���ñ������������еĵ�������(x3,y3)
		ln1 = linetmp->line;//��ʼ�������α�pt1 pt2 --ln1

		pt1 = linetmp->line->pixel1;
		pt2 = linetmp->line->pixel2;
		//��ǰ�����ε������������
		x1 = linetmp->line->pixel1->x;
		y1 = linetmp->line->pixel1->y;
		x2 = linetmp->line->pixel2->x;
		y2 = linetmp->line->pixel2->y;
		x3 = linetmp->line->pixel3->x;
		y3 = linetmp->line->pixel3->y;

		pixeltmp = list;//��ȡ�������ͷָ��
		//Ѱ����һ���pt3
		//getPoint3(pixeltmp, pt3, pt1, pt2);//���������������ɸ���
		pt3 = NULL;

		while (pixeltmp != NULL)
		{
			if (pixeltmp->pixel == pt1 || pixeltmp->pixel == pt2)
			{
				pixeltmp = pixeltmp->next;
				continue;
			}
			//�жϸý���Ƿ���ñ�������������һ���ͬ��--��������
			if (JudgeSameSide(x1, x2, y1, y2, x3, y3, pixeltmp->pixel))
			{
				pixeltmp = pixeltmp->next;
				continue;
			}

			//Ϊpt3��ֵ
			if (pt3 == NULL)pt3 = pixeltmp->pixel;
			else//�������н�㣬Ѱ�����pt3
			{
				if (JudgePoint(pt1, pt2, pt3, pixeltmp->pixel))
					pt3 = pixeltmp->pixel;
			}
			pixeltmp = pixeltmp->next;
		}

		//��ʼ�������α�ln2
		if (pt3 != NULL)
		{
			linetemp = linehead;
			flag = false;
			//�������бߣ���pt1 pt3���ڱ�ln2 flag��Ϊtrue
			setLineFlag(linetemp, pt1, pt2, pt3, ln2, linelast, flag);


			//��ʼ�������α�ln3
			linetemp = linehead;
			flag = false;
			//�������бߣ���pt2 pt3���ڱ�ln3 flag��Ϊtrue
			setLineFlag(linetemp, pt2, pt1, pt3, ln3, linelast, flag);

			//����������
			tglnode = new Triangle;
			tglnode->line1 = ln1;
			tglnode->line2 = ln2;
			tglnode->line3 = ln3;
			tglnode->next = NULL;
			//��������������
			tgllast->next = tglnode;
			tgllast = tglnode;
		}
		//��ǰ�ߵ�flag��Ϊtrue������������������ɣ�
		linetmp->line->flag = true;
		linetmp = linetmp->next;
	}
}

//�����ǳ����й��ڽ����Ĳ��֣�
//���ܣ�   �ø����������������ݣ��齨Delaunay������������
//�����������������list;
//����������������������������ε�ַ

Triangle* CreateDelaunayTIN(List* list)
{
	//�齨��һ��������

	//List *node;
	Pixel* pt1, * pt2, * pt3;//��������������
	bool flag;
	Line* ln1, * ln2, * ln3;//�����ε�������
	Linelist* linehead, * linelast;// *linenode ;//�������ı�����
	Triangle* tglhead, * tglnode, * tgllast;//������������������

	BuildFirstTri(list, tglnode, linehead, linelast, ln1, ln2, ln3, pt1, pt2, pt3);

	//��һ�������ι�����ϣ���������������
	tglhead = tgllast = tglnode;

	BuildTIN(list, linehead, linelast, ln1, ln2, ln3, pt1, pt2, pt3, flag, tglnode, tgllast);

	Triangle* TIN;
	//����������
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
	//�˴���ɾ������㼯�����ļ�
	//pts��Ϣ:vector<MyPoint>
	//MyPoint���ݽṹ����
	//ptNum:������ x:���ͶӰ����X y:���ͶӰ����Y z:��ĸ߳�Z
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