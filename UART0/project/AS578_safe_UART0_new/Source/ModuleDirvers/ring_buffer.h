#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#define BUFFER_SIZE			1024		/*���λ�������С*/

typedef struct
{
	unsigned char buffer[BUFFER_SIZE];	/*�������ռ�*/
	volatile unsigned int pW;						/*д��ַ*/
	volatile unsigned int pR;						/*����ַ*/
}ring_buffer;	


/*
 *  ��������void ring_buffer_init(ring_buffer *dst_buf)
 *  ���������dst_buf --> ָ��Ŀ�껺����
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ��������
*/
extern void ring_buffer_init(ring_buffer *dst_buf);

/*
 *  ��������void ring_buffer_write(unsigned char c, ring_buffer *dst_buf)
 *  ���������c --> Ҫд�������
 *            dst_buf --> ָ��Ŀ�껺����
 *  �����������
 *  ����ֵ����
 *  �������ã���Ŀ�껺����д��һ���ֽڵ����ݣ�������������˾Ͷ���������
*/
extern void ring_buffer_write(unsigned char c, ring_buffer *dst_buf);

/*
 *  ��������int ring_buffer_read(unsigned char *c, ring_buffer *dst_buf)
 *  ���������c --> ָ�򽫶��������ݱ��浽�ڴ��еĵ�ַ
 *            dst_buf --> ָ��Ŀ�껺����
 *  �����������
 *  ����ֵ���������ݷ���0�����򷵻�-1
 *  �������ã���Ŀ�껺������ȡһ���ֽڵ����ݣ�������������˷���-1������ȡʧ��
*/
extern int ring_buffer_read(unsigned char *c, ring_buffer *dst_buf);

#endif


