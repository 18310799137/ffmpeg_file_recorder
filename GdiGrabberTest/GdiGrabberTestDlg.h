
// GdiGrabberTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "VideoStatic.h"
#include <memory>

// CGdiGrabberTestDlg �Ի���
class CGdiGrabberTestDlg : public CDialogEx
{
// ����
public:
	CGdiGrabberTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CGdiGrabberTestDlg();
// �Ի�������
	enum { IDD = IDD_GDIGRABBERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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
	//MediaFileRecorder::IScreenAudioRecord* m_pRecorder;
	void* m_pRecorder;

	bool record_started_;
	bool record_interrupt_;
	CButton m_ButtonStart;
	int64_t start_capture_time_;

	int64_t duration_;
};
