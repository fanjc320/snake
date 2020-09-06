#pragma once
#include <vector>
#include <cmath>


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