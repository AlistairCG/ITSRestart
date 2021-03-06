
// ITSRestartDlg.h : header file
//

#pragma once


// CITSRestartDlg dialog
class CITSRestartDlg : public CDialogEx
{
// Construction
public:
	CITSRestartDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITSRESTART_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRestart();
	// OK, restart the App
	CMFCButton m_btnOk;
	// Cancel, dont restart the machine
	CMFCButton m_btnCancel;
	// Restart, reveals the confirmation buttons
	CMFCButton m_btnRestart;
	afx_msg void OnBnClickedRestok();

	CStatic m_textConf;
	CStatic m_textConf2;
	CStatic m_textAdvConf;
	afx_msg void OnBnClickedRestcancel();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedNotice();
	CStatic m_textTips;
	afx_msg void OnBnClickedLock();
	afx_msg void OnBnClickedRestartT();
	afx_msg void OnBnClickedLockT();
	afx_msg void OnBnClickedRestartT2();
	CMFCButton btnLock;
	CMFCButton m_btnLockT;
};
