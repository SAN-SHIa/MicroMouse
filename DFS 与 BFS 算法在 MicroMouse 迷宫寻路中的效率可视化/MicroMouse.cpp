#include<bits/stdc++.h>
#include <graphics.h> //图形库 
#define MaxSize 500
using namespace std; 
/*迷宫系数*/
int maze[30][30];
int MazeColor=0;

/*效率指数*/
float ValidLength=0;
float TotalLength=0;

/*BFS定义*/
typedef struct Node {
    int i;
    int j;
    int next;
    Node *pre;
} Node, *List;
/*初始BFS结点*/
void InitNode(List &bfsp, int x, int y) {
	bfsp=new Node;
    bfsp->i = x;
    bfsp->j = y;
    bfsp->next = -1;
    bfsp->pre = NULL;
}
/*定义BFS全局变量*/
List bfsStart, bfsEnd, bfsp;
int x=0, y=0;

/*定义DFS路径结点*/
struct Point 
{
	int i;          //当前行号
	int j;          //当前列号
	int next;       //下一步可走的方向号
};
/*初始化DFS结点*/
InitPoint(Point&p,int x,int y)
{
	p.i=x;
	p.j=y;
	p.next=-999;
}
/*初始化DFS栈与定义相关操作*/
struct Stack 
{
	Point pointstack[500];
	int top;
	Stack() {
		top = -1;
	}
    //进栈
	void Push(Point p) {
		if (top == MaxSize - 1)
			return;
		top++;
		pointstack[top] = p;
	}
	//出栈
	void Pop(Point& p) {
		if (top == -1)
			return;
		else 
		{
			p = pointstack[top];
			top--;
		}
	}
	//取栈顶元素
	void getTop(Point& p) {
		if (top == -1)
			return;
		p = pointstack[top];
	}
	//判断栈是否为空
	bool isEmpty() {
		if (top == -1)
			return true;
		else
			return false;
	}
	//修改next的数值
	void setNext(int next) {
		pointstack[top].next = next;
	}
};
/*定义DFS全局变量*/
Stack s;
Point pStart,pEnd,p;

/*从txt文件中获取地图样式*/
void ReadMaze(int maze[30][30], const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return; // 打开文件失败，函数返回
    }
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            fscanf(file, "%d", &maze[i][j]);
        }
    }
    fclose(file);
}

