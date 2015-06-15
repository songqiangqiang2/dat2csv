//��ʷ�����ݽṹ����

//��ʷ���Ϊ�ڴ�ʹ����ļ�2���֣��ڴ��б�����ǰ1Сʱ�����ݣ�ʵ��Ϊ2Сʱ����������������
#ifndef HISTDES
#define HISTDES

#include "..\include\COMMinclude\COMMconst.h"
#include "..\include\COMMinclude\COMMdes.h"

const char HDB_SEM[] = "HISTQUERY_SEM";		//��ʷ������źŵ�������

//----------------------ȫ��ʷ���ռ���λ���ݽṹ-------------------------------------//
struct AnaHist_Data_struct		//ģ����ȫ��ʷ���ռ���λ���ݽṹ
{
	DWORD dwValue;	//��ʷֵ
	BYTE bStat;	//��ʷ״̬��������Ч��ǡ����ɱ�ǡ��ֶ���ֹ״̬��ǿ��״̬������״̬
};

struct DigHist_Data_struct		//������ȫ��ʷ�������ռ���λ���ݽṹ
{
	BYTE bValue;//��ʷֵ��������ǰֵ��ǰ����Чֵ����Ч��ǡ����ɱ�ǡ��ֶ���ֹ״̬��ǿ��״̬
};

struct DigHist_ChangedData_struct
{
	time_t iTime;				//��λʱ��
	short shMs;					//������
	BYTE bValue;//��ʷֵ��������ǰֵ��ǰ����Чֵ����Ч��ǡ����ɱ�ǡ��ֶ���ֹ״̬��ǿ��״̬
};
struct DigHist_EventData_struct	//������ȫ��ʷ���¼��ռ���λ���ݽṹ
{
	time_t iTime;				//��λʱ��
	short shMs;					//������
	unsigned short shPid;		//���
	BYTE bValue;//��ʷֵ��������ǰֵ��ǰ����Чֵ����Ч��ǡ����ɱ�ǡ��ֶ���ֹ״̬��ǿ��״̬
};

//-----------------------�ռ������ű�ṹ----------------------------------------//
struct PDefTable_struct
{
	unsigned short shPid;		//���
	char chPName[PNAMELENGTH];
};

//-----------------------ģ����ȫ��ʷ���ռ�������ṹ------------------------------//

struct AllAnaHist_CollectGroup_struct	//ģ����ȫ��ʷ���ռ���ṹ
{
	short shStartOffsetSec;				//�����ռ�ʱ��������ռ����ڵ�ƫ������
	unsigned short shMaxPidNumber;		//���������������һ��Ϊ���ڳ�ʼ������120%��������=��ʼ������
	unsigned short shCurrentPidNumber;	//���ڵ�ǰ���������������ӵ㣩
	unsigned short shInitialPidNumber;	//���ڳ�ʼ��������̬�����
	unsigned short * pPidArea;			//�����ռ���ű���ָ��
	int iDataOffset;		//���ڵ�һ���һ����ʷֵ���ռ��������е�λ��
	int iCollectPos;		//���ڵ�һ��ĵ�ǰ�ռ�λ�ã���̬����ʼ��ʱ���ݿ���ʱ�䶨��
	short shDataAreaNo;		//�ڴ��ļ��ĵ�ǰ�ռ����ţ���̬����ʼ��ʱΪ0���Ժ�0/1���棩
};

struct AllAnaHist_CollectType_struct	//ģ����ȫ��ʷ���ռ����ͱ�ṹ
{
	short shCollectPeriod;		//���ռ����͵��ռ�����
	short shGrpNumber;			//���ռ����͵��ռ�����
	int iDataNumPerHour;		//���ռ�����ÿСʱÿ���ռ������ݸ���
	struct AllAnaHist_CollectGroup_struct * pCollectGroup;	//���ռ����͵��ռ���ָ��
	short shGrpNoForThePeriod;	//���ռ����ͱ������ռ���ţ�0 ~ shManageperiod-1
};

struct AllAnaHist_CollectTable_struct	//ģ����ȫ��ʷ���ռ�������ṹ
{
	short shTypeNumber;			//�ռ��������������м����ռ����ڣ�
	short shSaveDayNumber;		//��������
	short shSaveAreaNo;			//��ǰ��������
	struct AllAnaHist_CollectType_struct * pCollectType;	//�ռ����ͱ�ָ��
};

