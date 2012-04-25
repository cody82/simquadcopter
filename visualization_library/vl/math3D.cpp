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

#include "vl/math3D.hpp"
#include "vl/AABB.hpp"
#include "vl/vec2.hpp"
#include "vl/vec3d.hpp"
#include "vl/vec4.hpp"
#include "vl/quat.hpp"
#include "vl/Plane.hpp"
#include "vl/CHECK.hpp"

using namespace vl;

const double vl::PI = acos(-1.0);
const double vl::DEG_TO_RAD = PI / 180.0;
const double vl::RAD_TO_DEG = 180.0 / PI;

bool vl::rayCircleIntersection(const Ray &ray, const vec3d &cir_centre, double cir_radius, double *pt) {
	CHECK(pt);

	vec3d v = ray.Direction;
	v.y() = 0;
	vec3d d = ray.Origin - cir_centre;
	d.y() = 0;
	double dd = dot(d,d);
	double vd = dot(v,d);

	if ( vd > 0)
		return false;

	double rr = cir_radius*cir_radius;

	if ( dd<rr ) {
		*pt = 0;
		return false;
	}

	double a = dot(v,v);
	double b = 2*vd;
	double c = dd - rr;
	double delta = b*b - 4*a*c;

	if (delta<0)
		return false;

	*pt = (double)(( -b - sqrt(delta) ) / (2.0*a));
	if(*pt < 0)
		*pt = 0;

	return true;
}

bool vl::rayPlaneIntersection( const Ray& ray, const Plane& plane, double *pt) {

	double PnRd = dot(plane.normal(), ray.Direction);

	if (fabs(PnRd) <= - 0.001)
		return false;

	*pt = (plane.origin() - dot(plane.normal(), ray.Origin)) / PnRd;

	return *pt > 0;
}

bool vl::rayAABBIntersection( const Ray& ray, const AABB& aabb, double *pt) {

	bool inside = aabb.isInside(ray.Origin);

	Plane plane;

	if (fabs(ray.Direction.x()) > +0.001) {
		if (inside) {
			if (ray.Direction.x() < 0)
				plane = Plane(aabb.minCorner(), vec3d(-1,0,0));
			else
			if (ray.Direction.x() > 0)
				plane = Plane(aabb.maxCorner(), vec3d(+1,0,0));
		} else {
			if (ray.Direction.x() > 0)
				plane = Plane(aabb.minCorner(), vec3d(-1,0,0));
			else
			if (ray.Direction.x() < 0)
				plane = Plane(aabb.maxCorner(), vec3d(+1,0,0));
		}
		if (rayPlaneIntersection(ray,plane,pt)) {
			vec3d p = ray.Origin + ray.Direction * (*pt);
			if (p.y() >= aabb.minCorner().y() && p.y() <= aabb.maxCorner().y() && p.z() >= aabb.minCorner().z() && p.z() <= aabb.maxCorner().z())
				return true;
		}
	}

	if (fabs(ray.Direction.y()) > +0.001) {
		if (inside) {
			if (ray.Direction.y() < 0)
				plane = Plane(aabb.minCorner(), vec3d(0,-1,0));
			else
			if (ray.Direction.y() > 0)
				plane = Plane(aabb.maxCorner(), vec3d(0,+1,0));
		} else {
			if (ray.Direction.y() > 0)
				plane = Plane(aabb.minCorner(), vec3d(0,-1,0));
			else
			if (ray.Direction.y() < 0)
				plane = Plane(aabb.maxCorner(), vec3d(0,+1,0));
		}

		if (rayPlaneIntersection(ray,plane,pt)) {
			vec3d p = ray.Origin + ray.Direction * (*pt);
			if (p.x() >= aabb.minCorner().x() && p.x() <= aabb.maxCorner().x() && p.z() >= aabb.minCorner().z() && p.z() <= aabb.maxCorner().z())
				return true;
		}
	}

	if (fabs(ray.Direction.z()) > +0.001) {
		if(inside) {
			if (ray.Direction.z() < 0)
				plane = Plane(aabb.minCorner(), vec3d(0,0,-1));
			else
			if (ray.Direction.z() > 0)
				plane = Plane(aabb.maxCorner(), vec3d(0,0,+1));
		} else {
			if (ray.Direction.z() > 0)
				plane = Plane(aabb.minCorner(), vec3d(0,0,-1));
			else
			if (ray.Direction.z() < 0)
				plane = Plane(aabb.maxCorner(), vec3d(0,0,+1));
		}

		if (rayPlaneIntersection(ray,plane,pt)) {
			vec3d p = ray.Origin + ray.Direction * (*pt);
			if (p.x() >= aabb.minCorner().x() && p.x() <= aabb.maxCorner().x() && p.y() >= aabb.minCorner().y() && p.y() <= aabb.maxCorner().y())
				return true;
		}
	}

	return false;
}

bool vl::rayCylinderIntersection( const Ray& ray, const Cylinder& cyl, double *pt) {

	if ( dot((ray.Origin - cyl.Origin),(ray.Origin - cyl.Origin)) <= cyl.Radius*cyl.Radius )
		return false;

	if( rayCircleIntersection( ray, cyl.Origin, cyl.Radius, pt ) ) {
		vec3d p = ray.Origin + ray.Direction * (*pt);
		if (p.y() >= cyl.Origin.y() && p.y() <= cyl.Origin.y() + cyl.Height)
			return true;
	} else
		return false;

	if (fabs(ray.Direction.y()) > +0.001) {
		Plane plane;
		if (ray.Direction.y() < 0)
			plane = Plane( cyl.Origin.y() + cyl.Height, vec3d(0,1,0));
		else
		if (ray.Direction.y() > 0)
			plane = Plane( -cyl.Origin.y(), vec3d(0,-1,0));

		if ( rayPlaneIntersection( ray, plane, pt) ) {
			vec3d p = ray.Origin + ray.Direction * (*pt) - cyl.Origin;
			p.y() = 0;
			if ( dot(p,p) < cyl.Radius * cyl.Radius )
				return true;
		}
	}

	return false;
}

