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
	"保留",//CJ188Reserved,				// = 0x00,
	"读数据",//CJ188ReadData,				// = 0x01,
	"读地址",//CJ188ReadAddress,			// = 0x03,
	"写数据",//CJ188WriteData,				// = 0x04,
	"读秘钥版本号",//CJ188ReadKeyVersion,		// = 0x09,
	"写地址",//CJ188WriteAddress,			// = 0x15,
	"写机电同步数",//CJ188WriteMeterBase,		// = 0x16,
	"厂商自定义"//CJ188CustomBegin,			// = 0x20
};
KeyValue CJ188ControlList[] = {
	{ "保留", CJ188Reserved },				// = 0x00,
	{ "读数据", CJ188ReadData },				// = 0x01,
	{ "读地址", CJ188ReadAddress },			// = 0x03,
	{ "写数据", CJ188WriteData },				// = 0x04,
	{ "读秘钥版本号", CJ188ReadKeyVersion },		// = 0x09,
	{ "写地址", CJ188WriteAddress },			// = 0x15,
	{ "写机电同步数", CJ188WriteMeterBase },		// = 0x16,
	{ "厂商自定义", CJ188CustomBegin }			// = 0x20
};

UINT16 CJ188ReadDIList[] = {
	CJ188ReadCurrentData,	//= 0x901F,//读取当前数据
	CJ188ReadHistory1,		// = 0xD120,//读取历史第N月前的数据
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
	"读当前数据",//CJ188ReadCurrentData,	//= 0x901F,//读取当前数据
	"读上1月数据",//CJ188ReadHistory1,		// = 0xD120,//读取历史第N月前的数据
	"读上2月数据",//CJ188ReadHistory2,
	"读上3月数据",//CJ188ReadHistory3,
	"读上4月数据",//CJ188ReadHistory4,
	"读上5月数据",//CJ188ReadHistory5,
	"读上6月数据",//CJ188ReadHistory6,
	"读上7月数据",//CJ188ReadHistory7,
	"读上8月数据",//CJ188ReadHistory8,
	"读上9月数据",//CJ188ReadHistory9,
	"读上10月数据",//CJ188ReadHistory10,
	"读上11月数据",//CJ188ReadHistory11,
	"读上12月数据",//CJ188ReadHistory12,
	"读价格",//CJ188ReadPrice,			// = 0x8102,
	"读结算日",//CJ188ReadBillingDay,
	"读抄表日",//CJ188ReadReadingDay,
	"读购入金额",//CJ188ReadPurchasedAmount,
	"读秘钥版本号",//CJ188ReadKeyVersion,
	"读地址",//CJ188ReadCurrendAddress	// = 0x810A

};

KeyValue CJ188DIReadList[]{
	//读数据
	{ "读当前数据", CJ188ReadCurrentData },
	{ "读上1月数据", CJ188ReadHistory1 },		// = 0xD120,//读取历史第N月前的数据
	{ "读上2月数据", CJ188ReadHistory2 },
	{ "读上3月数据", CJ188ReadHistory3 },
	{ "读上4月数据", CJ188ReadHistory4 },
	{ "读上5月数据", CJ188ReadHistory5 },
	{ "读上6月数据", CJ188ReadHistory6 },
	{ "读上7月数据", CJ188ReadHistory7 },
	{ "读上8月数据", CJ188ReadHistory8 },
	{ "读上9月数据", CJ188ReadHistory9 },
	{ "读上10月数据", CJ188ReadHistory10 },
	{ "读上11月数据", CJ188ReadHistory11 },
	{ "读上12月数据", CJ188ReadHistory12 },
	{ "读价格", CJ188ReadPrice },			// = 0x8102,
	{ "读结算日", CJ188ReadBillingDay },
	{ "读抄表日", CJ188ReadReadingDay },
	{ "读购入金额", CJ188ReadPurchasedAmount },

	//读秘钥版本
	{ "读秘钥版本号", CJ188ReadKeyVersionDI },

	//读地址
	{ "读地址", CJ188ReadCurrendAddress }	// = 0x810A

};

