#include <iostream>
#include <fstream>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#include <easyx.h>
#include <graphics.h>
#pragma comment(lib,"Winmm.lib")


//宏定义部分
#define IMGWIDTH 20
#define IMGHEIGHT 20
#define WIDTH 1100
#define HEIGHT 650
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define HEADUP    "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\headup.jpg"
#define HEADDOWN  "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\headdown.jpg"
#define HEADLEFT  "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\headleft.jpg"
#define HEADRIGHT "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\headright.jpg"
#define BODYLR "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\bodylr.jpg"
#define BODYUD "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\bodyud.jpg"
#define TAILL "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\taill.jpg"
#define TAILR "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\tailr.jpg"
#define TAILU "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\tailu.jpg"
#define TAILD "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\taild.jpg"
#define WALL "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\wall.jpg"
#define FOOD "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\food.jpg"
#define GAMEOVER "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\gameover.jpg"
#define SNAKE "E:\\Mine\\Codes\\Cpp\\RetroSnaker\\snake.jpg"
#define L 60   //左
#define U 80   //上
#define R 900  //右
#define D 600  //下

using namespace std;

IMAGE wallimg; //一种图像类，来自EasyX图形库 IMAGE相当于int 声明了一个参数储存图片用的
int score = 0, grade = 1, rec[4];//rec是自己定义的数组，rec是起的名字
bool fight = false;

DWORD start, stop;

class Food;

class BodyNode {

private:


	IMAGE img;
	BodyNode() {} //用于赋值的构造函数
	friend class Body; //Body类可以访问BodyNode的private对象
	friend bool check(Body& snake, Food& food);

public: //public部分只有两个构造函数

	int x; //横坐标
	int y; //纵坐标
	BodyNode* next; //指向下一节身体

	BodyNode(const char* str, int X, int Y) {
		x = X;
		y = Y;
		loadimage(&img, _T(str));
		next = NULL;
	}

	BodyNode(BodyNode& bodynode) {
		x = bodynode.x;
		y = bodynode.y;
		next = bodynode.next;
		img = bodynode.img;
	}
};

class Body {
private:
	BodyNode * head;
	BodyNode* tail;
	int length;

public:
	bool alive = true;
	friend bool check(Body& snake, Food& food);
	//用来检查蛇是否吃到了食物，并避免食物产生在蛇身体所占的部位

	Body() { //构造函数，新建一个链表，包含头尾和一个体节
		head = new BodyNode(HEADRIGHT, L + (10 * (IMGWIDTH)), U + (10 * (IMGHEIGHT)));
		//默认头向右
		head->next = new BodyNode(BODYLR, head->x - head->img.getwidth(), head->y);
		//第一体节的横坐标为头的横坐标减去头的宽度，纵坐标和头一样
		head->next->next = new BodyNode(TAILR, head->next->x - head->img.getwidth(), head->y);
		//尾巴的初始位置在第一体节后
		tail = head->next->next;
		//让第一体节的next指针指向尾巴
		length = 2;
	}

	int Length() { return length; }

	bool IsDead() { //死亡条件的判断
		BodyNode* p = head->next;
		while (p != NULL) {//p从第一体节开始遍历整个链表，逐个确认体节的位置是否和头重合
			if (head->x == p->x && head->y == p->y || !alive) {
				return true;
			}
			p = p->next;
		}
		return false;
	}

