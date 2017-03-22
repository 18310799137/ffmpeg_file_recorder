
// GdiGrabberTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "VideoStatic.h"
#include "ScreenGdiGrabber.h"
#include "MediaFileRecorder.h"
#include <memory>

// CGdiGrabberTestDlg �Ի���
class CGdiGrabberTestDlg : public CDialogEx, IGdiGrabberDataCb
{
// ����
public:
	CGdiGrabberTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GDIGRABBERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void OnScreenData(void* data, int width, int height) override;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonInterrupt();
	DECLARE_MESSAGE_MAP()
private:
	CVideoStatic m_StaticPic;
	std::shared_ptr<CScreenGdiGrabber> gdi_grabber_;
	std::shared_ptr<CMediaFileRecorder> media_file_recorder_;
	bool record_started_;
	bool record_interrupt_;
	CButton m_ButtonStart;
	int64_t start_capture_time_;
};
