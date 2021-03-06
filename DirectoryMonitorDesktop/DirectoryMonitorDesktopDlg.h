
// DirectoryMonitorDesktopDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"


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
	CEdit CEDIT_SERVICE_RESPOND;
	CMFCEditBrowseCtrl CMFCEDITBROWSE_PATH;

	CButton CHECK_WATCH_SUBTREE;
	CButton CHECK_WATCH_NAME;
	CButton CHECK_WATCH_ATTRIBUTES;

	afx_msg void OnBnClickedButtonConnectWithPipe();
	afx_msg void OnBnClickedButtonSendPath();
	afx_msg void OnBnClickedButtonMonitor();

	HANDLE hNamedPipeWrite; // Канал для записи путей сервису
	HANDLE hThreadRead; // Поток для чтения изменений от сервиса
};