bool vl::rayConvexPolygonIntersection( const Ray& ray, const std::vector<vec3d>& poly, double *pt) {

	vec3d v1 = poly[1] - poly[0];
	vec3d v2 = poly[2] - poly[0];
	vec3d n = (cross(v1,v2)).normalize();
	Plane plane(poly[0], n);
	if (rayPlaneIntersection(ray,plane,pt)) {
		vec3d p = ray.Origin + ray.Direction * (*pt);
		for(unsigned i=0; i<poly.size(); i++) {
			int next = (i+1)%poly.size();
			vec3d edgen = (cross(poly[next] - poly[i],n)).normalize();
			if ( dot(p - poly[i],edgen) > 0 )
				return false;
		}
	}
	return true;
}

bool vl::pointInPolygonTest_XZ( const vec3d& pp, std::vector<vec3d> poly ) {

	const double EPSILON = 0.01f;

  vec3d p = pp;
	p.y() = 0;

	for(unsigned i=0; i<poly.size(); i++) {

		if ( (poly[i].z() > p.z()) && (poly[i].z() - p.z() < EPSILON) )
			poly[i].z() = p.z() + EPSILON;
		if ( (poly[i].z() < p.z()) && (p.z() - poly[i].z() < EPSILON) )
			poly[i].z() = p.z() - EPSILON;

		poly[i].y() = 0;
	}

	int cross_count = 0;

	for(unsigned i=0; i<poly.size(); i++) {
		unsigned next = (i+1)%poly.size();

		if ( (poly[i].z() > p.z()) && (poly[next].z() > p.z()) )
			continue;
		if ( (poly[i].z() < p.z()) && (poly[next].z() < p.z()) )
			continue;

		vec3d dir = poly[next] - poly[i];
		dir.normalize();
		double t = dot(p - poly[i], dir);

		vec3d w = poly[i] + dir*t;

		if (w.x() < p.x())
			cross_count++;
	}

	return cross_count & 0x01;
}

bool vl::rayExtrusionIntersection( const Ray& ray, const Extrusion& extrusion, double *pt) {

	bool  hit = false;
	double hit_t = 0;

	for(unsigned i=0; i<extrusion.Silhouette.size(); i++) {
		unsigned next = (i+1) % extrusion.Silhouette.size();

		vec3d A = extrusion.Silhouette[i];
		vec3d B = extrusion.Silhouette[next];
		A.y() = 0;
		B.y() = 0;

		vec3d n = (B - A);
		n.y() = n.z(); // tmp storage
		n.z() = n.x();
		n.x() = -n.y();
		n.y() = 0;
		n.normalize();

		vec3d o(ray.Origin - A);
		o.y() = 0;
		double dis = dot(o,n);
		double dir = dot(-ray.Direction, n);
		if (fabs(dir) < 0.001f)
			continue;
		double t = dis / dir;
		if (t < 0.001)
			continue;

		vec3d p = ray.Origin + ray.Direction * t;

		if (p.y() < extrusion.Bottom)
			continue;
		if (p.y() > extrusion.Bottom + extrusion.Height)
			continue;

		p.y() = 0;

		if ( dot(p - A, B - A) < 0.001f )
			continue;
		if ( dot(p - B, A - B) < 0.001f )
			continue;
		if (!hit || (hit && t<hit_t)) {
			hit_t = t;
			hit   = true;
		}
	}

	if (fabs(ray.Direction.y()) > +0.001) {
		double t = 0;
		Plane plane;

		plane = Plane( extrusion.Bottom + extrusion.Height, vec3d(0,1,0));
		if ( rayPlaneIntersection(ray, plane, &t) ) {

			vec3d p = ray.Origin + ray.Direction * t;
			if ( pointInPolygonTest_XZ(p, extrusion.Silhouette) ) {
				if (!hit || (hit && t<hit_t)) {
					hit_t = t;
					hit   = true;
				}
			}
		}

		plane = Plane( -extrusion.Bottom, vec3d(0,-1,0));
		if ( rayPlaneIntersection(ray, plane, &t) ) {

			vec3d p = ray.Origin + ray.Direction * t;
			if ( pointInPolygonTest_XZ(p, extrusion.Silhouette) ) {
				if (!hit || (hit && t<hit_t)) {
					hit_t = t;
					hit   = true;
				}
			}
		}
	}

	if (hit)
		*pt = hit_t;

	return hit;
}

double vl::random01()
{
	return rand() / (double)RAND_MAX;
}

double vl::randomMinMax(double min, double max)
{
	return min + (max-min)*random01();
}

int vl::greaterEqualPow2(int n)
{
	int pow2=2;
	for(int i=0; i<20; i++) {
		if (pow2 >= n)
			return pow2;
		pow2 = pow2 * 2;
	}
	return pow2;
}

int vl::smallerEqualPow2(int n)
{
	int pow2=2;
	for(int i=0; i<20; i++) {
		if (pow2 > n)
			return pow2/2;
		pow2 = pow2 * 2;
	}
	return pow2;
}

void vl::transform( const mat4d& m, std::vector<vec3d> &vec)
{
	for(unsigned int i=0; i<vec.size(); i++) {
		vec[i] = m * vec[i];
	}
}

