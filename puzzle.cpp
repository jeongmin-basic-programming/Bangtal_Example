#include <Bangtal.h>
#pragma comment(lib,"Bangtal.lib")
#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <cstdlib>
#include <utility>

using namespace std;

int puzzle[4][4];
ObjectID puzzleObject[4][4];

SceneID scene;

TimerID timer1;

int rows = 4;
int cols = 4;

int shuffleTimes = 100; //���� �� ������ Ƚ��

int minute = 0; // �ҿ��

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1, };

pair<int, int> corner; // ���� ���� ��������
const int pieceSize = 150; // ���� ���� ũ��

pair<int,int> cur; // ����ĭ�� ��ġ
pair<int, int> coord[4][4]; //�� ���� �������� ��ǥ��

bool gameStarted = false;

bool checkClear(void); 
void shuffleBoard(int n);
int randomDirection(void);
void initializeBoard(int x, int y);
void showPuzzle(bool hideEmpty);
bool checkValidMove(int i);
void movePiece(int i);

void timerCallback(TimerID timer);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);

ObjectID startButton;

int main(void)
{
	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);

	scene = createScene("������", "Images\\back.png");

	corner = make_pair(296, 504);
	initializeBoard(3, 3);
	showPuzzle(false);

	startButton = createObject("���۹�ư", "Images\\start.png");
	locateObject(startButton, scene, 590, 350);
	showObject(startButton);

	timer1 = createTimer(60.0f);
	
	startGame(scene);

	return 0;
}

void timerCallback(TimerID timer)
{
	if (timer == timer1)
	{
		minute++;
		setTimer(timer, 60.0f);
		startTimer(timer);
	}
	
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	bool targetFound = false;

	if (object == startButton)
	{
		minute = 0;
		hideObject(startButton);
		hideObject(puzzleObject[cur.first][cur.second]);
		shuffleBoard(shuffleTimes);
		setTimer(timer1, 60.0f);
		startTimer(timer1);
		gameStarted = true;
	}
	else if (gameStarted){
		for (int x = 0;x < rows; x++)
		{
			for (int y = 0; y < cols; y++)
			{
				if (object == puzzleObject[x][y])
				{
					targetFound = true;

					int nx = 0, ny = 0;
					for (int i = 0; i < 4; i++)
					{

						nx = dx[i] + x;
						ny = dy[i] + y;
						if (nx == cur.first&& ny == cur.second)
						{
							if (i == 0)//��
								movePiece(1);
							else if (i == 1)//��
								movePiece(0);
							else if (i == 2) // ��
								movePiece(3);
							else if (i == 3)//��
								movePiece(2);
						}
					}
					if (checkClear())
					{
						char buf[16];
						showObject(puzzleObject[cur.first][cur.second]);
						//���� ����
						setObjectImage(startButton, "Images\\restart.png");
						showObject(startButton);

						sprintf(buf, "%d�� %d�� �ҿ�",minute,int(60.0-getTimer(timer1)));
						stopTimer(timer1);
						showMessage(buf);

					}
					if (targetFound == true)
						return;
				}
			}
		}
	}
}

void initializeBoard(int x, int y) {
	int val = 0;
	char buf[16];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{

			val= i * rows + j;
			puzzle[i][j] = val;
			sprintf(buf, "images\\%d.png", val);
			puzzleObject[i][j]=createObject("piece", buf);

			coord[i][j] = make_pair(corner.first + pieceSize * j, corner.second - pieceSize * i);
			locateObject(puzzleObject[i][j],scene, coord[i][j].first, coord[i][j].second);
		}
	cur=make_pair(x, y);
}

bool checkClear(void) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (puzzle[i][j] != i * rows + j)
				return false;
	return true;
}

void showPuzzle(bool hideEmpty) {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			showObject(puzzleObject[i][j]);
			cout << puzzle[i][j] << ' ';
		}
		cout << '\n';
	}
	if(hideEmpty)
		hideObject(puzzleObject[cur.first][cur.second]);

}


void movePiece(int i)
{
	/*
	cur pos ����
	object[] ����
	puzzle[] ����
	locate ����
	*/
	int nx = cur.first + dx[i];
	int ny = cur.second+ dy[i];

	int tempValue = puzzle[nx][ny];
	ObjectID tempObject = puzzleObject[nx][ny];

	int coordX = coord[cur.first][cur.second].first;
	int coordY = coord[cur.first][cur.second].second;

	int ncoordX = coord[nx][ny].first;
	int ncoordY = coord[nx][ny].second;

	puzzle[nx][ny] = puzzle[cur.first][cur.second];
	puzzle[cur.first][cur.second] = tempValue;

	puzzleObject[nx][ny] = puzzleObject[cur.first][cur.second];
	puzzleObject[cur.first][cur.second] = tempObject;

	locateObject(puzzleObject[nx][ny], scene, ncoordX, ncoordY);
	locateObject(puzzleObject[cur.first][cur.second], scene, coordX,coordY);

	cur.first = nx;
	cur.second = ny;


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << puzzle[i][j] << ' ';
		}
		cout << '\n';
	}

}
bool checkValidMove(int i)
{
	int nx = cur.first + dx[i];
	int ny = cur.second + dy[i];
	bool valid = false;
	if (0 <= nx && nx < rows && 0 <= ny && ny < cols)
		valid = true;

	return valid;
}
int randomDirection(void) {
	return rand()%4;
}
void shuffleBoard(int n)
{
	int move = 0,cnt=0;
	
	do {
		move = randomDirection();
		if (!checkValidMove(move))
			continue;
		movePiece(move);
		cnt++;
	} while (cnt < n);
	if (checkClear()) // Ȥ�ö� ���� ����� �����϶�
		shuffleBoard(1);// ��ĭ�� �� �����δ�.
}
