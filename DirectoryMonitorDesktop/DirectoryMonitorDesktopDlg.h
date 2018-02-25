
// DirectoryMonitorDesktopDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CDirectoryMonitorDesktopDlg dialog
class CDirectoryMonitorDesktopDlg : public CDialog
{
// Construction
public:
	CDirectoryMonitorDesktopDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTORYMONITORDESKTOP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMonitor();
	CEdit CEDIT_SERVICE_RESPOND;
	CEdit CEDIT_DIRECTORY_NAME;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonConnectPipes();
	afx_msg void OnBnClickedButtonSendPath();
};
