#include <iostream>
#include <deque>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/correct.hpp>
#include <boost/foreach.hpp>

int main()
{
    typedef boost::geometry::model::d2::point_xy<double> point;
    typedef boost::geometry::model::polygon<point> polygon;

    //First Polygon
    polygon poly;
    // boost::geometry::append(poly, point(0, 0));
    // boost::geometry::append(poly, point(0, 5));
    // boost::geometry::append(poly, point(1, 5));
    // boost::geometry::append(poly, point(5, 5));
    poly.outer().push_back(point(0, 0));
    poly.outer().push_back(point(0, 1));
    poly.outer().push_back(point(1, 1));
    poly.outer().push_back(point(1, 0));

    // std::string reason;
    // bool first_valid = boost::geometry::is_valid(poly, reason);
    // std::cout << boost::geometry::dsv(poly) << " - " << (!first_valid ? "invalid (" + reason + ")" : "valid") << "\n";
    boost::geometry::correct(poly);
    // std::cout << boost::geometry::dsv(poly) << " - " << (!first_valid ? "invalid (" + reason + ")" : "valid") << "\n";

    polygon poly2;
    // poly2.outer().push_back(point(0.5, -0.5));
    // poly2.outer().push_back(point(0.5, 0.5));
    // poly2.outer().push_back(point(1.5, 0.5));
    // poly2.outer().push_back(point(1.5, -0.5));

    // poly2.outer().push_back(point(0, 0));
    // poly2.outer().push_back(point(0, 0.5));
    // poly2.outer().push_back(point(0.5, 0.5));
    // poly2.outer().push_back(point(0.5, 0));

    poly2.outer().push_back(point(0.3, 0));
    poly2.outer().push_back(point(0.3, 0.1));
    poly2.outer().push_back(point(0.6, 0.1));
    poly2.outer().push_back(point(0.6, 0));
    poly2.outer().push_back(point(0.6, -0.1));
    poly2.outer().push_back(point(1.8, -0.1));
    poly2.outer().push_back(point(1.8, 0.8));
    poly2.outer().push_back(point(0.7, 0.8));
    poly2.outer().push_back(point(0.7, 0.9));
    poly2.outer().push_back(point(2, 0.9));
    poly2.outer().push_back(point(2, -0.5));
    poly2.outer().push_back(point(0.3, -0.5));
    boost::geometry::correct(poly2);

    // std::cout << boost::geometry::wkt(poly) << std::endl;
    // std::cout << boost::geometry::wkt(poly2) << std::endl;
    std::deque<polygon> output;
    boost::geometry::intersection(poly, poly2, output);
    int k = 0;
    //finds intersections
    std::cout << "INTERSECTION\n";
    BOOST_FOREACH (polygon const &p, output)
    {
        std::cout << k++ << ": " << boost::geometry::wkt(p) << std::endl;
        polygon new_intersection_output = p;
        std::vector<point> const &points = new_intersection_output.outer();
        std::vector<double> Px;
        std::vector<double> Py;

        for (std::vector<point>::size_type i = 0; i < points.size(); ++i)
        {
            Px.push_back(boost::geometry::get<0>(points[i]));
            Py.push_back(boost::geometry::get<1>(points[i]));
        }

        std::cout << "Size Px = " << Px.size() << std::endl;
        std::cout << "Size Py = " << Py.size() << std::endl;

        for (int i = 0; i < Px.size(); i++)
        {
            std::cout << "Px[" << i << "] = " << Px[i] << ", Py[" << i << "] = " << Py[i] << std::endl;
        }
    }

    std::vector<polygon> output_union;
    boost::geometry::union_(poly, poly2, output_union);
    k = 0;
    std::cout << "UNION\n";
    BOOST_FOREACH (polygon const &p, output_union)
    {
        std::cout << k++ << ": " << boost::geometry::wkt(p) << std::endl;
        polygon new_union_output = p;
        std::vector<point> const &points = new_union_output.outer();
        std::vector<double> Px;
        std::vector<double> Py;

        for (std::vector<point>::size_type i = 0; i < points.size(); ++i)
        {
            Px.push_back(boost::geometry::get<0>(points[i]));
            Py.push_back(boost::geometry::get<1>(points[i]));
        }

        std::cout << "Size Px = " << Px.size() << std::endl;
        std::cout << "Size Py = " << Py.size() << std::endl;

        for (int i = 0; i < Px.size(); i++)
        {
            std::cout << "Px[" << i << "] = " << Px[i] << ", Py[" << i << "] = " << Py[i] << std::endl;
        }
    }

    return 0;
}
