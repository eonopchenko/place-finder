/*
 * Locator.hpp
 *
 *  Created on: 18/03/2018
 *      Author: jake
 */

#ifndef LOCATOR_HPP_
#define LOCATOR_HPP_

#include <iostream>

constexpr int MEREDIAN_LENGTH = 40008550;
constexpr int EQUATOR_LENGTH = 40075696;
constexpr int EARTH_RADIUS = 6371000;
const std::string cardinals[] = { "N","NE","E","SE","S","SW","W","NW" };

class Locator
{
public:
	static double getSphericalDistance(double lat1, double lng1, double lat2, double lng2);
	static double getCompassAngle(double lat1, double lng1, double lat2, double lng2);
};

#endif /* LOCATOR_HPP_ */
