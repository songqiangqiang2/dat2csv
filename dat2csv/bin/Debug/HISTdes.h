//历史库数据结构定义

//历史库分为内存和磁盘文件2部分，内存中保留当前1小时的数据，实际为2小时，并采用两区交替
#ifndef HISTDES
#define HISTDES

#include "..\include\COMMinclude\COMMconst.h"
#include "..\include\COMMinclude\COMMdes.h"

const char HDB_SEM[] = "HISTQUERY_SEM";		//历史库访问信号弹对象名

//----------------------全历史库收集单位数据结构-------------------------------------//
struct AnaHist_Data_struct		//模拟量全历史库收集单位数据结构
{
	DWORD dwValue;	//历史值
	BYTE bStat;	//历史状态：包括无效标记、可疑标记、手动禁止状态、强制状态，报警状态
};

struct DigHist_Data_struct		//开关量全历史库整点收集单位数据结构
{
	BYTE bValue;//历史值：包括当前值、前次有效值、无效标记、可疑标记、手动禁止状态、强制状态
};

struct DigHist_ChangedData_struct
{
	time_t iTime;				//变位时间
	short shMs;					//毫秒数
	BYTE bValue;//历史值：包括当前值、前次有效值、无效标记、可疑标记、手动禁止状态、强制状态
};
struct DigHist_EventData_struct	//开关量全历史库事件收集单位数据结构
{
	time_t iTime;				//变位时间
	short shMs;					//毫秒数
	unsigned short shPid;		//点号
	BYTE bValue;//历史值：包括当前值、前次有效值、无效标记、可疑标记、手动禁止状态、强制状态
};

//-----------------------收集定义点号表结构----------------------------------------//
struct PDefTable_struct
{
	unsigned short shPid;		//点号
	char chPName[PNAMELENGTH];
};

//-----------------------模拟量全历史库收集索引表结构------------------------------//

struct AllAnaHist_CollectGroup_struct	//模拟量全历史库收集组结构
{
	short shStartOffsetSec;				//本组收集时相对于整收集周期的偏移秒数
	unsigned short shMaxPidNumber;		//组内最大点数（最后一组为组内初始点数的120%，其余组=初始点数）
	unsigned short shCurrentPidNumber;	//组内当前点数（含在线增加点）
	unsigned short shInitialPidNumber;	//组内初始点数（组态结果）
	unsigned short * pPidArea;			//组内收集点号表区指针
	int iDataOffset;		//组内第一点第一个历史值在收集数据区中的位置
	int iCollectPos;		//组内第一点的当前收集位置（动态：初始化时根据开机时间定）
	short shDataAreaNo;		//内存文件的当前收集区号（动态：初始化时为0，以后0/1交替）
};

struct AllAnaHist_CollectType_struct	//模拟量全历史库收集类型表结构
{
	short shCollectPeriod;		//本收集类型的收集周期
	short shGrpNumber;			//本收集类型的收集组数
	int iDataNumPerHour;		//本收集类型每小时每点收集的数据个数
	struct AllAnaHist_CollectGroup_struct * pCollectGroup;	//本收集类型的收集组指针
	short shGrpNoForThePeriod;	//本收集类型本周期收集组号，0 ~ shManageperiod-1
};

struct AllAnaHist_CollectTable_struct	//模拟量全历史库收集索引表结构
{
	short shTypeNumber;			//收集类型数（即共有几档收集周期）
	short shSaveDayNumber;		//保存天数
	short shSaveAreaNo;			//当前存盘区号
	struct AllAnaHist_CollectType_struct * pCollectType;	//收集类型表指针
};

//---------------------------开关量全历史库收集索引表结构------------------------------//
struct AllDigHist_CollectTable_struct	//开关量全历史库收集组结构
{
	short shSaveDayNumber;				//保存天数
	unsigned short shMaxPidNumber;		//最大点数（最后一组为组内初始点数的120%，其余组=初始点数）
	unsigned short shCurrentPidNumber;	//当前点数（含在线增加点）
	unsigned short shInitialPidNumber;	//初始点数（组态结果）
	short shSaveAreaNo;					//当前存盘区号
	unsigned short * pPidArea;			//收集点号表区指针
	int iDataOffset;		//本小时内第一个变位事件在收集数据区中的位置
	int iCollectPos;		//变位事件的当前收集位置（动态：初始化时为0）
	short shDataAreaNo;		//内存文件的当前收集区号（动态：初始化时为0，以后0/1交替）
};

//----------------------------简化历史库收集索引表结构------------------------------//
//组类型:
//const short ANA_RT_POINT = 1;		//模拟量点
//const short DIG_RT_POINT = 2;		//开关量点
const short ANA_CAL_POINT = 3;		//模拟量计算点
const short DIG_CAL_POINT = 4;		//开关量计算点
//计算类型:
const short CAL_AVG = 1;		//求平均值
const short CAL_MAX = 2;		//求最大值
const short CAL_MIN = 3;		//求最小值

struct SimplyHist_CollectGroup_struct	//简化历史库收集组结构
{
	char chGrpName[12];		//组名
	char chGrpDes[32];		//组描述
	short shGrpType;		//组类型,1-模拟量组，2-开关量组
	short shCollectPeriod;	//收集周期（1秒，5秒，30秒，1分钟，10分钟，30分钟和1小时 7种）
	short shSaveDayNumber;	//保存天数（1天，3天，10天，1月，1年 5种）
	short shPidNumber;		//组内点数（30）
	int iDataSumNum;		//本收集类型收集的数据总数
	struct PDefTable_struct * pPidArea;	//组内收集点号表区指针
	int iDataOffset;		//组内第一点第一个历史值在收集数据区中的位置（没用啦）
	int iCollectPos;		//组内第一点的当前收集位置（动态：初始化时为-1）
	int iSavePos;			//当前存档位置
	int iStartOffsetSec;	//相对于整收集周期的偏移秒数
	short shCount;			//当前收集计数
	short shCalType;		//计算类型
	short shTakePeriod;		//采样周期
};

