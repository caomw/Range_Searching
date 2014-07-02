#include "point.h"
#include <vector>

class Node
{
public:
	std::vector <Point> CanonicalSet ;           // the Points in the subtree with the root of this node.
	int SetSize ;                           // number of Points in the subtree with the root of 
											//this node( size of CanonicalSet ).
	double Min ;                            // Minimum of the X components of the Points of this subtree.
	double Max ;                            // Maximum of the X components of the Points of this subtree.
	Node* LeftChild ;
	Node* RightChild ;

	Node( Point P )   // constructor.
	{
		CanonicalSet.push_back(P) ;    // builds a vector consists of only one Point(P).
		SetSize = 1 ;
		Min = Max = P.X ;
		LeftChild = RightChild = NULL ;
	}
	Node() {}    // default constructor
};
