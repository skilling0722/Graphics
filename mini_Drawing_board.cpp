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

void mouse(int, int, int, int);									//���콺 Ŭ�� �Լ�
void key(unsigned char, int, int);								//Ű �Է� �Լ�
void display(void);												//���÷��� �Լ�
void drawSquare(int, int);										//�� �Լ�
void myReshape(GLsizei, GLsizei);								//������â ����� ����
void myinit(void);												//������â ����
void screen_box(int, int, int);									//������â �ڽ� �Լ�
void right_menu(int);											//��Ŭ�� �޴�
void middle_menu(int);											//��Ŭ�� �޴�
void color_menu(int);											//�÷� �޴�
void pixel_menu(int);											//�ȼ� �޴�
void fill_menu(int);											//ä�� �޴�
int pick(int, int);												//��ǥ�� ���� �� ��ȯ �Լ�

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                    �߰��� �Լ�                                                     */
void eraser(int, int);											//���찳 �Լ�
void mouse_move(GLint, GLint);									//���콺 �̵� �Լ�
void pencil(int, int, int, int);								//�����׸��� �Լ�
void line_thick_menu(int);										//�� ���� ���� �Լ�
void dot_menu(int);												//���� �¿��� �Լ�
void dot_dis_menu(int);											//���� ���� ���� �Լ�
void eraser_size(int);											//���찳 ũ�� ���� �Լ�
void aliasing(int);												//��Ƽ���ϸ���� �¿��� �Լ�

/* globals */

GLsizei wh = 500, ww = 500;										/* initial window size */
GLfloat size = 1.0;												/* half side length of square */
int draw_mode = 0;												/* drawing mode */
int rx, ry;														/*raster position*/
GLfloat r = 1.0, g = 1.0, b = 1.0;								/* drawing color */
int fill = 0;													/* fill flag */

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*									    �߰��� ����  										     	  */
int dot = 0;													//���� �¿��� ����
int dot_dis = 1;												//���� ���� ����
int eraser_size_value = 6;										//���찳 ũ�� ����
int line_thick = 1;												//�� ���� ����
int start = 0;													//���� �ʱ�ȭ ����
int bx, by, tx, ty;												//���� ��ǥ ����
int aliasing_value = 0;											//��Ƽ���ϸ���� �¿��� ����
int List[100000000];											//���÷��� ����Ʈ
static int List_count = 0;										//����Ʈ ī��Ʈ

void drawSquare(int x, int y)									//�� ���
{
	y = wh - y;
	glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);	//�� ����
	glBegin(GL_POLYGON);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glEnd();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*								     �߰��� ���찳 ���                                               */
/*                                ������ �������� ����                                            */
/*                                      ���� ���찳                                                   */
/*                                     ũ�� ���� ���                                                 */
/*                    ��Ŭ�� Eraser size �޴��� ���� ���찳 ũ�� ���� ����                            */	
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
/*                                   �߰��� ���� ���                                               */
/*                          ��Ȱ�� ��Ƽ���ϸ���� ����� ���� �߰�                                    */
	glEnable(GL_BLEND);											
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*								     �߰��� ��Ƽ���ϸ���� ���							         	  */
/*                               ��������� ��ȭ��Ű�� ���Ͽ� �߰�                                    */
	if (aliasing_value == 1) {									//��Ƽ���ϸ���� �¿��� ���
		glEnable(GL_LINE_SMOOTH);								//�� ��Ƽ���ϸ���� Ȱ��ȭ
		glEnable(GL_POINT_SMOOTH);								//�� ��Ƽ���ϸ���� Ȱ��ȭ
		glEnable(GL_POLYGON_SMOOTH);							//�ٰ��� ��Ƽ���ϸ���� Ȱ��ȭ
	}
	else {
		glDisable(GL_LINE_SMOOTH);								//�� ��Ƽ���ϸ���� ��Ȱ��ȭ
		glDisable(GL_POINT_SMOOTH);								//�� ��Ƽ���ϸ���� ��Ȱ��ȭ
		glDisable(GL_POLYGON_SMOOTH);							//�ٰ��� ��Ƽ���ϸ���� ��Ȱ��ȭ
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                               �߰��� ���� ���                                                    */
	glEnable(GL_LINE_STIPPLE);									//���� ��� Ȱ��ȭ


	display();													
	glFlush();

	/* set global size for use by drawing routine */
	ww = w;
	wh = h;
}

