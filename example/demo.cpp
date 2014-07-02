#include "rangeSearchingTree.h"
#include <iostream>
#include <algorithm>
using namespace std ;



int main()
{


	//fstream fin( "test1.txt" , ios::in ) ;
	int n , q ;              
	double Xinput , Yinput ;
	vector <Point> Points ;

	cin >> n >> q ;                  // n is the number of Points. q is the number of queries.
	for( int i = 0 ; i < n ; i++ ) 
	{
		cin >> Xinput >> Yinput ;                // gets the Points from input and builds their objects.
		Point CurrentPoint( i+1 , Xinput , Yinput ) ;   // Point numbers are 1 to n.
		Points.push_back( CurrentPoint ) ;
	}
	sort( Points.begin() , Points.end() , SortByX ) ;   // sorts the Points according to their X components
														// these sorted Points will be the leaves of our orthogonal tree.

	//cerr<< "building tree started" << endl ;
	RangeSearchingTree OrthogonalTree( Points ) ;     // Builds the orthogonal tree with the points.
	//cerr<< "building tree finished" << endl ;
	
	bool Command ;
	double X1 , Y1 , X2 , Y2 ;
	vector<Point> Answers ;
	int AnswersNumber ;    // number of answers( for the case of Command==0 )
	for( int i=0 ; i < q ; i++ )
	{
		cin >> Command >> X1 >> Y1 >> X2 >> Y2 ;

		if( Command )  // case of Command==1==true. In this case we print the number of each point that is in that range.
						// The number of each point is according to the order of input.
		{
			Answers = OrthogonalTree.RangeQuery( X1 , Y1 , X2 , Y2 ) ;
			cout << Answers.size() << " " ;
			for ( int i=0 ; i < Answers.size() ; i++ ) 
			{
				if( i == ( Answers.size() - 1 ) )  // just for avoiding an extra space at the end of line.
				{
					cout << Answers[i].Num ;
					continue ;
				}
				cout << Answers[i].Num << " " ;
			}
			cout << endl ;
		}

		else if( !Command )   // case of Command==0==false . In this case we only print the count of points in that range.
		{
			AnswersNumber = OrthogonalTree.RangeQuery0( X1 , Y1 , X2 , Y2 ) ;
			cout << AnswersNumber ;
			cout << endl ;
		}
	}


	//system("pause") ;
	return 0 ;
}
