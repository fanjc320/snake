
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once
#include <vector>
#include <cmath>
#include "util.h"
using namespace std;



class CMFCApplication1View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// 特性
public:
	CMFCApplication1Doc* GetDocument() const;

// 操作
public:

	CPen m_pen[5];
	CPoint m_point[5];
	void DrawLine(CDC* pDC);
	void drawPoits(CDC* pDC, std::vector<CPoint> points);
	void DrawPolyline(CDC* pDC);
	void DrawPolygon(CDC* pDC);
	void DrawRect(CDC* pDC);
	void DrawRoundRect(CDC* pDC);
	//void DrawEllipse(CDC* pDC);
	void DrawArc(CDC* pDC);
	void DrawAngleArc(CDC* pDC);

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void DrawEllipse(CDC* pDC, CPoint mid);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void drawNode(CDC* pDC, CNode& node);
	/*void drawLine(CDC* pDC, CLine& line);
	void drawLines(CDC* pDC, std::vector<CLine>& lines);*/
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyPoint m_Direction;
	int m_ToIndex = 0;
	
	//afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

//void initLines(std::vector<CLine>& lines);
void initLines();

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

