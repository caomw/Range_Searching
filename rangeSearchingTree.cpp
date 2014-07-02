#include "rangeSearchingTree.h"
#include <algorithm>




RangeSearchingTree::RangeSearchingTree( std::vector <Point> PointsVector )  // constructor. Gets the Points and builds their orthogonal tree.
	{
		//cerr<< "building tree started" << endl ;
		Root = BuildTree( PointsVector , 0 , PointsVector.size() - 1 ) ;
		//cerr<< "building tree finished" << endl ;
	}
	

	// This function build the orthogonal tree recursively. we will call it in the constructor.
	Node* RangeSearchingTree::BuildTree ( const std::vector <Point> &PointsVector , int FirstIndex , int LastIndex )  
	{	
		Node* NewNodePtr ;

		if( FirstIndex == LastIndex )
		{
			NewNodePtr = new Node( PointsVector[FirstIndex] ) ;
			return NewNodePtr ;
		}
		NewNodePtr = new Node ;

		NewNodePtr->LeftChild = BuildTree( PointsVector , FirstIndex ,  (FirstIndex + LastIndex)/2 ) ;
		NewNodePtr->RightChild = BuildTree( PointsVector , ((FirstIndex + LastIndex)/2) + 1 , LastIndex ) ;

		NewNodePtr->Max = NewNodePtr->RightChild->Max ;   
		NewNodePtr->Min = NewNodePtr->LeftChild->Min ;

		NewNodePtr->SetSize = NewNodePtr->LeftChild->SetSize + NewNodePtr->RightChild->SetSize ;
		NewNodePtr->CanonicalSet.resize( NewNodePtr->LeftChild->CanonicalSet.size() + NewNodePtr->RightChild->CanonicalSet.size() ) ;
		// merges the two sorted vectors(according to Y coordinates) of children and makes the vector of their parent.
		merge(  NewNodePtr->LeftChild->CanonicalSet.begin()	, NewNodePtr->LeftChild->CanonicalSet.end() 
			, NewNodePtr->RightChild->CanonicalSet.begin() , NewNodePtr->RightChild->CanonicalSet.end() , &(NewNodePtr->CanonicalSet[0]) ) ;
		return NewNodePtr ;
	}

	// returns the Points in the requested range as a vector.
	std::vector<Point> RangeSearchingTree::RangeQuery( double X1 , double Y1 , double X2 , double Y2 )
	{
		std::vector<Point> Answers ;
		Node* LeftIter ;
		Node* RightIter ;
		LeftIter = RightIter = Root ;

		bool LeftIterCameLeft = true ;
		bool RightIterCameRight = true ;


		bool Departed = false ;   // shows that the paths of finding X1 and X2 in the tree have been departed or no.
		bool LeftSearchingFinished = false ;  // shows whether the finding of X1 has been finished or we should still iterate in the tree.
		bool RightSearchingFinished = false ;  // shows whether the finding of X2 has been finished or we should still iterate in the tree.
		while( true )
		{
			if( !LeftSearchingFinished )
			{
				if( X1 <= LeftIter->LeftChild->Max /*&& X1 >= LeftIter->LeftChild->Min*/ )
				{
					if( Departed )
						SearchForY( LeftIter->RightChild , Y1 , Y2 , Answers ) ; 
					LeftIter = LeftIter->LeftChild ;
					LeftIterCameLeft = true ;
					if ( LeftIter->LeftChild == NULL ) // also rightchild
						LeftSearchingFinished = true ;
				}
				else if( /*X1 <= LeftIter->RightChild->Max &&*/ X1 >= LeftIter->RightChild->Min )
				{
					/*if( Departed )
						SearchForY( LeftIter->RightChild , Y1 , Y2 , Answers  ) ; */
					LeftIter = LeftIter->RightChild ;
					LeftIterCameLeft = false ;
					if ( LeftIter->LeftChild == NULL ) // also rightchild
						LeftSearchingFinished = true ;
				}
				else
				{
					if( Departed )
						SearchForY( LeftIter->RightChild , Y1 , Y2 , Answers ) ; 
					LeftSearchingFinished = true ;
				}
			}
			if( !RightSearchingFinished )
			{
				if( X2 <= RightIter->LeftChild->Max /*&& X2 >= RightIter->LeftChild->Min*/ )
				{
					/*if( Departed )
						SearchForY( RightIter->LeftChild , Y1 , Y2 , Answers ) ; */
					RightIter = RightIter->LeftChild ;
					RightIterCameRight = false ;
					if ( RightIter->LeftChild == NULL ) // also Leftchild
						RightSearchingFinished = true ;
				}
				else if( /*X2 <= RightIter->RightChild->Max &&*/ X2 > RightIter->RightChild->Min )
				{
					if( Departed )
						SearchForY( RightIter->LeftChild , Y1 , Y2 , Answers ) ; 
					RightIter = RightIter->RightChild ;
					RightIterCameRight = true ;
					if ( RightIter->LeftChild == NULL ) // also Leftchild
						RightSearchingFinished = true ;
				}
				else
				{
					if( Departed )
						SearchForY( RightIter->LeftChild , Y1 , Y2 , Answers ) ; 
					RightSearchingFinished = true ;
				}
			}
			if( RightIter != LeftIter )
				Departed = true ;
			if( RightSearchingFinished && LeftSearchingFinished )
			{
				// now we should also check the nodes we have stopped there. (if they are leaves and haven't checked yet)
				if( X1 <= LeftIter->Min && LeftIter->LeftChild == NULL /*&& LeftIterCameLeft*/ )  
					SearchForY(LeftIter , Y1 , Y2 , Answers ) ;
				if( X2 > RightIter->Max && LeftIter->LeftChild == NULL /*&& RightIterCameRight*/ )
					SearchForY(RightIter , Y1 , Y2 , Answers ) ;
				break ;
			}
		}
		return Answers ;

	}

	// searches the the nodes with Y component between Y1 and Y2 in a canonical set of a node (with binary search).
	void RangeSearchingTree::SearchForY ( Node* NodePtr , double Y1 , double Y2 , std::vector <Point> &Answers )   
	{
		Point Y1Point(0 , 0 , Y1 ) ;
		Point Y2Point(0 , 0 , Y2 ) ;

		std::vector<Point>::iterator StartPtr = lower_bound( NodePtr->CanonicalSet.begin() , NodePtr->CanonicalSet.end() , Y1Point ) ;
		std::vector<Point>::iterator FinishPtr = lower_bound( NodePtr->CanonicalSet.begin() , NodePtr->CanonicalSet.end() , Y2Point ) ;

		std::vector<Point>::iterator i ;
		for ( i = StartPtr ; i < FinishPtr ; i++ )
			Answers.push_back( *i ) ;
	}
	
	// difference with RangeQuery function: this one only returns the number of Points(not the Points themselves as a vector).
	int RangeSearchingTree::RangeQuery0( double X1 , double Y1 , double X2 , double Y2 )
	{
		int AnswersNumber = 0 ;
		Node* LeftIter ;
		Node* RightIter ;
		LeftIter = RightIter = Root ;

		bool LeftIterCameLeft = true ;
		bool RightIterCameRight = true ;

		bool Departed = false ;
		bool LeftSearchingFinished = false ;
		bool RightSearchingFinished = false ;
		while( true )
		{
			if( !LeftSearchingFinished )
			{
				if( X1 <= LeftIter->LeftChild->Max /*&& X1 >= LeftIter->LeftChild->Min*/ )
				{
					if( Departed )
						AnswersNumber += SearchForY0( LeftIter->RightChild , Y1 , Y2  ) ; 
					LeftIter = LeftIter->LeftChild ;
					LeftIterCameLeft = true ;
					if ( LeftIter->LeftChild == NULL ) // also rightchild
						LeftSearchingFinished = true ;
				}
				else if( /*X1 <= LeftIter->RightChild->Max &&*/ X1 >= LeftIter->RightChild->Min )
				{
					/*if( Departed )
						AnswersNumber += SearchForY0( LeftIter->RightChild , Y1 , Y2   ) ; */
					LeftIter = LeftIter->RightChild ;
					LeftIterCameLeft = false ;
					if ( LeftIter->LeftChild == NULL ) // also rightchild
						LeftSearchingFinished = true ;
				}
				else
				{
					if( Departed )
						AnswersNumber += SearchForY0( LeftIter->RightChild , Y1 , Y2  ) ; 
					LeftSearchingFinished = true ;
				}
			}
			if( !RightSearchingFinished )
			{
				if( X2 <= RightIter->LeftChild->Max /*&& X2 >= RightIter->LeftChild->Min*/ )
				{
					/*if( Departed )
						AnswersNumber += SearchForY0( RightIter->LeftChild , Y1 , Y2  ) ; */
					RightIter = RightIter->LeftChild ;
					RightIterCameRight = false ;
					if ( RightIter->LeftChild == NULL ) // also Leftchild
						RightSearchingFinished = true ;
				}
				else if( /*X2 <= RightIter->RightChild->Max &&*/ X2 > RightIter->RightChild->Min )
				{
					if( Departed )
						AnswersNumber += SearchForY0( RightIter->LeftChild , Y1 , Y2  ) ; 
					RightIter = RightIter->RightChild ;
					RightIterCameRight = true ;
					if ( RightIter->LeftChild == NULL ) // also Leftchild
						RightSearchingFinished = true ;
				}
				else
				{
					if( Departed )
						AnswersNumber += SearchForY0( RightIter->LeftChild , Y1 , Y2  ) ; 
					RightSearchingFinished = true ;
				}
			}
			if( RightIter != LeftIter )
				Departed = true ;

			if( RightSearchingFinished && LeftSearchingFinished )
			{
				if( X1 <= LeftIter->Min && LeftIter->LeftChild == NULL /*&& LeftIterCameLeft*/  )
					AnswersNumber += SearchForY0(LeftIter , Y1 , Y2 ) ;
				if( X2 > RightIter->Max && RightIter->LeftChild == NULL /*&& RightIterCameRight*/ )
					AnswersNumber += SearchForY0(RightIter , Y1 , Y2 ) ;
				break ;
			}
		}
		return AnswersNumber ;
	}



	// difference with SearchForY: this one only returns the number of answers in that node.
	int RangeSearchingTree::SearchForY0 ( Node* NodePtr , double Y1 , double Y2 )
	{
		Point Y1Point(0 , 0 , Y1 ) ;
		Point Y2Point(0 , 0 , Y2 ) ;
		std::vector<Point>::iterator StartPtr = lower_bound( NodePtr->CanonicalSet.begin() , NodePtr->CanonicalSet.end() , Y1Point ) ;
		std::vector<Point>::iterator FinishPtr = lower_bound( NodePtr->CanonicalSet.begin() , NodePtr->CanonicalSet.end() , Y2Point ) ;
		return (FinishPtr - StartPtr) ;
		
	}
