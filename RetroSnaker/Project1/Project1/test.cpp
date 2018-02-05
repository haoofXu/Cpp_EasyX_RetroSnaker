#include <iostream>
#include <fstream>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#include <easyx.h>
#include <graphics.h>
#pragma comment(lib,"Winmm.lib")


//�궨�岿��
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
#define L 60   //��
#define U 80   //��
#define R 900  //��
#define D 600  //��

using namespace std;

IMAGE wallimg; //һ��ͼ���࣬����EasyXͼ�ο� IMAGE�൱��int ������һ����������ͼƬ�õ�
int score = 0, grade = 1, rec[4];//rec���Լ���������飬rec���������
bool fight = false;

DWORD start, stop;

class Food;

class BodyNode {

private:


	IMAGE img;
	BodyNode() {} //���ڸ�ֵ�Ĺ��캯��
	friend class Body; //Body����Է���BodyNode��private����
	friend bool check(Body& snake, Food& food);

public: //public����ֻ���������캯��

	int x; //������
	int y; //������
	BodyNode* next; //ָ����һ������

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
	//����������Ƿ�Ե���ʳ�������ʳ���������������ռ�Ĳ�λ

	Body() { //���캯�����½�һ����������ͷβ��һ�����
		head = new BodyNode(HEADRIGHT, L + (10 * (IMGWIDTH)), U + (10 * (IMGHEIGHT)));
		//Ĭ��ͷ����
		head->next = new BodyNode(BODYLR, head->x - head->img.getwidth(), head->y);
		//��һ��ڵĺ�����Ϊͷ�ĺ������ȥͷ�Ŀ�ȣ��������ͷһ��
		head->next->next = new BodyNode(TAILR, head->next->x - head->img.getwidth(), head->y);
		//β�͵ĳ�ʼλ���ڵ�һ��ں�
		tail = head->next->next;
		//�õ�һ��ڵ�nextָ��ָ��β��
		length = 2;
	}

	int Length() { return length; }

	bool IsDead() { //�����������ж�
		BodyNode* p = head->next;
		while (p != NULL) {//p�ӵ�һ��ڿ�ʼ���������������ȷ����ڵ�λ���Ƿ��ͷ�غ�
			if (head->x == p->x && head->y == p->y || !alive) {
				return true;
			}
			p = p->next;
		}
		return false;
	}

