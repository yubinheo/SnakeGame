// 전처리기 선언
#include <windows.h> 
#include <stdio.h> 
#include <conio.h> 
#include <time.h> 
#include <stralign.h>
#include <mmsystem.h>
#define MAX  100 
#pragma comment(lib, "winmm.lib")

/*

	Main Projected by Universe.
	 Sub Projected by Sirius.

	Copyright 2019. Re_Universe. All Rights Reserved.

*/

// 좌표
typedef struct {
	int x, y;
} POS;


int map_number = 3, song_number, count = 0; // 부수적 관여 데이터 (전역변수)

// 메인화면을 출력하는 함수 (실행이 이곳 제일 먼저 옴)
void menu(double* score, int* eeof, double* first_s, double* second_s, double* third_s, int* first_c, int* second_c, int* third_c, int* first_t, int* second_t, int* third_t);

// 화면을 출력해주는 함수
void write_screen(int map_number);
// 화면 커서를 x, y 좌표에 맞게 이동해주는 함수
void write_c(int x, int y, char* s);
// 캐릭터 (지렁이)가 이동하게 해주는 함수
void move_character(POS* character, int len, int* turn, int first_t);
// 지렁이의 현재상태를 인식하는 함수 (벽에 닿았는가 등)
int check_character(POS* character, int len);

// 점수를 보여주는 함수
void showscore(double score, double first_s, double second_s, double third_s, int first_c, int second_c, int third_c, int first_t, int second_t, int third_t);
// 점수를 연산해주는 함수
void addscore(double* score, int combo);
// 콤보를 연산해주는 함수
void combocompare(double* score, int* combo, int* combo_tmp, int vaule);
// 최고 점수를 연산해주는 함수
void scorecompare(double score, double* first, double* second, double* third);
// 최고 턴수를 연산해주는 함수
void turncompare(int turn, int* first, int* second, int* third);
// 최고 콤보를 연산해주는 함수
void highcombocompare(int combo_tmp, int* first, int* second, int* third);

// 도움말을 출력해주는 함수.
void help();
// 문자의 색을 설정해주는 함수.
void setcolor(int color);

// 업데이트 내용을 출력해주는 함수
void updatelist();
// 인게임 내 점수, 콤보, 턴 수를 출력해주는 함수
void showinfodata(double score, int combo, double first_s, int first_c);

// 게임 설정메뉴
void gamesetting(double** first_s, double** second_s, double** third_s, int** first_c, int** second_c, int** third_c, int** first_t, int** second_t, int** third_t);
// 맵 설정 관리자
void MAP_Manager();
// 배경음악 설정 관리자
void BGM_Manager();

int Warning();

