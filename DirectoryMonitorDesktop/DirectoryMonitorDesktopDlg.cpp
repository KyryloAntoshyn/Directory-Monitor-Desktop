
// DirectoryMonitorDesktopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DirectoryMonitorDesktop.h"
#include "DirectoryMonitorDesktopDlg.h"
#include "afxdialogex.h"

#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CDirectoryMonitorDesktopDlg dialog



CDirectoryMonitorDesktopDlg::CDirectoryMonitorDesktopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIRECTORYMONITORDESKTOP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirectoryMonitorDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SERVICE_RESPOND, CEDIT_SERVICE_RESPOND);
	DDX_Control(pDX, IDC_EDIT_DIRECTORY_NAME, CEDIT_DIRECTORY_NAME);
}

BEGIN_MESSAGE_MAP(CDirectoryMonitorDesktopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MONITOR, &CDirectoryMonitorDesktopDlg::OnBnClickedButtonMonitor)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CDirectoryMonitorDesktopDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CDirectoryMonitorDesktopDlg message handlers

BOOL CDirectoryMonitorDesktopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Меняю шрифт
	CClientDC dc(this);
	CFont *m_pFont = new CFont();
	m_pFont->CreatePointFont(120, _T("Arial"), &dc);

	GetDlgItem(IDC_STATIC_DIRECTORY_NAME)->SetFont(m_pFont, TRUE);
	GetDlgItem(IDC_STATIC_SERVICE_RESPOND)->SetFont(m_pFont, TRUE);
	GetDlgItem(IDC_EDIT_SERVICE_RESPOND)->SetFont(m_pFont, TRUE);
	GetDlgItem(IDC_EDIT_DIRECTORY_NAME)->SetFont(m_pFont, TRUE);
	GetDlgItem(IDC_BUTTON_CONNECT)->SetFont(m_pFont, TRUE);
	GetDlgItem(IDC_BUTTON_MONITOR)->SetFont(m_pFont, TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDirectoryMonitorDesktopDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDirectoryMonitorDesktopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDirectoryMonitorDesktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText)
{
	// get the initial text length
	int nLength = edit.GetWindowTextLength();
	// put the selection at the end of text
	edit.SetSel(nLength, nLength);
	// replace the selection
	edit.ReplaceSel(pszText);
}

HANDLE hNamedPipeWrite = CreateFile(
	L"\\\\.\\pipe\\DirectoryMonitorPipeRead", // имя канала сервиса для получения информации от клиента
	GENERIC_WRITE, // доступ на запись в канал 
	FILE_SHARE_WRITE,
	NULL, // доступ по умолчанию разрешен всем
	OPEN_ALWAYS, // открываем существующий канал
	FILE_ATTRIBUTE_NORMAL,
	NULL
);

HANDLE hThreadRead;

DWORD WINAPI SendFilePathProcess(LPVOID lpParam)
{
	CEdit * cedit_name = (CEdit*)lpParam;

	if (hThreadRead != NULL)
	{
		TerminateThread(hThreadRead, 0);
		CloseHandle(hThreadRead);
	}

	// Считывание введенных данных
	wchar_t buff[512];
	cedit_name->GetWindowTextW(buff, sizeof(buff));

	// Конвертирую данные в char[]
	size_t i = 0;
	char directory_path_msg[512];
	wcstombs_s(&i, directory_path_msg, sizeof(directory_path_msg), buff, _TRUNCATE);

	// Отправляем сервису сообщение с путём к директории
	DWORD cbWritten;
	WriteFile(hNamedPipeWrite, directory_path_msg, strlen(directory_path_msg) + 1,
		&cbWritten, NULL);

	return 0;
}

DWORD WINAPI ReadChangesProcess(LPVOID lpParam)
{
	CEdit * cedit_response = (CEdit*)lpParam;

	HANDLE hNamedPipeRead = CreateFile(
		L"\\\\.\\pipe\\DirectoryMonitorPipeWrite", // имя канала сервиса для отправки информации клиенту
		GENERIC_READ, // доступ на чтение из канала
		FILE_SHARE_READ, // доступ на чтение
		NULL, // доступ по умолчанию разрешен всем
		OPEN_EXISTING, // открываем существующий канал
		FILE_ATTRIBUTE_NORMAL, // у файла нет других атрибутов
		NULL // дексриптор шаблона файла
	);

	char directory_path_msg[512]; // Сообщение с директорией от клиента
	DWORD cbRead;

	AppendTextToEditCtrl(*cedit_response, L"Service started monitoring your directory.\r\n");

	while (true)
	{
		// Читаем сообщение об изменениях от сервиса
		ReadFile(hNamedPipeRead, directory_path_msg, 512, &cbRead, NULL);

		if (cbRead == 0)
		{
			AppendTextToEditCtrl(*cedit_response, L"Service was stopped!\r\n");
			return 0;
		}

		// Вывожу изменения в клиентской программе
		AppendTextToEditCtrl(*cedit_response, (CString)directory_path_msg + "\r\n");
	}

	return 0;
}

void CDirectoryMonitorDesktopDlg::OnBnClickedButtonConnect()
{
	// Создаю поток для отправки путей файлов клиенту
	HANDLE hThreadSend = CreateThread(NULL, 0, SendFilePathProcess, &CEDIT_DIRECTORY_NAME, THREAD_ALL_ACCESS, NULL);
	ResumeThread(hThreadSend);
	//CloseHandle(hThreadSend);
}

void CDirectoryMonitorDesktopDlg::OnBnClickedButtonMonitor()
{
	// Создаю поток для чтения изменений в файле от сервиса
	hThreadRead = CreateThread(NULL, 0, ReadChangesProcess, &CEDIT_SERVICE_RESPOND, THREAD_ALL_ACCESS, NULL);
	ResumeThread(hThreadRead);
	//CloseHandle(hThreadRead);
}