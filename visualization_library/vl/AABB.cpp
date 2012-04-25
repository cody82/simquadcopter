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

#include "vl/AABB.hpp"

using namespace vl;

AABB::AABB() 
{
  setEmpty();
}

AABB::AABB( const vec3d& center, double radius ) 
{
	mMax = center + radius;
	mMin = center - radius;
}

AABB::AABB( const vec3d& pt1, const vec3d& pt2, double displace) 
{
	mMax = mMin = pt1;
	if ( mMax.x() < pt2.x() ) mMax.x() = pt2.x();
	if ( mMax.y() < pt2.y() ) mMax.y() = pt2.y();
	if ( mMax.z() < pt2.z() ) mMax.z() = pt2.z();
	if ( mMin.x() > pt2.x() ) mMin.x() = pt2.x();
	if ( mMin.y() > pt2.y() ) mMin.y() = pt2.y();
	if ( mMin.z() > pt2.z() ) mMin.z() = pt2.z();

	mMax = mMax + displace;
	mMin = mMin - displace;
}

void AABB::enlarge(double displace) {
  if ( isEmpty() )
    return;

	mMax = mMax + displace;
	mMin = mMin - displace;
}

bool AABB::intersects(const AABB& bb) const
{
  if (isEmpty() || bb.isEmpty())
    return false;

	if (mMax.x() < bb.mMin.x())
		return false;

	if (mMax.z() < bb.mMin.z())
		return false;

	if (mMin.x() > bb.mMax.x())
		return false;

	if (mMin.z() > bb.mMax.z())
		return false;

	return true;
}

vec3d AABB::clip(const vec3d& v, bool clipx, bool clipy, bool clipz) const 
{
  if (isEmpty())
    return v;

  vec3d tmp = v;

	if (clipx) {
	    if (v.x() > mMax.x())
	    	tmp.x() = mMax.x();
	    if (v.x() < mMin.x())
	    	tmp.x() = mMin.x();
	}

	if (clipy) {
	    if (v.y() > mMax.y())
	    	tmp.y() = mMax.y();
	    if (v.y() < mMin.y())
	    	tmp.y() = mMin.y();
    }

	if (clipz) {
	    if (v.z() > mMax.z())
	    	tmp.z() = mMax.z();
	    if (v.z() < mMin.z())
	    	tmp.z() = mMin.z();
    }
    return v;
}

bool AABB::isInside(const vec3d& v, bool clipx, bool clipy, bool clipz) const 
{
	vec3d t = v;
	return v == clip(t, clipx, clipy, clipz);
}

void AABB::addPoint(const vec3d& v) 
{
  if (isEmpty())
  {
    mMax = v;
    mMin = v;
  }

  if ( mMax.x() < v.x() ) mMax.x() = v.x() ;
  if ( mMax.y() < v.y() ) mMax.y() = v.y() ;
  if ( mMax.z() < v.z() ) mMax.z() = v.z() ;
  if ( mMin.x() > v.x() ) mMin.x() = v.x() ;
  if ( mMin.y() > v.y() ) mMin.y() = v.y() ;
  if ( mMin.z() > v.z() ) mMin.z() = v.z() ;
}

void AABB::addPoint(const vec3d& v, double radius) 
{
  if (isEmpty())
  {
    mMax = v;
    mMin = v;
  }

  if ( mMax.x() < v.x() + radius) mMax.x() = v.x() + radius;
  if ( mMax.y() < v.y() + radius) mMax.y() = v.y() + radius;
  if ( mMax.z() < v.z() + radius) mMax.z() = v.z() + radius;
  if ( mMin.x() > v.x() - radius) mMin.x() = v.x() - radius;
  if ( mMin.y() > v.y() - radius) mMin.y() = v.y() - radius;
  if ( mMin.z() > v.z() - radius) mMin.z() = v.z() - radius;
}

double AABB::width() const {
  if (isEmpty())
    return 0;
  else
	  return mMax.x() - mMin.x();
}

double AABB::height() const {
  if (isEmpty())
    return 0;
  else
  	return mMax.y() - mMin.y();
}

double AABB::depth() const {
  if (isEmpty())
    return 0;
  else
  	return mMax.z() - mMin.z();
}

AABB AABB::operator+(const AABB& aabb) const 
{
  if(isEmpty())
    return aabb;
  if (aabb.isEmpty())
    return *this;
  AABB tmp = aabb;
	tmp.addPoint( mMin );
	tmp.addPoint( mMax );
	return tmp;
}

bool AABB::isEmpty() const {
	return mMin.x() > mMax.x() || mMin.y() > mMax.y() || mMin.z() > mMax.z();
}

vec3d AABB::center() const
{
  vec3d c = (minCorner() + maxCorner()) / 2.0f;
  return c;
}