void myinit(void)												//������â ����
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
	static int count;											//�׸���� ���� ���� ���� ����
	int where;													//Ŭ���� ��ǥ�� �ش��ϴ� �� ���� ����
	static int xp[2], yp[2];									//���� ���� �迭
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)			//����Ŭ����
	{
		start = 1;												//���� �ʱ�ȭ ����

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                 �߰��� ���÷��� ����Ʈ ���                                     */
/*                ������â�� �׷����� ��� ���� ���÷��� ����Ʈ�� ���� �� ��µǵ��� ����          */
/*         ���÷��� ����Ʈ�� �̿��Ͽ� ������â ũ�� ����ÿ��� �׷ȴ� �͵��� ��������ʰ� ����     */
/*               Ű �ݹ�� �Բ� ����Ͽ� �齺���̽�Ű ���� �� �۾� ��� ��� ����                    */
		List[List_count] = glGenLists(1);						//���÷��� ����Ʈ ����
		glNewList(List[List_count], GL_COMPILE_AND_EXECUTE);	//����Ʈ ���� ����

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		where = pick(x, y);										//Ŭ���� ��ǥ�� �� ����
		glColor3f(r, g, b);										//�� ����

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                  �߰��� �� ���� ���� ���                                         */
/*                           glLineWidth() �Լ��� ����Ͽ� �� ���� ���� ����                         */
/*                        ��Ŭ�� Line Thick �޴��� ���� ���ϴ� �� ���� ���� ����                     */
		glLineWidth(line_thick); 

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                  �߰��� ���� ���                                                 */
/*                          glLineStipple() �Լ��� �̿��Ͽ� ���� ����                                */
/*                          dot_dis() �Լ��� �̿��Ͽ� ���� ���� ���� ��� ����                       */
/*                     ��Ŭ�� Dotted Line Setting �޴��� ���� ���ϴ� ���� ���� ����                  */
		if (dot) glLineStipple(dot_dis, 0xAAAA);				//���� ��� �¿���

		if (where != 0)											//Ŭ���� ���� �׸��� ����� �ִٸ�
		{
			count = 0;											//�׸���� ���� ���� ���� ���� �ʱ�ȭ
			draw_mode = where;									//����׸�忡 �׸��� ��� ����
		}
		else switch (draw_mode)									//�ﰢ������ ������ �簢�������� ����
		{
		case(LINE):												//�� �׸���
			if (count == 0)										//���� 0�� �׷ȴٸ�
			{
				count++;										//���� ���� + 1
				xp[0] = x;										//���� �迭�� Ŭ���� ��ǥ ����
				yp[0] = y;
			}
			else
			{
				glBegin(GL_LINES);								//���� �׸���
				glVertex2i(x, wh - y);
				glVertex2i(xp[0], wh - yp[0]);
				glEnd();
				if (count == 1) List_count++;					//�׷ȴٸ� ����Ʈ �� + 1
				draw_mode = 0;									//���� ���� ����
				count = 0;										//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
			} 
			break;
			
		case(RECTANGLE):										//���簢�� �׸���
			if (count == 0)										//���� 0�� �׷ȴٸ�
			{
				count++;										//���� ���� + 1
				xp[0] = x;										//���� �迭�� Ŭ���� ��ǥ ����
				yp[0] = y;
			}
			else
			{
				if (fill) glBegin(GL_POLYGON);					//ä�� ��� �¿���
				else glBegin(GL_LINE_LOOP);						//�簢�� �׸���
				glVertex2i(x, wh - y);
				glVertex2i(x, wh - yp[0]);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[0], wh - y);
				glEnd();
				if (count == 1) List_count++;					//�׷ȴٸ� ����Ʈ �� + 1
				draw_mode = 0;									//�簢�� ���� ����
				count = 0;										//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
				
			}
			break;
	
		case (TRIANGLE):										//�ﰢ�� �׸���
			switch (count)										//���� ���� �����޾Ƽ�
			{
			case(0):											//���� 0�� �׷ȴٸ�
				count++;										//���� ���� + 1 
				xp[0] = x;										//���� �迭�� Ŭ���� ��ǥ ����
				yp[0] = y;
				break;
			case(1):											//���� 1�� �׷ȴٸ�
				count++;										//���� ���� + 1 
				xp[1] = x;										//���� �迭�� Ŭ���� ��ǥ ����
				yp[1] = y;
				break;
			case(2):											//���� 2�� �׷ȴٸ�
				if (fill) glBegin(GL_POLYGON);					//ä�� ��� �¿���
				else glBegin(GL_LINE_LOOP);						//�ﰢ�� �׸���
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[1], wh - yp[1]);
				glVertex2i(x, wh - y);
				glEnd();
				if (count == 2) List_count++;					//�׷ȴٸ� ����Ʈ �� + 1
				draw_mode = 0;									//�ﰢ�� ���� ����
				count = 0;										//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
			}
			break;

		case(POINTS):											//�� �׸���
		{
			drawSquare(x, y);									//�� �׸��� �Լ� ���
			count = 0;											//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
			List_count++;										//�׷ȴٸ� ����Ʈ �� + 1
		}
		break;

		case(TEXT):												//�ؽ�Ʈ �׸���
		{
			rx = x;
			ry = wh - y;
			glRasterPos2i(rx, ry);
			count = 0;											//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                  �߰��� �� �׸��� ���                                            */
/*                               ����ǥ�� �̿��� ���׸��� ����                                       */
/*                         ������: Ŭ���� ù��° ��ǥ�� �ι�° ��ǥ ������ �Ÿ��� ��                 */
		case(CIRCLE):											//�� �׸���
		{
			if (count == 0)										//���� 0�� �׷ȴٸ�
			{
				count++;										//���� ���� + 1
				xp[0] = x;										//���� �迭�� Ŭ���� ��ǥ ����
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
				draw_mode = 0;									//���׸��� ���� ����
				count = 0;										//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
			}
			break;
		}
		case(ERASER):											//���찳 ���
		{
			eraser(x,y);
			count = 0;											//�ٽ� ��ǥ ���� �� �ֵ��� �ʱ�ȭ
			List_count++;										//�׷ȴٸ� ����Ʈ �� + 1
		}
		}
		glEndList();											//����Ʈ ���� ����
	
		glPopAttrib();
		glFlush();
	}
}

