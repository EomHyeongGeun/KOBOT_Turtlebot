
// VideoProcessorDlg.h : ��� ����
//

#pragma once

#include "imageSrc/VideoProcessing.h"

// CVideoProcessorDlg ��ȭ ����
class CVideoProcessorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CVideoProcessorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIDEOPROCESSOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonStartCam();
	afx_msg void OnBnClickedButtonStopCam();
	afx_msg void OnBnClickedButtonLoadAvi();
	afx_msg void OnBnClickedButtonSetBg();
	afx_msg void OnBnClickedButtonRecCam();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStopRec();

protected:
	// ī�޶� ĸ�� ���� ��� ����
	CWebCam		m_webCam;

	// AVI ������ �б� ���� ��� ����
	CAviPlayer	m_aviPlayer;
	int			m_nFrame;
	int			m_idxFrame;
	bool		m_bPlay;
public:
	afx_msg void OnBnClickedButtonCloseAvi();
};
