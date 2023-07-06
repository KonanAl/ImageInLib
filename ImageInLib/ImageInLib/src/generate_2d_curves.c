#include "generate_2d_curves.h"

bool generateCircleCurve(Point2D* pCurve, const Point2D* pInitialPoints, const size_t initialPointsCount, const double pointsDistance)
{
    if (initialPointsCount != 2)
    {
        return false;
    }

    const double radius = getPoint2DDistance(pInitialPoints[0], pInitialPoints[1]);

    return false;
}

size_t howManyPointsForCircleCurve(const Point2D* pInitialPoints, const size_t initialPointsCount, const double pointsDistance)
{
    if (initialPointsCount != 2)
    {
        return 0;
    }

    const double radius = getPoint2DDistance(pInitialPoints[0], pInitialPoints[1]);

    return 0;
}