//---------------------------������ȫ��ʷ���ռ�������ṹ------------------------------//
struct AllDigHist_CollectTable_struct	//������ȫ��ʷ���ռ���ṹ
{
	short shSaveDayNumber;				//��������
	unsigned short shMaxPidNumber;		//�����������һ��Ϊ���ڳ�ʼ������120%��������=��ʼ������
	unsigned short shCurrentPidNumber;	//��ǰ���������������ӵ㣩
	unsigned short shInitialPidNumber;	//��ʼ��������̬�����
	short shSaveAreaNo;					//��ǰ��������
	unsigned short * pPidArea;			//�ռ���ű���ָ��
	int iDataOffset;		//��Сʱ�ڵ�һ����λ�¼����ռ��������е�λ��
	int iCollectPos;		//��λ�¼��ĵ�ǰ�ռ�λ�ã���̬����ʼ��ʱΪ0��
	short shDataAreaNo;		//�ڴ��ļ��ĵ�ǰ�ռ����ţ���̬����ʼ��ʱΪ0���Ժ�0/1���棩
};

//----------------------------����ʷ���ռ�������ṹ------------------------------//
//������:
//const short ANA_RT_POINT = 1;		//ģ������
//const short DIG_RT_POINT = 2;		//��������
const short ANA_CAL_POINT = 3;		//ģ���������
const short DIG_CAL_POINT = 4;		//�����������
//��������:
const short CAL_AVG = 1;		//��ƽ��ֵ
const short CAL_MAX = 2;		//�����ֵ
const short CAL_MIN = 3;		//����Сֵ

struct SimplyHist_CollectGroup_struct	//����ʷ���ռ���ṹ
{
	char chGrpName[12];		//����
	char chGrpDes[32];		//������
	short shGrpType;		//������,1-ģ�����飬2-��������
	short shCollectPeriod;	//�ռ����ڣ�1�룬5�룬30�룬1���ӣ�10���ӣ�30���Ӻ�1Сʱ 7�֣�
	short shSaveDayNumber;	//����������1�죬3�죬10�죬1�£�1�� 5�֣�
	short shPidNumber;		//���ڵ�����30��
	int iDataSumNum;		//���ռ������ռ�����������
	struct PDefTable_struct * pPidArea;	//�����ռ���ű���ָ��
	int iDataOffset;		//���ڵ�һ���һ����ʷֵ���ռ��������е�λ�ã�û������
	int iCollectPos;		//���ڵ�һ��ĵ�ǰ�ռ�λ�ã���̬����ʼ��ʱΪ-1��
	int iSavePos;			//��ǰ�浵λ��
	int iStartOffsetSec;	//��������ռ����ڵ�ƫ������
	short shCount;			//��ǰ�ռ�����
	short shCalType;		//��������
	short shTakePeriod;		//��������
};

struct SimplyHist_CollectTable_struct	//����ʷ���ռ���ṹ
{
	short shGrpNumber;			//�ռ�����
	struct SimplyHist_CollectGroup_struct * pCollectGroup;	//�ռ���ָ��
};

//��ʷ���ռ�������洢�ṹ

struct HDB_Region_struct
{
	int iVersionNo;											//��ʷ��汾��
	time_t iHDBCreateTime;									//��ʷ����������ʱ��
	int iAllAnaHist_CollectTableLength;						//ģ����ȫ��ʷ���ռ���������
	struct AllAnaHist_CollectTable_struct * pAllAnaHist_CollectTable;//ģ����ȫ��ʷ���ռ���������ַ
	int iAllDigHist_CollectTableLength;						//������ȫ��ʷ���ռ���������
	struct AllDigHist_CollectTable_struct * pAllDigHist_CollectTable;//������ȫ��ʷ���ռ���������ַ
	int iSimplyHist_CollectTableLength;						//����ʷ���ռ���������
	struct SimplyHist_CollectTable_struct * pSimplyHist_CollectTable;//����ʷ���ռ���������ַ
};