struct SimplyHist_CollectTable_struct	//简化历史库收集表结构
{
	short shGrpNumber;			//收集组数
	struct SimplyHist_CollectGroup_struct * pCollectGroup;	//收集组指针
};

//历史库收集索引表存储结构

struct HDB_Region_struct
{
	int iVersionNo;											//历史库版本号
	time_t iHDBCreateTime;									//历史库生成日期时间
	int iAllAnaHist_CollectTableLength;						//模拟量全历史库收集索引表长度
	struct AllAnaHist_CollectTable_struct * pAllAnaHist_CollectTable;//模拟量全历史库收集索引表首址
	int iAllDigHist_CollectTableLength;						//开关量全历史库收集索引表长度
	struct AllDigHist_CollectTable_struct * pAllDigHist_CollectTable;//开关量全历史库收集索引表首址
	int iSimplyHist_CollectTableLength;						//简化历史库收集索引表长度
	struct SimplyHist_CollectTable_struct * pSimplyHist_CollectTable;//简化历史库收集索引表首址
};

//------------------------历史库文件定义-----------------------------------------------//
const char HISTANA_INDEX_FILE[] = "HistAna.Tab";			//模拟量历史库索引表文件名
const char HISTDIG_INDEX_FILE[] = "HistDig.tab";			//开关量历史库索引表文件名
const char HISTSIM_INDEX_FILE[] = "HistSim.tab";			//简化量历史库索引表文件名
const char ALL_ANA_HDB_FILE[] = "ANAHIST";					//模拟量历史库文件名前缀
const char ALL_DIG_HDB_FILE[] = "DIGHIST";					//开关量历史库文件名前缀
const char SIMPLY_HDB_FILE[] = "SMPHIST";					//简化历史库文件名前缀
const char ALL_ANA_SAVEFLAG_FILE[] = "ANASAVEFLAG";	//模拟量历史库保存存盘文件号标识的文件名
const char ALL_DIG_SAVEFLAG_FILE[] = "DIGSAVEFLAG";	//开关量历史库保存存盘文件号标识的文件名
const char SIMPLY_SAVEFLAG_FILE[] = "SMPSAVEFLAG";	//简化历史库保存存盘位置的文件名
//历史库文件名由文件前缀加0~71构成，扩展名为.HDB

//-------------------------历史库访问信号灯定义---------------------------------------//
const char ANAHDB_INDEXTABLE_SEM[] = "ANAHDB_INDEXTABLE_SEM";		//模拟量收集索引表信号灯
const char DIGHDB_INDEXTABLE_SEM[] = "DIGHDB_INDEXTABLE_SEM";		//开关量收集索引表信号灯
const char SIMPLYHDB_INDEXTABLE_SEM[] = "SIMPLYHDB_INDEXTABLE_SEM";	//简化库收集索引表信号灯
const char HDB_READ_SEM[] = "HDB_READ_SEM";							//历史库读访问互斥信号灯

struct SaveFlagFile_struct	//简化历史库的收集位置信息结构
{
	int iCollectPos;		//组内第一点的当前收集位置（动态：初始化时为-1）
	int iSavePos;			//当前存档位置
	int iStartOffsetSec;	//相对于整收集周期的偏移秒数
	short shCount;			//当前收集计数
};

struct Hist_LocalResult_Head_struct		//本地历史库查询结果返回数据包头
{
	int iDataLength;	//返回结果集数据块长度
	short ret;			//返回成功标记
	short shRetDataNumber;	//返回结果集数据个数
	time_t stime;		//数据起始时间
	char other[4];		//废物
};

//2004.11.17增加三个文件的文件名字
const char ANACOLLECTFILE[] = "HistAnaCollectIndexFile";	//模拟量收集索引表文件
const char DIGCOLLECTFILE[] = "HistDigCollectIndexFile";	//开关量收集索引表文件
const char POINTINFOFILE[] = "HistOffQueryIndexFile";		//点信息文件

struct PidIndexBody_struct	//每个点的存储结构
{
	unsigned short shPid;		//全局点号
	short shPType;				//点类型
	char chPName[PNAMELENGTH];	//全局点名
	char chDes[DESLENGTH];		//点描述
	char chUT[MDESLENGTH];		//量纲（模拟量专有）
	float fMU;					//量程上限（模拟量专有）
	float fMD;					//量程下限（模拟量专有）
	short shOF;					//输出格式（模拟量专有）
	short shDefFlag;			//SOE属性（开关量专有）
	char chE0[MDESLENGTH];		//E0说明（开关量专有）
	char chE1[MDESLENGTH];		//E1说明（开关量专有）
};	//84

struct DBRead_Ret_struct	//需要查数据库的项
{
	char chPName[PNAMELENGTH];	//全局点名
	char chDes[DESLENGTH];		//点描述
	float fMU;					//量程上限（以下两项开关量当作E0）
	float fMD;					//量程下限
	char chUT[MDESLENGTH];		//量纲（开关量当作E1）
	short shOF;					//输出格式（开关量当做SOE标识）
};

#endif