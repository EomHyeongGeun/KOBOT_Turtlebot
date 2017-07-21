
// VideoProcessorDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CVideoProcessorDlg 대화 상자



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


// CVideoProcessorDlg 메시지 처리기

BOOL CVideoProcessorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CVideoProcessorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_aviPlayer.ShowFrame(GetDlgItem(IDC_STATIC_PREVIEW), m_idxFrame);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CVideoProcessorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 카메라 영상 및 정보를 저장하기 위한 전역 변수
CByteImage gImageBuf;
BITMAPINFO gBmpInfo;

// 물체 검출 관련 전역 변수
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

LRESULT ProcessCamFrame(HWND hWnd, LPVIDEOHDR lpVHdr) // 실제 메인이 되는 부분
{
	SobelEdge(RGB2Gray(gImageBuf), gImageBuf);
	if (gBmpInfo.bmiHeader.biCompression == BI_RGB) // RGB 영상
	{
		memcpy(gImageBuf.GetPtr(), lpVHdr->lpData, 
				gBmpInfo.bmiHeader.biHeight*gBmpInfo.bmiHeader.biWidth);
	}
	else if (gBmpInfo.bmiHeader.biCompression == MAKEFOURCC('Y','U','Y','2')) // 16비트 영상
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
		ShowImage(gSobelImage, "소벨 엣지 추출 영상");

		/*
		if (IsChanged(gBinImage))
		{
			AfxMessageBox("침입자 감지!!");
		}
		*/
	}
	/*else
	{
		SobelEdge(gBinImage, gSobelImage);
		ShowImage(gSobelImage, "소벨 엣지 추출 영상");
	}*/

	return TRUE;
}

void CVideoProcessorDlg::OnBnClickedButtonStartCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOZORDER|SWP_NOMOVE);
	m_webCam.StartCam(GetDlgItem(IDC_STATIC_PREVIEW), 640, 480, gBmpInfo);
	gImageBuf = CByteImage(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight, 3);
	
	m_webCam.SetCallBackOnFrame(ProcessCamFrame);
}

void CVideoProcessorDlg::OnBnClickedButtonStopCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_webCam.StopCam();
}

void CVideoProcessorDlg::OnBnClickedButtonRecCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szFilter[] = "Descriptor File (*.AVI) | *.AVI; | All Files(*.*)|*.*||";	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // 파일 대화 상자 열기
	{
		CString strPathName = dlg.GetPathName();
		m_webCam.StartRecord(strPathName);
	}
}

void CVideoProcessorDlg::OnBnClickedButtonStopRec()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_webCam.StopRecord();
}

void CVideoProcessorDlg::OnBnClickedButtonLoadAvi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szFilter[] = "Descriptor File (*.AVI) | *.AVI; | All Files(*.*)|*.*||";	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // 파일 대화 상자 열기
	{
		CString strPathName = dlg.GetPathName();
		int	nFrameRate;
		int nWidth, nHeight;

		if (m_aviPlayer.LoadAvi(strPathName, m_nFrame, nFrameRate, nWidth, nHeight))
		{
			// 화면 출력을 위한 윈도우의 크기 설정
			GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowPos(
				NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER|SWP_NOMOVE);

			// 프레임 획득을 위한 CByteImage 클래스의 공간 할당
			gImageBuf = CByteImage(nWidth, nHeight, 3);
		
			SetTimer(0, 1000/nFrameRate, NULL);
		}
	}
}

void CVideoProcessorDlg::OnBnClickedButtonPause()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bPlay = !m_bPlay;
}

void CVideoProcessorDlg::OnBnClickedButtonCloseAvi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_aviPlayer.CloseAvi();
	KillTimer(0);
	m_bPlay = false;
}

void CVideoProcessorDlg::OnBnClickedButtonSetBg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gBgImage  = gImageBuf;
	gBinImage = CByteImage(gImageBuf.GetWidth(), gImageBuf.GetHeight());
	gBinImage = gImageBuf;
	//ShowImage(gBgImage, "배경 영상");

}

void CVideoProcessorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent==0 && m_bPlay && m_idxFrame<m_nFrame)
	{
		m_aviPlayer.ShowFrame(GetDlgItem(IDC_STATIC_PREVIEW), m_idxFrame);
		m_aviPlayer.GetFrame(gImageBuf, m_idxFrame++);
		
		// 카메라 챕쳐 예제와 같은 코드
		if (gBgImage.GetChannel())
		{
			BinarizationBGCol(gImageBuf, gBgImage, gBinImage, 50);
			ShowImage(gBinImage, "이진화 영상");

			if (IsChanged(gBinImage))
			{
				AfxMessageBox("침입자 감지!!");
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

