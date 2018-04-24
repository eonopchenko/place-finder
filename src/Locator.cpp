/*
 * Locator.cpp
 *
 *  Created on: 18/03/2018
 *      Author: jake
 */

#include <cmath>
#include "Locator.hpp"

const double PI = 3.141592653589793;

inline double deg2rad(double deg)
{
	return deg * ((2 * PI) / 360);
}

inline double rad2deg(double rad)
{
	return rad * (360 / (2 * PI));
}

double Locator::getSphericalDistance(double lat1, double lng1, double lat2, double lng2)
{
  /// Ratio between equator radius and point radius
  /// (this is needed to consider flattening)
  double rad2len = cos(deg2rad(lat1));

  /// Linear shifts (chord length) between longitudes and latitudes
  double linLng = ((lng2 - lng1) * rad2len) * (EQUATOR_LENGTH / 360);
  double linLat = (lat2 - lat1) * (MEREDIAN_LENGTH / 360);

  /// Result distance (hypotenuse)
  return sqrt(linLng * linLng + linLat * linLat);
}

double Locator::getCompassAngle(double lat1, double lng1, double lat2, double lng2)
{
    double dLambda = deg2rad(lng2 - lng1);
    double y = sin(dLambda) * cos(deg2rad(lat2));
    double x = cos(deg2rad(lat1)) * sin(deg2rad(lat2)) - sin(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(dLambda);
    double theta = atan2(y, x);
    double bearing = remainder((rad2deg(theta) + 360), 360);
    return bearing;
}
