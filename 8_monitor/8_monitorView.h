// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// 8_monitorView.h : CMy8_monitorView 类的接口
//

#pragma once

#include <vlc/vlc.h>

class CMy8_monitorView : public CView
{
protected: // 仅从序列化创建
	CMy8_monitorView();
	DECLARE_DYNCREATE(CMy8_monitorView)

// 特性
public:
	CMy8_monitorDoc* GetDocument() const;

// 操作
public:
	void OpenVideo();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMy8_monitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	libvlc_media_player_t * vlc_player_;
	libvlc_media_t *        vlc_media_;
};

#ifndef _DEBUG  // 8_monitorView.cpp 中的调试版本
inline CMy8_monitorDoc* CMy8_monitorView::GetDocument() const
   { return reinterpret_cast<CMy8_monitorDoc*>(m_pDocument); }
#endif

