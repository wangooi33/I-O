#ifndef _FONT_C_
#define _FONT_C_


int   Init_Font(void);  //��ʾ�����ǰ�ȵ��ñ��������г�ʼ��
void  UnInit_Font(void); //�����˳�ǰ�����ñ�����
int   Clean_Area(int X,  //x������ʼ��
                 int Y,  //y������ʼ��
				 int width, //���ƵĿ���
				 int height,//���Ƶĸ߶�
				 unsigned long color); //����Ļָ�����������ɫ
				 
int   Display_characterX(unsigned int x,          //x������ʼ��
                         unsigned int y,          //y������ʼ��
						 unsigned char *string,   //GB2312 �����ַ���
						 unsigned int color ,     //������ɫֵ
						 int size);               //����Ŵ��� 1~8
#endif
