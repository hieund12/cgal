#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Arrangement_2.h>

#include <boost/function_output_iterator.hpp>

#include <array>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                Segment_traits_2;
typedef CGAL::Arr_polyline_traits_2<Segment_traits_2>     Geom_traits_2;

typedef Geom_traits_2::Point_2                            Point_2;
typedef Geom_traits_2::Segment_2                          Segment_2;
typedef Geom_traits_2::Curve_2                            Polyline_2;
typedef CGAL::Arrangement_2<Geom_traits_2>                Arrangement_2;
typedef Geom_traits_2::X_monotone_curve_2                 X_monotone_polyline;
typedef Geom_traits_2::X_monotone_subcurve_2              X_monotone_subcurve;

void test (const X_monotone_polyline& a, const X_monotone_polyline& b)
{
  Geom_traits_2 traits;
  Geom_traits_2::Intersect_2 intersect_2 =
    traits.intersect_2_object();

  std::cerr << " * Polyline A = " << a << std::endl
            << " * Polyline B = " << b << std::endl;

  intersect_2
    (a, b,
     boost::make_function_output_iterator
     ([&](const CGAL::Object& obj)
      {
        const X_monotone_polyline* poly
          = CGAL::object_cast<X_monotone_polyline>(&obj);
        CGAL_assertion_msg (poly != nullptr, "Intersection is not a polyline");
        std::cerr << " * Intersection = " << *poly << std::endl;
      }));
}

int main(int argc, char* argv[])
{
  Geom_traits_2 traits;
  Arrangement_2 arr(&traits);

  Geom_traits_2::Construct_x_monotone_curve_2 x_mono_polyline_construct =
    traits.construct_x_monotone_curve_2_object();

  auto ctr_x_curve = traits.construct_x_monotone_curve_2_object();

  std::array<Segment_2, 2> r2l
    = { Segment_2(Point_2(1, 0), Point_2(0, 1)),
        Segment_2(Point_2(0, 1), Point_2(-1, 0)) };

  std::array<Segment_2, 2> l2r
    = { Segment_2(Point_2(-1, 0), Point_2(0, 1)),
        Segment_2(Point_2(0, 1), Point_2(1, 0)), };

  X_monotone_polyline p0l2r
    = x_mono_polyline_construct (l2r.begin(), l2r.end());
  X_monotone_polyline p1l2r
    = x_mono_polyline_construct (l2r.begin(), l2r.end());
  X_monotone_polyline p0r2l
    = x_mono_polyline_construct (r2l.begin(), r2l.end());
  X_monotone_polyline p1r2l
    = x_mono_polyline_construct (r2l.begin(), r2l.end());


  std::cerr << "Testing intersection left-to-right / left-to-right" << std::endl;
  test (p0l2r, p1l2r);
  std::cerr << "Testing intersection left-to-right / right-to-left" << std::endl;
  test (p0l2r, p1r2l);
  std::cerr << "Testing intersection right-to-left / left-to-right" << std::endl;
  test (p0r2l, p1l2r);
  std::cerr << "Testing intersection right-to-left / right-to-left" << std::endl;
  test (p0r2l, p1r2l);

  return EXIT_FAILURE;
}
