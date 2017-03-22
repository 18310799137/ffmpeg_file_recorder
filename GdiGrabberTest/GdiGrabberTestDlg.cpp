
// GdiGrabberTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GdiGrabberTest.h"
#include "GdiGrabberTestDlg.h"
#include "afxdialogex.h"
#include <timeapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGdiGrabberTestDlg �Ի���



CGdiGrabberTestDlg::CGdiGrabberTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGdiGrabberTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	record_started_ = false;
	record_interrupt_ = false;
}

void CGdiGrabberTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_StaticPic);
	DDX_Control(pDX, IDC_BUTTON_START, m_ButtonStart);
}

BEGIN_MESSAGE_MAP(CGdiGrabberTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGdiGrabberTestDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_INTERRUPT, &CGdiGrabberTestDlg::OnBnClickedButtonInterrupt)
END_MESSAGE_MAP()


#define CAPTURE_LEFT 0
#define CAPTURE_TOP 0
#define CAPTURE_WIDTH 1920
#define CAPTURE_HEIGHT 1080
#define CAPTURE_FRAME_RATE 15

// CGdiGrabberTestDlg ��Ϣ�������

BOOL CGdiGrabberTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	
	gdi_grabber_.reset(new CScreenGdiGrabber());
	

	media_file_recorder_.reset(new CMediaFileRecorder());
	


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGdiGrabberTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGdiGrabberTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGdiGrabberTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGdiGrabberTestDlg::OnBnClickedButtonStart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (!record_started_)
	{
		RECT grab_rect = { CAPTURE_LEFT, CAPTURE_TOP, CAPTURE_WIDTH, CAPTURE_HEIGHT };
		gdi_grabber_->SetGrabFrameRate(CAPTURE_FRAME_RATE);
		gdi_grabber_->SetGrabRect(grab_rect);
		gdi_grabber_->RegisterDataCb(this);
		gdi_grabber_->RegisterDataCb(&m_StaticPic);

		CMediaFileRecorder::RecordInfo record_info;
		strcpy(record_info.file_name, "test.mp4");
		record_info.video_info.src_width = CAPTURE_WIDTH;
		record_info.video_info.src_height = CAPTURE_HEIGHT;
		record_info.video_info.dst_width = CAPTURE_WIDTH;
		record_info.video_info.dst_height = CAPTURE_HEIGHT;
		record_info.video_info.frame_rate = CAPTURE_FRAME_RATE;
		record_info.video_info.src_pix_fmt = AV_PIX_FMT_RGB24;
		media_file_recorder_->Init(record_info);

		if (gdi_grabber_->StartGrab())
		{
			record_started_ = true;
			m_ButtonStart.SetWindowTextW(L"ֹͣ");
			media_file_recorder_->Start();
			start_capture_time_ = timeGetTime();
		}
	}
	else
	{
		record_started_ = false;
		gdi_grabber_->StopGrab();
		media_file_recorder_->Stop();
		m_ButtonStart.SetWindowTextW(L"��ʼ");

		gdi_grabber_->UnRegisterDataCb(this);
		gdi_grabber_->UnRegisterDataCb(&m_StaticPic);

		media_file_recorder_->UnInit();

		int64_t duration = timeGetTime() - start_capture_time_;
		char log[128] = { 0 };
		_snprintf(log, 128, "duration: %lld \n", duration);
		OutputDebugStringA(log);
	}
		
}


void CGdiGrabberTestDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}

void CGdiGrabberTestDlg::OnScreenData(void* data, int width, int height)
{
	if (media_file_recorder_)
	{
		media_file_recorder_->FillVideo(data);
	}
}


void CGdiGrabberTestDlg::OnBnClickedButtonInterrupt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (record_started_)
	{
		if (!record_interrupt_)
		{
			media_file_recorder_->Stop();
			record_interrupt_ = true;
			GetDlgItem(IDC_BUTTON_INTERRUPT)->SetWindowTextW(L"����");
		}
		else
		{
			media_file_recorder_->Start();
			record_interrupt_ = false;
			GetDlgItem(IDC_BUTTON_INTERRUPT)->SetWindowTextW(L"��ͣ");
		}
	}
	
}
