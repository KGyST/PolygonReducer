#ifndef POLYGONUSERDATA_HPP
#define POLYGONUSERDATA_HPP
#define ACExtension
#include "ACAPinc.h"
#include <APIdefs.h>

//Storing all relevant data 

class PolygonUserData
{
private:
	int				m_iCurrentPoint;
	API_ElementMemo m_memo;
public:
	int						getCurrentPoint()	const		{ return m_iCurrentPoint; };
	const API_ElementMemo&	getMemo()			const		{ return m_memo; };
	void					setCurrentPoint(int iPoint)		{m_iCurrentPoint = iPoint;};
	void					setMemo(API_ElementMemo memo)	{ m_memo = memo; };
};

#endif // !POLYGONUSERDATA_HPP