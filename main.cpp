#include <iostream>
#include "subnet.h"
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

template<typename T> void printElement(T t, const int& width) {
  std::cout << "| "<< std::left << std::setw(width) << std::setfill(' ') << t;
}

void printNet(const std::vector<subnet::ValueNetwork>& vec) {
  std::cout<<"+";
  std::cout<< std::left << std::setw(84) << std::setfill('-') << "";
  std::cout<<"+"<<std::endl;
  printElement("NetID", 15);
  printElement("FirstAddr", 15);
  printElement("Broadcast", 15);
  printElement("NetMask", 15);
  printElement("MaxHosts", 15);
  std::cout<<"|"<<std::endl;
  std::cout<<"+";
  std::cout<< std::left << std::setw(84) << std::setfill('-') << "";
  std::cout<<"+"<<std::endl;

  for(const auto& rn : vec) {
    printElement(subnet::toIP(rn.NetId), 15);
    printElement(subnet::toIP(rn.FirstAddr), 15);
    printElement(subnet::toIP(rn.BroadcastAddr), 15);
    printElement(subnet::toIP(rn.Mask), 15);
    printElement(rn.MaxHosts, 15);
    std::cout<<"|"<<std::endl;
  }

  std::cout<<"+";
  std::cout<< std::left << std::setw(84) << std::setfill('-') << "";
  std::cout<<"+"<<std::endl;

}

int main(int argc, char **argv) {
  long int netcount = 1;
  bool help = false;
  std::string ipstr;

  for(int i = 1; i < argc; ++i) {
    if(std::strcmp("-n", argv[i]) == 0)
      netcount = atol(argv[i+1]);

    if(std::strcmp("-h", argv[i]) == 0) {
      help = true;
      break;
    }

    if(std::strcmp("-ip", argv[i]) == 0)
      ipstr = argv[i+1];

    if(std::strcmp("-m", argv[i]) == 0);
  }

  if(help) {
    std::cout<<"usage: subcalc [-h] [-ip {ipaddr}] [-n {count}] [-m {mask}]"<<std::endl<<std::endl;
    std::cout<<"Very simple subnet calculator"<<std::endl;
    std::cout<<"  -h\tShow this help message and exit"<<std::endl;
    std::cout<<"  -n\tSet quantity of subnets count[10]"<<std::endl;
    std::cout<<"  -ip\tSet ip address of host network ipaddr[127.0.0.1]"<<std::endl;
    std::cout<<"  -m\t[Set subnet mask of host network mask[255.255.255.0] "<<std::endl;
    return 0;
  }

  std::vector<size_t> maxhost_in_sub;
  for(int i = 0; i < netcount; ++i) {
    std::string input;
    std::cout<<"Subnet "<<i+1<<" max. hosts: ";
    std::getline(std::cin, input);
    maxhost_in_sub.push_back(std::stol(input));
  }


  std::vector<subnet::ValueNetwork> vec;
  std::sort(maxhost_in_sub.rbegin(), maxhost_in_sub.rend());
  bool first = true;
  subnet::ValueNetwork r;
  for(auto e : maxhost_in_sub) {
    if(first) {
      r = subnet::calculateVal(subnet::toValue(ipstr), e);
      first = false;
    }
    else
      r = subnet::calculateVal(r.BroadcastAddr + 1, e);

    vec.push_back(r);
  }

  printNet(vec);


  return 0;
}
