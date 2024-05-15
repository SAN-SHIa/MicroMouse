#include<bits/stdc++.h>
#include <graphics.h> //ͼ�ο� 
#define MaxSize 500
using namespace std; 
/*�Թ�ϵ��*/
int maze[30][30];
int MazeColor=0;

/*Ч��ָ��*/
float ValidLength=0;
float TotalLength=0;

/*BFS����*/
typedef struct Node {
    int i;
    int j;
    int next;
    Node *pre;
} Node, *List;
/*��ʼBFS���*/
void InitNode(List &bfsp, int x, int y) {
	bfsp=new Node;
    bfsp->i = x;
    bfsp->j = y;
    bfsp->next = -1;
    bfsp->pre = NULL;
}
/*����BFSȫ�ֱ���*/
List bfsStart, bfsEnd, bfsp;
int x=0, y=0;

/*����DFS·�����*/
struct Point 
{
	int i;          //��ǰ�к�
	int j;          //��ǰ�к�
	int next;       //��һ�����ߵķ����
};
/*��ʼ��DFS���*/
InitPoint(Point&p,int x,int y)
{
	p.i=x;
	p.j=y;
	p.next=-999;
}
/*��ʼ��DFSջ�붨����ز���*/
struct Stack 
{
	Point pointstack[500];
	int top;
	Stack() {
		top = -1;
	}
    //��ջ
	void Push(Point p) {
		if (top == MaxSize - 1)
			return;
		top++;
		pointstack[top] = p;
	}
	//��ջ
	void Pop(Point& p) {
		if (top == -1)
			return;
		else 
		{
			p = pointstack[top];
			top--;
		}
	}
	//ȡջ��Ԫ��
	void getTop(Point& p) {
		if (top == -1)
			return;
		p = pointstack[top];
	}
	//�ж�ջ�Ƿ�Ϊ��
	bool isEmpty() {
		if (top == -1)
			return true;
		else
			return false;
	}
	//�޸�next����ֵ
	void setNext(int next) {
		pointstack[top].next = next;
	}
};
/*����DFSȫ�ֱ���*/
Stack s;
Point pStart,pEnd,p;

/*��txt�ļ��л�ȡ��ͼ��ʽ*/
void ReadMaze(int maze[30][30], const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return; // ���ļ�ʧ�ܣ���������
    }
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            fscanf(file, "%d", &maze[i][j]);
        }
    }
    fclose(file);
}

/*���Ƶ�ͼ����*/
void DrawPath()
{
	setbkcolor(BLACK);// ���õ�ǰ��ͼ����ɫ 
	if(1==MazeColor)setfillcolor(EGERGB(71, 71, 255));
	else if(2==MazeColor)setfillcolor(EGERGB(139, 37, 0));
	else if(3==MazeColor)setfillcolor(EGERGB(162, 205, 9));
	bar(0, 0, 600, 600);
	for (int n = 0; n <30; n++){
		for (int m = 0; m <30; m++) {
			if (1==maze[n][m] ) {
				if(1==MazeColor)setfillcolor(EGERGB(28, 134, 238));
				else if(2==MazeColor)setfillcolor(EGERGB(205, 38, 38));
				else if(3==MazeColor)setfillcolor(EGERGB(0, 139, 0));
			} 
			else if (0==maze[n][m]) {
				if(1==MazeColor)setfillcolor(EGERGB(144, 194, 246));
				else if(2==MazeColor)setfillcolor(EGERGB(255, 240, 245));
				else if(3==MazeColor)setfillcolor(EGERGB(192, 255, 62));
			} 
			else if (2==maze[n][m]) {
				if(1==MazeColor)setfillcolor(EGERGB(145, 193, 62));
				else if(2==MazeColor)setfillcolor(EGERGB(255, 255, 0));
				else if(3==MazeColor)setfillcolor(EGERGB(250, 250, 210));
			} 
			else if (3==maze[n][m]) {
				if(1==MazeColor)setfillcolor(EGERGB(25, 25, 112));
				else if(2==MazeColor)setfillcolor(EGERGB(75, 0, 130));
				else if(3==MazeColor)setfillcolor(EGERGB(105, 139,34));
			}
			bar(m * 20 + 1, n * 20 + 1, m * 20 + 19, n * 20 + 19);
		}
	}
} 

/*�����ƶ�����*/
void DrawCharacter(int x,int y)
{
		PIMAGE img;
		img = newimage();
		getimage(img, "mouse.png");	//���ļ��л�ȡͼ��
		putimage( y * 20, x * 20, 20, 20, img, 0, 0, getwidth(img), getheight(img));
};

