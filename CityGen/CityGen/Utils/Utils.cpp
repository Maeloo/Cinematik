#include "Utils.h"

// Given three colinear points (x1, y1), (x2, y2), (x3, y3) the function checks if
// point (x1, y1) lies on line segment (x2, y2), (x3, y3)
bool Utils::onSegment(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (x2 <= std::max(x1, x3) && x2 >= std::min(x1, x3) &&
		y2 <= std::max(y1, y3) && y2 >= std::min(y1, y3));
}

// To find orientation of ordered triplet (x1, y1), (x2, y2), (x3, y3)
// The function returns following values
// 0 --> (x1, y1), (x2, y2), (x3, y3) are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int Utils::orientation(float x1, float y1, float x2, float y2, float x3, float y3)
{
	int val = (int)((y2 - y1) * (x3 - x2) -
		(x2 - x1) * (y3 - y2));

	if (val == 0) return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool Utils::isSegmentIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(x1, y1, x2, y2, x3, y3);
	int o2 = orientation(x1, y1, x2, y2, x4, y4);
	int o3 = orientation(x3, y3, x4, y4, x1, y1);
	int o4 = orientation(x3, y3, x4, y4, x2, y2);

	// General case
	return (o1 != o2 && o3 != o4) ||

		// Special Cases
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		(o1 == 0 && onSegment(x1, y1, x3, y3, x2, y2)) ||

		// p1, q1 and p2 are colinear and q2 lies on segment p1q1
		(o2 == 0 && onSegment(x1, y1, x4, y4, x2, y2)) ||

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2
		(o3 == 0 && onSegment(x3, y3, x1, y1, x4, y4)) ||

		// p2, q2 and q1 are colinear and q1 lies on segment p2q2
		(o4 == 0 && onSegment(x3, y3, x2, y2, x4, y4));

	//return false; // Doesn't fall in any of the above cases
}

float Utils::randf(float low, float high)
{
	return low + (float)rand() / (float) (RAND_MAX / (high - low));
}