/* This program illustrates the use of the glut library for
interfacing with a window system */

#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE  3
#define POINTS 4
#define TEXT 5
#define CIRCLE 6
#define ERASER 7
#define PENCIL_ON 8
#define PENCIL_OFF 9

#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void mouse(int, int, int, int);									//마우스 클릭 함수
void key(unsigned char, int, int);								//키 입력 함수
void display(void);												//디스플레이 함수
void drawSquare(int, int);										//점 함수
void myReshape(GLsizei, GLsizei);								//윈도우창 변경시 설정
void myinit(void);												//윈도우창 설정
void screen_box(int, int, int);									//윈도우창 박스 함수
void right_menu(int);											//우클릭 메뉴
void middle_menu(int);											//휠클릭 메뉴
void color_menu(int);											//컬러 메뉴
void pixel_menu(int);											//픽셀 메뉴
void fill_menu(int);											//채움 메뉴
int pick(int, int);												//좌표에 따른 값 반환 함수

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                    추가한 함수                                                     */
void eraser(int, int);											//지우개 함수
void mouse_move(GLint, GLint);									//마우스 이동 함수
void pencil(int, int, int, int);								//자유그리기 함수
void line_thick_menu(int);										//선 굵기 조절 함수
void dot_menu(int);												//점선 온오프 함수
void dot_dis_menu(int);											//점선 간격 조절 함수
void eraser_size(int);											//지우개 크기 조절 함수
void aliasing(int);												//안티에일리어싱 온오프 함수

/* globals */

GLsizei wh = 500, ww = 500;										/* initial window size */
GLfloat size = 1.0;												/* half side length of square */
int draw_mode = 0;												/* drawing mode */
int rx, ry;														/*raster position*/
GLfloat r = 1.0, g = 1.0, b = 1.0;								/* drawing color */
int fill = 0;													/* fill flag */

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*									    추가한 변수  										     	  */
int dot = 0;													//점선 온오프 변수
int dot_dis = 1;												//점선 간격 변수
int eraser_size_value = 6;										//지우개 크기 변수
int line_thick = 1;												//선 굵기 변수
int start = 0;													//연필 초기화 변수
int bx, by, tx, ty;												//연필 좌표 변수
int aliasing_value = 0;											//안티에일리어싱 온오프 변수
int List[100000000];											//디스플레이 리스트
static int List_count = 0;										//리스트 카운트

void drawSquare(int x, int y)									//점 찍기
{
	y = wh - y;
	glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);	//색 랜덤
	glBegin(GL_POLYGON);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glEnd();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*								     추가한 지우개 기능                                               */
/*                                덮어씌우는 형식으로 구현                                            */
/*                                      원형 지우개                                                   */
/*                                     크기 조절 기능                                                 */
/*                    휠클릭 Eraser size 메뉴를 통해 지우개 크기 조절 가능                            */	
void eraser(int x, int y)									
{																
	glColor3f(0.0, 0.0, 0.0);									
	glBegin(GL_POLYGON);
	double xc[360], yc[360];
	for (int i = 0; i < 360; i++)
	{
		float angle;
		angle = i*3.141592 / 180;
		xc[i] = eraser_size_value*cos(angle);
		yc[i] = eraser_size_value*sin(angle);
		glVertex2i(x + xc[i], (wh - y) + yc[i]);
	}
	glEnd();
}

/* rehaping routine called whenever window is resized
or moved */

