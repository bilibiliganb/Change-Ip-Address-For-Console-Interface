
// ChangeIpAddressDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChangeIpAddress.h"
#include "ChangeIpAddressDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include<vector>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>
#include <iostream>



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


int adapterComboIndex = 0;

struct MyAdapter
{
	int adapterIndex;
	char * Adapter_Name;
	char * Adapter_Desc;
	char * Adapter_Gateway;
	char * Adapter_Addr;
};



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeIpAddressDlg 对话框



CChangeIpAddressDlg::CChangeIpAddressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEIPADDRESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangeIpAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, now_ip);
	DDX_Control(pDX, IDC_EDIT1, set_ip_btn);
	DDX_Control(pDX, IDC_RADIO1, ip1_radio);
	DDX_Control(pDX, IDC_RADIO3, ip2_radio);
	DDX_Control(pDX, IDC_RADIO2, ip3_radio);
	DDX_Control(pDX, IDC_COMBO1, adapter_combox);
}

BEGIN_MESSAGE_MAP(CChangeIpAddressDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChangeIpAddressDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChangeIpAddressDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CChangeIpAddressDlg 消息处理程序




BOOL CChangeIpAddressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	GetAdapter();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChangeIpAddressDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChangeIpAddressDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChangeIpAddressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CChangeIpAddressDlg::OnBnClickedButton1()
{

	STARTUPINFO si = { 0 };
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi = { 0 };
	CString ipstr;

	set_ip_btn.GetWindowTextW(ipstr);

	if (ipstr != "") {
		TCHAR cmdstr[200];


		CString AdapterStr;

		AdapterStr.Format(L"%d", GetIndexFromSelected());

		wcscpy(cmdstr, L"C:\\windows\\system32\\cmd.exe /c netsh interface ipv4 set address ");

		wcscat(cmdstr, AdapterStr);

		wcscat(cmdstr, L" static ");

		wcscat(cmdstr, ipstr);

		wcscat(cmdstr, L" 255.255.255.0 ");

		wcscat(cmdstr, L"0.0.0.0");

		wcscat(cmdstr, L" 1");


		if (CreateProcess(NULL,   // No module name (use command line)
			cmdstr,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			MessageBox((LPCWSTR)L"Change Success", (LPCWSTR)L"show");
		}

	}


	
	// TODO: 在此添加控件通知处理程序代码
}


void CChangeIpAddressDlg::OnBnClickedButton2()
{
	
	CString ipstr;

	int ip1, ip2, ip3;
	CString gateway;
	ip1= ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	ip2 = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	ip3 = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();



	if (ip1 == 1) {
		ip1_radio.GetWindowTextW(ipstr);
		gateway = L"192.168.0.1";
	}

	if (ip2 == 1) {
		ip2_radio.GetWindowTextW(ipstr);
		gateway = L"192.168.1.1";
	}

	if (ip3 == 1) {
		ip3_radio.GetWindowTextW(ipstr);
		gateway = L"192.168.2.1";
	}

	

	if (ipstr != "") {
		TCHAR cmdstr[200];
		

		CString AdapterStr;

		AdapterStr.Format(L"%d", GetIndexFromSelected());

		wcscpy(cmdstr, L"C:\\windows\\system32\\cmd.exe /c netsh interface ipv4 set address ");

		wcscat(cmdstr, AdapterStr);

		wcscat(cmdstr, L" static ");

		wcscat(cmdstr, ipstr);

		wcscat(cmdstr, L" 255.255.255.0 ");

		wcscat(cmdstr, gateway);

		wcscat(cmdstr, L" 1");


		

		STARTUPINFO si = { 0 };
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi = { 0 };

		if (CreateProcess(NULL,   // No module name (use command line)
			cmdstr,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			MessageBox((LPCWSTR)L"Change Success", (LPCWSTR)L"show");
		}

	}

	

	// TODO: 在此添加控件通知处理程序代码
}



int GetAdapterIndexOflist(std::vector<MyAdapter>MyAdapterList, TCHAR target[]) {
	for (std::vector<MyAdapter>::size_type i = 0; i != MyAdapterList.size(); ++i)
	{
		TCHAR temp[100];

#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, MyAdapterList[i].Adapter_Desc, -1, temp, 100);
#else
		strcpy(Name, strUsr);
#endif

		TCHAR cmp1[20];
		TCHAR cmp2[20];

		wcsncpy(cmp1, target, 20);
		wcsncpy(cmp2, temp, 20);



		int flag = wcscmp(cmp1, cmp2);
		if (flag)
		{
			return i;
		}
		if (i+1 == MyAdapterList.size()) return MyAdapterList.size();
	}

}

std::vector<MyAdapter>MyAdapterList;
int CChangeIpAddressDlg::GetAdapter() {
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return 1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return 1;
		}
	}
	
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {

			MyAdapter myAdapter;
			myAdapter.adapterIndex = pAdapter->ComboIndex;
			myAdapter.Adapter_Name = pAdapter->AdapterName;
			myAdapter.Adapter_Desc = pAdapter->Description;
			//myAdapter.Adapter_Addr= 
			myAdapter.Adapter_Gateway = pAdapter->GatewayList.IpAddress.String;

			MyAdapterList.push_back(myAdapter);

			pAdapter = pAdapter->Next;
		}


	}

	/*for (vector<MyAdapter>::size_type i = 0; i != MyAdapterList.size(); ++i)
	{
		cout << MyAdapterList[i].adapterIndex << endl;
		cout << MyAdapterList[i].Adapter_Name << endl;
		cout << MyAdapterList[i].Adapter_Desc << endl;
		cout << MyAdapterList[i].Adapter_Gateway << endl;
		cout << endl;
	}*/

	for (std::vector<MyAdapter>::size_type i = 0; i != MyAdapterList.size(); ++i)
	{
		TCHAR temp[100];

#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, MyAdapterList[i].Adapter_Desc, -1, temp, 100);
#else
		strcpy(Name, strUsr);
#endif
		

		
		adapter_combox.AddString(temp);
	}


	int indexOfIntel = adapter_combox.FindString(0, L"Intel(R) Ethernet");
	adapter_combox.SetCurSel(indexOfIntel);

}