/*绘制地图界面*/
void DrawPath()
{
	setbkcolor(BLACK);// 设置当前绘图背景色 
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

/*绘制移动人物*/
void DrawCharacter(int x,int y)
{
		PIMAGE img;
		img = newimage();
		getimage(img, "mouse.png");	//从文件中获取图像
		putimage( y * 20, x * 20, 20, 20, img, 0, 0, getwidth(img), getheight(img));
};

/*设置DFS终点的图标*/
void DFSDrawEnd()
{
	PIMAGE img;
	img = newimage();
	getimage(img, "star.png");	//从文件中获取图像
	putimage_withalpha(NULL, img, pEnd.i*20-5, pEnd.j*20-7);
	delimage(img);	//销毁图像，释放内存 
}

/*设置BFS终点的图标*/
void BFSDrawEnd()
{
	PIMAGE img;
	img = newimage();
	getimage(img, "star.png");	//从文件中获取图像
	putimage_withalpha(NULL, img, bfsEnd->j*20-7,bfsEnd->i*20-5);
	delimage(img);	//销毁图像，释放内存
}

/*DFS画图*/
void DFSDrawMap()
{
	initgraph(900, 600); // 初始化，显示一个窗口，窗口宽为600像素，长为800像素
	DrawPath(); 
	DFSDrawEnd();
	DrawCharacter(x,y);
	setbkcolor(YELLOW);
	setcolor(RED);
	setfont(42, 0, "微软雅黑");
	xyprintf(650, 50, "DFS|MAZE %d",MazeColor);
	setbkcolor(WHITE);
	setcolor(BLACK);
	setfont(22, 0, "宋体");
	xyprintf(655, 150, "TotalLength:%.0f",TotalLength);
	xyprintf(655, 200, "ValidLength:%.0f",ValidLength);
	setbkcolor(YELLOW);
	setcolor(RED);
	xyprintf(655, 250, "Efficiency:%.2f",1.0*ValidLength/TotalLength);
	setbkcolor(WHITE);
	return;
}

/*BFS画图*/
void BFSDrawMap()
{
	initgraph(900, 600); // 初始化，显示一个窗口，窗口宽为600像素，长为800像素
	DrawPath(); 
	BFSDrawEnd();
	DrawCharacter(x,y);
	setbkcolor(YELLOW);
	setcolor(RED);
	setfont(42, 0, "微软雅黑");
	xyprintf(650, 50, "BFS|MAZE %d",MazeColor);
	setbkcolor(WHITE);
	setcolor(BLACK);
	setfont(22, 0, "宋体");
	xyprintf(655, 150, "TotalLength:%.0f",TotalLength);
	xyprintf(655, 200, "ValidLength:%.0f",ValidLength);
	setbkcolor(YELLOW);
	setcolor(RED);
	xyprintf(655, 250, "Efficiency:%.2f",1.0*ValidLength/TotalLength);
	setbkcolor(WHITE);
	return;
}

/*DFS搜索*/
int DFS(int (*maze)[30])
{
	//设置起点终点坐标
	InitPoint(pStart,1,1);
	InitPoint(pEnd,15,15);
	//InitPoint(pEnd,28,27);
	InitPoint(p,1,1);
	//迷宫入口进栈
	s.Push(pStart);			
	maze[pStart.i][pStart.j] = 2;
	int i, j, next;
	///当栈中元素不为空时
	while (!s.isEmpty()) 
	{
		s.getTop(p);//取栈顶元素
		next = p.next;
		if (p.i== pEnd.j&&p.j== pEnd.i) 
		{
			return 1;
		}
		while (next < 4) //直到找到下一个可走的方向
		{	
			next++;
			i = p.i;
			j = p.j;
			switch (next)//下一处的坐标
			{ 
				case 0:
					i = i -1;
					break;//上 
				case 1: 
					j = j + 1;
					break;//右 
				case 2:
					i = i + 1;
					break;//下 
				case 3:
					j = j - 1;
					break;//左 
			}
			if (maze[i][j] == 0)break;			//找到下一个可走的相邻位置，则退出循环
		}
		cout<<next<<endl;
		//判断是否可以继续 
		if (next < 4) {	
			maze[i][j] = 2;//记录走过该方块
			TotalLength+=1;
			ValidLength+=1; 
			s.setNext(next);//修改原栈顶元素的next值
			p.i = i;
			p.j = j;
			p.next = -1;
			s.Push(p);
		} 
		else 
		{					//next == 4	 表示该点没有任何路径可走
			s.Pop(p);				//出栈
			maze[p.i][p.j] = 3; 	//在地图上记录当前结点为死结点
			ValidLength-=1; 
		}
		s.getTop(p);	//取栈顶元素
		cout<<"i:"<<p.i<<" i:"<<p.j<<endl;
		x = p.i;
		y = p.j;
		cout<<"x:"<<x<<" y:"<<y<<endl;
		cleardevice();	//清屏 
		DFSDrawMap();	//画地图 
		Sleep(50);
	}
	return 0;			//表示没有可走路径,返回0
}

/*打印BFS回溯路径*/
void PrintPath(List& np)
{
    stack<List> s;
    List cur = np;
    List tmp = NULL;
    cout<<"ready"<<endl;
    while (cur)  // 更新条件为检查 cur 是否为 NULL
    {
        s.push(cur);
        cur = cur->pre;  // 更新 cur 为前驱节点
    }
    while (!s.empty())
    {
        tmp = s.top();  // 弹出栈顶元素来进行绘制
        s.pop();
        int x = tmp->i;
        int y = tmp->j;
        cout<<"x:"<<x<<"y:"<<y<<endl;
        cout<<"maze[i][j]"<<maze[x][y]<<endl;
        maze[x][y]=2; 
        ValidLength+=1; 
        BFSDrawMap();
        Sleep(20);
        // 可以在这里添加延迟或者暂停一段时间，以便观察每一步的移动
    }
}

/*BFS搜索*/
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
                // 绘制人物移动轨迹
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
		            BFSDrawMap();  // 绘制最终路径
		            Sleep(50);
		            
		            PrintPath(np);
		            return;
		        }
		        cleardevice();	//清屏 ,不然上一个页面的背景任然在 
		        BFSDrawMap();
                Sleep(50);
            }
        }
    }
}