void myReshape(GLsizei w, GLsizei h)
{

	/* adjust clipping box */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* adjust viewport and  clear */
	glViewport(0, 0, w, h);
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                   추가한 블랜딩 기능                                               */
/*                          원활한 안티에일리어싱 사용을 위해 추가                                    */
	glEnable(GL_BLEND);											
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*								     추가한 안티에일리어싱 기능							         	  */
/*                               계단현상을 완화시키기 위하여 추가                                    */
	if (aliasing_value == 1) {									//안티에일리어싱 온오프 기능
		glEnable(GL_LINE_SMOOTH);								//선 안티에일리어싱 활성화
		glEnable(GL_POINT_SMOOTH);								//점 안티에일리어싱 활성화
		glEnable(GL_POLYGON_SMOOTH);							//다각형 안티에일리어싱 활성화
	}
	else {
		glDisable(GL_LINE_SMOOTH);								//선 안티에일리어싱 비활성화
		glDisable(GL_POINT_SMOOTH);								//점 안티에일리어싱 비활성화
		glDisable(GL_POLYGON_SMOOTH);							//다각형 안티에일리어싱 비활성화
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                               추가한 점선 기능                                                    */
	glEnable(GL_LINE_STIPPLE);									//점선 기능 활성화


	display();													
	glFlush();

	/* set global size for use by drawing routine */
	ww = w;
	wh = h;
}

void myinit(void)												//윈도우창 설정
{
	glViewport(0, 0, ww, wh);

	/* Pick 2D clipping window to match size of X window
	This choice avoids having to scale object coordinates
	each time window is resized */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1, 1);

	/* set clear color to black and clear window */
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


void mouse(int btn, int state, int x, int y)								
{
	static int count;											//그리기시 정점 개수 설정 변수
	int where;													//클릭한 좌표에 해당하는 값 저장 변수
	static int xp[2], yp[2];									//정점 저장 배열
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)			//왼쪽클릭시
	{
		start = 1;												//연필 초기화 변수

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                 추가한 디스플레이 리스트 기능                                     */
/*                윈도우창에 그려지는 모든 것은 디스플레이 리스트에 저장 및 출력되도록 구현          */
/*         디스플레이 리스트를 이용하여 윈도우창 크기 변경시에도 그렸던 것들이 사라지지않게 구현     */
/*               키 콜백과 함께 사용하여 백스페이스키 누를 시 작업 취소 기능 구현                    */
		List[List_count] = glGenLists(1);						//디스플레이 리스트 선언
		glNewList(List[List_count], GL_COMPILE_AND_EXECUTE);	//리스트 저장 시작

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		where = pick(x, y);										//클릭한 좌표의 값 저장
		glColor3f(r, g, b);										//색 지정

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                  추가한 선 굵기 조절 기능                                         */
/*                           glLineWidth() 함수를 사용하여 선 굵기 조절 가능                         */
/*                        휠클릭 Line Thick 메뉴를 통해 원하는 선 굵기 선택 가능                     */
		glLineWidth(line_thick); 

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                  추가한 점선 기능                                                 */
/*                          glLineStipple() 함수를 이용하여 점선 구현                                */
/*                          dot_dis() 함수를 이용하여 점선 간격 조절 기능 구현                       */
/*                     휠클릭 Dotted Line Setting 메뉴를 통해 원하는 간격 조절 가능                  */
		if (dot) glLineStipple(dot_dis, 0xAAAA);				//점선 기능 온오프

		if (where != 0)											//클릭한 곳이 그리기 기능이 있다면
		{
			count = 0;											//그리기시 정점 개수 설정 변수 초기화
			draw_mode = where;									//드로잉모드에 그리기 모드 저장
		}
		else switch (draw_mode)									//삼각형인지 원인지 사각형인지에 따라
		{
		case(LINE):												//선 그리기
			if (count == 0)										//정점 0개 그렸다면
			{
				count++;										//정점 개수 + 1
				xp[0] = x;										//정점 배열에 클릭한 좌표 저장
				yp[0] = y;
			}
			else
			{
				glBegin(GL_LINES);								//직선 그리기
				glVertex2i(x, wh - y);
				glVertex2i(xp[0], wh - yp[0]);
				glEnd();
				if (count == 1) List_count++;					//그렸다면 리스트 값 + 1
				draw_mode = 0;									//직선 선택 해제
				count = 0;										//다시 좌표 받을 수 있도록 초기화
			} 
			break;
			
		case(RECTANGLE):										//직사각형 그리기
			if (count == 0)										//정점 0개 그렸다면
			{
				count++;										//정점 개수 + 1
				xp[0] = x;										//정점 배열에 클릭한 좌표 저장
				yp[0] = y;
			}
			else
			{
				if (fill) glBegin(GL_POLYGON);					//채움 기능 온오프
				else glBegin(GL_LINE_LOOP);						//사각형 그리기
				glVertex2i(x, wh - y);
				glVertex2i(x, wh - yp[0]);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[0], wh - y);
				glEnd();
				if (count == 1) List_count++;					//그렸다면 리스트 값 + 1
				draw_mode = 0;									//사각형 선택 해제
				count = 0;										//다시 좌표 받을 수 있도록 초기화
				
			}
			break;
	
		case (TRIANGLE):										//삼각형 그리기
			switch (count)										//정점 결정 변수받아서
			{
			case(0):											//정점 0개 그렸다면
				count++;										//정점 개수 + 1 
				xp[0] = x;										//정점 배열에 클릭한 좌표 저장
				yp[0] = y;
				break;
			case(1):											//정점 1개 그렸다면
				count++;										//정점 개수 + 1 
				xp[1] = x;										//정점 배열에 클릭한 좌표 저장
				yp[1] = y;
				break;
			case(2):											//정점 2개 그렸다면
				if (fill) glBegin(GL_POLYGON);					//채움 기능 온오프
				else glBegin(GL_LINE_LOOP);						//삼각형 그리기
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[1], wh - yp[1]);
				glVertex2i(x, wh - y);
				glEnd();
				if (count == 2) List_count++;					//그렸다면 리스트 값 + 1
				draw_mode = 0;									//삼각형 선택 해제
				count = 0;										//다시 좌표 받을 수 있도록 초기화
			}
			break;

		case(POINTS):											//점 그리기
		{
			drawSquare(x, y);									//점 그리는 함수 사용
			count = 0;											//다시 좌표 받을 수 있도록 초기화
			List_count++;										//그렸다면 리스트 값 + 1
		}
		break;

		case(TEXT):												//텍스트 그리기
		{
			rx = x;
			ry = wh - y;
			glRasterPos2i(rx, ry);
			count = 0;											//다시 좌표 받을 수 있도록 초기화
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                  추가한 원 그리기 기능                                            */
/*                               극좌표를 이용한 원그리기 구현                                       */
/*                         반지름: 클릭한 첫번째 좌표와 두번째 좌표 사이의 거리의 반                 */
		case(CIRCLE):											//원 그리기
		{
			if (count == 0)										//정점 0개 그렸다면
			{
				count++;										//정점 개수 + 1
				xp[0] = x;										//정점 배열에 클릭한 좌표 저장
				yp[0] = y;
				yp[1] = wh - yp[0];
			}
			else
			{
				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_LINE_LOOP);
				double xc[360], yc[360];
				double rad = sqrt((x - xp[0])*(x - xp[0]) + (y - yp[0])*(y - yp[0])) / 2;
				for (int i = 0; i < 360; i++) 
				{
					float angle = i * 3.141592 / 180;
					xc[i] = rad*cos(angle);
					yc[i] = rad*sin(angle);
					glVertex2i((xp[0] + x)/2 + xc[i], (yp[1]+(wh - y))/2 + yc[i]);
				}
				glEnd();
				if(count == 1) List_count++;
				draw_mode = 0;									//원그리기 선택 해제
				count = 0;										//다시 좌표 받을 수 있도록 초기화
			}
			break;
		}
		case(ERASER):											//지우개 기능
		{
			eraser(x,y);
			count = 0;											//다시 좌표 받을 수 있도록 초기화
			List_count++;										//그렸다면 리스트 값 + 1
		}
		}
		glEndList();											//리스트 저장 종료
	
		glPopAttrib();
		glFlush();
	}
}

