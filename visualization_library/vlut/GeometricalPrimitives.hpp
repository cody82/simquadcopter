/*
  Copyright (C) 2008 Michele Bosi

  This file is part of Visualization Library.

  Visualization Library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Visualization Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Visualization Library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GeometricalPrimitives_INCLUDE_DEFINE
#define GeometricalPrimitives_INCLUDE_DEFINE

#include "vl/vec4.hpp"
#include "vl/vec3d.hpp"
#include "vl/Geometry.hpp"
#include "vlut/Colors.hpp"

namespace vlut
{
  vl::ref<vl::Geometry> makeIcosphere( const vl::vec3d& pos, double radius=1, int detail=2 );
  vl::ref<vl::Geometry> makeCylinder( const vl::vec3d& origin, double diameter=1, double height=1, int phi=20, int theta=2, bool top=true, bool bottom=true );
  vl::ref<vl::Geometry> makeUVSphere( const vl::vec3d& origin, double diameter=1, int phi=20, int theta=20 );
  vl::ref<vl::Geometry> makeTorus( const vl::vec3d& origin, double diameter=1, double thickness=0.2, int phi=10, int theta=10 );
  vl::ref<vl::Geometry> makeCone( const vl::vec3d& origin, double diameter=1, double height=1, int phi=10, bool bottom=true );
  vl::ref<vl::Geometry> makePyramid( const vl::vec3d& origin, double side=1, double height=1, bool bottom=true );
  vl::ref<vl::Geometry> makeBox( const vl::vec3d& origin, double xside=1, double yside=1, double zside=1, bool tex_coords=true );
  vl::ref<vl::Geometry> makeGrid( const vl::vec3d& origin, double xside, double zside, int x, int z, bool gen_tex_coord );
  vl::ref<vl::Geometry> makeIcosahedron( const vl::vec3d& origin, double diameter );
  vl::ref<vl::Geometry> makePoints( const std::vector< vl::vec3d >& pos, const vl::vec4& color = vlut::white);
  vl::ref<vl::Geometry> makeCircle( vl::vec3d origin, double radius, int slices = 60 );
}

#endif