int GetAdapterIndex(std::vector<MyAdapter>MyAdapterList,TCHAR target[]) {
	for (std::vector<MyAdapter>::size_type i = 0; i != MyAdapterList.size(); ++i)
	{
		TCHAR temp[100];

#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, MyAdapterList[i].Adapter_Desc, -1, temp, 100);
#else
		strcpy(Name, strUsr);
#endif

		TCHAR cmp1[20];
		TCHAR cmp2[20];
		
		wcsncpy(cmp1, target,20);
		wcsncpy(cmp2, temp,20);

		
		int flag = wcscmp(cmp1, cmp2);
		if (flag)
		{
			return MyAdapterList[i].adapterIndex;
		}
	}
}




int CChangeIpAddressDlg::GetIndexFromSelected() {
	int index;
	int selectIndex = adapter_combox.GetCurSel();

	TCHAR szText[100] = { 0 };
	adapter_combox.GetLBText(selectIndex, szText);
	index = GetAdapterIndex(MyAdapterList, szText);
	return index;
}


//void CChangeIpAddressDlg::OnBnClickedButton3()
//{
//	int index;
//	int selectIndex = adapter_combox.GetCurSel();
//
//	TCHAR szText[100] = { 0 };
//	adapter_combox.GetLBText(selectIndex, szText);
//	index = GetAdapterIndex(MyAdapterList, szText);
//	
//	CString message;
//	message.Format(L"%d", index);
//	message
//	MessageBox(message);
//
//	 TODO: 在此添加控件通知处理程序代码
//}