KeyValue CJ188DIWriteList[]{
	//写数据
	{ "写价格", CJ188WritePrice },// = 0xA010,
	{ "写结算日", CJ188WriteBillingDay },
	{ "写抄表日", CJ188WriteReadingDay },
	{ "写购入金额", CJ188WritePurchasedAmount },
	{ "写秘钥", CJ188WriteKey },
	{ "写标准时间", CJ188WriteStandardTime },
	{ "写阀门控制", CJ188WriteValveControl },// = 0xA017,
	{ "出厂启用", CJ188WriteEnabled },

	//写地址
	{ "写地址", CJ188WriteCurrentAddress },

	//写机电同步数
	{ "写机电同步数", CJ188WriteMeterBaseData },//写机电同步数据
};

KeyValue CJ188DIList[]{
	//读数据
	{ "读当前数据", CJ188ReadCurrentData },
	{ "读上1月数据", CJ188ReadHistory1 },		// = 0xD120,//读取历史第N月前的数据
	{ "读上2月数据", CJ188ReadHistory2 },
	{ "读上3月数据", CJ188ReadHistory3 },
	{ "读上4月数据", CJ188ReadHistory4 },
	{ "读上5月数据", CJ188ReadHistory5 },
	{ "读上6月数据", CJ188ReadHistory6 },
	{ "读上7月数据", CJ188ReadHistory7 },
	{ "读上8月数据", CJ188ReadHistory8 },
	{ "读上9月数据", CJ188ReadHistory9 },
	{ "读上10月数据", CJ188ReadHistory10 },
	{ "读上11月数据", CJ188ReadHistory11 },
	{ "读上12月数据", CJ188ReadHistory12 },
	{ "读价格", CJ188ReadPrice },			// = 0x8102,
	{ "读结算日", CJ188ReadBillingDay },
	{ "读抄表日", CJ188ReadReadingDay },
	{ "读购入金额", CJ188ReadPurchasedAmount },

	//读秘钥版本 offset=17
	{ "读秘钥版本号", CJ188ReadKeyVersionDI },

	//读地址 offset=18
	{ "读地址", CJ188ReadCurrendAddress },	// = 0x810A

	//写数据 offset=19
	{ "写价格", CJ188WritePrice },// = 0xA010,
	{ "写结算日", CJ188WriteBillingDay },
	{ "写抄表日", CJ188WriteReadingDay },
	{ "写购入金额", CJ188WritePurchasedAmount },
	{ "写秘钥", CJ188WriteKey },
	{ "写标准时间", CJ188WriteStandardTime },
	{ "写阀门控制", CJ188WriteValveControl },// = 0xA017,
	{ "出厂启用", CJ188WriteEnabled },

	//写地址 offset=27
	{ "写地址", CJ188WriteCurrentAddress },

	//写机电同步数 offset=28
	{ "写机电同步数", CJ188WriteMeterBaseData },//写机电同步数据
};

UCHAR CJ188::ser = rand() % 256;

CJ188::CJ188(MeterInfo* info)
{
	CJ188::ser = rand() % 256;
	this->meterInfo = info;
	buffer = new ComBuffer();
}


CJ188::~CJ188()
{
	//delete meterInfo;
}

UCHAR CJ188::CreateCS(CJ188Frame &frame){
	UINT cs = 0;
	UCHAR* pchar = (UCHAR*)(&frame);
	//数据域之前部分求和
	for (int i = 0; i < CJ188_FRAME_LENGTH_BEFORE_DATA;i++){
		cs += *pchar;
		pchar++;
	}
	//数据域部分
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
	//生成广播地址0xAAAAAAAAAAAAAA
	UCHAR* addr = new UCHAR[CJ188_ADDRESS_LENGTH];
	for (int i = 0; i < CJ188_ADDRESS_LENGTH; i++){
		addr[i] = 0xAA;
	}
	//得到控制字0x03
	UCHAR ctrl = CJ188ReadAddress;

	//获得DI——读地址0x810A
	KeyValue* kvlist;
	int kvCount;
	kvlist = &CJ188DIList[CJ188_READ_ADDRESS_DI_OFFSET];
	kvCount = CJ188_READ_ADDRESS_DI_COUNT;
	int index = 0;
	UCHAR* di = (UCHAR*)(&kvlist[index].key);

	//生成读表地址指令帧
	CJ188Frame* frame = CreateRequestFrame(addr, ctrl, di);
	//发送指令帧
	SendFrame(frame);

	delete frame->data;
	delete frame;
}