int pick(int x, int y)											//클릭한 좌표에 따른 그리기 기능 반환
{
	y = wh - y;
	if (y < wh - ww / 10) return 0;
	else if (x < ww / 10) return LINE;							//0~50		직선
	else if (x < ww / 5) return RECTANGLE;						//50~100	사각형
	else if (x < 3 * ww / 10) return TRIANGLE;					//100~150	삼각형
	else if (x < 2 * ww / 5) return POINTS;						//150~200	점
	else if (x < ww / 2) return TEXT;							//200~250	문자
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                클릭한 위치에 따른 기능 반환                                       */
/*                             각 기능이 해당하는 좌표에 메뉴판 구현                                 */
	else if (x < 6 * ww / 10) return CIRCLE;					//250~300	원
	else if (x < 7 * ww / 10 + 25) return ERASER;				//300~375	지우개
	else if (x < 9 * ww / 10) return PENCIL_ON;					//375~450	연필켜기
	else if (x < ww) return PENCIL_OFF;							//450~500	연필끄기
	else return 0;
}

void pencil(int tx, int ty, int bx, int by)						//연필 기능
{
	glColor3f(r, g, b);
	glLineWidth(line_thick);
	if (dot) glLineStipple(dot_dis, 0xAAAA);
	glBegin(GL_LINES);
	glVertex2i(tx, wh - ty);
	glVertex2i(bx, wh - by);
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                추가한 연필 기능                                                   */
/*                       glutMoitionFunc() 함수 이용하여 구현                                        */
/*                       드래그시 각 좌표를 받아서 선으로 연결하여 구현                              */
/*                       이전 좌표와 이후 좌표를 계속해서 저장, 변경하며 구현                        */
/*         시작좌표를 설정하는 start 변수를 클릭할때마다 초기화 시켜줌으로써 끊어그리기 구현         */
void mouse_move(GLint x, GLint y)				
{
	switch (draw_mode)											//드로잉모드 검사
	{
	case(PENCIL_ON):											//연필 기능일때 사용
		List[List_count] = glGenLists(1);
		glNewList(List[List_count], GL_COMPILE_AND_EXECUTE);
		bx = x;													//좌표 저장
		by = y;
		if (start == 1) {										//시작했다면 시작좌표 설정
			tx = bx;
			ty = by;
			start = 0;
		}
		pencil(tx, ty, bx, by);									//pencil()함수 사용하여 그리기
		glutPostRedisplay();									//윈도우창 새로고침
		tx = bx;												//좌표 저장
		ty = by;
		glEndList();	
		List_count++;
	}
}

void screen_box(int x, int y, int s)							//윈도우창 박스 함수
{
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);
	glEnd();
}

