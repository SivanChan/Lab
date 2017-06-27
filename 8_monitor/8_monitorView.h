// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// 8_monitorView.h : CMy8_monitorView ��Ľӿ�
//

#pragma once

#include <vlc/vlc.h>
#include <thread>

class CMy8_monitorView : public CView
{
protected: // �������л�����
	CMy8_monitorView();
	DECLARE_DYNCREATE(CMy8_monitorView)

// ����
public:
	CMy8_monitorDoc* GetDocument() const;

// ����
public:
	void OpenVideo();
	void CaptureVideo();

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMy8_monitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	libvlc_media_player_t * vlc_player_;
	libvlc_media_t *        vlc_media_;
	std::string             ip_port_;
	std::string             path_;
};

#ifndef _DEBUG  // 8_monitorView.cpp �еĵ��԰汾
inline CMy8_monitorDoc* CMy8_monitorView::GetDocument() const
   { return reinterpret_cast<CMy8_monitorDoc*>(m_pDocument); }
#endif