void CJ188::ReadMeterData(MeterInfo* meterInfo, CJ188DataReciever* reciever){
	this->cjReciever = reciever;
	//获取要读的表的地址
	UCHAR* addr = meterInfo->address;
	//得到控制字0x03
	UCHAR ctrl = CJ188ReadData;

	//获得DI——读地址0x810A
	KeyValue* kvlist;
	int kvCount;
	kvlist = &CJ188DIList[CJ188_READ_DATA_DI_OFFSET];
	kvCount = CJ188_READ_DATA_DI_COUNT;
	int index = 0;
	UCHAR* di = (UCHAR*)(&kvlist[index].key);

	//生成指令帧
	requestFrame = CreateRequestFrame(addr, ctrl, di);
	//发送指令帧
	SendFrame(requestFrame);

	delete requestFrame->data;
	delete requestFrame;
}

void CJ188::SendFrame(CJ188Frame* frame){
	//指令帧封装为uchar数组
	//串口发送
	//整合为16进制指令
	int length;
	char* data = new char[CJ188_FRAME_LENGTH_AFTER_DATA + frame->dataLength + CJ188_FRAME_LENGTH_BEFORE_DATA + CJ188_FRAME_INIT_LENGTH];
	ToHexCommand(data, frame, length);
	//发送指令
	CSerial* serial = &(meterInfo->serial);
	//serial->OpenSerialPort(meterInfo->com,)
	TCHAR* comID;
	CString strText = meterInfo->com;
	int iLen = strText.GetLength();
	comID = new TCHAR[iLen];
	lstrcpy(comID, strText.GetBuffer(iLen));
	cdReciever = this;
	bool serialAvailable = true;
	if (!serial->IsOpened()){
		//打开串口后，自动接收数据
		serialAvailable = serial->OpenSerialPort(comID, comConfig.baudRate, comConfig.dataBits, comConfig.stopBits, comConfig.parityIndex, cdReciever);
	}
	if (serialAvailable)
		serial->SendData(data, length);
	delete data;
}

