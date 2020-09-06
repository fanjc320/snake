
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once
#include <vector>
#include <cmath>
#include "util.h"

static int g_unitNum = 32;


struct CLines {
	CLines()
	{
		CMyPoint begin(500, 200);
		int iLength = 10;
		CMyPoint end = begin;
		for (int i = 0; i < 50; ++i)
		{
			CMyPoint end(begin.m_x - iLength, begin.m_y);
			CLine line(begin, end);
			m_lines.push_back(line);
			begin = end;
		}
		m_unitAngle.getPoints(g_unitNum);
	}

	std::vector<CLine> m_lines;
	
	/*void moveRight(int iDistance)
	{
		for (auto& line : m_lines)
		{
			line.moveRight(iDistance);
		}
	}*/

	void addLine(CLine line)
	{
		m_lines.push_back(line);
	}

	void moveTo(CLine& line, int iToIndex)
	{

	}
	void getTravelDirect(int index1, int index2, std::vector<int> vIndexs)//单位向量的集合,按顺序，正序或逆序
	{
		bool bReverse = false;
		if (abs(index1-index2) > g_unitNum/2 )
		{
			bReverse = true;
		}
		int minIdx = min(index1, index2);
		int maxIdx = max(index1, index2);
		
		for (int i = index1;i!=index2;++i)
		{
			vIndexs.push_back(i);
			if (index1<=index2)
			{
				++i;
			}
			else
			{
				--i;
			}
			
		}

	}

	CLine moveToIndex(int iToIndex)
	{
		CLine& line0 = m_lines[0];
		int iDistance = 10;
		int size = m_unitAngle.m_vPoint.size();
		
		CMyPoint mvVec;
		CString str;
		CMyPoint myP_last;
		bool bFlag = false;
		for (int i=0; i < m_lines.size(); ++i)
		{
			CLine& line = m_lines[i];

			int index = 0;
			int index_next = 0;
			CMyPoint mp;
			if (m_moveNum + m_Angle - i > 0)
			{
				index = (m_moveNum + m_Angle - i) % size;
				index_next = (m_moveNum + m_Angle - i + 1) % size;
				mp = m_unitAngle.m_vPoint[index];//一个单位向量，表示方向
				CMyPoint mp_next = m_unitAngle.m_vPoint[index_next];//一个单位向量，表示方向

				/*str.Format(_T("moveAngle index:%d index_next:%d mpx:%6f mpy:%6f mpnx:%6f mpny:%6f \n"), 
					index, index_next, mp.m_x, mp.m_y, mp_next.m_x, mp_next.m_y);
				OutputDebugString(str);*/

				if (index == iToIndex)
				{
					str.Format(_T("===============moveAngle index:%d toindex:%d mpx:%6f mpy:%6f mpnx:%6f mpny:%6f===============\n"), 
						index, iToIndex, mp.m_x, mp.m_y, mp_next.m_x, mp_next.m_y);
					OutputDebugString(str);
					bFlag = true;
				}
			}
			
			if (i == 0)
			{
				mvVec = CMyPoint(mp.m_x, mp.m_y);
				mvVec.m_x = mp.m_x * iDistance; 
				mvVec.m_y = mp.m_y * iDistance;
				str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), index, mp.m_x, mp.m_y);
				OutputDebugString(str);

				line.moveVec(mvVec);
				myP_last.m_x = mvVec.m_x;
				myP_last.m_y = mvVec.m_y;
			}
			else 
			{
				CMyPoint vec_now;
				vec_now.m_x = mp.m_x * iDistance;
				vec_now.m_y = mp.m_y * iDistance;
				//str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), index, mp.m_x, mp.m_y);
				//OutputDebugString(str);

				CMyPoint now;
				now.m_x = myP_last.m_x - vec_now.m_x;
				now.m_y = myP_last.m_y - vec_now.m_y;

				line.moveToPoint(now);
			}
			myP_last.m_x = line.m_mid.m_x;
			myP_last.m_y = line.m_mid.m_y;
		}
		if (bFlag)
		{

		}
		else
		{
			++m_moveNum;
		}
		return line0;
	}
	
	int m_distance = 50;
	CUnitAngle m_unitAngle;
	int m_Angle = 5;//5个单位向量
	int m_Index = 5;//
	int m_moveNum = 0;
};

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
	void drawLine(CDC* pDC, CLine& line);
	void drawLines(CDC* pDC, std::vector<CLine>& lines);
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyPoint m_Direction;
	int m_ToIndex;
	
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

//void initLines(std::vector<CLine>& lines);
void initLines();

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

