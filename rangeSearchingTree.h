#include "node.h"

class RangeSearchingTree
{
public:
	Node* Root ;           // Pointer to the root of the tree.

	// constructor. Gets the Points and builds their orthogonal tree.
	RangeSearchingTree( std::vector <Point> PointsVector ) ;

	// This function build the orthogonal tree recursively. we will call it in the constructor.
	Node* BuildTree ( const std::vector <Point> &PointsVector , int FirstIndex , int LastIndex ) ;

	// returns the Points in the requested range as a vector.
	std::vector<Point> RangeQuery( double X1 , double Y1 , double X2 , double Y2 ) ;

	// searches the the nodes with Y component between Y1 and Y2 in a canonical set of a node (with binary search).
	void SearchForY ( Node* NodePtr , double Y1 , double Y2 , std::vector <Point> &Answers ) ;

	// difference with RangeQuery function: this one only returns the number of Points(not the Points themselves as a vector).
	int RangeQuery0( double X1 , double Y1 , double X2 , double Y2 ) ;

	// difference with SearchForY: this one only returns the number of answers in that node.
	int SearchForY0 ( Node* NodePtr , double Y1 , double Y2 ) ;
};