CJ188Frame* CJ188::CreateRequestFrame(UCHAR* addr, UCHAR control, UCHAR* di){
	CJ188Frame *frame = new CJ188Frame();
	//开始字节68H
	frame->start = CJ188_START_BYTE;
	//表计类型：热能表20H
	frame->meterType = HeatMeter;

	//获取当前表计地址（0为默认广播地址14个A）
	UCHAR* address = addr;
	memcpy(&(frame->address), address, CJ188_ADDRESS_LENGTH);
	//delete address;
	//获取控制码
	frame->controlCode = control;
	//根据控制码得到长度
	frame->dataLength = 3;
	//获取数据标识
	//UCHAR* di = di;
	//生成数据域
	frame->data = CreateData(di, frame->dataLength);
	//生成校验码
	frame->cs = CJ188::CreateCS(*frame);
	//生成停止位
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

void CJ188::OnDataRecieved(UCHAR* buf, DWORD bufferLen){
	//接收到的可能为不完整数据帧，需要多次数据的连接，成为一个完整数据帧
	//1.接收到的数据填入接收Buffer尾部
	buffer->Append(buf, bufferLen);
	bool hasFrame = false;
	//2.尝试解析接收buffer中的帧数据
	CJ188FrameInBuffer *frame;
	do{
		hasFrame = false;
		frame = (CJ188FrameInBuffer*)RawDataToFrame(buffer->GetData(), buffer->GetCurrentLength());
		//3.判断解析结果是否为完整帧,若完整,则
		if (frame != nullptr){
			//3.1校验frame的CS是否为有效帧
			if (this->IsValidFrame(frame)){
				//3.1.1回调OnFrameDataRecieved带入数据帧与对应部分原始数据
				size_t rawDataLen = frame->bufferEnd - frame->bufferStart;
				UCHAR* rawData = new UCHAR[rawDataLen];
				memcpy(rawData, &(buffer[frame->bufferStart]), rawDataLen);
				if (cjReciever != nullptr){
					meterInfo->SetActive(true);
					cjReciever->OnFrameDataRecieved(rawData, rawDataLen, frame, this);
				}
			}
			//3.2.清空对应buffer部分
			buffer->Clean(((CJ188FrameInBuffer*)frame)->bufferEnd);
			hasFrame = true;
		}
		//3.3.若仍有剩余数据，则
		//		3.3.1返回2继续解析帧数据
	} while ((!buffer->IsEmpty())&&hasFrame);
	/*
	//等待起始字节0x68开始拼接
	//到结束字节是0x16为止
	frame = RawDataToFrame(buf, bufferLen);
	//回调CJ188DataReciever的OnDataRecieved
	if (cjReciever != nullptr){
		meterInfo->SetActive(true);
		if (frame != nullptr){
			cjReciever->OnFrameDataRecieved(buf, bufferLen, frame, this);
		}
	}
	*/
}

CJ188Frame* CJ188::RawDataToFrame(UCHAR* buf, DWORD bufLen){
	//按字节解析buf成为CJ188Frame格式
	int cur = 0;
	//跳过前几个字节的0xFE直接到起始码0x68
	while (buf[cur] != CJ188_START_BYTE && cur < bufLen){
		cur++;
	}
	if (cur == bufLen){//全为0xFE，无效帧
		return nullptr;
	}
	CJ188FrameInBuffer* frame = new CJ188FrameInBuffer();
	frame->bufferStart = cur;
	if ((cur + CJ188_FRAME_LENGTH_BEFORE_DATA) >= bufLen){//总长度不到帧数据项之前所需长度，无效帧
		return nullptr;
	}

	memcpy(frame, &(buf[cur]), CJ188_FRAME_LENGTH_BEFORE_DATA);
	cur += CJ188_FRAME_LENGTH_BEFORE_DATA;
	//总长度不到帧所需长度，无效帧
	if ((cur + frame->dataLength + CJ188_FRAME_LENGTH_AFTER_DATA) > bufLen){
		return nullptr;
	}
	frame->data = new UCHAR[frame->dataLength];
	memcpy((frame->data), &(buf[cur]), frame->dataLength);
	cur += frame->dataLength;
	frame->cs = buf[cur++];
	frame->end = buf[cur++];
	frame->bufferEnd = cur;

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

CString CJ188::GetUnit(UCHAR code){
	CString unit;
	switch (code){
		//热量部分
	case WH:
		unit = CString("Wh");
		break;
	case KWH:
		unit = CString("kWh");
		break;
	case MWH:
		unit = CString("MWh");
		break;
	case MWHx100:
		unit = CString("MWhx100");
		break;
	case J:
		unit = CString("J");
		break;
	case KJ:
		unit = CString("kj");
		break;
	case MJ:
		unit = CString("Mj");
		break;
	case GJ:
		unit = CString("GJ");
		break;
	case GJx100:
		unit = CString("GJx100");
		break;
		//功率部分
	case W:
		unit = CString("W");
		break;
	case KW:
		unit = CString("kW");
		break;
	case MW:
		unit = CString("MW");
		break;
		//流速部分
	case LPH:
		unit = CString("L/h");
		break;
	case M3PH:
		unit = CString("MWh");
		break;
		//流量部分
	case L:
		unit = CString("L");
		break;
	case M3:
		unit = CString("m^3");
		break;
	default:
		unit = CString("N/A");
		break;
	}
	return unit;
}

UCHAR* CJ188::GetDI(CJ188Frame* frame){
	//UCHAR* di = new UCHAR[2];
	//memcpy(di, frame->data, 2);
	return frame->data;
}

bool CJ188::LookUpByteOrder(UCHAR meterType, UCHAR* vandorID){
	if (vandorID[0] == 0x11 && vandorID[1] == 0x11){
		return false;
	}
	return true;
}