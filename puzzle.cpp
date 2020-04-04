#include <Bangtal.h>
#pragma comment(lib,"Bangtal.lib")
#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <cstdlib>
#include <queue>

using namespace std;

int puzzle[4][4];
ObjectID puzzleObject[4][4];
SceneID scene;

int rows = 4;
int cols = 4;

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1, };

pair<int, int> corner;
const int pieceSize = 150;

pair<int,int> cur;
pair<int, int> coord[4][4];

bool checkClear(void);
void shuffleBoard(int n);
int randomDirection(void);
void initializeBoard(int x, int y);
void printPuzzle(bool hideEmpty);
bool  checkValidMove(int i);
void movePiece(int i);


void mouseCallback(ObjectID object, int x, int y, MouseAction action);

ObjectID startButton;

int main(void)
{

	setMouseCallback(mouseCallback);


	scene = createScene("개굴맨", "Images\\back.png");

	corner = make_pair(296, 504);
	initializeBoard(3, 3);
	printPuzzle(false);


	startButton = createObject("시작버튼", "Images\\start.png");
	locateObject(startButton, scene, 590, 350);
	showObject(startButton);

	startGame(scene);

	//TODO : 클릭 처리
	//while (!checkClear()) {
	//	int inputVal = 0;
	//	bool valid = false;
	//	cout << "상, 하, 좌, 우" << endl;
	//	cin >> inputVal; // 1~4
	//	valid = checkValidMove(inputVal);
	//	if (!valid)
	//	{
	//		cout << "invalid move!" << endl;
	//		continue;

	//	}
	//	movePiece(inputVal);
	//}

	return 0;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {


	if (object == startButton)
	{
		hideObject(startButton);
		hideObject(puzzleObject[cur.first][cur.second]);
		shuffleBoard(50);

	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (object == puzzleObject[j][i])
			{
				//move valid 
				//move 


				if (checkClear())
					endGame();
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

void printPuzzle(bool hideEmpty) {
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
	cur pos 변경
	object[] 스왑
	puzzle[] 스왑
	locate 변경
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

}
