#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "cxy.h"
#include <wex.h>
#include "cStarterGUI.h"

typedef std::vector<cxy> poly_t;

std::vector<poly_t> thePolygons;

/// @brief Find contiguous polygons
/// @return vector of contiguous polygon pairs

std::vector<std::pair<int, int>> areContig()
{
    std::vector<std::pair<int, int>> ret;

    // loop over every pair of polygons
    for (int ip1 = 0; ip1 < thePolygons.size(); ip1++)
        for (int ip2 = ip1 + 1; ip2 < thePolygons.size(); ip2++)
        {
            auto &p1 = thePolygons[ip1];
            auto &p2 = thePolygons[ip2];

            // loop over every pair of edges
            bool f = false;
            for (int iv1 = 0; iv1 < p1.size() - 1; iv1++)
                for (int iv2 = 0; iv2 < p2.size() - 1; iv2++)
                {
                    // edge end points
                    cxy l1e1 = p1[iv1];
                    cxy l1e2 = p1[iv1 + 1];
                    cxy l2e1 = p2[iv2];
                    cxy l2e2 = p2[iv2 + 1];

                    // normalize edge vectors
                    double d1 = sqrt(l1e1.dist2(l1e2));
                    double d2 = sqrt(l2e1.dist2(l2e2));
                    cxy v1 = l1e1.vect(l1e2);
                    cxy v2 = l2e1.vect(l2e2);
                    cxy nv1(v1.x / d1, v1.y / d1);
                    cxy nv2(v2.x / d2, v2.y / d2);
                    if (nv1 == nv2)
                    {
                        f = true;
                    }
                    else
                    {
                        nv1.x *= -1;
                        nv1.y *= -1;
                        if (nv1 == nv2)
                            f = true;
                    }
                    if (f)
                    {
                        // normalised edge vectors are identical
                        if (
                            l1e1.dis2toline(l2e1, l2e2) < 0.1 ||
                            l1e2.dis2toline(l2e1, l2e2) < 0.1 ||
                            l2e1.dis2toline(l1e1, l1e2) < 0.1 ||
                            l2e1.dis2toline(l1e1, l1e2) < 0.1 )
                        {
                            // edges are continguous
                            f = true;
                            ret.push_back(std::make_pair(ip1, ip2));
                            break;
                        }
                        else
                            f = false;
                    }
                }
            if (f)
                break;
        }
    return ret;
}
bool areContigTest()
{
    thePolygons.clear();
    poly_t p = {cxy(0, 0), cxy(10, 0), cxy(10, 10), cxy(0, 10), cxy(0, 0)};
    thePolygons.push_back(p);
    p = {cxy(3, 10), cxy(7, 10), cxy(5, 20), cxy(3, 10)};
    thePolygons.push_back(p);
    auto cp = areContig();
    if( cp.size() != 1 )
        return false;
    if(cp[0].first != 0 || cp[0].second != 1)
        return false;
    thePolygons.clear();
    p = {cxy(0, 0), cxy(10, 0), cxy(10, 10), cxy(0, 10), cxy(0, 0)};
    thePolygons.push_back(p);
    p = {cxy(3, 12), cxy(7, 12), cxy(5, 20), cxy(3, 12)};
    thePolygons.push_back(p);
    cp = areContig();
    if( cp.size() != 0 )
        return false;
    return true;
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

main()
{
    areContigTest();

    cGUI theGUI;
    return 0;
}
