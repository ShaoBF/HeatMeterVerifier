#include "stdafx.h"
#include "CJ188.h"
#include "ComConfig.h"

UCHAR CJ188ControlCodeList[] = {
	CJ188Reserved,				// = 0x00,
	CJ188ReadData,				// = 0x01,
	CJ188ReadAddress,			// = 0x03,
	CJ188WriteData,				// = 0x04,
	CJ188ReadKeyVersion,		// = 0x09,
	CJ188WriteAddress,			// = 0x15,
	CJ188WriteMeterBase,		// = 0x16,
	CJ188CustomBegin,			// = 0x20
};

char* CJ188ControlCodeCaptionList[] = {
	"����",//CJ188Reserved,				// = 0x00,
	"������",//CJ188ReadData,				// = 0x01,
	"����ַ",//CJ188ReadAddress,			// = 0x03,
	"д����",//CJ188WriteData,				// = 0x04,
	"����Կ�汾��",//CJ188ReadKeyVersion,		// = 0x09,
	"д��ַ",//CJ188WriteAddress,			// = 0x15,
	"д����ͬ����",//CJ188WriteMeterBase,		// = 0x16,
	"�����Զ���"//CJ188CustomBegin,			// = 0x20
};
KeyValue CJ188ControlList[] = {
	{ "����", CJ188Reserved },				// = 0x00,
	{ "������", CJ188ReadData },				// = 0x01,
	{ "����ַ", CJ188ReadAddress },			// = 0x03,
	{ "д����", CJ188WriteData },				// = 0x04,
	{ "����Կ�汾��", CJ188ReadKeyVersion },		// = 0x09,
	{ "д��ַ", CJ188WriteAddress },			// = 0x15,
	{ "д����ͬ����", CJ188WriteMeterBase },		// = 0x16,
	{ "�����Զ���", CJ188CustomBegin }			// = 0x20
};

UINT16 CJ188ReadDIList[] = {
	CJ188ReadCurrentData,	//= 0x901F,//��ȡ��ǰ����
	CJ188ReadHistory1,		// = 0xD120,//��ȡ��ʷ��N��ǰ������
	CJ188ReadHistory2,
	CJ188ReadHistory3,
	CJ188ReadHistory4,
	CJ188ReadHistory5,
	CJ188ReadHistory6,
	CJ188ReadHistory7,
	CJ188ReadHistory8,
	CJ188ReadHistory9,
	CJ188ReadHistory10,
	CJ188ReadHistory11,
	CJ188ReadHistory12,
	CJ188ReadPrice,			// = 0x8102,
	CJ188ReadBillingDay,
	CJ188ReadReadingDay,
	CJ188ReadPurchasedAmount,
	CJ188ReadKeyVersionDI,
	CJ188ReadCurrendAddress	// = 0x810A

};
char* CJ188ReadDICaptionList[] = {
	"����ǰ����",//CJ188ReadCurrentData,	//= 0x901F,//��ȡ��ǰ����
	"����1������",//CJ188ReadHistory1,		// = 0xD120,//��ȡ��ʷ��N��ǰ������
	"����2������",//CJ188ReadHistory2,
	"����3������",//CJ188ReadHistory3,
	"����4������",//CJ188ReadHistory4,
	"����5������",//CJ188ReadHistory5,
	"����6������",//CJ188ReadHistory6,
	"����7������",//CJ188ReadHistory7,
	"����8������",//CJ188ReadHistory8,
	"����9������",//CJ188ReadHistory9,
	"����10������",//CJ188ReadHistory10,
	"����11������",//CJ188ReadHistory11,
	"����12������",//CJ188ReadHistory12,
	"���۸�",//CJ188ReadPrice,			// = 0x8102,
	"��������",//CJ188ReadBillingDay,
	"��������",//CJ188ReadReadingDay,
	"��������",//CJ188ReadPurchasedAmount,
	"����Կ�汾��",//CJ188ReadKeyVersion,
	"����ַ",//CJ188ReadCurrendAddress	// = 0x810A

};

KeyValue CJ188DIReadList[]{
	//������
	{ "����ǰ����", CJ188ReadCurrentData },
	{ "����1������", CJ188ReadHistory1 },		// = 0xD120,//��ȡ��ʷ��N��ǰ������
	{ "����2������", CJ188ReadHistory2 },
	{ "����3������", CJ188ReadHistory3 },
	{ "����4������", CJ188ReadHistory4 },
	{ "����5������", CJ188ReadHistory5 },
	{ "����6������", CJ188ReadHistory6 },
	{ "����7������", CJ188ReadHistory7 },
	{ "����8������", CJ188ReadHistory8 },
	{ "����9������", CJ188ReadHistory9 },
	{ "����10������", CJ188ReadHistory10 },
	{ "����11������", CJ188ReadHistory11 },
	{ "����12������", CJ188ReadHistory12 },
	{ "���۸�", CJ188ReadPrice },			// = 0x8102,
	{ "��������", CJ188ReadBillingDay },
	{ "��������", CJ188ReadReadingDay },
	{ "��������", CJ188ReadPurchasedAmount },

	//����Կ�汾
	{ "����Կ�汾��", CJ188ReadKeyVersionDI },

	//����ַ
	{ "����ַ", CJ188ReadCurrendAddress }	// = 0x810A

};

