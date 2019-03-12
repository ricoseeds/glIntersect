// #include <iostream>
// #include <boost/geometry.hpp>
// #include <boost/geometry/core/point_type.hpp>
// #include <boost/geometry/geometries/point.hpp>
// #include <boost/geometry/geometries/register/point.hpp>
// #include <boost/geometry/geometries/register/linestring.hpp>

// namespace bg = boost::geometry;

// struct MyPoint
// {
//     double x, y;
// };

// typedef std::vector<MyPoint> MyPolygon;

// BOOST_GEOMETRY_REGISTER_POINT_2D(MyPoint, double, bg::cs::cartesian, x, y)
// BOOST_GEOMETRY_REGISTER_LINESTRING(MyPolygon)

// template <typename Polygon, typename Point>
// std::deque<Polygon> test_polygon()
// {

//     Polygon poly1{
//         {0.0, 0.0},
//         {0.0, 1.0},
//         {1.0, 1.0},
//         {1.0, 0.0},
//         {0.050, 0.0},
//     };
//     Polygon poly2{
//         {0.5, -0.5},
//         {0.5, 0.5},
//         {1.5, 0.5},
//         {1.5, -0.5},
//         {0.5, -0.5},
//     };

//     std::deque<Polygon> output;
//     boost::geometry::intersection(poly1, poly2, output);

//     return output;
// }

// int main()
// {
//     for (auto &p : test_polygon<MyPolygon, MyPoint>())
//         std::cout << "Intersection: " << bg::wkt(p) << "\n";
// }
#include <iostream>
#include <deque>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/foreach.hpp>