void main()
{
	char musiclist[] = { "Louis_Almas_Castle_Remix(Mansion_Dance_Battle)" }; // 노래 목록
	int combo = 0, combo_tmp = 0, first_c = 0, second_c = 0, third_c = 0, music = 0, eof = 1;
	// 콤보 저장, 최고 콤보값 저장,  콤보1st,	   콤보2nd,	     콤보3rd,    음악실행번호, 프로그램 반복문 제어
	int turn = 0, first_t = 0, second_t = 0, third_t = 0;
	// 턴 수,     턴 1st,        턴 2nd,        턴3rd
	double score = 0, first_s = 0, second_s = 0, third_s = 0;
	// 점수 저장,		점수1st,		점수2nd,		  점수3rd

	while (eof) {
		POS character[MAX], item, slow;
		int i, n = 0, dir = -1, len = 2, loop = 1; // 부수적 관여 데이터
		int speed = 150; // 게임 관여 데이터

		// 기록 비교
		scorecompare(score, &first_s, &second_s, &third_s);
		turncompare(turn, &first_t, &second_t, &third_t);
		highcombocompare(combo_tmp, &first_c, &second_c, &third_c); combo = 0, combo_tmp = 0, turn = 0;

		// 메뉴 호출
		do {
			menu(&score, &eof, &first_s, &second_s, &third_s, &first_c, &second_c, &third_c, &first_t, &second_t, &third_t);
			if (eof == 2) { continue; }
		} while (eof != 1);

		system("mode con: cols=100 lines=30");
		score = 0;
		character[0].x--;

		// 랜덤 초기화. 
		srand(time(NULL));

		// 배경 그리기. 
		system("cls");
		write_screen(map_number);

		// 지렁이 초기위치. 
		for (i = 0; i < len; i++)
		{
			character[i].x = 15 + i;
			character[i].y = 10;
			write_c(character[i].x, character[i].y, "▣");
		}
		setcolor(4);
		write_c(1, 21, "Music : "); /*printf("%s", musiclist[music]);*/
		setcolor(15);

		// 먹이 처음 위치 
		item.x = rand() % 28 + 1;
		item.y = rand() % 18 + 1;
		setcolor(0);
		write_c(item.x, item.y, "◈");
		setcolor(15);
		write_c(1, 22, "Score : 0.0");
		write_c(1, 23, "Combo : x0");
		write_c(1, 24, "Turn : 0");

		// 아이템 처음 위치 
		slow.x = rand() % 28 + 1;
		slow.y = rand() % 18 + 1;
		setcolor(0);
		write_c(slow.x, slow.y, "♨");
		setcolor(15);

		// 게임 루프. 
		while (loop)
		{
			// 지렁이가 벽, 몸통에 닿았는가?
			if (check_character(character, len) == 0)
				break;

			// 지렁이가 먹이를 먹었는가? 
			if (character[0].x == item.x && character[0].y == item.y)
			{
				addscore(&score, combo);

				item.x = rand() % 28 + 1;
				item.y = rand() % 18 + 1;

				combocompare(&score, &combo, &combo_tmp, 1);
				showinfodata(score, combo, first_s, first_c, first_t, turn);

				// 먹이를 먹을때 마다 스피드 증가
				speed -= 5;

				// 먹이를 먹을때 마다 꼬리 생성
				if (len < MAX)
				{
					character[len] = character[len - 1];
					len++;
				}
			}

			// 지렁이가 아이템을 먹었는가? 
			if (character[0].x == slow.x && character[0].y == slow.y)
			{
				slow.x = rand() % 28 + 1;
				slow.y = rand() % 18 + 1;
				combocompare(&score, &combo, &combo_tmp, 2);

				showinfodata(score, combo, first_s, first_c, first_t, turn);

				// 먹이를 먹을때 마다 스피드 영구 감소
				speed += 3;
			}

			// 아이템 (먹이) 출력 
			setcolor(6);
			write_c(item.x, item.y, "◈");
			setcolor(15);

			// 아이템 출력 
			setcolor(6);
			write_c(slow.x, slow.y, "♨");
			setcolor(15);

			// 지렁이의 움직임 처리
			move_character(character, len, &turn, first_t);

			// 스피드에 맞게 조절해줌.
			Sleep(speed);
		}
		combocompare(&score, &combo, &combo_tmp, 2);
		write_c(5, 25, "Game Over!\n");
		system("pause");
		system("cls");
	}
	printf("\n프로그램 종료됨.\n");
}

void write_c(int x, int y, char* s)
{
	COORD Pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	printf("%s", s);
}

void write_screen(int map_number)
{
	int i;
	system("cls");
	if (map_number == 3) {
		setcolor(8);
		write_c(0, 0, "▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");
		setcolor(15);

		for (i = 1; i < 20; i++)
		{
			setcolor(8);
			write_c(0, i, "▩");
			write_c(33, i, "▩");
			setcolor(15);
		}
		setcolor(8);
		write_c(0, 20, "▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩\n");
		setcolor(15);
	}

	else if (map_number == 1) {
		setcolor(8);
		write_c(0, 0, "♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣"); // 34
		setcolor(15);

		for (i = 1; i < 20; i++)
		{
			setcolor(8);
			write_c(0, i, "♣");
			write_c(33, i, "♣");
			setcolor(15);
		}
		setcolor(8);
		write_c(0, 20, "♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣\n");  // 34
		setcolor(15);
	}

	else if (map_number == 2) {
		setcolor(8);
		write_c(0, 0, "♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥"); // 34
		setcolor(15);

		for (i = 1; i < 20; i++)
		{
			setcolor(8);
			write_c(0, i, "♥");
			write_c(33, i, "♥");
			setcolor(15);
		}
		setcolor(8);
		write_c(0, 20, "♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥♥\n");  // 34
		setcolor(15);
	}
}