	int Move(int dir) {     //1 上 2下  3左 4 右
		BodyNode* p = head->next;
		int tempx = head->x, tempy = head->y, temp_x, temp_y;
		while (p != tail->next) {
			temp_x = p->x;
			temp_y = p->y; //把p的坐标存在两个变量中
			p->x = tempx;
			p->y = tempy; //把上一次读取的坐标存进来
			tempx = temp_x;
			tempy = temp_y; //把p的坐标存留下来
			p = p->next; //指向下一节
		}
		if (dir == 1) {
			loadimage(&(head->img), _T(HEADUP));
			//_T宏可以把一个引号引起来的字符串，根据你的环境设置，使得编译器会根据编译目标环境选择合适的（Unicode还是ANSI）字符处理方式
			head->y = head->next->y - head->img.getheight();
			//头的纵坐标为第一体节的纵坐标减去图片的高度
		}
		if (dir == 2) {
			loadimage(&(head->img), _T(HEADDOWN));
			head->y = head->next->y + head->img.getheight();
		}
		if (dir == 3) {
			loadimage(&(head->img), _T(HEADLEFT));
			head->x = head->next->x - head->img.getwidth();
		}
		if (dir == 4) {
			loadimage(&(head->img), _T(HEADRIGHT));
			head->x = head->next->x + head->img.getwidth();
		}

		if ((head->x + head->img.getwidth()) > R) { head->x = L; }
		if (head->x < L) { head->x = R - head->img.getwidth(); }
		if ((head->y + head->img.getheight()) > D) { head->y = U; }
		if (head->y < U) { head->y = D - head->img.getheight(); }
		//这是能穿墙？

		p = head;
		while (p->next != tail) { //寻找尾巴前的最后一个体节
			p = p->next;
		}
		if (p->x - tail->x > 0) { loadimage(&(tail->img), _T(TAILR)); }
		if (p->x - tail->x < 0) { loadimage(&(tail->img), _T(TAILL)); }
		if (p->y - tail->y > 0) { loadimage(&(tail->img), _T(TAILD)); }
		if (p->y - tail->y < 0) { loadimage(&(tail->img), _T(TAILU)); }
		//判断尾巴朝向并加载相应图片
		return 0;
	}

	int Grow() {
		BodyNode* p = head;
		while (p->next != tail) {
			p = p->next;
		}
		tail->img = p->img;
		tail->next = new BodyNode(&(TAILL[0]), tail->x - (p->x - tail->x), tail->y - (p->y - tail->y));

		tail = tail->next; //尾巴上+1节，判断尾巴朝向并加载相应图片
		if (p->x - tail->x > 0) { loadimage(&(tail->img), _T(TAILR)); }
		if (p->x - tail->x < 0) { loadimage(&(tail->img), _T(TAILL)); }
		if (p->y - tail->y > 0) { loadimage(&(tail->img), _T(TAILD)); }
		if (p->y - tail->y < 0) { loadimage(&(tail->img), _T(TAILU)); }

		tail->next = NULL;
		length++;
		return 0;
	}

	int Show() { //显示图像
		BodyNode* p = head;
		while (p != NULL) {
			putimage(p->x, p->y, &(p->img));
			p = p->next;
		}
		return 0;
	}
};

class Food {
private:
	int x;
	int y;
	IMAGE rat; //位置和图片信息
public:
	friend bool check(Body& snake, Food& food);
	Food() {
		x = 400;
		y = 300;
		loadimage(&(rat), _T(FOOD));
	}
	int givefood(Body& snake) {
		srand((unsigned)time(0)); //利用现在的时间初始化随机数种子
		while (check(snake, *this) == true) {
			//如果蛇吃到食物或者新生成的食物落在了蛇身体的范围内
			x = 0; y = 0;
			while (!(x>L && y>U)) {
				//保证食物落在地图内
				x = rand() % 900;
				y = rand() % 600;
			}
			x = x - (x % 20);
			y = y - (y % 20);
			//也许是考虑墙的宽度？
		}
		return 0;
	}

	int Move(int dir) {     //1 上 2下  3左 4 右
		if (dir == 1) {
			if (y > 80){
				y -= 20;
			}
			
		}
		if (dir == 2) {
			if (y < 580){
				y += 20;
			}
		}
		if (dir == 3) {
			if (x > 60){
				x -= 20;
			}
		}
		if (dir == 4) {
			if (x < 880){
				x += 20;
			}
		}

		return 0;
	}

	int Show() {
		putimage(x, y, &(rat));
		return 0;
	}
};

