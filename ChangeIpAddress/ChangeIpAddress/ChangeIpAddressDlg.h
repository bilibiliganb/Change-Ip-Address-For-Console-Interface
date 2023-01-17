
// ChangeIpAddressDlg.h: 头文件
//

#pragma once


// CChangeIpAddressDlg 对话框
class CChangeIpAddressDlg : public CDialogEx
{
// 构造
public:
	CChangeIpAddressDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEIPADDRESS_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton now_ip;
	CEdit set_ip_btn;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void adapter_comboxGetLBText(int nIndex, LPTSTR lpszText);
	void adapter_setindex(CComboBox adapter_combox,int i);
	int GetAdapter();
	CButton ip1_radio;
	CButton ip2_radio;
	CButton ip3_radio;
	CComboBox adapter_combox;
	afx_msg
		int GetIndexFromSelected();
};
