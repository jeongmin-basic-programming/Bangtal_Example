#include <Bangtal.h>
#pragma comment(lib,"Bangtal.lib")

#include <iostream>
#include <cstdlib>
#include <queue>

using namespace std;

int puzzle[3][3];

int rows = 3;
int cols = 3;

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1, };

pair<int,int> cur;

bool checkClear(void);
void shuffleBoard(int n);
int randomDirection(void);
void initializeBoard(int x, int y);
void printPuzzle(void);
bool  checkValidMove(int i);
void movePiece(int i);

int main(void)
{
	initializeBoard(2, 2);
	printPuzzle();
	shuffleBoard(5);


	while (!checkClear()) {
		int inputVal = 0;
		bool valid = false;
		cout << "╩С, го, аб, ©Л" << endl;
		cin >> inputVal; // 1~4
		valid = checkValidMove(inputVal);
		if (!valid)
		{
			cout << "invalid move!" << endl;
			continue;

		}
		movePiece(inputVal);
	}

	return 0;
}

void initializeBoard(int x, int y) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			puzzle[i][j] = i * rows + j;
	cur=make_pair(x, y);
}

bool checkClear(void) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (puzzle[i][j] != i * rows + j)
				return false;

	return true;
}

void printPuzzle(void) {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			cout << puzzle[i][j] << ' ';
		cout << '\n';
	}
}
void movePiece(int i)
{
	int nx = cur.first + dx[i];
	int ny = cur.second+ dy[i];

	int tempValue = puzzle[nx][ny];

	puzzle[nx][ny] = puzzle[cur.first][cur.second];
	puzzle[cur.first][cur.second] = tempValue;

	cur.first = nx;
	cur.second = ny;


	printPuzzle();
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
