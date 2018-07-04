#ifndef SUBNET_H
#define SUBNET_H

#include <math.h>
#include <bitset>
#include <sstream>
#include <string>

namespace subnet {
  struct ValueNetwork {
    std::uint32_t NetId;
    std::uint32_t FirstAddr;
    std::uint32_t LastAddr;
    std::uint32_t BroadcastAddr;
    std::uint32_t Mask;
    int ShortMask;
    size_t MaxHosts;
  };

  std::uint32_t calcMask (size_t hosts) {
    std::uint32_t a = std::pow(2, std::ceil(std::log2(hosts + 2)));
    std::uint32_t b = UINT32_MAX;

    return ~(b & (a - 1));
  }

  ValueNetwork calculateVal(const std::uint32_t& ip, size_t hosts) {
    ValueNetwork net;
    auto mask = calcMask(hosts);
    auto addr = ip & mask;
    auto hostBits = std::ceil(std::log2(hosts + 2));

    net.NetId = addr;
    net.FirstAddr = addr + 1;
    net.BroadcastAddr = ((ip & mask) +
                         static_cast<uint32_t>(std::pow(2, hostBits))) - 1;
    net.LastAddr = net.BroadcastAddr - 1;
    net.Mask = mask;
    net.MaxHosts = std::pow(2, hostBits) - 2;
    net.ShortMask = 32 - hostBits;

    return net;
  }

  std::string toIP(std::uint32_t addr) {
    std::string str;
    str += std::to_string(addr >> 24) + ".";
    str += std::to_string(((addr << 8) >> 8) >> 16) + ".";
    str += std::to_string(((addr << 16) >> 16) >> 8) + ".";
    str += std::to_string((addr << 24) >> 24);
    return str;
  }

  std::uint32_t toValue(const std::string& ip) {
    std::uint32_t ret = UINT32_MAX;
    std::string ipstr = ip;

    std::string token;
    std::istringstream iss(ipstr);
    auto pot = 24;
    while(getline(iss, token, '.')) {
      ret = ret == UINT32_MAX ? 0 : ret;
      std::uint32_t val = std::stoi(token);
      val = val << pot;
      ret |= val;
      pot -= 8;
    }

    return ret;
  }

};

#endif
