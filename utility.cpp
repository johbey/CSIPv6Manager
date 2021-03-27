
#include "utility.hpp"

// using \a QHostAddress::SpecialAddress for localhost seems flawed for IPv4 only (?)
// btw. this also saves some heap allocations for \a QHostAddress
const QHostAddress LocalHostIPv4{ "127.0.0.1" };
const QHostAddress LocalHostIPv6{ "::1" };
const QHostAddress AnyIPv6{ QHostAddress::AnyIPv6 };