void move_character(POS* character, int len, int* turn, int first_t)
{
	static int dir = -1;

	// 키보드에서 입력을 감지함.
	if (_kbhit())
	{
		int key;
		do { key = _getch(); } while (key == 224);
		// 키입력이 감지되었다면 상,하,좌,우로 지렁이 이동
		switch (key)
		{
		case 72: dir = 0; break; // 이동_상
		case 80: dir = 1; break; // 이동_하
		case 75: dir = 2; break; // 이동_좌
		case 77: dir = 3; break; // 이동_우
		}
		*turn += 1;
		setcolor(15);
		write_c(1, 24, "Turn : ");
		setcolor(2);
		printf("%d  ", *turn);
		setcolor(15);
	}

	// 지렁이의 몸통 이동처리
	if (dir != -1)
	{
		int i;
		write_c(character[len - 1].x, character[len - 1].y, "  ");
		for (i = len - 1; i > 0; i--) character[i] = character[i - 1];
		setcolor(4);
		write_c(character[1].x, character[1].y, "□");
		setcolor(15);
	}

	// 지렁이 머리 이동처리
	switch (dir)
	{
	case 0: character[0].y--; break;
	case 1: character[0].y++; break;
	case 2: character[0].x--; break;
	case 3: character[0].x++; break;
	}
	setcolor(11);
	write_c(character[0].x, character[0].y, "◎");
	setcolor(15);
}

int check_character(POS * character, int len)
{
	int i;

	// 지렁이 머리가 벽에 닿았는가?
	if (character[0].x == 0 || character[0].y == 0 || character[0].x == 33 || character[0].y == 20)
		return 0;

	// 지렁이 머리가 몸통에 닿았는가? 
	for (i = 1; i < len; i++)
	{
		if (character[0].x == character[i].x && character[0].y == character[i].y)
			return 0;
	}

	return 1;
}

void showscore(double score, double first_s, double second_s, double third_s, int first_c, int second_c, int third_c, int first_t, int second_t, int third_t)
{
	system("mode con: cols=30 lines=21");
	system("cls");

	printf("\n    --- High Score ---\n\n");
	printf("\t1st : %.1f\n", first_s);
	printf("\t2nd : %.1f\n", second_s);
	printf("\t3rd : %.1f\n\n", third_s);

	printf("    --- High Combo ---\n\n");
	printf("\t1st : %d\n", first_c);
	printf("\t2nd : %d\n", second_c);
	printf("\t3rd : %d\n\n", third_c);

	printf("    --- High Turn ---\n\n");
	printf("\t1st : %d\n", first_t);
	printf("\t2nd : %d\n", second_t);
	printf("\t3rd : %d\n\n", third_t);

	printf("  Press any key to continue\n");

	system("pause > nul");
}

