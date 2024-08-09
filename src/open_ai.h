#ifndef OPEN_AI_H
#define OPEN_AI_H

#include <iostream>
#include <sstream>
#include <string>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace std;
using namespace boost::property_tree;

string request();

#endif