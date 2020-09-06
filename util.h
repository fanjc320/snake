#pragma once
#include <vector>
#include <cmath>

static int g_Id = 0;
struct CMyPoint
{
	CMyPoint() {};
	CMyPoint(float x, float y)
	{
		m_x = x;
		m_y = y;
	}
	float m_x = 0.0f;
	float m_y = 0.0f;
};
struct CUnitAngle
{
	std::vector<CMyPoint> m_vPoint;
	void getPoints(int iNum)
	{
		m_vPoint.clear();
		float angle = 360.0f / iNum;
		for (int i = 0; i < iNum; ++i)
		{
			float x = cos(angle * i / 180 * 3.1415926);
			float y = sin(angle * i / 180 * 3.1415926);
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
		float s = sqrt(all);//2次开根bai号函数,必须du包含头文件zhicmath.
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
		m_x = x / distance;
		m_y = y / distance;

	}
	/*CAngle(CMyPoint point)
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
	CMyPoint m_ang_point = CMyPoint(1, 1);
	float m_x = 0;
	float m_y = 0;

	float distancePoint(float x1, float y1, float x2, float y2)
	{
		return  pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
	}
	CVec(float x, float y, float fLength)
	{
		m_angle = CAngle(x, y);
		m_fLength = fLength;

		m_x = m_angle.m_x * fLength;
		m_y = m_angle.m_y * fLength;
	}
	CVec(CAngle angle, float fLength)
	{
		CVec(angle.m_x, angle.m_y, fLength);
	}
	void setXY(CAngle angle, float fLength)
	{
		m_angle = angle;
		m_x = m_angle.m_x * fLength;
		m_y = m_angle.m_y * fLength;
	}
	CVec(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

};


//向量toP是否在p1和p2夹角内，即是否可被p1和p2组合
static bool bInAngle(CMyPoint toP, CMyPoint p1, CMyPoint p2)
{
	//https://wenku.baidu.com/view/f69f660976c66137ee061977.html
	//a*p1 + b*p2 = toP, a>=0,b>=0 则说明toP在p1和p2夹角以内
	//a*p1.m_x+b*p2.m_x = toP.m_x;
	//a*p1.m_y+b*p2.m_y = toP.m_y;

	//a*p1.m_x+b*p2.m_x - toP.m_x = 0;
	//a*p1.m_y+b*p2.m_y - toP.m_y = 0;
	//A1x+B1y+C1=0
	//A2x+B2y+C2=0
	//x=(B1C2-B2C1)/(A1B2-A2B1);y=(C1A2-C2A1)/(A1B2-A2B1);
	float A1 = p1.m_x; float B1 = p2.m_x; float C1 = -toP.m_x;
	float A2 = p1.m_y; float B2 = p2.m_y; float C2 = -toP.m_y;
	float fGap = 0.0001f;
	//a和b都>=0,则存在
	if (A1 * B2 - A2 * B1 < fGap || A1 * B2 - A2 * B1 < fGap)
	{
		return false;
	}
	float a = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	float b = (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1);
	if (a < 0 || b < 0)
	{
		return false;
	}

	return true;
}

struct CLine {
	CLine(CMyPoint begin, CMyPoint end) {
		m_begin = begin;
		m_end = end;
		m_mid = CMyPoint((m_begin.m_x + m_end.m_x) / 2, (m_end.m_y + m_end.m_y) / 2);
		m_unitAngle.getPoints(12);
		++g_Id;

	}
	CLine()
	{
		m_unitAngle.getPoints(12);
		++g_Id;
	}

	CMyPoint m_begin;
	CMyPoint m_end;
	CMyPoint m_mid;
	CAngle m_moveAng = CAngle(1.0f, 0.0f);
	CUnitAngle m_unitAngle;
	int m_angleCnt = 0;

	void moveVec(CMyPoint p)
	{
		m_mid.m_x += p.m_x;
		m_begin.m_x += p.m_x;
		m_end.m_x += p.m_x;

		m_mid.m_y += p.m_x;
		m_begin.m_y += p.m_y;
		m_end.m_y += p.m_y;
	}

	//CVec moveAngle(int iDistance)
	//{
	//	CString str;

	//	int size = m_unitAngle.m_vPoint.size();
	//	int cnt = m_angleCnt % size;
	//	CMyPoint mp = m_unitAngle.m_vPoint[cnt];//一个单位向量，表示方向

	//	/*str.Format(_T("moveAngle cnt:%d x:%6f y:%6f distance:%d\n"),
	//		cnt, mp.m_x, mp.m_y, iDistance);
	//	OutputDebugString(str);*/

	//	++m_angleCnt;
	//	CVec vv(mp.m_x, mp.m_y, iDistance);

	//	vv.m_x = mp.m_x * iDistance; vv.m_y = mp.m_y * iDistance;

	//	moveVec(vv);
	//	return vv;
	//}
	//在angle的基础上加上长度

	//void moveVec(CVec vec)
	//{
	//	int xx = (int)round(vec.m_x);
	//	m_mid.x += xx;
	//	m_begin.x += xx;
	//	m_end.x += xx;

	//	int yy = (int)round(vec.m_y);
	//	m_mid.y += yy;
	//	m_begin.y += yy;
	//	m_end.y += yy;
	//	//return CLine(m_begin, m_end);

	//	/*CString str;
	//	str.Format(_T("moveVec x:%6f y:%6f roundx:%d roundy:%d\n"), vec.m_x, vec.m_y, xx, yy);
	//	OutputDebugString(str);*/
	//}


	void moveToPoint(CMyPoint cp)
	{
		m_mid = cp;
		m_begin.m_x = cp.m_x - 0.05f;
		m_begin.m_y = cp.m_y;
		m_end.m_x = cp.m_x + 0.05f;
		m_end.m_y = cp.m_y;
	}

	/*void moveRight(int iDistance)
	{
		m_mid.x += iDistance;
		m_begin.x += iDistance;
		m_end.x += iDistance;
	}*/
};