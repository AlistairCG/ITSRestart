
// ITSRestartDlg.cpp : implementation file

#include <iostream>
#include <atltypes.h>
#include <string>

#include "stdafx.h"
#include "wtypes.h"
#include "ITSRestart.h"
#include "ITSRestartDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
//Configurable - Please Edit Me
//-------------------------------------------------
#define RESTARTTIMER 0  //users will be forced to restart if above 0 (in seconds)
#define CONFIRM "Are you sure you want to restart this machine?"  //The Question asked after a user presses restart (OK or Cancel Prompt)
#define CONFIRM_TMR "Goodbye!" //The last thing a user sees. "No Mr Bond, I expect you to die!"
#define DEFAULT_QUEST "Press Restart to End Your Session\r\n"
#define DEFAULT_QUEST_TIP " - Delete files saved on temp Drive (D:\)\r\n - All data will be wiped on restarting" // The intial message to users
#define FORCERESTART 1  // Will force open programs to shutdown (1 or 0). De-jure force with RESTARTTIMER
//-------------------------------------------------
//****************************************************//
// 9/26/2018
//A: Alistair Godwin
//C: alistair.godwin@senecacollege.ca
//ITS Technical Support 
//
//This is the primary dialog for this app. 
//Due note, that most of this code is boiler-plate in nature, meaning dont modify unless you actually understand what it does! Please use the configurable section above as desired however!
//I highly recommend using the GUI editor to drag and drop resources onto the dialogue window. Double click on a resource to generate an onclick event.
//In its current iteration, this app only performs basic restarts.
//
//V: 0.5 
//Configuration can be found in ITSRestartDLG.cpp
//*************************************************//

CITSRestartDlg::CITSRestartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ITSRESTART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CITSRestartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RestOk, m_btnOk);
	DDX_Control(pDX, IDC_RestCancel, m_btnCancel);
	DDX_Control(pDX, IDC_Restart, m_btnRestart);
	DDX_Control(pDX, IDC_Notice, m_textConf);
	DDX_Control(pDX, IDC_NoticeConf, m_textAdvConf);
	DDX_Control(pDX, IDC_Tips, m_textTips);
}

BEGIN_MESSAGE_MAP(CITSRestartDlg, CDialogEx)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Restart, &CITSRestartDlg::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_RestOk, &CITSRestartDlg::OnBnClickedRestok)
	ON_BN_CLICKED(IDC_RestCancel, &CITSRestartDlg::OnBnClickedRestcancel)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_Notice, &CITSRestartDlg::OnStnClickedNotice)
END_MESSAGE_MAP()


// CITSRestartDlg message handlers

BOOL CITSRestartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//================Colour & Theme===========//
	SetBackgroundColor(RGB(255, 255, 255));

	m_btnRestart.EnableWindowsTheming(false);
	m_btnRestart.SetFaceColor(RGB(193, 0, 0));
	m_btnRestart.SetTextColor(RGB(255, 255, 255));
	

	m_btnOk.EnableWindowsTheming(false);
	m_btnOk.SetFaceColor(RGB(39, 113, 229));
	m_btnOk.SetTextColor(RGB(255, 255, 255));


	m_btnCancel.EnableWindowsTheming(false);
	m_btnCancel.SetFaceColor(RGB(114, 114, 114));
	m_btnCancel.SetTextColor(RGB(255, 255, 255));
	//===============================================//


	//sets the default question to be asked to the user.
	//Ex: Do you want to restart?
	// =====Bold font, larger text size====//

    CFont * pfont = m_textConf.GetFont();
	LOGFONT it, it2; 

	pfont->GetLogFont(&it);

	it.lfWeight = FW_BLACK;
	it.lfHeight -= 1;
	it2 = it;
	it2.lfWeight = FW_BOLD;
	it2.lfHeight -= 1;
	it.lfUnderline = 1;
	pfont->CreateFontIndirect(&it);
	m_textConf.SetFont(pfont);
	pfont->CreateFontIndirect(&it2);

	LPTSTR st = _T(DEFAULT_QUEST);
	m_textConf.SetWindowTextW(st);
	//m_btnRestart.SetFont(pfont);


	//================================//

	//Set the default tip list for the user
	//Ex: Dont restart without saving!
	LPTSTR st2 = _T(DEFAULT_QUEST_TIP);
	m_textTips.SetWindowTextW(st2);


	//The original form size as set in visual studio
	CRect origRect;
	GetWindowRect(origRect);

	//The placeholder rectangle of the desktop's resolution
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	int Wid = rect.Width();
	int Len = rect.Height();
	
	//Moves the window to the right hand corner of any resolution's display
	MoveWindow(Wid - origRect.Width(), Len - origRect.Height(), origRect.Width(), origRect.Height(), true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CITSRestartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//Event Handler for Restarting the Application
//Displays the Confirmation message and hides external buttons
void CITSRestartDlg::OnBnClickedRestart()
{


	m_btnOk.ShowWindow(SW_SHOW);
	m_btnOk.EnableWindow(true);

	m_btnCancel.ShowWindow(SW_SHOW);
	m_btnCancel.EnableWindow(true);

	m_btnRestart.ShowWindow(SW_HIDE);
	m_btnRestart.EnableWindow(false);

	m_textAdvConf.ShowWindow(SW_SHOW);
	m_textAdvConf.EnableWindow(true);
	LPTSTR st = _T(CONFIRM);
	m_textAdvConf.SetWindowTextW(st);

	m_textConf.ShowWindow(SW_HIDE);
	m_textConf.EnableWindow(false);

	m_textTips.ShowWindow(SW_HIDE);
	m_textTips.EnableWindow(false);

}


//Event Handler for Restarting the Application - OK Button
void CITSRestartDlg::OnBnClickedRestok()
{
	//ShowWindow(SW_HIDE);

	if (FORCERESTART == 0 && RESTARTTIMER == 0) {
		system("shutdown /r");
	}
	else if (FORCERESTART == 1 && RESTARTTIMER == 0) {
	//	system("shutdown /r /t 0");
		WinExec("shutdown /r /f /t 0", SW_HIDE);

	}
	else {

		//TODO Future feature
		system("shutdown /r /t 5");

	}
	EndDialog(0); //Ends Program
}

//Event Handler for Restarting the Application - Cancel Button
//Restores the Original Restart Button
void CITSRestartDlg::OnBnClickedRestcancel()
{
	
	m_btnOk.ShowWindow(SW_HIDE);
	m_btnOk.EnableWindow(false);

	m_btnCancel.ShowWindow(SW_HIDE);
	m_btnCancel.EnableWindow(false);

	m_btnRestart.ShowWindow(SW_SHOW);
	m_btnRestart.EnableWindow(true);

	m_textAdvConf.ShowWindow(SW_HIDE);
	m_textAdvConf.EnableWindow(false);

	m_textConf.ShowWindow(SW_SHOW);
	m_textConf.EnableWindow(true);

	m_textTips.ShowWindow(SW_SHOW);
	m_textTips.EnableWindow(true);
}


//Required for painting
HBRUSH CITSRestartDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}


void CITSRestartDlg::OnStnClickedNotice()
{
	// TODO: Add your control notification handler code here
}