int showgame(Body& snake, Food food) {

	cleardevice(); //用当前背景色清空屏幕，并将当前点移至 (0, 0)
	BeginBatchDraw();
	//开始批量绘图。执行后，任何绘图操作都将暂时不输出到屏幕上，直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出。
	setbkcolor(RGB(0, 0, 0));    //设置背景色
	setcolor(YELLOW);         //设置绘图色
	outtextxy((WIDTH / 2) - 80, 10, "贪吃蛇");
	outtextxy(L + 300, U - IMGHEIGHT - 20, "按空格 暂停");
	outtextxy(R + IMGWIDTH + 20, U + IMGHEIGHT + 20, "最高记录：");
	
	char rec1[10];//,rec2[10],rec3[10];
	for (int index = 0; index <= 2; index++) {
		sprintf_s(rec1, "%d", rec[index]);
		outtextxy(R + IMGWIDTH + 20, U + IMGHEIGHT + 20 + (index + 1) * 30, rec1);
	} //输出最高的3次得分

	int i, j;
	for (i = L - IMGWIDTH; i <= R; i = i + IMGWIDTH) { //搬砖码墙
		j = U - IMGHEIGHT;
		putimage(i, j, &(wallimg));
	}
	for (j = U; j <= D; j = j + IMGHEIGHT) {
		i = L - IMGWIDTH;
		putimage(i, j, &(wallimg));
	}
	for (j = U; j <= D; j = j + IMGHEIGHT) {
		i = R;
		putimage(i, j, &(wallimg));
	}
	for (i = L - IMGWIDTH; i <= R; i = i + IMGWIDTH) {
		j = D;
		putimage(i, j, &(wallimg));
	}

	outtextxy(L - 10, U - IMGHEIGHT - 20, "得分:");
	char s[10];
	sprintf_s(s, "%d", score);
	outtextxy(L + 60, U - IMGHEIGHT - 20, s);


	if (fight){
		outtextxy(L + 600, U - IMGHEIGHT - 20, "倒计时：");
		char tt[1000];
		sprintf_s(tt, "%f", 50-(stop - start)*1.0/1000);
		outtextxy(L + 700, U - IMGHEIGHT - 20, tt);
	}

	outtextxy(L + 120, U - IMGHEIGHT - 20, "长度:");
	char l[10];
	sprintf_s(l, "%d", snake.Length());
	outtextxy(L + 180, U - IMGHEIGHT - 20, l);

	snake.Show();

	food.Show();
	FlushBatchDraw(); //在屏幕上显示
	int clock = grade;
	if (clock == 0){ clock = 2; }
	Sleep(50 * clock); //延时50*grade毫秒

	return 0;
}

bool check(Body& snake, Food& food) {
	BodyNode* pl = snake.head;
	bool flag = false;
	while (pl != snake.tail->next) {

		if ((pl->x == food.x) && (pl->y == food.y)) { flag = true; break; }

		pl = pl->next;
	}
	return flag;
}

DWORD WINAPI Fun1Proc(LPVOID IpParameter) //BGM???
{
	mciSendString("play yeah.mp3 repeat", NULL, 0, NULL);//播放
	return 0;
}

int menu() {
	//cleardevice();
	outtextxy((WIDTH / 2) - 80, 120, "贪吃蛇");
	outtextxy((WIDTH / 2) - 180, 230, "按↑和↓选择难度");
	outtextxy((WIDTH / 2) - 180, 250, "按空格键确定选择");
	return 0;
}

int record() {
	fstream f1("record.txt");   //打开文件，若文件不存在就创建它
	if (!f1) return -1;
	//int a=0,b=0,c=0;
	f1 >> rec[0] >> rec[1] >> rec[2];
	f1.close();
return 0;
}

int saverecord() {
	system("del record.txt"); //发出一个dos命令
	ofstream f1("record.txt");   //打开文件，若文件不存在就创建它
	if (!f1) return -1;
	int flag = 1;
	int temp;
	rec[3] = score;
	while (flag == 1) {
		flag = 0;
		for (int i = 0; i <= 2; i++) {
			if (rec[i] < rec[i + 1]) {
				flag = 1;
				temp = rec[i];
				rec[i] = rec[i + 1];
				rec[i + 1] = temp;
			}
		} //使rec[0]>rec[1]>rec[2]
	}

	f1 << rec[0] << "\n" << rec[1] << "\n" << rec[2];

	f1.close(); //关闭文件
	if (score != rec[3]) {
		settextstyle(50, 0, _T("宋体"));
		outtextxy(WIDTH / 2 - (50 * 5), U + 40, "恭喜你创造了新纪录！");
	}
	return 0;
}

