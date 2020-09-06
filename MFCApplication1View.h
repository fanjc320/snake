
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once
#include <vector>
#include <cmath>
#include "util.h"

static int g_Id = 0;
static int g_unitNum = 32;
struct CLine {
	CLine(CPoint begin, CPoint end) {
		m_begin = begin;
		m_end = end;
		m_mid = CPoint((m_begin.x + m_end.x) / 2, (m_end.y + m_end.y) / 2);
		m_unitAngle.getPoints(12);
		++g_Id;

	}
	CLine()
	{
		m_unitAngle.getPoints(12);
		++g_Id;
	}

	CPoint m_begin;
	CPoint m_end;
	CPoint m_mid;
	CAngle m_moveAng = CAngle(1.0f,0.0f);
	CUnitAngle m_unitAngle;
	int m_angleCnt = 0;
	

	void moveTo(CPoint point)
	{
		m_mid += point;
		m_begin += point;
		m_end += point;
	}

	CVec moveAngle(int iDistance)
	{
		CString str;
	
		int size = m_unitAngle.m_vPoint.size();
		int cnt = m_angleCnt % size;
		CMyPoint mp = m_unitAngle.m_vPoint[cnt];//一个单位向量，表示方向

		/*str.Format(_T("moveAngle cnt:%d x:%6f y:%6f distance:%d\n"),
			cnt, mp.m_x, mp.m_y, iDistance);
		OutputDebugString(str);*/

		++m_angleCnt;
		CVec vv(mp.m_x, mp.m_y, iDistance);

		vv.m_x = mp.m_x*iDistance; vv.m_y = mp.m_y *iDistance;  

		moveVec(vv);
		return vv;
	}
	//在angle的基础上加上长度
	
	void moveVec(CVec vec)
	{
		int xx = (int)round(vec.m_x);
		m_mid.x += xx;
		m_begin.x += xx;
		m_end.x += xx;

		int yy = (int)round(vec.m_y);
		m_mid.y += yy;
		m_begin.y += yy;
		m_end.y += yy;
		//return CLine(m_begin, m_end);

		/*CString str;
		str.Format(_T("moveVec x:%6f y:%6f roundx:%d roundy:%d\n"), vec.m_x, vec.m_y, xx, yy);
		OutputDebugString(str);*/
	}

	void moveToPoint(CMyPoint cp)
	{
		CVec vec(cp.m_x - m_mid.x, cp.m_y - m_mid.y);
		moveVec(vec);
	}

	void moveRight(int iDistance)
	{
		m_mid.x += iDistance;
		m_begin.x += iDistance;
		m_end.x += iDistance;
	}
};

struct CLines {
	CLines()
	{
		CPoint begin(500, 200);
		int iLength = 50;
		CPoint end = begin;
		for (int i = 0; i < 10; ++i)
		{
			CPoint end(begin.x - iLength, begin.y);
			CLine line(begin, end);
			m_lines.push_back(line);
			begin = end;
		}
		m_unitAngle.getPoints(g_unitNum);
	}

	std::vector<CLine> m_lines;
	
	void moveRight(int iDistance)
	{
		for (auto& line : m_lines)
		{
			line.moveRight(iDistance);
		}
	}

	void addLine(CLine line)
	{
		m_lines.push_back(line);
	}

	CLine moveAngle(int iToIndex)
	{
		//int distance = m_distance;
		CLine& line0 = m_lines[0];
		int iDistance = 20;
		int size = m_unitAngle.m_vPoint.size();
		
		CVec mvVec = CVec(0, 0, 0);
		//CMyPoint myP = m_unitAngle.m_vPoint[index];//当前单位向量
		CString str;
		CMyPoint myP_last;
		bool bFlag = false;
		for (int i=0; i < m_lines.size(); ++i)
		{
			//CMyPoint mp_last = m_unitAngle.m_vPoint[index_last];//上一个单位向量，表示方向
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
				mvVec = CVec(mp.m_x, mp.m_y, iDistance);
				mvVec.m_x = mp.m_x * iDistance; 
				mvVec.m_y = mp.m_y * iDistance;
				str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), index, mp.m_x, mp.m_y);
				OutputDebugString(str);

				line.moveVec(mvVec);
			}
			else 
			{
				CMyPoint vec_now;
				vec_now.m_x = mp.m_x * iDistance;
				vec_now.m_y = mp.m_y * iDistance;
				
				str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), index, mp.m_x, mp.m_y);
				//OutputDebugString(str);

				CMyPoint now;
				now.m_x = myP_last.m_x - vec_now.m_x;
				now.m_y = myP_last.m_y - vec_now.m_y;

				line.moveToPoint(now);
			}
			myP_last.m_x = line.m_mid.x;
			myP_last.m_y = line.m_mid.y;
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