KeyValue CJ188DIWriteList[]{
	//д����
	{ "д�۸�", CJ188WritePrice },// = 0xA010,
	{ "д������", CJ188WriteBillingDay },
	{ "д������", CJ188WriteReadingDay },
	{ "д������", CJ188WritePurchasedAmount },
	{ "д��Կ", CJ188WriteKey },
	{ "д��׼ʱ��", CJ188WriteStandardTime },
	{ "д���ſ���", CJ188WriteValveControl },// = 0xA017,
	{ "��������", CJ188WriteEnabled },

	//д��ַ
	{ "д��ַ", CJ188WriteCurrentAddress },

	//д����ͬ����
	{ "д����ͬ����", CJ188WriteMeterBaseData },//д����ͬ������
};

KeyValue CJ188DIList[]{
	//������
	{ "����ǰ����", CJ188ReadCurrentData },
	{ "����1������", CJ188ReadHistory1 },		// = 0xD120,//��ȡ��ʷ��N��ǰ������
	{ "����2������", CJ188ReadHistory2 },
	{ "����3������", CJ188ReadHistory3 },
	{ "����4������", CJ188ReadHistory4 },
	{ "����5������", CJ188ReadHistory5 },
	{ "����6������", CJ188ReadHistory6 },
	{ "����7������", CJ188ReadHistory7 },
	{ "����8������", CJ188ReadHistory8 },
	{ "����9������", CJ188ReadHistory9 },
	{ "����10������", CJ188ReadHistory10 },
	{ "����11������", CJ188ReadHistory11 },
	{ "����12������", CJ188ReadHistory12 },
	{ "���۸�", CJ188ReadPrice },			// = 0x8102,
	{ "��������", CJ188ReadBillingDay },
	{ "��������", CJ188ReadReadingDay },
	{ "��������", CJ188ReadPurchasedAmount },

	//����Կ�汾 offset=17
	{ "����Կ�汾��", CJ188ReadKeyVersionDI },

	//����ַ offset=18
	{ "����ַ", CJ188ReadCurrendAddress },	// = 0x810A

	//д���� offset=19
	{ "д�۸�", CJ188WritePrice },// = 0xA010,
	{ "д������", CJ188WriteBillingDay },
	{ "д������", CJ188WriteReadingDay },
	{ "д������", CJ188WritePurchasedAmount },
	{ "д��Կ", CJ188WriteKey },
	{ "д��׼ʱ��", CJ188WriteStandardTime },
	{ "д���ſ���", CJ188WriteValveControl },// = 0xA017,
	{ "��������", CJ188WriteEnabled },

	//д��ַ offset=27
	{ "д��ַ", CJ188WriteCurrentAddress },

	//д����ͬ���� offset=28
	{ "д����ͬ����", CJ188WriteMeterBaseData },//д����ͬ������
};

UCHAR CJ188::ser = rand() % 256;

CJ188::CJ188(MeterInfo* info)
{
	CJ188::ser = rand() % 256;
	this->meterInfo = info;
}


CJ188::~CJ188()
{
	//delete meterInfo;
}

UCHAR CJ188::CreateCS(CJ188Frame &frame){
	UINT cs = 0;
	UCHAR* pchar = (UCHAR*)(&frame);
	//������֮ǰ�������
	for (int i = 0; i < CJ188_FRAME_LENGTH_BEFORE_DATA;i++){
		cs += *pchar;
		pchar++;
	}
	//�����򲿷�
	for (int i = 0; i < frame.dataLength; i++){
		cs += frame.data[i];
	}
	return cs & 0xFF;
}
void CJ188::ToHexCommand(char* command, CJ188Frame *frame, int &length, int prefixLen){
	length = 0;
	for (int i = 0; i < prefixLen; i++){
		command[length++] = CJ188_FRAME_PREFIX_BYTE;
	}
	command[length++] = (char)frame->start;
	char m = frame->meterType;
	command[length++] = m;
	for (int i = 0; i < CJ188_ADDRESS_LENGTH; i++){
		command[length++] = frame->address[i];
	}
	command[length++] = frame->controlCode;
	command[length++] = frame->dataLength;
	for (int i = 0; i < frame->dataLength; i++){
		command[length++] = frame->data[i];
	}
	command[length++] = frame->cs;
	command[length++] = frame->end;
}
extern ComConfig comConfig;