//------------------------��ʷ���ļ�����-----------------------------------------------//
const char HISTANA_INDEX_FILE[] = "HistAna.Tab";			//ģ������ʷ���������ļ���
const char HISTDIG_INDEX_FILE[] = "HistDig.tab";			//��������ʷ���������ļ���
const char HISTSIM_INDEX_FILE[] = "HistSim.tab";			//������ʷ���������ļ���
const char ALL_ANA_HDB_FILE[] = "ANAHIST";					//ģ������ʷ���ļ���ǰ׺
const char ALL_DIG_HDB_FILE[] = "DIGHIST";					//��������ʷ���ļ���ǰ׺
const char SIMPLY_HDB_FILE[] = "SMPHIST";					//����ʷ���ļ���ǰ׺
const char ALL_ANA_SAVEFLAG_FILE[] = "ANASAVEFLAG";	//ģ������ʷ�Ᵽ������ļ��ű�ʶ���ļ���
const char ALL_DIG_SAVEFLAG_FILE[] = "DIGSAVEFLAG";	//��������ʷ�Ᵽ������ļ��ű�ʶ���ļ���
const char SIMPLY_SAVEFLAG_FILE[] = "SMPSAVEFLAG";	//����ʷ�Ᵽ�����λ�õ��ļ���
//��ʷ���ļ������ļ�ǰ׺��0~71���ɣ���չ��Ϊ.HDB

//-------------------------��ʷ������źŵƶ���---------------------------------------//
const char ANAHDB_INDEXTABLE_SEM[] = "ANAHDB_INDEXTABLE_SEM";		//ģ�����ռ��������źŵ�
const char DIGHDB_INDEXTABLE_SEM[] = "DIGHDB_INDEXTABLE_SEM";		//�������ռ��������źŵ�
const char SIMPLYHDB_INDEXTABLE_SEM[] = "SIMPLYHDB_INDEXTABLE_SEM";	//�򻯿��ռ��������źŵ�
const char HDB_READ_SEM[] = "HDB_READ_SEM";							//��ʷ������ʻ����źŵ�

struct SaveFlagFile_struct	//����ʷ����ռ�λ����Ϣ�ṹ
{
	int iCollectPos;		//���ڵ�һ��ĵ�ǰ�ռ�λ�ã���̬����ʼ��ʱΪ-1��
	int iSavePos;			//��ǰ�浵λ��
	int iStartOffsetSec;	//��������ռ����ڵ�ƫ������
	short shCount;			//��ǰ�ռ�����
};

struct Hist_LocalResult_Head_struct		//������ʷ���ѯ����������ݰ�ͷ
{
	int iDataLength;	//���ؽ�������ݿ鳤��
	short ret;			//���سɹ����
	short shRetDataNumber;	//���ؽ�������ݸ���
	time_t stime;		//������ʼʱ��
	char other[4];		//����
};

//2004.11.17���������ļ����ļ�����
const char ANACOLLECTFILE[] = "HistAnaCollectIndexFile";	//ģ�����ռ��������ļ�
const char DIGCOLLECTFILE[] = "HistDigCollectIndexFile";	//�������ռ��������ļ�
const char POINTINFOFILE[] = "HistOffQueryIndexFile";		//����Ϣ�ļ�

struct PidIndexBody_struct	//ÿ����Ĵ洢�ṹ
{
	unsigned short shPid;		//ȫ�ֵ��
	short shPType;				//������
	char chPName[PNAMELENGTH];	//ȫ�ֵ���
	char chDes[DESLENGTH];		//������
	char chUT[MDESLENGTH];		//���٣�ģ����ר�У�
	float fMU;					//�������ޣ�ģ����ר�У�
	float fMD;					//�������ޣ�ģ����ר�У�
	short shOF;					//�����ʽ��ģ����ר�У�
	short shDefFlag;			//SOE���ԣ�������ר�У�
	char chE0[MDESLENGTH];		//E0˵����������ר�У�
	char chE1[MDESLENGTH];		//E1˵����������ר�У�
};	//84

struct DBRead_Ret_struct	//��Ҫ�����ݿ����
{
	char chPName[PNAMELENGTH];	//ȫ�ֵ���
	char chDes[DESLENGTH];		//������
	float fMU;					//�������ޣ����������������E0��
	float fMD;					//��������
	char chUT[MDESLENGTH];		//���٣�����������E1��
	short shOF;					//�����ʽ������������SOE��ʶ��
};

#endif