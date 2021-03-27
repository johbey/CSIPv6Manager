
#ifndef CSV6_CONFIG_HPP
#define CSV6_CONFIG_HPP

#include <cstdint>
#include <QAbstractSocket>

#ifndef NDEBUG
#define LOG_RECEIVED_DGRAMS
#endif

namespace
{

const int MaxConnectionCount = 64;
const int MaxBindRetryCount = 4;
const QAbstractSocket::BindMode BindMode = QAbstractSocket::DontShareAddress;

} // namespace

#endif // CSV6_CONFIG_HPP
