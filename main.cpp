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
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
#include "subnet.h"

void printNetwork(const std::vector<subnet::Network>& vec);

int main(int argc, char** argv) {
  size_t netcount = 1;
  bool help = false;
  bool err = false;
  std::string ipstr, mask;

  if (argc == 1) help = true;

  for (int i = 1; i < argc; ++i) {
    if (std::strcmp("-n", argv[i]) == 0) {
      if (argc <= i + 1) {
        err = true;
        break;
      }

      auto val = std::atol(argv[i + 1]);

      if (val <= 0 || val > std::pow(2, 32)) {
        err = true;
        break;
      }

      netcount = static_cast<size_t>(val);
    }

    if (std::strcmp("-h", argv[i]) == 0) {
      help = true;
      break;
    }

    if (std::strcmp("-ip", argv[i]) == 0) {
      if (argc <= i + 1) {
        err = true;
        break;
      }

      ipstr = argv[i + 1];

      try {
        auto test = subnet::toValue(ipstr);

        if (test == UINT32_MAX) {
          err = true;
          break;
        }

      } catch (std::invalid_argument& c) {
        err = true;
        break;
      }
    }

    if (std::strcmp("-m", argv[i]) == 0) {
      if (argc <= i + 1) {
        err = true;
        break;
      }

      mask = argv[i + 1];

      try {
        auto test = subnet::toValue(mask);

        if (test == UINT32_MAX) {
          err = true;
          break;
        }

      } catch (std::invalid_argument& c) {
        err = true;
        break;
      }
    }
  }

  if (err) {
    std::cout << "Invalid input" << std::endl;
    return EXIT_FAILURE;
  }

  if (help) {
    std::cout << "usage: subcalc [-h] [-ip {ipaddr}] [-n {count}] [-m {mask}]"
              << std::endl
              << std::endl;
    std::cout << "Very simple subnet calculator" << std::endl;
    std::cout << "  -h\tShow this help message and exit" << std::endl;
    std::cout << "  -n\tSet quantity of subnets {count}[10]" << std::endl;
    std::cout << "  -ip\tSet ip address of host network {ipaddr}[127.0.0.1]"
              << std::endl;
    std::cout << "  -m\tSet subnet mask for host network {mask}[255.255.255.0]"
              << std::endl;
    return EXIT_SUCCESS;
  }

  if (ipstr == "") {
    std::cout << "Missing ip address" << std::endl;
    return EXIT_FAILURE;
  }

  if (mask != "") {
    auto maskval = subnet::toValue(mask);
    if(!subnet::validSubnet(maskval, netcount)) {
      std::cout<<"Invalid net count for given mask"<<std::endl;
      return EXIT_FAILURE;
    }

    ipstr = subnet::toIP(
        subnet::mask(subnet::toValue(ipstr), subnet::toValue(mask)));
    std::cout << ipstr << std::endl;
  }

  std::vector<size_t> maxhost_in_sub;
  for (size_t i = 0; i < netcount; ++i) {
    std::string input;
    std::cout << "Subnet " << i + 1 << " max. hosts: ";
    std::getline(std::cin, input);
    maxhost_in_sub.push_back(std::stol(input));
  }

  std::vector<subnet::Network> vec;
  std::sort(maxhost_in_sub.rbegin(), maxhost_in_sub.rend());
  bool first = true;
  subnet::Network r;
  for (auto e : maxhost_in_sub) {
    if (first) {
      r = subnet::calculate(subnet::toValue(ipstr), e);
      first = false;
    } else
      r = subnet::calculate(r.BroadcastAddr + 1, e);

    vec.push_back(r);
  }

  printNetwork(vec);


  return EXIT_SUCCESS;
}

template <typename T>
void printElement(T t, const int& width) {
  std::cout << "| " << std::left << std::setw(width) << std::setfill(' ') << t;
}

void printNetwork(const std::vector<subnet::Network>& vec) {
  const int fillw = 100;
  std::cout << "+";
  std::cout << std::left << std::setw(fillw) << std::setfill('-') << "";
  std::cout << "+" << std::endl;
  printElement("NetID/Mask", 18);
  printElement("FirstAddr", 15);
  printElement("LastAddr", 15);
  printElement("Broadcast", 15);
  printElement("NetMask", 15);
  printElement("MaxHosts", 11);
  std::cout << "|" << std::endl;
  std::cout << "+";
  std::cout << std::left << std::setw(fillw) << std::setfill('-') << "";
  std::cout << "+" << std::endl;

  for (const auto& rn : vec) {
    printElement(subnet::toIP(rn.NetId) + "/" + std::to_string(rn.ShortMask),
                 18);
    printElement(subnet::toIP(rn.FirstAddr), 15);
    printElement(subnet::toIP(rn.LastAddr), 15);
    printElement(subnet::toIP(rn.BroadcastAddr), 15);
    printElement(subnet::toIP(rn.Mask), 15);
    std::cout << "| " << std::right << std::setw(10) << std::setfill(' ')
              << rn.MaxHosts;
    std::cout << " |" << std::endl;
  }

  std::cout << "+";
  std::cout << std::left << std::setw(fillw) << std::setfill('-') << "";
  std::cout << "+" << std::endl;
}
