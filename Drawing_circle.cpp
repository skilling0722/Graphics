#include <math.h>
#include <glut.h>
#include <time.h>
#include <cstdio>


void CIRCLE1() {													//������ǥ ���׸���
	clock_t start, end;												//����ð� üũ ����
	start = clock();												//���۽ð�
	glClear(GL_COLOR_BUFFER_BIT);									//���� ���� �ʱ�ȭ
	glColor3f(0.0, 1.0, 0.0);										//���
	glBegin(GL_POINTS);												//��

	double r = 180;													//������ 180
	double y;
	for (int i = 0; i < 1; i++) {									//i�� �ǹ�: ���� �׸� Ƚ��
		for (double x = -180; x <= 180; x += 0.1) {					//�� 1���� 3600���� ������ ����
			y = sqrt((r*r) - (x*x));								
			glVertex2f(x, y);
			glVertex2f(x, -y);
		}
	}
	end = clock() - start;											//����ð�-���۽ð�
	printf("������ǥ ���׸��� ");
	printf("�ҿ�ð�: %0.5f\n", (float)end / CLOCKS_PER_SEC);		//����ð� ���
	glEnd();
	glFlush();
}

void CIRCLE2(){														//����ǥ ���׸���
	clock_t start, end;												//����ð� üũ ����
	start = clock();												//���۽ð�
	glClear(GL_COLOR_BUFFER_BIT);									//���� ���� �ʱ�ȭ
	glColor3f(0.0, 1.0, 0.0);										//���
	glBegin(GL_POINTS);												//��

	double r = 636;													//������ 636
	double pie = 3.141592;											//����
	double x;
	double y;
	double angle;
	int k = 0;
	for (int i = 0; i < 1; i++) {								//i�� �ǹ�: ���� �׸� Ƚ��
		for (double t = 0; t < 360; t += 0.1) {						//�� 1���� 3600���� ������ ����
			angle = t*pie / 180;
			x = r*cos(angle);
			y = r*sin(angle);
			glVertex2f(x, y);
			k++;
		}
	}
	end = clock() - start;											//����ð�-���۽ð�
	printf("����ǥ ���׸��� ");
	printf("�ҿ�ð�: %0.5f\n", (float)end / CLOCKS_PER_SEC);		//����ð� ���
	printf("%d", k);
	glEnd();
	glFlush();
}

void bresenham(int x, int y) {										//30���� �ش��ϴ���ǥ �Է� ��
	glVertex2i(x, y);												//360��, 8������� ���
	glVertex2i(-x, y);
	glVertex2i(x, -y);
	glVertex2i(-x, -y);
	glVertex2i(y, x);
	glVertex2i(-y, x);
	glVertex2i(y, -x);
	glVertex2i(-y, -x);
}
void CIRCLE3() {													//�극���� �˰��� ���׸���
	clock_t start, end;												//����ð� üũ ����
	start = clock();												//���۽ð�
	glClear(GL_COLOR_BUFFER_BIT);									//���� ���� �ʱ�ȭ
	glColor3f(0.0, 1.0, 0.0);										//���
	glBegin(GL_POINTS);												//��

	int r = 636;													//������ 636
	int x, y;
	int p = 3 - 2 * r;												//�Ǻ���
	for (int i = 0; i < 100000; i++) {									//i�� �ǹ�: ���� �׸� Ƚ��
		x = 0;														//ù x ��ǥ
		y = 0 + r;													//ù y ��ǥ
		while (x < y) {												//�� 1���� 3600���� ������ ����
			bresenham(x, y);										//8��鿡 ���
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
	end = clock() - start;											//����ð�-���۽ð�
	printf("�극���� �˰��� ���׸��� ");
	printf("�ҿ�ð�: %0.5f\n", (float)end / CLOCKS_PER_SEC);		//����ð� ���
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