void right_menu(int id)											//우클릭 메뉴 동작
{
	if (id == 1) exit(0);
	else {
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                  Clear시 디스플레이 리스트 초기화, 리스트 카운트 변수 초기화                      */
		for (int i = 0; i < List_count; i++) {
			glDeleteLists(List[i], 1);
			List_count = 0;
		}
		display();
	}
}

void middle_menu(int id)										//휠클릭 메뉴 동작
{

}

void color_menu(int id)											//컬러 메뉴
{
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; }
	else if (id == 4) { r = 0.0; g = 1.0; b = 1.0; }
	else if (id == 5) { r = 1.0; g = 0.0; b = 1.0; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; }
	else if (id == 7) { r = 1.0; g = 1.0; b = 1.0; }
	else if (id == 8) { r = 0.0; g = 0.0; b = 0.0; }
}


void pixel_menu(int id)											//픽셀 크기 조절
{
	if (id == 1) size = 2 * size;
	else if (size > 1) size = size / 2;
}

void fill_menu(int id)											//채움 온오프
{
	if (id == 1) {
		dot = 0;
		fill = 1;
	}
	else fill = 0;
}

void line_thick_menu(int id)									//선 굵기 조절
{
	line_thick = id;
}

void dot_menu(int id)											//점선 온오프
{
	if (id == 1) {
		fill = 0;
		dot = 1;
	}
	else  dot = 0;
}

void dot_dis_menu(int id)										//점선 간격 조절
{
	dot_dis = id;
}

void eraser_size(int id)										//지우개 크기 조절
{
	eraser_size_value = id;
}

void aliasing(int id)											//안티에일리어싱 온오프
{
	aliasing_value = id;
}