int pick(int x, int y)											//Ŭ���� ��ǥ�� ���� �׸��� ��� ��ȯ
{
	y = wh - y;
	if (y < wh - ww / 10) return 0;
	else if (x < ww / 10) return LINE;							//0~50		����
	else if (x < ww / 5) return RECTANGLE;						//50~100	�簢��
	else if (x < 3 * ww / 10) return TRIANGLE;					//100~150	�ﰢ��
	else if (x < 2 * ww / 5) return POINTS;						//150~200	��
	else if (x < ww / 2) return TEXT;							//200~250	����
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                Ŭ���� ��ġ�� ���� ��� ��ȯ                                       */
/*                             �� ����� �ش��ϴ� ��ǥ�� �޴��� ����                                 */
	else if (x < 6 * ww / 10) return CIRCLE;					//250~300	��
	else if (x < 7 * ww / 10 + 25) return ERASER;				//300~375	���찳
	else if (x < 9 * ww / 10) return PENCIL_ON;					//375~450	�����ѱ�
	else if (x < ww) return PENCIL_OFF;							//450~500	���ʲ���
	else return 0;
}

void pencil(int tx, int ty, int bx, int by)						//���� ���
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
/*                                �߰��� ���� ���                                                   */
/*                       glutMoitionFunc() �Լ� �̿��Ͽ� ����                                        */
/*                       �巡�׽� �� ��ǥ�� �޾Ƽ� ������ �����Ͽ� ����                              */
/*                       ���� ��ǥ�� ���� ��ǥ�� ����ؼ� ����, �����ϸ� ����                        */
/*         ������ǥ�� �����ϴ� start ������ Ŭ���Ҷ����� �ʱ�ȭ ���������ν� ����׸��� ����         */
void mouse_move(GLint x, GLint y)				
{
	switch (draw_mode)											//����׸�� �˻�
	{
	case(PENCIL_ON):											//���� ����϶� ���
		List[List_count] = glGenLists(1);
		glNewList(List[List_count], GL_COMPILE_AND_EXECUTE);
		bx = x;													//��ǥ ����
		by = y;
		if (start == 1) {										//�����ߴٸ� ������ǥ ����
			tx = bx;
			ty = by;
			start = 0;
		}
		pencil(tx, ty, bx, by);									//pencil()�Լ� ����Ͽ� �׸���
		glutPostRedisplay();									//������â ���ΰ�ħ
		tx = bx;												//��ǥ ����
		ty = by;
		glEndList();	
		List_count++;
	}
}

void screen_box(int x, int y, int s)							//������â �ڽ� �Լ�
{
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);
	glEnd();
}

void right_menu(int id)											//��Ŭ�� �޴� ����
{
	if (id == 1) exit(0);
	else {
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                  Clear�� ���÷��� ����Ʈ �ʱ�ȭ, ����Ʈ ī��Ʈ ���� �ʱ�ȭ                      */
		for (int i = 0; i < List_count; i++) {
			glDeleteLists(List[i], 1);
			List_count = 0;
		}
		display();
	}
}