	int Move(int dir) {     //1 �� 2��  3�� 4 ��
		BodyNode* p = head->next;
		int tempx = head->x, tempy = head->y, temp_x, temp_y;
		while (p != tail->next) {
			temp_x = p->x;
			temp_y = p->y; //��p�������������������
			p->x = tempx;
			p->y = tempy; //����һ�ζ�ȡ����������
			tempx = temp_x;
			tempy = temp_y; //��p�������������
			p = p->next; //ָ����һ��
		}
		if (dir == 1) {
			loadimage(&(head->img), _T(HEADUP));
			//_T����԰�һ���������������ַ�����������Ļ������ã�ʹ�ñ���������ݱ���Ŀ�껷��ѡ����ʵģ�Unicode����ANSI���ַ�����ʽ
			head->y = head->next->y - head->img.getheight();
			//ͷ��������Ϊ��һ��ڵ��������ȥͼƬ�ĸ߶�
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
		//�����ܴ�ǽ��

		p = head;
		while (p->next != tail) { //Ѱ��β��ǰ�����һ�����
			p = p->next;
		}
		if (p->x - tail->x > 0) { loadimage(&(tail->img), _T(TAILR)); }
		if (p->x - tail->x < 0) { loadimage(&(tail->img), _T(TAILL)); }
		if (p->y - tail->y > 0) { loadimage(&(tail->img), _T(TAILD)); }
		if (p->y - tail->y < 0) { loadimage(&(tail->img), _T(TAILU)); }
		//�ж�β�ͳ��򲢼�����ӦͼƬ
		return 0;
	}

	int Grow() {
		BodyNode* p = head;
		while (p->next != tail) {
			p = p->next;
		}
		tail->img = p->img;
		tail->next = new BodyNode(&(TAILL[0]), tail->x - (p->x - tail->x), tail->y - (p->y - tail->y));

		tail = tail->next; //β����+1�ڣ��ж�β�ͳ��򲢼�����ӦͼƬ
		if (p->x - tail->x > 0) { loadimage(&(tail->img), _T(TAILR)); }
		if (p->x - tail->x < 0) { loadimage(&(tail->img), _T(TAILL)); }
		if (p->y - tail->y > 0) { loadimage(&(tail->img), _T(TAILD)); }
		if (p->y - tail->y < 0) { loadimage(&(tail->img), _T(TAILU)); }

		tail->next = NULL;
		length++;
		return 0;
	}

	int Show() { //��ʾͼ��
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
	IMAGE rat; //λ�ú�ͼƬ��Ϣ
public:
	friend bool check(Body& snake, Food& food);
	Food() {
		x = 400;
		y = 300;
		loadimage(&(rat), _T(FOOD));
	}
	int givefood(Body& snake) {
		srand((unsigned)time(0)); //�������ڵ�ʱ���ʼ�����������
		while (check(snake, *this) == true) {
			//����߳Ե�ʳ����������ɵ�ʳ��������������ķ�Χ��
			x = 0; y = 0;
			while (!(x>L && y>U)) {
				//��֤ʳ�����ڵ�ͼ��
				x = rand() % 900;
				y = rand() % 600;
			}
			x = x - (x % 20);
			y = y - (y % 20);
			//Ҳ���ǿ���ǽ�Ŀ�ȣ�
		}
		return 0;
	}

	int Move(int dir) {     //1 �� 2��  3�� 4 ��
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

	cleardevice(); //�õ�ǰ����ɫ�����Ļ��������ǰ������ (0, 0)
	BeginBatchDraw();
	//��ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
	setbkcolor(RGB(0, 0, 0));    //���ñ���ɫ
	setcolor(YELLOW);         //���û�ͼɫ
	outtextxy((WIDTH / 2) - 80, 10, "̰����");
	outtextxy(L + 300, U - IMGHEIGHT - 20, "���ո� ��ͣ");
	outtextxy(R + IMGWIDTH + 20, U + IMGHEIGHT + 20, "��߼�¼��");
	
	char rec1[10];//,rec2[10],rec3[10];
	for (int index = 0; index <= 2; index++) {
		sprintf_s(rec1, "%d", rec[index]);
		outtextxy(R + IMGWIDTH + 20, U + IMGHEIGHT + 20 + (index + 1) * 30, rec1);
	} //�����ߵ�3�ε÷�

	int i, j;
	for (i = L - IMGWIDTH; i <= R; i = i + IMGWIDTH) { //��ש��ǽ
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

	outtextxy(L - 10, U - IMGHEIGHT - 20, "�÷�:");
	char s[10];
	sprintf_s(s, "%d", score);
	outtextxy(L + 60, U - IMGHEIGHT - 20, s);


	if (fight){
		outtextxy(L + 600, U - IMGHEIGHT - 20, "����ʱ��");
		char tt[1000];
		sprintf_s(tt, "%f", 50-(stop - start)*1.0/1000);
		outtextxy(L + 700, U - IMGHEIGHT - 20, tt);
	}

	outtextxy(L + 120, U - IMGHEIGHT - 20, "����:");
	char l[10];
	sprintf_s(l, "%d", snake.Length());
	outtextxy(L + 180, U - IMGHEIGHT - 20, l);

	snake.Show();

	food.Show();
	FlushBatchDraw(); //����Ļ����ʾ
	int clock = grade;
	if (clock == 0){ clock = 2; }
	Sleep(50 * clock); //��ʱ50*grade����

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
	mciSendString("play yeah.mp3 repeat", NULL, 0, NULL);//����
	return 0;
}

int menu() {
	//cleardevice();
	outtextxy((WIDTH / 2) - 80, 120, "̰����");
	outtextxy((WIDTH / 2) - 180, 230, "�����͡�ѡ���Ѷ�");
	outtextxy((WIDTH / 2) - 180, 250, "���ո��ȷ��ѡ��");
	return 0;
}

int record() {
	fstream f1("record.txt");   //���ļ������ļ������ھʹ�����
	if (!f1) return -1;
	//int a=0,b=0,c=0;
	f1 >> rec[0] >> rec[1] >> rec[2];
	f1.close();
return 0;
}

int saverecord() {
	system("del record.txt"); //����һ��dos����
	ofstream f1("record.txt");   //���ļ������ļ������ھʹ�����
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
		} //ʹrec[0]>rec[1]>rec[2]
	}

	f1 << rec[0] << "\n" << rec[1] << "\n" << rec[2];

	f1.close(); //�ر��ļ�
	if (score != rec[3]) {
		settextstyle(50, 0, _T("����"));
		outtextxy(WIDTH / 2 - (50 * 5), U + 40, "��ϲ�㴴�����¼�¼��");
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
		initgraph(WIDTH, HEIGHT); //��ͼ����
		char temp = 0, choose;
		while (temp != ' ') {
			cleardevice();
			if (_kbhit()) { //��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0
				choose = _getch(); //�ӿ���̨�޻��Ե�ȡһ���ַ�������Ҫ�س���
				fflush(stdin); //ǿ�Ƚ��������ڵ�����д��stdinָ�����ļ���
				switch (choose) {
				case UP: {
					grade++; //�Ѷ�ѡ��
				}; break;
				case DOWN: {
					grade--;
				};
				}
			}
			if (grade == 4) { grade = 3; }
			if (grade == -1) { grade = 0; } //����Ƶ�����֮���޷������ƣ��׶�����
			outtextxy(420, 350, "��");
			outtextxy(420, 380, "�е�");
			outtextxy(420, 410, "��");
			outtextxy(420, 600, "��սģʽ");

			menu();

			outtextxy(300, 300, "��ѡ���Ѷȣ�");
			if (grade == 3) {
				outtextxy(390, 350, "��");
			}
			if (grade == 2) {
				outtextxy(390, 380, "��");
			}
			if (grade == 1) {
				outtextxy(390, 410, "��");
			}
			if (grade == 0) {
				fight = true;
				outtextxy(390, 600, "��");
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
			while (snake.IsDead() == false && (!_kbhit())) { //û������Ҳû������
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
						mciSendString("play yeah.wav", NULL, 0, NULL); //����
						cout << '\a'; //���һ���������
						score += 10;
						food.givefood(snake); //Ͷ��ʳ��
						snake.Grow();
						snake.Grow();
						snake.Grow();
					}
					else{
						mciSendString("play yeah.wav", NULL, 0, NULL); //����
						cout << '\a'; //���һ���������
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
				case ' ': { //��ͣ
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
				outtextxy(450, 400, "̰���߻�ʤ��");
			}
			else{
				outtextxy(450, 400, "����ɹ������ˣ�");
			}
		}
		outtextxy(450, 500, "����y�����¿�ʼ");
		outtextxy(450, 550, "����n���˳���Ϸ");
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