void menu(double* score, int* eeof, double* first_s, double* second_s, double* third_s, int* first_c, int* second_c, int* third_c, int* first_t, int* second_t, int* third_t) {
	int sel, eof = 1;
	int mnumber = 0, snumber = 0; // 부수적 관여 데이터
	while (eof)
	{
		system("mode con: cols=70 lines=17"); // 콘솔창 크기 설정 (70칸, 17줄)
		system("cls");
		*eeof = 2;

		setcolor(11);
		write_c(11, 5, " Snake  Game  [v 2.9]\n\n");

		setcolor(12);
		write_c(12, 7, "| 1 | 게임 시작\n");

		setcolor(10);
		write_c(12, 8, "| 2 | 게임 방법\n");

		setcolor(6);
		write_c(12, 9, "| 3 | 기록 보기\n");
		
		setcolor(14);
		write_c(12, 10, "| 4 | 패치 노트\n");
		
		setcolor(3);
		write_c(12, 11, "| 5 | 게임 설정\n");
		
		setcolor(8);
		write_c(12, 12, "| 6 | 게임 종료\n\n");
	
		setcolor(15);
		write_c(12, 14, ">> ");
		setcolor(15);
		scanf_s(" %d", &sel);

		switch (sel)
		{
		case 1: *eeof = 1; eof = 0; break;
		case 2: help(); break;
		case 3: showscore(*score, *first_s, *second_s, *third_s, *first_c, *second_c, *third_c, *first_t, *second_t, *third_t); break;
		case 4: updatelist(); break;
		case 5: gamesetting(&*first_s, &*second_s, &*third_s, &*first_c, &*second_c, &*third_c, &*first_t, &*second_t, &*third_t); break;
		case 6: exit(0); break;
		default:
			printf("잘못된 입력입니다.\n");
			break;
		}
		//map_number = mnumber; song_number = snumber;
		system("cls");
	}
}

void addscore(double* score, int combo)
{
	if (combo == 0) // 콤보가 0 이라면, 점수를 10점 증가
		*score += 10;

	else if (combo != 0) // 콤보가 0 이 아니라면, 점수를 아래 수식만큼 증가
		*score += 10 + (combo * 1.15);
}

void combocompare(double* score, int* combo, int* combo_tmp, int vaule)
{
	if (vaule == 1) { *combo += 1; } // 먹이를 먹은 경우
	else if (vaule == 2) {
		if (*combo > *combo_tmp)
			*combo_tmp = *combo;
		*combo = 0;
		*score += 4;
	} // 아이템을 먹은 경우
}

void scorecompare(double score, double* first, double* second, double* third) {
	double high;
	high = score;

	if (high >= *first) {
		*third = *second;
		*second = *first;
		*first = high;
	}

	else if (high >= *second) {
		*third = *second;
		*second = high;
	}
	else if (high >= *third) {
		*third = high;
	}
}

void turncompare(int turn, int* first, int* second, int* third) {
	int high;
	high = turn;

	if (high >= *first) {
		*third = *second;
		*second = *first;
		*first = high;
	}

	else if (high >= *second) {
		*third = *second;
		*second = high;
	}
	else if (high >= *third) {
		*third = high;
	}
}

void highcombocompare(int combo_tmp, int* first, int* second, int* third)
{
	int high;
	high = combo_tmp;

	if (high >= *first) {
		*third = *second;
		*second = *first;
		*first = high;
	}

	else if (high >= *second) {
		*third = *second;
		*second = high;
	}
	else if (high >= *third) {
		*third = high;
	}
}

void help() {
	system("cls");
	setcolor(11);
	write_c(10, 3, "+--------------------------+");
	write_c(10, 4, "|        S N A K E         |");
	write_c(10, 5, "+--------------------------+");
	setcolor(15);

	setcolor(8);
	write_c(10, 7, "< Press any key to continue > ");
	setcolor(15);

	setcolor(1);
	write_c(10, 9, "   ◇ ←,→,↑,↓ : Move    ");
	write_c(10, 10, "   ◇ ◈ - Food");
	write_c(10, 11, "   ◇ ♨ - Slow Item");
	setcolor(15);
	printf("\t\t\t\t\t\t\t\t\t");
	system("pause > nul");
}

