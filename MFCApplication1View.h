
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once
#include <vector>
#include <cmath>

struct CMyPoint
{
	float m_x;
	float m_y;
};
struct CUnitAngle
{
	std::vector<CMyPoint> m_vPoint;
	void getPoints(int iNum)
	{
		float angle = 360.0f / iNum;
		for (int i=0;i<iNum;++i)
		{
			float x = cos(angle*i / 180 * 3.1415926);
			float y = sin(angle*i / 180 * 3.1415926);
			CMyPoint p;
			p.m_x = x;
			p.m_y = y;
			m_vPoint.push_back(p);
		}
	}
	void printPoints()
	{
		for (auto p : m_vPoint)
		{
			//std::cout << p.m_x << p.m_y << std::endl;
		}
	}
};



//单位向量，用来表示角度
struct CAngle
{
	float distancePoint(float x1, float y1, float x2, float y2)
	{
		//return  pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
		float all = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
		float s =sqrt(all);//2次开根bai号函数,必须du包含头文件zhicmath.
		return s;
	}

	CAngle(float x, float y)//输入目标向量
	{
		//int distance = (int)distancePoint(0, 0, x, y);
		float distance = distancePoint(x, y, 0, 0);
		if (distance < 0.0001)
		{
			distance = 0.0001;
		}
		//求差向量
		x -= distance;
		distance = distancePoint(x, y, 0, 0);
		if (distance < 0.0001)
		{
			distance = 0.0001;
		}
		m_x = x  / distance;
		m_y = y  / distance;
		 
	}
	/*CAngle(CPoint point)
	{
		CAngle(point.x, point.y);
	}*/
	float m_x = 1;
	float m_y = 0;
};

struct CVec// 在angle的基础上加上长度
{
	CAngle m_angle = CAngle(1, 0);
	//CAngle m_angle(1,0);
	float m_fLength = 0;
	CPoint m_ang_point = CPoint(1, 1);
	float m_x = 0;
	float m_y = 0;

	float distancePoint(float x1, float y1, float x2, float y2)
	{
		return  pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
	}
	CVec(float x , float y, float fLength)
	{
		m_angle = CAngle(x, y);
		m_fLength = fLength;
		
		m_x = m_angle.m_x * fLength;
		m_y = m_angle.m_y * fLength;
	}
	CVec(CAngle angle, float fLength)
	{
		CVec(angle.m_x,angle.m_y, fLength);
	}
	void setXY(CAngle angle, float fLength)
	{
		m_angle = angle;
		m_x = m_angle.m_x * fLength;
		m_y = m_angle.m_y * fLength;
	}
	/*CVec(float x , float y)
	{
		m_x = x;
		m_y = y;
		m_angle = CAngle(x,y);
		m_fLength = distancePoint(0, 0, x, y);
	}*/

	
};

static int g_Id = 0;

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

	//CLine moveAngle(CAngle angle, int iDistance)
	void moveAngle(CAngle angle, int iDistance)
	{
		//m_moveAng = CAngle(1.0f, 0.0f);
		CAngle newAng = CAngle(m_moveAng.m_x+angle.m_x, m_moveAng.m_y+angle.m_y);
		m_moveAng = newAng;

		CString str;
		float length = m_moveAng.m_x* m_moveAng.m_x + m_moveAng.m_y * m_moveAng.m_y;
		str.Format(_T("moveAngle id:%d x:%6f y:%6f length:%6f angle_x:%6f angle_y:%6f\n"), 
			g_Id, m_moveAng.m_x, m_moveAng.m_y ,length, angle.m_x, angle.m_y);
		//OutputDebugString(str);

		int size = m_unitAngle.m_vPoint.size();
		int cnt = m_angleCnt % size;
		CMyPoint mp = m_unitAngle.m_vPoint[cnt];//一个单位向量，表示方向

		str.Format(_T("moveAngle cnt:%d x:%6f y:%6f \n"),
			cnt, mp.m_x, mp.m_y);
		OutputDebugString(str);
		++m_angleCnt;
		CVec vv(mp.m_x,mp.m_y, iDistance);
		moveVec(vv);

		/*CVec vec(m_moveAng, iDistance);
		CVec vec(m_moveAng,iDistance);
		vec.setXY(m_moveAng,iDistance);
		moveVec(vec);*/
		//return CLine(m_begin, m_end);

		
	}
	//在angle的基础上加上长度
	//CLine moveVec(CVec vec)
	void moveVec(CVec vec)
	{
		CString str;
		//str.Format(_T("moveVec x:%6f y:%6f\n"), vec.m_x, vec.m_y);
		OutputDebugString(str);

		m_mid.x += vec.m_x;
		m_begin.x += vec.m_x;
		m_end.x += vec.m_x;

		int yy = (int)round(vec.m_y);
		m_mid.y += yy;
		m_begin.y += yy;
		m_end.y += yy;
		//return CLine(m_begin, m_end);
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
		CPoint begin(200, 200);
		int iLength = 50;
		CPoint end = begin;
		for (int i = 0; i < 10; ++i)
		{
			CPoint end(begin.x - iLength, begin.y);
			CLine line(begin, end);
			m_lines.push_back(line);
			begin = end;
		}
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

	CLine moveAngle()
	{
		int distance = m_distance;
		CAngle angle = CAngle(10, 1);
		CLine& line0 = m_lines[0];
		for (int i=0; i < m_lines.size(); ++i)
		{
			CLine& line = m_lines[i];
			//CLine newLine = line.moveAngle(angle, distance);
			line.moveAngle(angle, distance);
			/*
			CPoint angPoint = (newLine.m_mid.x - line.m_mid.x , newLine.m_mid.y - line.m_mid.y);
			//angle = CAngle(angPoint.x, angPoint.y);
			if (i==0)
			{
				line0 = newLine;
			}*/
			
		}
		return line0;
	}
	
	CAngle m_angle = CAngle(10, 1);
	int m_distance = 10;
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
};

//void initLines(std::vector<CLine>& lines);
void initLines();

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