void key(unsigned char k, int xx, int yy)						//입력 키 처리
{
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                키 콜백 함수와 디스플레이 리스트를 사용하여 작업 취소 기능 구현                    */
/*                                  백스페이스 키를 누를 시 작동                                     */
/*         가장 최근에 저장된 디스플레이 리스트를 제거하고 리스트 카운트 변수 - 1 함으로써 구현      */
	if (draw_mode != TEXT)										
	{
		switch (k) {
		case 8:													//백스페이스 키(아스키코드 8)에 반응
			if (List_count == 0) {
				break;
			}
			glDeleteLists(List[List_count], 1);							
			List_count--;
			glutPostRedisplay();
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                 문자 입력시 윈도우창에 찍어내는 기능                              */
/*                                    문자 입력 후 엔터키 입력시 종료                                */
/*                             문자 입력할때마다 윈도우창에 찍어내도록 수정                          */
	if (draw_mode == TEXT) {									
		if (k == 13) {											//엔터키(아스키코드 13)에 반응
			draw_mode = 0;										//종료
		} else {
			List[List_count] = glGenLists(1);
			glNewList(List[List_count], GL_COMPILE_AND_EXECUTE);
			glColor3f(r, g, b);
			glRasterPos2i(rx, ry);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, k);
			/*glutStrokeCharacter(GLUT_STROKE_ROMAN,i); */
			rx += glutBitmapWidth(GLUT_BITMAP_9_BY_15, k);
			glEndList();
			glutPostRedisplay();
			List_count++;
		}
	}
}

void display(void)
{
	int shift = 0;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//배경				glClearColor, glclear로 설정하여도 적용이 되지않아 추가하여 배경색으로 사용
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2i(0, 0);
	glVertex2i(ww, 0);
	glVertex2i(ww, wh);
	glVertex2i(0, wh);
	glEnd();
	
	//메뉴 박스 그리기
	glColor3f(1.0, 1.0, 1.0);
	screen_box(0, wh - ww / 10, ww / 10);
	glColor3f(1.0, 0.0, 0.0);
	screen_box(ww / 10, wh - ww / 10, ww / 10);
	glColor3f(0.0, 1.0, 0.0);
	screen_box(ww / 5, wh - ww / 10, ww / 10);
	glColor3f(0.0, 0.0, 1.0);
	screen_box(3 * ww / 10, wh - ww / 10, ww / 10);
	glColor3f(1.0, 1.0, 0.0);
	screen_box(2 * ww / 5, wh - ww / 10, ww / 10);
	glColor3f(0.0, 1.0, 1.0);
	screen_box(5 * ww / 10, wh - ww / 10, ww / 10);
	glColor3f(1.0, 0.0, 1.0);
	screen_box(6 * ww / 10, wh - ww / 10, ww / 10 * 1.5);
	glColor3f(0.5, 0.5, 0.5);
	screen_box(7 * ww / 10 + ww / 20, wh - ww / 10, ww / 10 * 2.5);
	glColor3f(0.0, 0.0, 0.0);
	screen_box(ww / 10 + ww / 40 , wh - ww / 10 + ww / 40, ww / 20);

	
	//선 그려놓기
	glBegin(GL_LINES);
	glVertex2i(wh / 40, wh - ww / 20);
	glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
	glEnd();

	//삼각형 그려놓기
	glBegin(GL_TRIANGLES);
	glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
	glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
	glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
	glEnd();

	//점 그려놓기
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2i(3 * ww / 10 + ww / 20, wh - ww / 20);
	glEnd();

	//ABC 문자 찍어놓기
	glRasterPos2i(2 * ww / 5 + 12, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(2 * ww / 5 + 12 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
	glRasterPos2i(2 * ww / 5 + 12 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');

	//원 그려놓기
	glBegin(GL_POLYGON);
	double xp[360], yp[360];
	for (int i = 0; i < 360; i++) {
		float angle;
		int rad = 10;
		angle = i*3.141592 / 180;
		xp[i] = rad*cos(angle);
		yp[i] = rad*sin(angle);
		glVertex2f(11 * ww / 20 + xp[i], wh - ww / 20 + yp[i]);
	}
	glEnd();

	//지우개 문자 찍어놓기
	glRasterPos2i(3 * ww / 5 + ww / 40, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'E');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'E');
	glRasterPos2i(3 * ww / 5 + ww / 40 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'R');
	glRasterPos2i(3 * ww / 5 + ww / 40 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(3 * ww / 5 + ww / 40 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'S');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'S');
	glRasterPos2i(3 * ww / 5 + ww / 40 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'E');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'E');
	glRasterPos2i(3 * ww / 5 + ww / 40 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
	
	//연필 문자 찍어놓기
	glRasterPos2i(8.2 * ww / 10, wh - ww / 20 + 9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'P');
	glRasterPos2i(8.2 * ww / 10 + shift, wh - ww / 20 + 9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'E');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'E');
	glRasterPos2i(8.2 * ww / 10 + shift, wh - ww / 20 + 9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'N');
	glRasterPos2i(8.2 * ww / 10 + shift, wh - ww / 20 + 9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'C');
	glRasterPos2i(8.2 * ww / 10 + shift, wh - ww / 20 + 9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'I');
	glRasterPos2i(8.2 * ww / 10 + shift, wh - ww / 20 + 9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'L');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'L');
	glRasterPos2i(7.9 * ww / 10, wh - ww / 20 - 10);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'O');
	glRasterPos2i(7.9 * ww / 10 + shift, wh - ww / 20 - 10);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'N');
	glRasterPos2i(9 * ww / 10, wh - ww / 20 - 10);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'O');
	glRasterPos2i(9 * ww / 10 + shift, wh - ww / 20 - 10);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'F');
	glRasterPos2i(9 * ww / 10 + shift, wh - ww / 20 - 10);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'F');

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                       디스플레이 리스트에 저장된 것들을 불러와서 출력                             */
	for (int i = 0; i < List_count; i++)
	{
		glCallList(List[i]);
	}
	glFlush();
	glPopAttrib();
}