void CJ188::ReadAddress(MeterInfo* meterInfo, CJ188DataReciever* reciever){
	this->cjReciever = reciever;
	//���ɹ㲥��ַ0xAAAAAAAAAAAAAA
	UCHAR* addr = new UCHAR[CJ188_ADDRESS_LENGTH];
	for (int i = 0; i < CJ188_ADDRESS_LENGTH; i++){
		addr[i] = 0xAA;
	}
	//�õ�������0x03
	UCHAR ctrl = CJ188ReadAddress;

	//���DI��������ַ0x810A
	KeyValue* kvlist;
	int kvCount;
	kvlist = &CJ188DIList[CJ188_READ_ADDRESS_DI_OFFSET];
	kvCount = CJ188_READ_ADDRESS_DI_COUNT;
	int index = 0;
	UCHAR* di = (UCHAR*)(&kvlist[index].key);

	//���ɶ����ַָ��֡
	CJ188Frame* frame = CreateRequestFrame(addr, ctrl, di);
	//ָ��֡��װΪuchar����
	//���ڷ���
	//����Ϊ16����ָ��
	int length;
	char* data = new char[CJ188_FRAME_LENGTH_AFTER_DATA + frame->dataLength + CJ188_FRAME_LENGTH_BEFORE_DATA + CJ188_FRAME_INIT_LENGTH];
	ToHexCommand(data, frame, length);
	//����ָ��
	CSerial* serial = &(meterInfo->serial);
	//serial->OpenSerialPort(meterInfo->com,)
	TCHAR* comID;
	CString strText = meterInfo->com;
	int iLen = strText.GetLength();
	comID = new TCHAR[iLen];
	lstrcpy(comID, strText.GetBuffer(iLen));
	cdReciever = this;

	if (serial->OpenSerialPort(comID, comConfig.baudRate, comConfig.dataBits, comConfig.stopBits, comConfig.parityIndex,
		//&ComDataRecievedCB, 
		cdReciever)){  //�򿪴��ں��Զ���������
		//comHandle[comBoxIndex] = serial.m_hComm;
		//comOn[comBoxIndex] = true;
		serial->SendData(data, length);
	}
	delete data;
	delete frame->data;
	delete frame;
}

CJ188Frame* CJ188::CreateRequestFrame(UCHAR* addr, UCHAR control, UCHAR* di){
	CJ188Frame *frame = new CJ188Frame();
	//��ʼ�ֽ�68H
	frame->start = CJ188_START_BYTE;
	//������ͣ����ܱ�20H
	frame->meterType = HeatMeter;

	//��ȡ��ǰ��Ƶ�ַ��0ΪĬ�Ϲ㲥��ַ14��A��
	UCHAR* address = addr;
	memcpy(&(frame->address), address, CJ188_ADDRESS_LENGTH);
	delete address;
	//��ȡ������
	frame->controlCode = control;
	//���ݿ�����õ�����
	frame->dataLength = 3;
	//��ȡ���ݱ�ʶ
	//UCHAR* di = di;
	//����������
	frame->data = CreateData(di, frame->dataLength);
	//����У����
	frame->cs = CJ188::CreateCS(*frame);
	//����ֹͣλ
	frame->end = CJ188_END_BYTE;
	return frame;
}

UCHAR* CJ188::CreateData(UCHAR* di, int dataLength){
	UCHAR* data = new UCHAR[dataLength];
	for (int i = 0; i < dataLength; i++){
		data[i] = 0;
	}
	data[0] = di[1];
	data[1] = di[0];
	data[2] = NextSerial();
	return data;
}

UCHAR CJ188::NextSerial(){
	CJ188::ser++;
	return CJ188::ser;
}

void CJ188::OnDataRecieved(char* buf, DWORD bufferLen){
	frame = RawDataToFrame(buf, bufferLen);
	//�ص�CJ188DataReciever��OnDataRecieved
	if (cjReciever != nullptr){
		meterInfo->SetActive(true);
		if (frame != nullptr){
			cjReciever->OnFrameDataRecieved(buf, bufferLen, frame, this);
		}
	}

}
CJ188Frame* CJ188::RawDataToFrame(char* buf, DWORD bufferLen){
	CJ188Frame* frame = new CJ188Frame();
	//���ֽڽ���buf��ΪCJ188Frame��ʽ
	int cur = 0;
	//����ǰ�����ֽڵ�0xFEֱ�ӵ���ʼ��0x68
	while (buf[cur] != CJ188_START_BYTE && cur < bufferLen){
		cur++;
	}
	if (cur == bufferLen){//ȫΪ0xFE����Ч֡
		return nullptr;
	}
	memcpy(frame, &(buf[cur]), CJ188_FRAME_LENGTH_BEFORE_DATA);
	frame->data = new UCHAR[frame->dataLength];
	cur += CJ188_FRAME_LENGTH_BEFORE_DATA;
	memcpy((frame->data), &(buf[cur]), frame->dataLength);
	cur += frame->dataLength;
	frame->cs = buf[cur++];
	frame->end = buf[cur++];
	return frame;
}

bool CJ188::IsValidFrame(CJ188Frame *frame){
	int cs = CJ188::CreateCS(*frame);
	if (cs == frame->cs){
		return true;
	}
	else {
		return false;
	}
}