/*����DFS�յ��ͼ��*/
void DFSDrawEnd()
{
	PIMAGE img;
	img = newimage();
	getimage(img, "star.png");	//���ļ��л�ȡͼ��
	putimage_withalpha(NULL, img, pEnd.i*20-5, pEnd.j*20-7);
	delimage(img);	//����ͼ���ͷ��ڴ� 
}

/*����BFS�յ��ͼ��*/
void BFSDrawEnd()
{
	PIMAGE img;
	img = newimage();
	getimage(img, "star.png");	//���ļ��л�ȡͼ��
	putimage_withalpha(NULL, img, bfsEnd->j*20-7,bfsEnd->i*20-5);
	delimage(img);	//����ͼ���ͷ��ڴ�
}

/*DFS��ͼ*/
void DFSDrawMap()
{
	initgraph(900, 600); // ��ʼ������ʾһ�����ڣ����ڿ�Ϊ600���أ���Ϊ800����
	DrawPath(); 
	DFSDrawEnd();
	DrawCharacter(x,y);
	setbkcolor(YELLOW);
	setcolor(RED);
	setfont(42, 0, "΢���ź�");
	xyprintf(650, 50, "DFS|MAZE %d",MazeColor);
	setbkcolor(WHITE);
	setcolor(BLACK);
	setfont(22, 0, "����");
	xyprintf(655, 150, "TotalLength:%.0f",TotalLength);
	xyprintf(655, 200, "ValidLength:%.0f",ValidLength);
	setbkcolor(YELLOW);
	setcolor(RED);
	xyprintf(655, 250, "Efficiency:%.2f",1.0*ValidLength/TotalLength);
	setbkcolor(WHITE);
	return;
}

/*BFS��ͼ*/
void BFSDrawMap()
{
	initgraph(900, 600); // ��ʼ������ʾһ�����ڣ����ڿ�Ϊ600���أ���Ϊ800����
	DrawPath(); 
	BFSDrawEnd();
	DrawCharacter(x,y);
	setbkcolor(YELLOW);
	setcolor(RED);
	setfont(42, 0, "΢���ź�");
	xyprintf(650, 50, "BFS|MAZE %d",MazeColor);
	setbkcolor(WHITE);
	setcolor(BLACK);
	setfont(22, 0, "����");
	xyprintf(655, 150, "TotalLength:%.0f",TotalLength);
	xyprintf(655, 200, "ValidLength:%.0f",ValidLength);
	setbkcolor(YELLOW);
	setcolor(RED);
	xyprintf(655, 250, "Efficiency:%.2f",1.0*ValidLength/TotalLength);
	setbkcolor(WHITE);
	return;
}

/*DFS����*/
int DFS(int (*maze)[30])
{
	//��������յ�����
	InitPoint(pStart,1,1);
	InitPoint(pEnd,15,15);
	//InitPoint(pEnd,28,27);
	InitPoint(p,1,1);
	//�Թ���ڽ�ջ
	s.Push(pStart);			
	maze[pStart.i][pStart.j] = 2;
	int i, j, next;
	///��ջ��Ԫ�ز�Ϊ��ʱ
	while (!s.isEmpty()) 
	{
		s.getTop(p);//ȡջ��Ԫ��
		next = p.next;
		if (p.i== pEnd.j&&p.j== pEnd.i) 
		{
			return 1;
		}
		while (next < 4) //ֱ���ҵ���һ�����ߵķ���
		{	
			next++;
			i = p.i;
			j = p.j;
			switch (next)//��һ��������
			{ 
				case 0:
					i = i -1;
					break;//�� 
				case 1: 
					j = j + 1;
					break;//�� 
				case 2:
					i = i + 1;
					break;//�� 
				case 3:
					j = j - 1;
					break;//�� 
			}
			if (maze[i][j] == 0)break;			//�ҵ���һ�����ߵ�����λ�ã����˳�ѭ��
		}
		cout<<next<<endl;
		//�ж��Ƿ���Լ��� 
		if (next < 4) {	
			maze[i][j] = 2;//��¼�߹��÷���
			TotalLength+=1;
			ValidLength+=1; 
			s.setNext(next);//�޸�ԭջ��Ԫ�ص�nextֵ
			p.i = i;
			p.j = j;
			p.next = -1;
			s.Push(p);
		} 
		else 
		{					//next == 4	 ��ʾ�õ�û���κ�·������
			s.Pop(p);				//��ջ
			maze[p.i][p.j] = 3; 	//�ڵ�ͼ�ϼ�¼��ǰ���Ϊ�����
			ValidLength-=1; 
		}
		s.getTop(p);	//ȡջ��Ԫ��
		cout<<"i:"<<p.i<<" i:"<<p.j<<endl;
		x = p.i;
		y = p.j;
		cout<<"x:"<<x<<" y:"<<y<<endl;
		cleardevice();	//���� 
		DFSDrawMap();	//����ͼ 
		Sleep(50);
	}
	return 0;			//��ʾû�п���·��,����0
}