int main(int argc, char** argv)
{
	int c_menu, p_menu, f_menu, l_menu, d_menu,d_s_menu, e_menu, a_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("그림판");
	glutDisplayFunc(display);
	
	//컬러 메뉴
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);
	
	//픽셀 메뉴
	p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("increase pixel size", 1);
	glutAddMenuEntry("decrease pixel size", 2);

	//선굵기 메뉴
	l_menu = glutCreateMenu(line_thick_menu);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("20", 20);
	glutAddMenuEntry("30", 30);

	//채움 메뉴
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);

	//점선 메뉴
	d_menu = glutCreateMenu(dot_menu);
	glutAddMenuEntry("dot on", 1);
	glutAddMenuEntry("dot off", 2);

	//점선 간격 메뉴
	d_s_menu = glutCreateMenu(dot_dis_menu);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("7", 7);
	glutAddMenuEntry("8", 8);
	glutAddMenuEntry("9", 9);
	glutAddMenuEntry("10", 10);

	//지우개 크기 메뉴
	e_menu = glutCreateMenu(eraser_size);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("7", 7);
	glutAddMenuEntry("8", 8);
	glutAddMenuEntry("9", 9);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("15", 15);
	glutAddMenuEntry("20", 20);
	glutAddMenuEntry("30", 30);
	glutAddMenuEntry("50", 50);

	//안티에일리어싱 메뉴
	a_menu = glutCreateMenu(aliasing);
	glutAddMenuEntry("Anti on", 1);
	glutAddMenuEntry("Anti off", 2);

	//우클릭 메뉴
	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("clear", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//휠 클릭 메뉴
	glutCreateMenu(middle_menu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Pixel Size", p_menu);
	glutAddSubMenu("Line Thick", l_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAddSubMenu("Dotted Line", d_menu);
	glutAddSubMenu("Dotted Line Setting", d_s_menu);
	glutAddSubMenu("Eraser size",e_menu);
	glutAddSubMenu("Antialiasing", a_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	myinit();
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_move);
	glutMainLoop();
	
}
