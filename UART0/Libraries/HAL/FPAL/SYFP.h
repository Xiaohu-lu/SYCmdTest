/***************************************************************************************
* File name    :	SYFP.h
* Function     :	
* Author       : 	Yang
* Date         :	2015/03/16
* Version      :  v0.1
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SYFP_H_
#define _SYFP_H_

typedef int (*ReadBaseMProc)(int,unsigned char*,unsigned int,unsigned int);

/*******************************************************************************
����ԭ�ͣ�	void YEGetVersion(UINT8* pStr)
����˵����	��ȡָ���㷨�汾�š�
����˵����	pStr -- 100�ֽڵ��ַ�����
����ֵ��	  ��
********************************************************************************/
void YEGetVersion(UINT8* pStr);

/*******************************************************************************
����ԭ�ͣ�	INT32 ImageToChar3_1(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv)
����˵����	��ȡ������һ��
����˵����	
            pImage -- ָ��ͼ��ָ��
            pBuffer -- 80K�ֽڻ���ռ�ָ��
            pF -- 384�ֽ���������ָ�룬����������ΪNULL��
            Conv -- �������Ʒ�����ָ��¼��ʱȡ120������ʱȡ1000��
����ֵ��	  
            -1 -- ��������
            0~100 -- ָ�����
********************************************************************************/
INT32 ImageToChar3_1(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv);

/*******************************************************************************
����ԭ�ͣ�	INT32 ImageToChar3_2(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv)
����˵����	��ȡ������һ��
����˵����	
            pImage -- ָ��ͼ��ָ��
            pBuffer -- 80K�ֽڻ���ռ�ָ��
            pF -- 384�ֽ���������ָ��
            Conv -- �������Ʒ�����ָ��¼��ʱȡ120������ʱȡ1000��
����ֵ��	  
            0 -- �ɹ�
            ���� -- ʧ��
********************************************************************************/
INT32 ImageToChar3_2(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv);

///*******************************************************************************
//����ԭ�ͣ�	INT32 MatchTwoM(UINT8* pF1,UINT8* pF2)
//����˵����	�ȶ�2�������õ����Ʒ���
//����˵����	
//            pF1 -- �ȶ�����1
//            pF2 -- �ȶ�����2
//�� �� ֵ��	���Ʒ���
//********************************************************************************/
//INT32 MatchTwoM(UINT8* pF1,UINT8* pF2);

/*******************************************************************************
����ԭ�ͣ�	INT16 LMatchScore(UINT8 *pF1,UINT8 *pF2,UINT8* pBuffer,UINT32 BufferSize,UINT8 flag)
����˵����	�ȶ�2�������õ����Ʒ���
����˵����	
            pF1 -- �ȶ�����1
            pF2 -- �ȶ�����2
            pBuffer -- ����ָ��
            BufferSize -- �����С��>=70KB��
            flag -- ��������������Ϊ0
�� �� ֵ��	���Ʒ���
********************************************************************************/
INT16 LMatchScore(UINT8 *pF1,UINT8 *pF2,UINT8* pBuffer,UINT32 BufferSize,UINT8 flag);

///*******************************************************************************
//����ԭ�ͣ�	INT32 QuickSearch3(UINT8* Feature,ReadBaseMProc ReadFeatureFunctionM,
//            UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchScore,
//            UINT16* pFindID,UINT16* pScore)
//����˵����	��ָ�ƿ�������ָ������
//����˵����  
//            Feature -- ����������
//            ReadFeatureFunctionM -- ��ȡģ��ص�����
//            StartID -- ������ʼID
//            SearchIDNumber -- ����ģ�����
//            SearchIndex -- ģ��������
//            MatchScore -- �����ɹ���ֵ������ȡ50
//            pFindID -- ������ģ��ID��
//            pScore -- �������÷�
//�� �� ֵ��	
//            0 -- �����ɹ�
//            ���� -- ����ʧ��
//********************************************************************************/
//INT32 QuickSearch3(UINT8* Feature,ReadBaseMProc ReadFeatureFunctionM,UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchScore,UINT16* pFindID,UINT16* pScore);

/*******************************************************************************
����ԭ�ͣ�	INT32 QuickSearchB(UINT8* Feature,ReadBaseMProc ReadFeatureFunctionM,
            UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchScore,
            UINT16* pFindID,UINT16* pScore,UINT8* pBuffer);
����˵����	��ָ�ƿ�������ָ������
����˵����  
            Feature -- ����������
            ReadFeatureFunctionM -- ��ȡģ��ص�����
            StartID -- ������ʼID
            SearchIDNumber -- ����ģ�����
            SearchIndex -- ģ��������
            MatchScore -- �����ɹ���ֵ������ȡ50
            pFindID -- ������ģ��ID��
            pScore -- �������÷�
            pBuffer -- ʹ�û��棨>=70KB��
�� �� ֵ��	
            0 -- �����ɹ�
            ���� -- ����ʧ��
********************************************************************************/
INT32 QuickSearchB(UINT8* Feature,ReadBaseMProc ReadMFunction,UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchOKScore,UINT16* pFindID,UINT16* pScore,UINT8* pBuffer);

#endif
