#include<easyx.h>
#include <graphics.h>	
#include<bits/stdc++.h>
#include<conio.h>
#include <mmsystem.h>
#include <thread>
#include <windows.h>
#pragma comment(lib, "winmm.lib") // 链接 Windows 多媒体库
using namespace std;
constexpr auto wid = 591;
constexpr auto hig = 864;
bool PointInRect(int x, int y, RECT& r) {
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}
bool  RectCrashRect(RECT& r1, RECT& r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);
}
void welcome() {
	LPCTSTR title = _T("FYB社保CRK");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");
	RECT tplayr, texitr;

	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(wid / 2 - textwidth(title) / 2, hig / 5, title);

	settextstyle(40, 0, _T("黑体"));
	tplayr.left = wid / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = hig / 5*2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = wid / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = hig / 5*3;
	texitr.bottom = texitr.top + textheight(texit);

	
	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);
	mciSendString(_T("open ..\\mic\\bkmic.mp3 alias bkm"), NULL, 0, NULL);
	mciSendString(_T("play bkm"), NULL, 0,NULL);

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) {
			mciSendString(_T("close bkm"), NULL, 0, NULL);
			if (PointInRect(mess.x, mess.y, tplayr)) {
				return;
			}
			else if (PointInRect(mess.x, mess.y, texitr)) {
				exit(0);
			}
		}
	}
}
void Over(unsigned long long& kill) {
	IMAGE q, h,b;
	loadimage(&q, _T("..\\im\\qinqin.png"));
	loadimage(&h, _T("..\\im\\aixin.png"));
	loadimage(&b,_T("..\\im\\chulaile.jpg"));


	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("拒绝了:%llu个女人"), kill);

	// 绘制背景矩形
	setfillstyle(SOLID_FILL, BLACK); // 使用背景颜色填充
	bar(0, 0, wid, hig); // 绘制整个窗口作为背景
	settextcolor(RED);
	outtextxy(wid / 2 - textwidth(str) / 2, hig *2/3, str);
	settextstyle(65, 0, _T("黑体"));
	LPCTSTR qwq = _T("fyb被女人脚刹了");

	outtextxy(wid / 2 - textwidth(qwq) / 2, hig / 3, qwq);

	// 键盘事件enter
	LPCTSTR info = _T("按Enter使fyb退出贤者时刻");
	settextstyle(20, 0, _T("黑体"));
	outtextxy(wid - textwidth(info), hig + -textheight(info), info);


	while (true) {
		ExMessage mess;
		getmessage(&mess, EM_KEY);
		if (mess.vkcode == 0x0D) {
			return;
		}
	}
}

