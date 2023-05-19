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
函数原型：	void YEGetVersion(UINT8* pStr)
函数说明：	获取指纹算法版本号。
参数说明：	pStr -- 100字节的字符串。
返回值：	  无
********************************************************************************/
void YEGetVersion(UINT8* pStr);

/*******************************************************************************
函数原型：	INT32 ImageToChar3_1(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv)
函数说明：	提取特征第一步
参数说明：	
            pImage -- 指纹图像指针
            pBuffer -- 80K字节缓存空间指针
            pF -- 384字节特征缓存指针，（可以设置为NULL）
            Conv -- 质量控制分数（指纹录入时取120，搜索时取1000）
返回值：	  
            -1 -- 质量过差
            0~100 -- 指纹面积
********************************************************************************/
INT32 ImageToChar3_1(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv);

/*******************************************************************************
函数原型：	INT32 ImageToChar3_2(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv)
函数说明：	提取特征第一步
参数说明：	
            pImage -- 指纹图像指针
            pBuffer -- 80K字节缓存空间指针
            pF -- 384字节特征缓存指针
            Conv -- 质量控制分数（指纹录入时取120，搜索时取1000）
返回值：	  
            0 -- 成功
            其他 -- 失败
********************************************************************************/
INT32 ImageToChar3_2(UINT8* pImage,UINT8* pBuffer,UINT8* pF,UINT32 Conv);

///*******************************************************************************
//函数原型：	INT32 MatchTwoM(UINT8* pF1,UINT8* pF2)
//函数说明：	比对2个特征得到相似分数
//参数说明：	
//            pF1 -- 比对特征1
//            pF2 -- 比对特征2
//返 回 值：	相似分数
//********************************************************************************/
//INT32 MatchTwoM(UINT8* pF1,UINT8* pF2);

/*******************************************************************************
函数原型：	INT16 LMatchScore(UINT8 *pF1,UINT8 *pF2,UINT8* pBuffer,UINT32 BufferSize,UINT8 flag)
函数说明：	比对2个特征得到相似分数
参数说明：	
            pF1 -- 比对特征1
            pF2 -- 比对特征2
            pBuffer -- 缓存指针
            BufferSize -- 缓存大小（>=70KB）
            flag -- 保留参数，必须为0
返 回 值：	相似分数
********************************************************************************/
INT16 LMatchScore(UINT8 *pF1,UINT8 *pF2,UINT8* pBuffer,UINT32 BufferSize,UINT8 flag);

///*******************************************************************************
//函数原型：	INT32 QuickSearch3(UINT8* Feature,ReadBaseMProc ReadFeatureFunctionM,
//            UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchScore,
//            UINT16* pFindID,UINT16* pScore)
//函数说明：	在指纹库中搜索指纹特征
//参数说明：  
//            Feature -- 待搜索特征
//            ReadFeatureFunctionM -- 读取模板回调函数
//            StartID -- 搜索起始ID
//            SearchIDNumber -- 搜索模板个数
//            SearchIndex -- 模板索引表
//            MatchScore -- 搜索成功阈值，建议取50
//            pFindID -- 搜索到模板ID号
//            pScore -- 搜索到得分
//返 回 值：	
//            0 -- 搜索成功
//            其他 -- 搜索失败
//********************************************************************************/
//INT32 QuickSearch3(UINT8* Feature,ReadBaseMProc ReadFeatureFunctionM,UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchScore,UINT16* pFindID,UINT16* pScore);

/*******************************************************************************
函数原型：	INT32 QuickSearchB(UINT8* Feature,ReadBaseMProc ReadFeatureFunctionM,
            UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchScore,
            UINT16* pFindID,UINT16* pScore,UINT8* pBuffer);
函数说明：	在指纹库中搜索指纹特征
参数说明：  
            Feature -- 待搜索特征
            ReadFeatureFunctionM -- 读取模板回调函数
            StartID -- 搜索起始ID
            SearchIDNumber -- 搜索模板个数
            SearchIndex -- 模板索引表
            MatchScore -- 搜索成功阈值，建议取50
            pFindID -- 搜索到模板ID号
            pScore -- 搜索到得分
            pBuffer -- 使用缓存（>=70KB）
返 回 值：	
            0 -- 搜索成功
            其他 -- 搜索失败
********************************************************************************/
INT32 QuickSearchB(UINT8* Feature,ReadBaseMProc ReadMFunction,UINT16 StartID,UINT16 SearchIDNumber,UINT8* SearchIndex,UINT8 MatchOKScore,UINT16* pFindID,UINT16* pScore,UINT8* pBuffer);

#endif
