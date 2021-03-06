
// ITSRestartDlg.cpp : implementation file


#include "afxdialogex.h"
#include "afxwin.h"

#include <iostream>
#include <string>

#include "stdafx.h"
#include "ITSRestart.h"
#include "ITSRestartDlg.h"





using namespace std;
//Configurable - Please Edit Me
//-------------------------------------------------
#define RESTARTTIMER 0  //users will be forced to restart if above 0 (in seconds)
#define CONFIRM "Are you sure you want to restart this machine?"  //The Question asked after a user presses restart (OK or Cancel Prompt)
#define CONFIRM_TMR "Goodbye!" //The last thing a user sees. "No Mr Bond, I expect you to die!"
#define DEFAULT_QUEST "Press Restart to End Your Session\r\n"
#define DEFAULT_QUEST_TIP " - Delete files saved on temp drives! \r\n \r\n - All your data will be wiped if restarted!" // The intial message to users
#define FORCERESTART 1  // Will force close programs to shutdown (1 or 0). De-jure force with RESTARTTIMER
bool isEdgeCase = false; //Determines if the machine gets special priviliges
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
//V: 1.1
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
	DDX_Control(pDX, IDC_Restart_T, btnLock);
	DDX_Control(pDX, IDC_Lock_T, m_btnLockT);
}

BEGIN_MESSAGE_MAP(CITSRestartDlg, CDialogEx)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Restart, &CITSRestartDlg::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_RestOk, &CITSRestartDlg::OnBnClickedRestok)
	ON_BN_CLICKED(IDC_RestCancel, &CITSRestartDlg::OnBnClickedRestcancel)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Lock_T, &CITSRestartDlg::OnBnClickedLockT)
	ON_BN_CLICKED(IDC_Restart_T, &CITSRestartDlg::OnBnClickedRestartT2)
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

	
	btnLock.EnableWindowsTheming(false);
	btnLock.SetFaceColor(RGB(193, 0, 0));
	btnLock.SetTextColor(RGB(255, 255, 255));

	m_btnLockT.EnableWindowsTheming(false);
	m_btnLockT.SetFaceColor(RGB(193, 0, 0));
	m_btnLockT.SetTextColor(RGB(255, 255, 255));


	m_btnOk.EnableWindowsTheming(false);
	m_btnOk.SetFaceColor(RGB(39, 113, 229));
	m_btnOk.SetTextColor(RGB(255, 255, 255));


	m_btnCancel.EnableWindowsTheming(false);
	m_btnCancel.SetFaceColor(RGB(114, 114, 114));
	m_btnCancel.SetTextColor(RGB(255, 255, 255));
	//===============================================//


	//sets the default question to be asked to the user.
	//Ex: Do you want to restart?
	//=====Bold font, larger text size====//

    CFont * pfont = m_textConf.GetFont();
	CFont * pfont2 = m_textAdvConf.GetFont();
	LOGFONT it, it2; 

	pfont->GetLogFont(&it);
	pfont2->GetLogFont(&it2);

	it.lfWeight = FW_BLACK;
	it.lfHeight -= 1;
	it.lfUnderline = 1;
	pfont->CreateFontIndirect(&it);
	m_textConf.SetFont(pfont);
	m_btnLockT.SetFont(pfont);
	btnLock.SetFont(pfont);
	m_btnOk.SetFont(pfont);
	m_btnCancel.SetFont(pfont);

	it2.lfWeight = FW_BLACK;
	it2.lfHeight -= 1;
	it2.lfWeight = FW_BOLD;
	it2.lfHeight -= 1;
	pfont2->CreateFontIndirect(&it2);
	m_btnRestart.SetFont(pfont2);
	//================================//

	//Sets the textConf textbox with the set text
	LPTSTR st = _T(DEFAULT_QUEST);
	m_textConf.SetWindowTextW(st);


	//Set the default tip list for the user
	//Ex: Dont restart without saving!
	LPTSTR st2 = _T(DEFAULT_QUEST_TIP);
	m_textTips.SetWindowTextW(st2);

	if (!isEdgeCase) {

		btnLock.ShowWindow(SW_HIDE);
		btnLock.EnableWindow(false);

		m_btnLockT.ShowWindow(SW_HIDE);
		m_btnLockT.EnableWindow(false);

	}

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

	if (isEdgeCase == false) {
		btnLock.ShowWindow(SW_HIDE);
		btnLock.EnableWindow(false);

		m_btnLockT.ShowWindow(SW_HIDE);
		m_btnLockT.EnableWindow(false);

	}
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
	
	if (FORCERESTART == 0 && RESTARTTIMER == 0) {
		//Soft Restart, will wait if you have stuff open
		system("shutdown /r");
	}
	else if (FORCERESTART == 1 && RESTARTTIMER == 0) {
		//Harsh Restart, nothing can stop it.
		WinExec("shutdown /r /f /t 0", SW_HIDE);

	}
	else {

		//TODO Future feature timer
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

	//If this machine is considered edge case, allow locking
	if (isEdgeCase){
	btnLock.ShowWindow(SW_SHOW);
	btnLock.EnableWindow(true);

	m_btnLockT.ShowWindow(SW_SHOW);
	m_btnLockT.EnableWindow(true);
	}
	else{
		m_btnRestart.ShowWindow(SW_SHOW);
		m_btnRestart.EnableWindow(true);

		btnLock.ShowWindow(SW_HIDE);
		btnLock.EnableWindow(false);

		m_btnLockT.ShowWindow(SW_HIDE);
		m_btnLockT.EnableWindow(false);
	}

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




//The lock button for edge cases
void CITSRestartDlg::OnBnClickedLockT()
{
	//This is the lock button
	WinExec("rundll32.exe user32.dll,LockWorkStation",SW_HIDE);
}

//The restart button for edge cases
void CITSRestartDlg::OnBnClickedRestartT2()
{

	OnBnClickedRestart();
}
