
// VideoProcessorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "VideoProcessor.h"
#include "VideoProcessorDlg.h"
#include "afxdialogex.h"
#include "imageSrc\MyImageFunc.h"
#include "Features.h"

#include "imageSrc\ImageFrameWndManager.h"
#include "imageSrc\LoadImageFromFileDialog.h"
#include "imageSrc\Segmentation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoProcessorDlg ��ȭ ����



CVideoProcessorDlg::CVideoProcessorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoProcessorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nFrame = 0;
	m_idxFrame = 0;
	m_bPlay = true;
}

void CVideoProcessorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoProcessorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_CAM, &CVideoProcessorDlg::OnBnClickedButtonStartCam)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAM, &CVideoProcessorDlg::OnBnClickedButtonStopCam)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_AVI, &CVideoProcessorDlg::OnBnClickedButtonLoadAvi)
	ON_BN_CLICKED(IDC_BUTTON_SET_BG, &CVideoProcessorDlg::OnBnClickedButtonSetBg)
	ON_BN_CLICKED(IDC_BUTTON_REC_CAM, &CVideoProcessorDlg::OnBnClickedButtonRecCam)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CVideoProcessorDlg::OnBnClickedButtonPause)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP_REC, &CVideoProcessorDlg::OnBnClickedButtonStopRec)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_AVI, &CVideoProcessorDlg::OnBnClickedButtonCloseAvi)
END_MESSAGE_MAP()


// CVideoProcessorDlg �޽��� ó����

BOOL CVideoProcessorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CVideoProcessorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CVideoProcessorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_aviPlayer.ShowFrame(GetDlgItem(IDC_STATIC_PREVIEW), m_idxFrame);
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CVideoProcessorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ī�޶� ���� �� ������ �����ϱ� ���� ���� ����
CByteImage gImageBuf;
BITMAPINFO gBmpInfo;

// ��ü ���� ���� ���� ����
CByteImage	gBgImage;
CByteImage	gBinImage;
CByteImage  gSobelImage;

bool IsChanged(CByteImage& binImage)
{
	int nSize = binImage.GetWidth()*binImage.GetHeight();
	int nSum = 0;

	BYTE* pData = binImage.GetPtr();
	for (int i=0 ; i<nSize ; i++)
	{
		if (pData[i])
			nSum++;
	}

	if (nSum > 0.5*nSize)
		return true;
}

LRESULT ProcessCamFrame(HWND hWnd, LPVIDEOHDR lpVHdr) // ���� ������ �Ǵ� �κ�
{
	SobelEdge(RGB2Gray(gImageBuf), gImageBuf);
	if (gBmpInfo.bmiHeader.biCompression == BI_RGB) // RGB ����
	{
		memcpy(gImageBuf.GetPtr(), lpVHdr->lpData, 
				gBmpInfo.bmiHeader.biHeight*gBmpInfo.bmiHeader.biWidth);
	}
	else if (gBmpInfo.bmiHeader.biCompression == MAKEFOURCC('Y','U','Y','2')) // 16��Ʈ ����
	{
		YUY2ToRGB24(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight,
					lpVHdr->lpData, gImageBuf.GetPtr());
	}
	else
	{
		return FALSE;
	}

	if (gBgImage.GetChannel() == 3)
	{
		
		SobelEdge(RGB2Gray(gBinImage), gSobelImage);
		//BinarizationBGCol(gImageBuf, gBgImage, gBinImage, 50);
		ShowImage(gSobelImage, "�Һ� ���� ���� ����");

		/*
		if (IsChanged(gBinImage))
		{
			AfxMessageBox("ħ���� ����!!");
		}
		*/
	}
	/*else
	{
		SobelEdge(gBinImage, gSobelImage);
		ShowImage(gSobelImage, "�Һ� ���� ���� ����");
	}*/

	return TRUE;
}

void CVideoProcessorDlg::OnBnClickedButtonStartCam()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOZORDER|SWP_NOMOVE);
	m_webCam.StartCam(GetDlgItem(IDC_STATIC_PREVIEW), 640, 480, gBmpInfo);
	gImageBuf = CByteImage(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight, 3);
	
	m_webCam.SetCallBackOnFrame(ProcessCamFrame);
}

void CVideoProcessorDlg::OnBnClickedButtonStopCam()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_webCam.StopCam();
}

void CVideoProcessorDlg::OnBnClickedButtonRecCam()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = "Descriptor File (*.AVI) | *.AVI; | All Files(*.*)|*.*||";	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // ���� ��ȭ ���� ����
	{
		CString strPathName = dlg.GetPathName();
		m_webCam.StartRecord(strPathName);
	}
}

void CVideoProcessorDlg::OnBnClickedButtonStopRec()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_webCam.StopRecord();
}

void CVideoProcessorDlg::OnBnClickedButtonLoadAvi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = "Descriptor File (*.AVI) | *.AVI; | All Files(*.*)|*.*||";	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // ���� ��ȭ ���� ����
	{
		CString strPathName = dlg.GetPathName();
		int	nFrameRate;
		int nWidth, nHeight;

		if (m_aviPlayer.LoadAvi(strPathName, m_nFrame, nFrameRate, nWidth, nHeight))
		{
			// ȭ�� ����� ���� �������� ũ�� ����
			GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowPos(
				NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER|SWP_NOMOVE);

			// ������ ȹ���� ���� CByteImage Ŭ������ ���� �Ҵ�
			gImageBuf = CByteImage(nWidth, nHeight, 3);
		
			SetTimer(0, 1000/nFrameRate, NULL);
		}
	}
}

void CVideoProcessorDlg::OnBnClickedButtonPause()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bPlay = !m_bPlay;
}

void CVideoProcessorDlg::OnBnClickedButtonCloseAvi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_aviPlayer.CloseAvi();
	KillTimer(0);
	m_bPlay = false;
}

void CVideoProcessorDlg::OnBnClickedButtonSetBg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	gBgImage  = gImageBuf;
	gBinImage = CByteImage(gImageBuf.GetWidth(), gImageBuf.GetHeight());
	gBinImage = gImageBuf;
	//ShowImage(gBgImage, "��� ����");

}

void CVideoProcessorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent==0 && m_bPlay && m_idxFrame<m_nFrame)
	{
		m_aviPlayer.ShowFrame(GetDlgItem(IDC_STATIC_PREVIEW), m_idxFrame);
		m_aviPlayer.GetFrame(gImageBuf, m_idxFrame++);
		
		// ī�޶� é�� ������ ���� �ڵ�
		if (gBgImage.GetChannel())
		{
			BinarizationBGCol(gImageBuf, gBgImage, gBinImage, 50);
			ShowImage(gBinImage, "����ȭ ����");

			if (IsChanged(gBinImage))
			{
				AfxMessageBox("ħ���� ����!!");
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