class Bullet {
public:
	Bullet(IMAGE& img,RECT pr) :img(img){

		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();

	}
	bool Show() {
		if (rect.bottom <= 0) {
			return false;
		}
		rect.top -= 3;
		rect.bottom -= 3;
		putimage(rect.left, rect.top, &img);
		return true;

	}
	RECT& GetRect() {
		return rect;
	}
private:
	IMAGE& img;
	RECT rect;
};
class EBullet {
public:
	EBullet(IMAGE& img, RECT pr) :img(img) {
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight()+3;
		rect.bottom = rect.top + img.getheight()+3;
	}
	bool Show() {
		if (rect.top >= hig) {
			return false;
		}
		rect.top += 1;
		rect.bottom += 1;
		putimage(rect.left, rect.top, &img);
		return true;
	}
	RECT& GetRect() {
		return rect;
	}

private:
	IMAGE& img;
	RECT rect;
};
class BK {
public:
	BK(IMAGE& img) 
		:img(img) ,y(-hig){

	}
	void Show() {
		if (y == 0)y = -hig;
			y += 3;
		putimage(0, y, &img);
	}
private:IMAGE& img;
	   int   y;
};
class fyb {
public:
	fyb(IMAGE& img)
		:img(img) {
		rect.left = wid / 2 - img.getheight() / 2;
		rect.top = hig - img.getheight();
		rect.right = rect.left + img.getheight();
		rect.bottom = hig;
	}
	void Show(){
		putimage(rect.left, rect.top, &img, SRCCOPY);
	}
	void Control() {
		ExMessage mess;
		if (peekmessage(&mess, EM_MOUSE)) {
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();
		}
	}
	RECT& GetRect() {
		return rect;
	}
private:
	IMAGE& img;
	RECT rect;
};
class Enemy {
public:
	Enemy(IMAGE& img ,int x) 
		:img(img) {
		rect.left = x;
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();
		rect.bottom = 0;
	}
	bool Show() {
		if (rect.top >= hig)return false;
		rect.top += 2;
		rect.bottom += 2;
		putimage(rect.left, rect.top, &img);
		return true;
	}
	RECT& GetRect() {
		return rect;
	}
private:
	IMAGE& img;
	RECT rect;

};
bool AddEnemy(vector<Enemy*>& es, IMAGE& enemying) {
	Enemy* e = new Enemy(enemying, abs(rand()) % (wid - enemying.getwidth()));

	for (auto& i : es) {
		if (RectCrashRect(i->GetRect(),e->GetRect())) {
			delete e;
			return false;
		}
	}
	es.push_back(e);
	return true;
}
bool Play() {
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;
		IMAGE fu,bking,en, but,ebut;
	loadimage(&fu, _T("..\\im\\fu.png"));
	loadimage(&bking, _T("..\\im\\bk.png",591));
	loadimage(&en, _T("..\\im\\enemy.png"));
	loadimage(&but, _T("..\\im\\but.png"));
	loadimage(&ebut, _T("..\\im\\ebut.png"));

	BK bk = BK(bking);
	fyb yb = fyb(fu);
	unsigned long long kill = 0;

	vector<Enemy*> es;
	vector<Bullet*> bs;
	vector<EBullet*> ebs;

	int bsing = 0,ebsing=0;
	int num = 1 + rand() % 4;
	for (int i = 0; i < num; i++) {
		AddEnemy(es, en);
	}

	while (is_play){
		BeginBatchDraw();
		bsing++;
		ebsing++;
		if (bsing == 22) {
			bsing = 0;
			bs.push_back(new Bullet(but, yb.GetRect()));
		}

		auto eit = es.begin();
		bool flag = 0;
		while (eit != es.end()) {
			if (ebsing == 32&&rand()%2==0) {
				ebs.push_back(new EBullet(ebut, (*eit)->GetRect()));
				flag = 1;
			}
			eit++;
		}
		if(flag)ebsing=0;

		bk.Show();
		Sleep(6);
		flushmessage();
		Sleep(2);
		yb.Control();
		if (_kbhit()) {
			char v = _getch();
			if (v == 0x20) {
				Sleep(500);
				while (true) {
					if (_kbhit()) {
						v = _getch();
						if(v==0x20)break;
					}
					Sleep(16) ; 
				}
			}
		}
		yb.Show();

		for (auto i : bs) {
			i->Show();
		}
		for (auto i : ebs) {
			i->Show();
		}
		auto it = es.begin();
		while (it != es.end()) {
			if (RectCrashRect((*it)->GetRect(), yb.GetRect())) {

				is_play = false;
			}
			auto bit = bs.begin();
			while (bit != bs.end()) {
				if (RectCrashRect((*bit)->GetRect(), (*it)->GetRect())) {
					PlaySound(_T("..\\mic\\jibai.wav"), NULL, SND_FILENAME | SND_ASYNC);
					delete(*it);
					es.erase(it);
					it = es.begin();
					delete(*bit);
					bs.erase(bit);
					kill++;
					break;
					if (!(*bit)->Show()) {
						delete(*bit);
						bit = bs.erase(bit);
					}
				}

				bit++;
			}
			auto ebit = ebs.begin();
			while (ebit != ebs.end()) {
				if (RectCrashRect((*ebit)->GetRect(), yb.GetRect())) {
					is_play = false;
					break;
				}
				if (!(*ebit)->Show()) {
					delete(*ebit);
					ebit = ebs.erase(ebit);
				}
				else ebit++;
			}
			if (!(*it)->Show()) { 
				delete(*it);
				it=es.erase(it);
			}
			else it ++;
		}
		for (int i = 0; i < 6 - es.size(); i++)
			AddEnemy(es, en);
		EndBatchDraw();
	}
	Over(kill);
	return true;
}
int main() {

	initgraph(wid, hig);
	bool is_live = true;
	while (is_live) {
		welcome();
		is_live = Play();
	}
	return 0;
}