/*��ӡBFS����·��*/
void PrintPath(List& np)
{
    stack<List> s;
    List cur = np;
    List tmp = NULL;
    cout<<"ready"<<endl;
    while (cur)  // ��������Ϊ��� cur �Ƿ�Ϊ NULL
    {
        s.push(cur);
        cur = cur->pre;  // ���� cur Ϊǰ���ڵ�
    }
    while (!s.empty())
    {
        tmp = s.top();  // ����ջ��Ԫ�������л���
        s.pop();
        int x = tmp->i;
        int y = tmp->j;
        cout<<"x:"<<x<<"y:"<<y<<endl;
        cout<<"maze[i][j]"<<maze[x][y]<<endl;
        maze[x][y]=2; 
        ValidLength+=1; 
        BFSDrawMap();
        Sleep(20);
        // ��������������ӳٻ�����ͣһ��ʱ�䣬�Ա�۲�ÿһ�����ƶ�
    }
}

/*BFS����*/
void BFS(int (*maze)[30]) {
    InitNode(bfsStart, 1, 1);
    InitNode(bfsEnd, 15, 15);
    InitNode(bfsp, 1, 1);
    cout<<bfsStart->i<<endl;
    queue<List> q;
    
    q.push(bfsStart);
    
    maze[bfsStart->i][bfsStart->j] = 3;

    while (!q.empty()) {
        List cur = q.front();
        q.pop();

        int dir[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int k = 0; k < 4; ++k) {
            int ni = cur->i + dir[k][0];
            int nj = cur->j + dir[k][1];
            if (maze[ni][nj] == 0) {
            	
                maze[ni][nj] = 3;
                TotalLength+=1; 
                // ���������ƶ��켣
                x = ni;
                y = nj;
                List np = new Node;
                InitNode(np, ni, nj);
            	np->pre=cur;
                q.push(np);
                if (x== bfsEnd->i &&y== bfsEnd->j) 
				{
					x=bfsEnd->i;
					y=bfsEnd->j;
		            BFSDrawMap();  // ��������·��
		            Sleep(50);
		            
		            PrintPath(np);
		            return;
		        }
		        cleardevice();	//���� ,��Ȼ��һ��ҳ��ı�����Ȼ�� 
		        BFSDrawMap();
                Sleep(50);
            }
        }
    }
}