int main() {
	while (1){
		grade = 3;
		bool snakeWin = false;
		for (int i = 0; i <= 3; i++) {
			rec[i] = 0;
		}
		record();
		IMAGE snakeimg;

		HANDLE hThread1;
		hThread1 = CreateThread(NULL, 0, Fun1Proc, NULL, 0, NULL);
		char key;
		int movekey = 4;
		int movekeyold = movekey;
		initgraph(WIDTH, HEIGHT); //地图绘制
		char temp = 0, choose;
		while (temp != ' ') {
			cleardevice();
			if (_kbhit()) { //检查当前是否有键盘输入，若有则返回一个非0值，否则返回0
				choose = _getch(); //从控制台无回显地取一个字符（不需要回车）
				fflush(stdin); //强迫将缓冲区内的数据写回stdin指定的文件中
				switch (choose) {
				case UP: {
					grade++; //难度选择
				}; break;
				case DOWN: {
					grade--;
				};
				}
			}
			if (grade == 4) { grade = 3; }
			if (grade == -1) { grade = 0; } //光标移到顶端之后无法再上移，底端类似
			outtextxy(420, 350, "简单");
			outtextxy(420, 380, "中等");
			outtextxy(420, 410, "难");
			outtextxy(420, 600, "对战模式");

			menu();

			outtextxy(300, 300, "请选择难度：");
			if (grade == 3) {
				outtextxy(390, 350, "→");
			}
			if (grade == 2) {
				outtextxy(390, 380, "→");
			}
			if (grade == 1) {
				outtextxy(390, 410, "→");
			}
			if (grade == 0) {
				fight = true;
				outtextxy(390, 600, "→");
			}
			FlushBatchDraw();
			temp = _getch();
		}
		loadimage(&(wallimg), _T(WALL));
		Body snake;
		Food food;
		if (fight){
			for (int i = 0; i < 8; i++){ snake.Grow(); }
			start = timeGetTime();
		}
		showgame(snake, food);
		while (snake.IsDead() == false) {
			while (snake.IsDead() == false && (!_kbhit())) { //没有死，也没有输入
				if (fight){

					stop = timeGetTime();
					if (((stop - start)*1.0 / 1000) > 50){
						snake.alive = false;
					}
				}
				if (movekey == 1 && movekeyold == 2 || movekey == 2 && movekeyold == 1 || movekey == 3 && movekeyold == 4 || movekey == 4 && movekeyold == 3) {
					movekey = movekeyold;
				}
				movekeyold = movekey;
				snake.Move(movekey);
				showgame(snake, food);
				if (check(snake, food) == true) {
					if (!fight){
						mciSendString("play yeah.wav", NULL, 0, NULL); //播放
						cout << '\a'; //输出一个特殊符号
						score += 10;
						food.givefood(snake); //投放食物
						snake.Grow();
						snake.Grow();
						snake.Grow();
					}
					else{
						mciSendString("play yeah.wav", NULL, 0, NULL); //播放
						cout << '\a'; //输出一个特殊符号
						snakeWin = true;
						snake.alive = false;
					}
				}
			}
			if (_kbhit()) {
				key = _getch();
				if (fight){
					if (key == 119){
						food.Move(1);
					}
					else if (key == 115){
						food.Move(2);
					}
					else if (key == 97){
						food.Move(3);
					}
					if (key == 100){
						food.Move(4);
					}
				}
				switch (key) {
				case UP: {movekey = 1; }; break;
				case DOWN: {movekey = 2; }; break;
				case LEFT: {movekey = 3; }; break;
				case RIGHT: {movekey = 4; }; break;
				case ' ': { //暂停
					int t = 0;
					while (t == 0) {
						if (_kbhit()) {
							key = _getch();
							if (key == ' ') {
								t = 1;
							}
						}
					}
				};
				}
			}
		}
		CloseHandle(hThread1);
		BeginBatchDraw();
		showgame(snake, food);
		IMAGE gameoverimg;
		loadimage(&(gameoverimg), _T(GAMEOVER));
		putimage(WIDTH / 2 - (gameoverimg.getwidth() / 2), HEIGHT / 2 - (gameoverimg.getheight() / 2), &(gameoverimg));
		if (fight){
			if (snakeWin){
				outtextxy(450, 400, "贪吃蛇获胜！");
			}
			else{
				outtextxy(450, 400, "老鼠成功逃跑了！");
			}
		}
		outtextxy(450, 500, "按【y】重新开始");
		outtextxy(450, 550, "按【n】退出游戏");
		saverecord();
		FlushBatchDraw();
		saverecord();
		while (true)
		{
			key = _getch();
			if (key == 121){
				fight = false;
				snakeWin = false;
				cleardevice();
				break;
			}
			else if (key == 110){
				return 0;
			}
		}
	}
}