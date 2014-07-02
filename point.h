#include <iostream>

class Point 
{
public:
	int Num ;
	double X ;
	double Y ;

	Point( int Number , double XX , double YY )
	{
		Num =  Number ;
		X = XX ;
		Y = YY ;
	}
	Point() {}

	bool operator<(  const Point &b )
	{
		return ( this->Y < b.Y ) ;
	}

	
};


bool inline SortByX( const Point &a , const Point &b )
	{
		return ( a.X < b.X ) ;
	}