void setcolor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void updatelist() {
	system("mode con: cols=50 lines=50"); system("cls");

	printf("\n");

	setcolor(9);
	write_c(3, 2, "19-04-20 [B.1 Updated]");
	setcolor(7);
	write_c(3, 3, "1. 메인 시스템 구축");

	setcolor(9);
	write_c(3, 5, "19-04-21 [1.0 Updated]");
	setcolor(7);
	write_c(3, 6, "1. 메뉴화 시스템 구축");
	write_c(3, 7, "2. 기록 저장 기능 추가");
	write_c(3, 8, "3. 게임 가이드 추가");
	write_c(3, 9, "4. 배경음악(BGM) 추가");

	setcolor(9);
	write_c(3, 11, "19-04-21 [1.1 Updated]");
	setcolor(7);
	write_c(3, 12, "1. 메뉴 페이지 리뉴얼");
	write_c(3, 13, "2. 게임 버그 및 시스템 오류 수정");

	setcolor(9);
	write_c(3, 15, "19-04-21 [2.0 updated] ");
	setcolor(7);
	write_c(3, 16, "1. 신규 아이템 추가 (감속)");
	write_c(3, 17, "2. 시스템 버그 수정");

	setcolor(9);
	write_c(3, 19, "19-04-27 [2.4 updated] ");
	setcolor(7);
	write_c(3, 20, "1. 콤보 기능 추가");
	write_c(3, 21, "2. 회전 횟수(키보드 동작) 카운터 추가");

	setcolor(9);
	write_c(3, 23, "19-05-05 [2.7 updated] ");
	setcolor(7);
	write_c(3, 24, "1. 게임 설정창 추가");
	write_c(3, 25, "2. 시스템 최적화");

	setcolor(9);
	write_c(3, 27, "19-05-06 [2.9 updated] ");
	setcolor(7);
	write_c(3, 28, "1. 게임 설정창 리뉴얼");
	write_c(3, 29, "2. 시스템 최적화 및 오류 수정");

	setcolor(8);
	write_c(5, 31, "Press any key to continue");
	system("pause > nul");
}

void showinfodata(double score, int combo, double first_s, int first_c) {
	write_c(1, 22, "Score : ");
	if (first_s > score) setcolor(9);
	else setcolor(6);
	printf("%.1f  ", score);
	setcolor(15);

	write_c(1, 23, "Combo : x");
	setcolor(9);
	printf("%d  ", combo);
	setcolor(15);
}

void gamesetting(double** first_s, double** second_s, double** third_s, int** first_c, int** second_c, int** third_c, int** first_t, int** second_t, int** third_t) {
	int sel, eof = 1;
	while (eof)
	{
		system("mode con: cols=70 lines=17"); // 콘솔창 크기 설정 (70칸, 17줄)
		system("cls");

		setcolor(15);
		write_c(13, 5, "[ Game Setting ]\n");
		setcolor(7);
		write_c(12, 7, "| 1 | 인게임 맵 변경\n");
		write_c(12, 8, "| 2 | 배경음악 On / Off\n");
		write_c(12, 9, "| 3 | 게임 데이터 초기화 (사용금지)\n"); // 경고! 오류 발생으로 인한 사용 금지 처분.
		write_c(12, 10, "| 4 | 메뉴로 돌아가기\n");

		write_c(12, 12, ">> "); scanf_s(" %d", &sel);

		switch (sel)
		{
		case 1:
			MAP_Manager(); // 인게임 맵 관리자
			break;

		case 2:
			BGM_Manager(); // 배경음악 관리자
			break;

		case 3:
			if (Warning() == 1) {
				**first_s = 0; **second_s = 0; **third_s = 0; **first_c = 0; **second_c = 0; **third_c = 0; **first_t = 0; **second_t = 0; **third_t = 0; // 기록 데이터 초기화 ( 사용 금지 )
			}
			break;

		case 4:
			eof = 0;
			break;

		default:
			write_c(11, 14, "잘못된 입력입니다.\n");
			break;
		}

		if (sel != 4) {
			setcolor(8);
			write_c(11, 15, "Press any key to continue");
			system("pause > nul");
			setcolor(15);
		}
	}
}