int main()
{
    typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

    polygon green, blue;

    boost::geometry::read_wkt(
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
        green);
    //sanity check
    std::string reason;
    bool valid = boost::geometry::is_valid(green, reason);
    // std::cout << boost::geometry::dsv(green) << " - " << (!valid ? "invalid (" + reason + ")" : "valid") << "\n";

    boost::geometry::correct(green);

    boost::geometry::read_wkt(
        "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);
    boost::geometry::correct(blue);

    std::deque<polygon> output;
    boost::geometry::intersection(green, blue, output);

    int i = 0;
    std::cout << "green && blue:" << std::endl;
    BOOST_FOREACH (polygon const &p, output)
    {
        // std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
        std::cout << i++ << ": " << boost::geometry::wkt(p) << std::endl;
    }

    return 0;
}

// #include <boost/geometry.hpp>
// #include <boost/geometry/geometries/point_xy.hpp>
// #include <boost/geometry/geometries/linestring.hpp>
// #include <boost/geometry/geometries/polygon.hpp>

// #include <iostream>
// #include <boost/geometry/io/io.hpp>

// namespace bg = boost::geometry;
// namespace bgm = bg::model;

// using point = bgm::d2::point_xy<int>;
// using polygon = bgm::polygon<point>;

// void debug_print(polygon const &p)
// {
//     std::string reason;
//     bool valid = bg::is_valid(p, reason);
//     std::cout << bg::dsv(p) << " - " << (!valid ? "invalid (" + reason + ")" : "valid") << "\n";
// }

// int main()
// {

//     point arr[]{{{1, 3}, {4, 5}, {9, 0}}};
//     for (auto &p : arr)
//         std::cout << bg::wkt(p) << " ";
//     std::cout << "\n";

//     polygon p{{std::begin(arr), std::end(arr)}};
//     debug_print(p);

//     bg::correct(p);
//     debug_print(p);

//     std::cout << bg::wkt(p) << "\n";
// }
// #include <iostream>
// #include <boost/fusion/adapted/boost_tuple.hpp>
// #include <boost/spirit/include/karma.hpp>
// #include <deque>

// #include <boost/geometry.hpp>
// #include <boost/geometry/geometries/point_xy.hpp>
// #include <boost/geometry/geometries/polygon.hpp>

// #include <boost/foreach.hpp>

// typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

// int main()
// {
//     using Ring = std::vector<boost::tuple<double, double>>;

//     std::vector<Ring> rings = {
//         {
//             {2, 1.3},
//             {2.4, 1.7},
//             {2.8, 1.8},
//             {3.4, 1.2},
//             {3.7, 1.6},
//             {3.4, 2},
//             {4.1, 3},
//             {5.3, 2.6},
//             {5.4, 1.2},
//             {4.9, 0.8},
//             {2.9, 0.7},
//             {2, 1.3},
//         },
//         {
//             {4.0, 2.0},
//             {4.2, 1.4},
//             {4.8, 1.9},
//             {4.4, 2.2},
//             {4.0, 2.0},
//         }};
//     polygon poly;

//     boost::geometry::read_wkt(format_delimited("POLYGON(" << *('(' << auto_ % ',' << ')') << ")\n", ' ', rings),
//       poly);
//     // boost::geometry::read_wkt(format_delimited("POLYGON(" << *('(' << auto_ % ',' << ')') << ")\n", ' ', rings), poly);

//         using namespace boost::spirit::karma;
//     // std::string poly = <dynamic_cast>(std::string)(format_delimited("POLYGON(" << *('(' << auto_ % ',' << ')') << ")\n", ' ', rings));
//     // std::cout << poly;
// }
// // #include <iostream>

// // #include <boost/geometry.hpp>
// // #include <boost/geometry/geometries/point_xy.hpp>
// // #include <boost/geometry/geometries/polygon.hpp>
// // #include <boost/geometry/geometries/linestring.hpp>
// // #include <boost/geometry/multi/geometries/multi_linestring.hpp>

// // using point_type = boost::geometry::model::d2::point_xy<double>;
// // using polygon_type = boost::geometry::model::polygon<point_type>;
// // using linestring_type = boost::geometry::model::linestring<point_type>;
// // using multi_linestring_type = boost::geometry::model::multi_linestring<linestring_type>;

// // int main()
// // {
// //     polygon_type polygon;

// //     polygon.outer().push_back(point_type{10, 10});
// //     polygon.outer().push_back(point_type{10, 12});
// //     polygon.outer().push_back(point_type{12, 12});
// //     polygon.outer().push_back(point_type{12, 10});
// //     polygon.outer().push_back(point_type{10, 10});
// //     // linestring_type polygon;

// //     // polygon.push_back(point_type{10, 10});
// //     // polygon.push_back(point_type{10, 12});
// //     // polygon.push_back(point_type{12, 12});
// //     // polygon.push_back(point_type{12, 10});
// //     // polygon.push_back(point_type{10, 10});

// //     linestring_type linestring;
// //     linestring.push_back(point_type{11, 9});
// //     linestring.push_back(point_type{11, 11});
// //     linestring.push_back(point_type{13, 11});

// //     // Expected intersections at (11, 10) and (12, 11)

// //     multi_linestring_type intersection;
// //    boost::geometry::intersection(polygon, linestring, intersection);

// for (auto intersectionIter = intersection.begin(); intersectionIter != intersection.end(); ++intersectionIter)
// {
//     linestring_type intersectionPiece = *intersectionIter;
//     std::cout << "Piece:" << std::endl;
//     for (auto intersectionPieceIter = intersectionPiece.begin(); intersectionPieceIter != intersectionPiece.end(); ++intersectionPieceIter)
//     {
//         std::cout << boost::geometry::get<0>(*intersectionPieceIter) << " " << boost::geometry::get<1>(*intersectionPieceIter) << std::endl;
//     }
// }

// //     return 0;
// // }
// // // // #include <iostream>
// // // // #include <deque>

// // // // #include <boost/geometry.hpp>
// // // // #include <boost/geometry/geometries/point_xy.hpp>
// // // // #include <boost/geometry/geometries/polygon.hpp>

// // // // #include <boost/foreach.hpp>

// // // // int main()
// // // // {
// // // //     typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

// // // //     polygon green, blue;

// // // //     boost::geometry::read_wkt(
// // // //         "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
// // // //         "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
// // // //         green);

// // // //     boost::geometry::read_wkt(
// // // //         "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);

// // // //     std::deque<polygon> output;
// // // //     boost::geometry::intersection(green, blue, output);

// // // //     int i = 0;
// // // //     std::cout << "green && blue:" << std::endl;
// // // //     BOOST_FOREACH (polygon const &p, output)
// // // //     {
// // // //         std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
// // // //     }

// // // //     return 0;
// // // // }

// // // #include <iostream>
// // // #include <vector>

// // // #include <boost/geometry.hpp>
// // // #include <boost/geometry/geometries/point_xy.hpp>
// // // #include <boost/geometry/geometries/polygon.hpp>
// // // #include <boost/geometry/io/wkt/wkt.hpp>

// // // #include <boost/foreach.hpp>
// // // #include <string>

// // // #include <boost/geometry.hpp>
// // // namespace bg = boost::geometry;
// // // typedef bg::model::d2::point_xy<double> boost_point;
// // // typedef bg::model::polygon<boost_point> boost_polygon;

// // // //getting the vertices back
// // // int main()
// // // {
// // //     boost_polygon poly;
// // //     bg::append(poly.outer(), boost_point(-1, -1));
// // //     bg::append(poly.outer(), boost_point(-1, 1));
// // //     bg::append(poly.outer(), boost_point(1, 1));
// // //     bg::append(poly.outer(), boost_point(1, -1));
// // //     bg::append(poly.outer(), boost_point(-1, -1));
// // //     for (auto it = boost::begin(boost::geometry::exterior_ring(poly)); it != boost::end(boost::geometry::exterior_ring(poly)); ++it)
// // //     {
// // //         double x = bg::get<0>(*it);
// // //         double y = bg::get<1>(*it);
// // //         //use the coordinates...

// // //     }
// // // }

// // // // int main()
// // // // {
// // // //     typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

// // // //     polygon green, blue;

// // // //     boost::geometry::read_wkt(
// // // //         "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
// // // //         "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
// // // //         green);

// // // //     boost::geometry::read_wkt(
// // // //         "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);

// // // //     std::vector<polygon> output;
// // // //     boost::geometry::union_(green, blue, output);

// // // //     int i = 0;
// // // //     std::cout << "green || blue:" << std::endl;
// // // //     BOOST_FOREACH (polygon const &p, output)
// // // //     {
// // // //         // std::string poly_string = <static_cast>(std::string)(boost::geometry::wkt(p));
// // // //         std::cout << i++ << ": " << std::to_string(boost::geometry::wkt(p)) << std::endl;
// // // //     }

// // // //     return 0;
// // // // }