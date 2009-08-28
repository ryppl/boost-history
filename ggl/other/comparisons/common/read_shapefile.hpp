// Generic Geometry Library - COMPARISONS
//
// Copyright Barend Gehrels, 2008, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef COMPARE_READ_SHAPEFILE_HPP_INCLUDED
#define COMPARE_READ_SHAPEFILE_HPP_INCLUDED



// If this does NOT compile:
// -> download shapelib from http://shapelib.maptools.org/
#include "shapefil.h"

#include <vector>
#include <string>

template <typename T, typename F>
void read_shapefile(std::string const& filename, std::string const & idfieldname,
                    std::vector<T>& polygons, std::vector<int>& ids,
                    F functor)
{
    try
    {
        std::string shape_filename = filename + ".shp";
        std::string dbase_filename = filename + ".dbf";

        SHPHandle handle = SHPOpen(shape_filename.c_str(), "rb");
        if (handle <= 0)
        {
            throw std::string("File " + shape_filename + " not found");
        }
        DBFHandle dbf = DBFOpen(dbase_filename.c_str(), "rb+");
        if (dbf <= 0)
        {
            throw std::string("File " + dbase_filename + " not found");
        }

        int nShapeType, nEntities;
        double adfMinBound[4], adfMaxBound[4];
        SHPGetInfo(handle, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

        int nr_index = DBFGetFieldIndex(dbf, idfieldname.c_str());

        for (int i = 0; i < nEntities; i++)
        {
            SHPObject* psShape = SHPReadObject(handle, i );

            // Process only polygons, and from them only single-polygons without holes
            if (psShape->nSHPType == SHPT_POLYGON && psShape->nParts == 1)
            {
                polygons.push_back(functor(psShape));
                ids.push_back(DBFReadIntegerAttribute(dbf, i, nr_index));
            }
            SHPDestroyObject( psShape );
        }
        SHPClose(handle);
        DBFClose(dbf);
    }
    catch(std::exception const& e)
    {
        throw e;
    }
    catch(const std::string& s)
    {
        throw s;
    }
    catch(...)
    {
        throw std::string("Unknown exception ('...')");
    }
}




#endif // COMPARE_READ_SHAPEFILE_HPP_INCLUDED