void MAP_Manager()
{
	int eof = 1, sel;

	while (eof)
	{
		do
		{
			system("mode con: cols=70 lines=17");
			system("cls");

			setcolor(15);
			write_c(10, 5, "[ Game Setting - MAP Manager ]\n");
			setcolor(2);
			write_c(12, 7, "| 1 | 나무벽 맵 "); if (map_number == 1) { printf("[선택됨]\n"); }
			setcolor(4);
			write_c(12, 8, "| 2 | 하트벽 맵 "); if (map_number == 2) { printf("[선택됨]\n"); }
			setcolor(6);
			write_c(12, 9, "| 3 | 네모벽 맵 "); if (map_number == 3) { printf("[선택됨]\n"); }
			setcolor(7);
			write_c(12, 10, "| 4 | 메뉴로 돌아가기\n");
			setcolor(15);

			write_c(12, 12, ">> "); scanf_s(" %d", &sel);
			setcolor(15);

		} while (sel < 0);
		
		if (sel == 1 || sel == 2 || sel == 3) {
			map_number = sel;
		}
		else if (sel == 4) { eof = 0; }
		else { 
		setcolor(15);
		write_c(12, 14, "잘못된 입력입니다.\n");
		setcolor(8);
		write_c(11, 15, "Press any key to continue");
		system("pause > nul");
		setcolor(15);
		}
	}
}

void BGM_Manager()
{
	int eof = 1, eeof, sel;

	while (eof)
	{ 
		eeof = 1;

		system("mode con: cols=70 lines=17");
		system("cls");

		setcolor(15);
		write_c(10, 5, "[ Game Setting - BGM Manager ]\n");
		setcolor(7);
		write_c(12, 7, "| 1 | 배경음악 On \n");
		write_c(12, 8, "| 2 | 배경음악 Off\n");
		write_c(12, 9, "| 3 | 메뉴로 돌아가기\n");

		write_c(12, 11, ">> "); scanf_s(" %d", &sel);
		setcolor(15);

		switch (sel)
		{
		case 1:
			while (eeof) {
				do {
					system("mode con: cols=70 lines=17");
					system("cls");

					setcolor(15);
					write_c(10, 5, "[ Game Setting - BGM Manager ]\n");
					setcolor(7);
					write_c(10, 7, "| 1 | Castle_Remix (Kart Rider) "); if (song_number == 1) { printf("[선택됨]\n"); }
					write_c(10, 8, "| 2 | 119 (키드밀리) "); if (song_number == 2) { printf("[선택됨]\n"); }
					write_c(10, 9, "| 3 | Heu ! (수퍼비) "); if (song_number == 3) { printf("[선택됨]\n"); }
					write_c(10, 10, "| 0 | 메뉴로 돌아가기\n");

					write_c(11, 12, ">> "); scanf_s(" %d", &sel);
				} while (sel < 0);

				if (sel != 0) {
					song_number = sel;
					if (sel == 1)
						PlaySound(TEXT("Louis_Almas_Castle_Remix(Mansion_Dance_Battle).wav"), NULL, SND_ASYNC | SND_LOOP);

					else if (sel == 2)
						PlaySound(TEXT("119.wav"), NULL, SND_ASYNC | SND_LOOP);

					else if (sel == 3)
						PlaySound(TEXT("Heu.wav"), NULL, SND_ASYNC | SND_LOOP);
				}
				else { eeof = 0; }
			}
			break;

		case 2:
			PlaySound(NULL, 0, 0);
			song_number = 0;
			break;

		case 3:
			eof = 0;
			break;

		default:
			setcolor(15);
			write_c(12, 14, "잘못된 입력입니다.\n");
			setcolor(8);
			write_c(11, 15, "Press any key to continue");
			system("pause > nul");
			setcolor(15);
			break;
		}
	}
}

int Warning()
{
	int warn;

	do
	{
		system("mode con: cols=70 lines=17"); // 콘솔창 크기 설정 (70칸, 17줄)
		system("cls");

		setcolor(15);
		write_c(12, 5, "[ Game Setting ]\n");
		setcolor(4);
		write_c(6, 7, "경고! 정말로 게임 데이터를 초기화하시겠습니까?\n");
		setcolor(15);
		write_c(12, 9, "| 1 |   예\n");
		write_c(12, 10, "| 2 | 아니오\n");

		write_c(11, 12, ">> "); scanf_s(" %d", &warn);

	} while (warn != 1 && warn != 2);

	return warn;
}
