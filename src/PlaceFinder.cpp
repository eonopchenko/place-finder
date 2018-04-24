//============================================================================
// Name        : PlaceFinder.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "json/json.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <chrono>

#include <stdio.h>
#include <string.h>

#include "Locator.hpp"
#include "restClient.h"
#include "tools.h"

const char GoogleAPIUrl[] = {"https://maps.googleapis.com/maps/api/geocode/json?latlng=%s,%s&key=AIzaSyDx9QurkrHG1Y1e0YdRG3Tpym1GyiA7NUg"};

int main()
{
#ifdef __DEBUG__
  std::cout << "Debug version:" << std::endl;
#endif
  std::cout << "c++ v." << __cplusplus << std::endl;

#ifdef __DEBUG__
  auto t0 = std::chrono::high_resolution_clock::now();
#endif

  std::string s;
  std::ifstream ifs;
  std::ofstream ofs;

  ifs.open("test.csv", std::ifstream::in);
  ofs.open("populated_places.csv", std::ofstream::out);

  char url[MAX_URL_LENGTH];
  char response[MAX_RESPONSE_LENGTH];

  /// Init REST client
  restClient_Init();

  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();

  Json::Value root;
  std::string errors;

  /// Iterate through the CSV lines
  while (ifs.good())
  {
	/// Extract Latitude and Longitude from input line
	std::getline(ifs, s);
	boost::erase_all(s, "\r");
	if (s.length() < 6)
	{
		continue;
	}
	std::string lat1str = s.substr(0, s.find(", "));
	std::string lng1str = s.substr(lat1str.length() + 2, s.find("\n"));

	/// Request Google API reverse geocoding
	static_assert(std::strlen(GoogleAPIUrl) < MAX_URL_LENGTH, "API request string buffer overflow!");
	std::sprintf(url, GoogleAPIUrl, lat1str.c_str(), lng1str.c_str());

#ifdef __DEBUG__
	/// Start high resolution clock
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto t3 = std::chrono::high_resolution_clock::now();
#endif

	if (restClient_Get(url, response) != -1)
	{
#ifdef __DEBUG__
	  t2 = std::chrono::high_resolution_clock::now();
#endif

	  /// Trim all line endings (O(n)) (required for parser) and parse
	  if (!reader->parse(response, response + Trim(response, 1, '\n'), &root, &errors))
	  {
	    std::cout << errors << std::endl;
	  }
	  else
	  {
	    /// Pick a required data from the JSON
	    auto it = root.begin();

	    /// Pick place name and coordinates (O(1))
	    std::string name = (*(*it).begin())["formatted_address"].asString();
	    double lat2 = (*(*it).begin())["geometry"]["location"]["lat"].asDouble();
	    double lng2 = (*(*it).begin())["geometry"]["location"]["lng"].asDouble();

	    /// Convert initial coordinates to double
	    std::stringstream lat1ss(lat1str);
	    std::stringstream lng1ss(lng1str);
	    double lat1 = lat1ss >> lat1 ? lat1 : 0;
	    double lng1 = lng1ss >> lng1 ? lng1 : 0;

	    /// Get distance and compass direction
	    double distance = Locator::getSphericalDistance(lat1, lng1, lat2, lng2);
	    double angle = Locator::getCompassAngle(lat1, lng1, lat2, lng2);
	    angle = angle < 0 ? 360 + angle : angle;
	    int sector = ((angle / 360) * 8);

	    /// O(n)
	    boost::erase_all(name, ",");

	    ofs << name << "," << distance << "," << cardinals[sector] << std::endl;
	    std::cout << name << "," << distance << "," << cardinals[sector];

#ifdef __DEBUG__
	    /// Align columns for debug output
	    std::ostringstream dststr;
	    dststr << distance;
	    int len = name.length() + dststr.str().length() + cardinals[sector].length();
	    for (int i = 0; i < (80 - len); i++)
	    {
	    	std::cout << " ";
	    }
#endif
	  }

#ifdef __DEBUG__
	  t3 = std::chrono::high_resolution_clock::now();
#endif
	}

	/// Reset the input buffer
	restClient_Reset();

#ifdef __DEBUG__
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
    std::cout << "(exec time: " << duration1 << "us, " << duration2 << "us)";
#endif
    std::cout << std::endl;
  }

  delete reader;

  /// Dispose REST client handle
  restClient_Cleanup();

  ifs.close();
  ofs.close();

#ifdef __DEBUG__
  auto t4 = std::chrono::high_resolution_clock::now();
  auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>( t4 - t0 ).count();
  std::cout << "---" << std::endl << "Total execution time: " << duration3 / 1E6f << "s" << std::endl;
#endif

  return 0;
}
