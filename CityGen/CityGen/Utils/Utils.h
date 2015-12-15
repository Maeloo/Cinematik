#pragma once
#include <algorithm>
namespace Utils
{
	// Given three colinear points (x1, y1), (x2, y2), (x3, y3) the function checks if
	// point (x1, y1) lies on line segment (x2, y2), (x3, y3)
	bool onSegment(float, float, float, float, float, float);

	// To find orientation of ordered triplet (x1, y1), (x2, y2), (x3, y3)
	// The function returns following values
	// 0 --> (x1, y1), (x2, y2), (x3, y3) are colinear
	// 1 --> Clockwise
	// 2 --> Counterclockwise
	int orientation(float, float, float, float, float, float);

	// The main function that returns true if line segment 'p1q1'
	// and 'p2q2' intersect.
	bool isSegmentIntersect(float, float, float, float, float, float, float, float);

	/* rand flottant
	* low, high : interval 
	* return : float between low and high
	*/
	float randf(float, float);
};

