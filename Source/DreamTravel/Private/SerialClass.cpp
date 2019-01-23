// Fill out your copyright notice in the Description page of Project Settings.

#include "SerialClass.h"

DEFINE_LOG_CATEGORY(CSerialClass);

CSerialClass::CSerialClass()
{
    m_hIDComDev = NULL;
	m_bOpened = FALSE;
}

CSerialClass::~CSerialClass()
{
    Close();
	UE_LOG(LogTemp, Warning, TEXT("Serial closed !"));
}

void CSerialClass::Initialise()
{

}

//打开串口
BOOL CSerialClass::Open(int nPort, int nBaud)
{
	if (m_bOpened) return(TRUE);

	TCHAR szPort[15];
	TCHAR szComParams[50];
	DCB dcb;
	
    //构建字符串，如COM3
	wsprintf(szPort, _T("com%d"), nPort);
    //打开串口
	m_hIDComDev = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (m_hIDComDev == NULL) return(FALSE);



    //初始化Overlap
	memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));

	COMMTIMEOUTS CommTimeOuts;
	//CommTimeOuts.ReadIntervalTimeout = 10;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts(m_hIDComDev, &CommTimeOuts);

	wsprintf(szComParams, _T("COM%d:%d,n,8,1"), nPort, nBaud);

	m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hIDComDev, &dcb);
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;
	unsigned char ucSet;
	ucSet = (unsigned char)((FC_RTSCTS & FC_DTRDSR) != 0);
	ucSet = (unsigned char)((FC_RTSCTS & FC_RTSCTS) != 0);
	ucSet = (unsigned char)((FC_RTSCTS & FC_XONXOFF) != 0);
	if (!SetCommState(m_hIDComDev, &dcb) ||     //设置状态
		!SetupComm(m_hIDComDev, 10000, 10000) ||    //设置推荐的缓冲区大小
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL){
		DWORD dwError = GetLastError();
		if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
		if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
		CloseHandle(m_hIDComDev);
		return(FALSE);
	}

	m_bOpened = TRUE;
	Sleep(50);
	return(m_bOpened);

}

//关闭串口
BOOL CSerialClass::Close(void)
{

	if (!m_bOpened || m_hIDComDev == NULL) return(TRUE);

	if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
	if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
	CloseHandle(m_hIDComDev);
	m_bOpened = FALSE;
	m_hIDComDev = NULL;

	return(TRUE);
}

//写串口字节
BOOL CSerialClass::WriteCommByte(unsigned char ucByte)
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;
	//If the function succeeds, the return value is nonzero (TRUE).
	//If the function fails, or is completing asynchronously, the return value is zero (FALSE). To get extended error information, call the GetLastError function.
	//Note  The GetLastError code ERROR_IO_PENDING is not a failure; it designates the write operation is pending completion asynchronously. For more information, see Remarks.
	bWriteStat = WriteFile(m_hIDComDev, (LPSTR)&ucByte, 1, &dwBytesWritten, &m_OverlappedWrite);
	if (!bWriteStat && (GetLastError() == ERROR_IO_PENDING)){
		//WaitForSingleObject: If the function succeeds, the return value indicates the event that caused the function to return. It can be one of the following values.
		//当返回值大于0时说明超时，或者等待失败
		if (WaitForSingleObject(m_OverlappedWrite.hEvent, 1000)) dwBytesWritten = 0;
		else{
			//Retrieves the results of an overlapped operation on the specified file, named pipe, or communications device.
			//If the function succeeds, the return value is nonzero.
			//If the function fails, the return value is zero. To get extended error information, call GetLastError.
			GetOverlappedResult(m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE);
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}

	return(TRUE);
}

int CSerialClass::SendData(const char *buffer, int size)
{

	if (!m_bOpened || m_hIDComDev == NULL) return(0);

	DWORD dwBytesWritten = 0;
	int i;
	for (i = 0; i<size; i++){
		WriteCommByte(buffer[i]);
		dwBytesWritten++;
	}

	return((int)dwBytesWritten);
}

int CSerialClass::ReadData(void *buffer, int limit)
{

	if (!m_bOpened || m_hIDComDev == NULL) return(0);

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;
	//获取com流中等待的自己数
	ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);
	if (!ComStat.cbInQue) return(0);

	dwBytesRead = (DWORD)ComStat.cbInQue;
	if (limit < (int)dwBytesRead) dwBytesRead = (DWORD)limit;
	UE_LOG(LogTemp, Warning, TEXT("将要读取数据。。。time %f"), FPlatformTime::Seconds());
	bReadStatus = ReadFile(m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead);
	UE_LOG(LogTemp, Warning, TEXT("3将要读取数据。。。time %f"), FPlatformTime::Seconds());
	if (!bReadStatus){
		if (GetLastError() == ERROR_IO_PENDING){
			UE_LOG(LogTemp, Warning, TEXT("2将要读取数据。。。time %f"), FPlatformTime::Seconds());
			WaitForSingleObject(m_OverlappedRead.hEvent, 2000);
			UE_LOG(LogTemp, Warning, TEXT("已读取数据。。。time %f"), FPlatformTime::Seconds());
//			UE_LOG(LogTemp, Warning, TEXT("已读取数据。。。time %f"), GetWorld()->GetTimeSeconds());
			return((int)dwBytesRead);
		}
		return(0);
	}
	//UE_LOG(LogTemp, Warning, TEXT("连接失败，正在重连。。。"));
	return((int)dwBytesRead);
}