/*DFS地图选择*/
void DFSChooseMap()
{
	//初始化地图选择界面 
	initgraph(900, 600); 
	PIMAGE choosepage;
	choosepage = newimage();
	getimage(choosepage, "DFSmaze.jpg");	//从文件中获取图像
	putimage( 0, 0, 900, 600, choosepage, 0, 0,getwidth(choosepage), getheight(choosepage));
	//记录读取到的最后一条鼠标消息
	mouse_msg msgRecord = { 0 };
	bool redraw = true;
	for (; is_run(); delay_fps(20))
	{
		//检查是否有鼠标消息,没有则跳过(避免程序阻塞)
		while (mousemsg()){
			msgRecord = getmouse();
			redraw = true;
		}
		//检测到鼠标信号 
		if (redraw) 
		{
			//暂时隐藏xyprintf(40, 20, "鼠标位置：  x   = %4d     y = %4d",msgRecord.x, msgRecord.y);
			redraw = false;
			//重置数值
			ValidLength=0;
			TotalLength=0;
			
			if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 40 && msgRecord.x < 264 && msgRecord.y > 107 && msgRecord.y < 471)
			{
				ReadMaze(maze, "maze1.txt");
				MazeColor=1;
				DFS(maze);//运行过程 
				setcolor(GREEN);
				xyprintf(610, 500,"按回车键返回主页");
				getch();
				closegraph();
				return; 
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 282 && msgRecord.x < 507 && msgRecord.y > 112 && msgRecord.y < 464)
			{
				ReadMaze(maze, "maze2.txt");
				MazeColor=2;
				DFS(maze);//运行过程 
				setcolor(GREEN);
				xyprintf(610, 500,"按回车键返回主页");
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
				xyprintf(610, 500,"按回车键返回主页");
				
				getch();
				closegraph();
				return; 
			}
		}
	}
}
/*BFS地图选择*/
void BFSChooseMap()
{
	initgraph(900, 600); 
	PIMAGE choosepage;
	choosepage = newimage();
	getimage(choosepage, "BFSmaze.jpg");
	putimage( 0, 0, 900, 600, choosepage, 0, 0,getwidth(choosepage), getheight(choosepage));
	//记录读取到的最后一条鼠标消息
	mouse_msg msgRecord = { 0 };
	bool redraw = true;
	for (; is_run(); delay_fps(20))
	{
		//获取鼠标信息 
		while (mousemsg())
		{
			msgRecord = getmouse();
			redraw = true;
		}
		//检测到鼠标信号 
		if (redraw) 
		{
			//暂时隐藏xyprintf(40, 20, "鼠标位置：  x   = %4d     y = %4d",msgRecord.x, msgRecord.y);
			redraw = false;
			//重置数值
			ValidLength=0;
			TotalLength=0;
			if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 40 && msgRecord.x < 264 && msgRecord.y > 107 && msgRecord.y < 471)
			{
				ReadMaze(maze, "maze1.txt");
				MazeColor=1;
				BFS(maze);
				setcolor(GREEN);
				xyprintf(610, 500,"按回车键返回主页");			
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
				xyprintf(610, 500,"按回车键返回主页");
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
				xyprintf(610, 500,"按回车键返回主页");
				getch();
				closegraph();
				return; 
			}
		}
	}
}

/*主页窗口创建*/
void StartPage()
{
	// 初始化主页窗口 
	initgraph(800, 600); 
	//initgraph(800, 600,INIT_WITHLOGO); 
	PIMAGE startimg;
	startimg = newimage();
	getimage(startimg, "startpage.jpg");
	putimage( 0, 0, 800, 600, startimg, 0, 0,getwidth(startimg), getheight(startimg)); 
	//打印相关信息 
	setcaption("DFS与BFS算法在迷宫探索中的效率可视化");	//设置窗口标题
	setbkcolor(WHITE);
	setcolor(BLACK);
	setfont(30, 0, "宋体");
	//获取鼠标信息 
	mouse_msg msgRecord = { 0 };
	bool redraw = true;
	int leftMargin = 80;
	int topMargin=200;
	for (; is_run(); delay_fps(20))
	{
		//检查是否有鼠标消息,没有则跳过(避免程序阻塞)
		while (mousemsg())
		{
			msgRecord = getmouse();
			redraw = true;
		}
		//当鼠标有信号时 
		if (redraw) 
		{
			redraw = false;
			//暂时隐藏xyprintf(150, 40, "鼠标位置：  x   = %4d     y = %4d",msgRecord.x, msgRecord.y);
			if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 30 && msgRecord.x < 370 && msgRecord.y > 310 && msgRecord.y < 412){
				DFSChooseMap();//运行结束后输入回车按键自动回到主页 
				StartPage();
			}
			else if(msgRecord.is_left() && msgRecord.is_down() && msgRecord.x > 442 && msgRecord.x < 782 && msgRecord.y > 316 && msgRecord.y < 423){
				BFSChooseMap(); 
				StartPage();
			}
		}
	}
} 

/*主函数*/
int main()
{
	StartPage(); 
	return 0;
}