void middle_menu(int id)										//��Ŭ�� �޴� ����
{

}

void color_menu(int id)											//�÷� �޴�
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


void pixel_menu(int id)											//�ȼ� ũ�� ����
{
	if (id == 1) size = 2 * size;
	else if (size > 1) size = size / 2;
}

void fill_menu(int id)											//ä�� �¿���
{
	if (id == 1) {
		dot = 0;
		fill = 1;
	}
	else fill = 0;
}

void line_thick_menu(int id)									//�� ���� ����
{
	line_thick = id;
}

void dot_menu(int id)											//���� �¿���
{
	if (id == 1) {
		fill = 0;
		dot = 1;
	}
	else  dot = 0;
}

void dot_dis_menu(int id)										//���� ���� ����
{
	dot_dis = id;
}

void eraser_size(int id)										//���찳 ũ�� ����
{
	eraser_size_value = id;
}

void aliasing(int id)											//��Ƽ���ϸ���� �¿���
{
	aliasing_value = id;
}

void key(unsigned char k, int xx, int yy)						//�Է� Ű ó��
{
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                Ű �ݹ� �Լ��� ���÷��� ����Ʈ�� ����Ͽ� �۾� ��� ��� ����                    */
/*                                  �齺���̽� Ű�� ���� �� �۵�                                     */
/*         ���� �ֱٿ� ����� ���÷��� ����Ʈ�� �����ϰ� ����Ʈ ī��Ʈ ���� - 1 �����ν� ����      */
	if (draw_mode != TEXT)										
	{
		switch (k) {
		case 8:													//�齺���̽� Ű(�ƽ�Ű�ڵ� 8)�� ����
			if (List_count == 0) {
				break;
			}
			glDeleteLists(List[List_count], 1);							
			List_count--;
			glutPostRedisplay();
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                 ���� �Է½� ������â�� ���� ���                              */
/*                                    ���� �Է� �� ����Ű �Է½� ����                                */
/*                             ���� �Է��Ҷ����� ������â�� ������ ����                          */
	if (draw_mode == TEXT) {									
		if (k == 13) {											//����Ű(�ƽ�Ű�ڵ� 13)�� ����
			draw_mode = 0;										//����
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
	
	//���				glClearColor, glclear�� �����Ͽ��� ������ �����ʾ� �߰��Ͽ� �������� ���
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2i(0, 0);
	glVertex2i(ww, 0);
	glVertex2i(ww, wh);
	glVertex2i(0, wh);
	glEnd();
	
	//�޴� �ڽ� �׸���
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

	
	//�� �׷�����
	glBegin(GL_LINES);
	glVertex2i(wh / 40, wh - ww / 20);
	glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
	glEnd();

	//�ﰢ�� �׷�����
	glBegin(GL_TRIANGLES);
	glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
	glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
	glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
	glEnd();

	//�� �׷�����
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2i(3 * ww / 10 + ww / 20, wh - ww / 20);
	glEnd();

	//ABC ���� ������
	glRasterPos2i(2 * ww / 5 + 12, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(2 * ww / 5 + 12 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
	glRasterPos2i(2 * ww / 5 + 12 + shift, wh - ww / 20 - 5);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');

	//�� �׷�����
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

	//���찳 ���� ������
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
	
	//���� ���� ������
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
/*                       ���÷��� ����Ʈ�� ����� �͵��� �ҷ��ͼ� ���                             */
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
	glutCreateWindow("�׸���");
	glutDisplayFunc(display);
	
	//�÷� �޴�
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);
	
	//�ȼ� �޴�
	p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("increase pixel size", 1);
	glutAddMenuEntry("decrease pixel size", 2);

	//������ �޴�
	l_menu = glutCreateMenu(line_thick_menu);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("20", 20);
	glutAddMenuEntry("30", 30);

	//ä�� �޴�
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);

	//���� �޴�
	d_menu = glutCreateMenu(dot_menu);
	glutAddMenuEntry("dot on", 1);
	glutAddMenuEntry("dot off", 2);

	//���� ���� �޴�
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

	//���찳 ũ�� �޴�
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

	//��Ƽ���ϸ���� �޴�
	a_menu = glutCreateMenu(aliasing);
	glutAddMenuEntry("Anti on", 1);
	glutAddMenuEntry("Anti off", 2);

	//��Ŭ�� �޴�
	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("clear", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//�� Ŭ�� �޴�
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
