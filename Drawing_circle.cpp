#include <math.h>
#include <glut.h>
#include <time.h>
#include <cstdio>


void CIRCLE1() {													//직교좌표 원그리기
	clock_t start, end;												//수행시간 체크 변수
	start = clock();												//시작시각
	glClear(GL_COLOR_BUFFER_BIT);									//컬퍼 버퍼 초기화
	glColor3f(0.0, 1.0, 0.0);										//녹색
	glBegin(GL_POINTS);												//점

	double r = 180;													//반지름 180
	double y;
	for (int i = 0; i < 1; i++) {									//i의 의미: 원을 그릴 횟수
		for (double x = -180; x <= 180; x += 0.1) {					//원 1개는 3600개의 점으로 구성
			y = sqrt((r*r) - (x*x));								
			glVertex2f(x, y);
			glVertex2f(x, -y);
		}
	}
	end = clock() - start;											//종료시각-시작시각
	printf("직교좌표 원그리기 ");
	printf("소요시간: %0.5f\n", (float)end / CLOCKS_PER_SEC);		//수행시간 출력
	glEnd();
	glFlush();
}

void CIRCLE2(){														//극좌표 원그리기
	clock_t start, end;												//수행시간 체크 변수
	start = clock();												//시작시간
	glClear(GL_COLOR_BUFFER_BIT);									//컬퍼 버퍼 초기화
	glColor3f(0.0, 1.0, 0.0);										//녹색
	glBegin(GL_POINTS);												//점

	double r = 636;													//반지름 636
	double pie = 3.141592;											//파이
	double x;
	double y;
	double angle;
	int k = 0;
	for (int i = 0; i < 1; i++) {								//i의 의미: 원을 그릴 횟수
		for (double t = 0; t < 360; t += 0.1) {						//원 1개는 3600개의 점으로 구성
			angle = t*pie / 180;
			x = r*cos(angle);
			y = r*sin(angle);
			glVertex2f(x, y);
			k++;
		}
	}
	end = clock() - start;											//종료시각-시작시각
	printf("극좌표 원그리기 ");
	printf("소요시간: %0.5f\n", (float)end / CLOCKS_PER_SEC);		//수행시간 출력
	printf("%d", k);
	glEnd();
	glFlush();
}

void bresenham(int x, int y) {										//30도에 해당하는좌표 입력 후
	glVertex2i(x, y);												//360도, 8방면으로 찍기
	glVertex2i(-x, y);
	glVertex2i(x, -y);
	glVertex2i(-x, -y);
	glVertex2i(y, x);
	glVertex2i(-y, x);
	glVertex2i(y, -x);
	glVertex2i(-y, -x);
}
void CIRCLE3() {													//브레스넘 알고리즘 원그리기
	clock_t start, end;												//수행시간 체크 변수
	start = clock();												//시작시간
	glClear(GL_COLOR_BUFFER_BIT);									//컬퍼 버퍼 초기화
	glColor3f(0.0, 1.0, 0.0);										//녹색
	glBegin(GL_POINTS);												//점

	int r = 636;													//반지름 636
	int x, y;
	int p = 3 - 2 * r;												//판별식
	for (int i = 0; i < 100000; i++) {									//i의 의미: 원을 그릴 횟수
		x = 0;														//첫 x 좌표
		y = 0 + r;													//첫 y 좌표
		while (x < y) {												//원 1개는 3600개의 점으로 구성
			bresenham(x, y);										//8방면에 찍기
			if (p < 0) {
				p = p + 4 * x + 6;
				x++;
			}
			else {
				p = p + 4 * (x - y) + 10;
				x++;
				y--;
			}
		}
		
	}
	end = clock() - start;											//종료시각-시작시각
	printf("브레스넘 알고리즘 원그리기 ");
	printf("소요시간: %0.5f\n", (float)end / CLOCKS_PER_SEC);		//수행시간 출력
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutCreateWindow("DRAW CIRCLE");
	glOrtho(-1000, 1000, -1000, 1000, -1, 1);
	glutDisplayFunc(CIRCLE3);
	glutMainLoop();
	return 0;
}

