/*
MIT License

Copyright (c) 2018 Alex Ssembly

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef SUBNET_H
#define SUBNET_H

#include <math.h>
#include <bitset>
#include <sstream>
#include <string>

namespace subnet {
struct Network {
  std::uint32_t NetId;
  std::uint32_t FirstAddr;
  std::uint32_t LastAddr;
  std::uint32_t BroadcastAddr;
  std::uint32_t Mask;
  int ShortMask;
  size_t MaxHosts;
};

std::uint32_t calcMask(size_t hosts) {
  std::uint32_t a = std::pow(2, std::ceil(std::log2(hosts + 2)));
  std::uint32_t b = UINT32_MAX;

  return ~(b & (a - 1));
}

Network calculate(const std::uint32_t& ip, size_t hosts) {
  Network net;
  auto mask = calcMask(hosts);
  auto addr = ip & mask;
  auto hostBits = std::ceil(std::log2(hosts + 2));

  net.NetId = addr;
  net.FirstAddr = addr + 1;
  net.BroadcastAddr =
      ((ip & mask) + static_cast<uint32_t>(std::pow(2, hostBits))) - 1;
  net.LastAddr = net.BroadcastAddr - 1;
  net.Mask = mask;
  net.MaxHosts = std::pow(2, hostBits) - 2;
  net.ShortMask = 32 - hostBits;

  return net;
}

std::uint32_t mask(const std::uint32_t& ipaddr, const std::uint32_t& mask) {
  return ipaddr & mask;
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
  auto count = 0;
  auto pot = 24;
  while (getline(iss, token, '.')) {
    count++;
    ret = ret == UINT32_MAX ? 0 : ret;
    std::uint32_t val = std::stoi(token);

    if (val > 255 || val < 0) {
      ret = UINT32_MAX;
      return ret;
    }

    val = val << pot;
    ret |= val;
    pot -= 8;
  }

  return count == 4 ? ret : UINT32_MAX;
}

bool validSubnet(const std::uint32_t& mask, size_t netcount) {
  return ~mask + 1 >= netcount + (2 * netcount);
}

};  // namespace subnet

#endif