/*DFS��ͼѡ��*/
void DFSChooseMap()
{
	//��ʼ����ͼѡ����� 
	initgraph(900, 600); 
	PIMAGE choosepage;
	choosepage = newimage();
	getimage(choosepage, "DFSmaze.jpg");	//���ļ��л�ȡͼ��
	putimage( 0, 0, 900, 600, choosepage, 0, 0,getwidth(choosepage), getheight(choosepage));
	//��¼��ȡ�������һ�������Ϣ
	mouse_msg msgRecord = { 0 };
	bool redraw = true;
	for (; is_run(); delay_fps(20))
	{
		//����Ƿ��������Ϣ,û��������(�����������)
		while (mousemsg()){
			msgRecord = getmouse();
			redraw = true;
		}
		//��⵽����ź� 
		if (redraw) 
		{
			//��ʱ����xyprintf(40, 20, "���λ�ã�  x   = %4d     y = %4d",msgRecord.x, msgRecord.y);
			redraw = false;
			//������ֵ
			ValidLength=0;
			TotalLength=0;
			
			if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 40 && msgRecord.x < 264 && msgRecord.y > 107 && msgRecord.y < 471)
			{
				ReadMaze(maze, "maze1.txt");
				MazeColor=1;
				DFS(maze);//���й��� 
				setcolor(GREEN);
				xyprintf(610, 500,"���س���������ҳ");
				getch();
				closegraph();
				return; 
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 282 && msgRecord.x < 507 && msgRecord.y > 112 && msgRecord.y < 464)
			{
				ReadMaze(maze, "maze2.txt");
				MazeColor=2;
				DFS(maze);//���й��� 
				setcolor(GREEN);
				xyprintf(610, 500,"���س���������ҳ");
				getch();
				closegraph();
				return; 
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 526 && msgRecord.x < 746 && msgRecord.y > 108 && msgRecord.y < 470)
			{
				ReadMaze(maze, "maze3.txt");
				MazeColor=3;	
				DFS(maze);

				setcolor(GREEN);
				xyprintf(610, 500,"���س���������ҳ");
				
				getch();
				closegraph();
				return; 
			}
		}
	}
}
/*BFS��ͼѡ��*/
void BFSChooseMap()
{
	initgraph(900, 600); 
	PIMAGE choosepage;
	choosepage = newimage();
	getimage(choosepage, "BFSmaze.jpg");
	putimage( 0, 0, 900, 600, choosepage, 0, 0,getwidth(choosepage), getheight(choosepage));
	//��¼��ȡ�������һ�������Ϣ
	mouse_msg msgRecord = { 0 };
	bool redraw = true;
	for (; is_run(); delay_fps(20))
	{
		//��ȡ�����Ϣ 
		while (mousemsg())
		{
			msgRecord = getmouse();
			redraw = true;
		}
		//��⵽����ź� 
		if (redraw) 
		{
			//��ʱ����xyprintf(40, 20, "���λ�ã�  x   = %4d     y = %4d",msgRecord.x, msgRecord.y);
			redraw = false;
			//������ֵ
			ValidLength=0;
			TotalLength=0;
			if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 40 && msgRecord.x < 264 && msgRecord.y > 107 && msgRecord.y < 471)
			{
				ReadMaze(maze, "maze1.txt");
				MazeColor=1;
				BFS(maze);
				setcolor(GREEN);
				xyprintf(610, 500,"���س���������ҳ");			
				getch();
				closegraph();
				return; 
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 282 && msgRecord.x < 507 && msgRecord.y > 112 && msgRecord.y < 464)
			{
				ReadMaze(maze, "maze2.txt");
				MazeColor=2;
				BFS(maze);
				setcolor(GREEN);
				xyprintf(610, 500,"���س���������ҳ");
				getch();
				closegraph();
				return; 
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 526 && msgRecord.x < 746 && msgRecord.y > 108 && msgRecord.y < 470)
			{
				ReadMaze(maze, "maze3.txt");
				MazeColor=3;	
				BFS(maze);
				setcolor(GREEN);
				xyprintf(610, 500,"���س���������ҳ");
				getch();
				closegraph();
				return; 
			}
		}
	}
}

/*��ҳ���ڴ���*/
void StartPage()
{
	// ��ʼ����ҳ���� 
	initgraph(800, 600); 
	//initgraph(800, 600,INIT_WITHLOGO); 
	PIMAGE startimg;
	startimg = newimage();
	getimage(startimg, "startpage.jpg");
	putimage( 0, 0, 800, 600, startimg, 0, 0,getwidth(startimg), getheight(startimg)); 
	//��ӡ�����Ϣ 
	setcaption("DFS��BFS�㷨���Թ�̽���е�Ч�ʿ��ӻ�");	//���ô��ڱ���
	setbkcolor(WHITE);
	setcolor(BLACK);
	setfont(30, 0, "����");
	//��ȡ�����Ϣ 
	mouse_msg msgRecord = { 0 };
	bool redraw = true;
	int leftMargin = 80;
	int topMargin=200;
	for (; is_run(); delay_fps(20))
	{
		//����Ƿ��������Ϣ,û��������(�����������)
		while (mousemsg())
		{
			msgRecord = getmouse();
			redraw = true;
		}
		//��������ź�ʱ 
		if (redraw) 
		{
			redraw = false;
			//��ʱ����xyprintf(150, 40, "���λ�ã�  x   = %4d     y = %4d",msgRecord.x, msgRecord.y);
			if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 30 && msgRecord.x < 370 && msgRecord.y > 310 && msgRecord.y < 412){
				DFSChooseMap();//���н���������س������Զ��ص���ҳ 
				StartPage();
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 442 && msgRecord.x < 782 && msgRecord.y > 316 && msgRecord.y < 423){
				BFSChooseMap(); 
				StartPage();
			}
		}
	}
} 

/*������*/
int main()
{
	StartPage(); 
	return 0;
}

