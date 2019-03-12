#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>

using point_type = boost::geometry::model::d2::point_xy<double>;
using polygon_type = boost::geometry::model::polygon<point_type>;
using linestring_type = boost::geometry::model::linestring<point_type>;
using multi_linestring_type = boost::geometry::model::multi_linestring<linestring_type>;

int main()
{
    polygon_type polygon;

    polygon.outer().push_back(point_type{0, 0});
    polygon.outer().push_back(point_type{0, 1});
    polygon.outer().push_back(point_type{1, 1});
    polygon.outer().push_back(point_type{1, 0});

    polygon_type polygon2;

    polygon2.outer().push_back(point_type{0.5, -0.5});
    polygon2.outer().push_back(point_type{0.5, 0.5});
    polygon2.outer().push_back(point_type{1.5, 0.5});
    polygon2.outer().push_back(point_type{1.5, -0.5});

    // linestring_type polygon;

    // polygon.push_back(point_type{10, 10});
    // polygon.push_back(point_type{10, 12});
    // polygon.push_back(point_type{12, 12});
    // polygon.push_back(point_type{12, 10});
    // polygon.push_back(point_type{10, 10});

    // linestring_type linestring;
    // linestring.push_back(point_type{11, 9});
    // linestring.push_back(point_type{11, 11});
    // linestring.push_back(point_type{13, 11});

    // Expected intersections at (11, 10) and (12, 11)

    multi_linestring_type intersection;
    boost::geometry::intersection(polygon, polygon2, intersection);

    for (auto intersectionIter = intersection.begin(); intersectionIter != intersection.end(); ++intersectionIter)
    {
        linestring_type intersectionPiece = *intersectionIter;
        std::cout << "Piece:" << std::endl;
        for (auto intersectionPieceIter = intersectionPiece.begin(); intersectionPieceIter != intersectionPiece.end(); ++intersectionPieceIter)
        {
            std::cout << boost::geometry::get<0>(*intersectionPieceIter) << " " << boost::geometry::get<1>(*intersectionPieceIter) << std::endl;
        }
    }

    return 0;
}