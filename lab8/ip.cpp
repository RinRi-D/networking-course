#include <iostream>

void printIp(const std::string &name, uint32_t ip) {
	std::cout << name << ": " << (ip >> 24) << '.'
		<< ((ip << 8) >> 24) << '.'
		<< ((ip << 16) >> 24) << '.'
		<< ((ip << 24) >> 24) << '\n';
}

int main() {
	uint32_t a, b, c, d, cidr;
	char temp;
	std::cin >> a >> temp >> b >> temp >> c >> temp >> d >> temp >> cidr;

	uint32_t ip, mask = 0;
	ip = (a << 24) + (b << 16) + (c << 8) + d;
	for (uint32_t i = 0; i < cidr; ++i)
		mask += (1u << (31u-i));
	
	uint32_t neta = ip & mask, broadcast = neta;

	for (uint32_t i = 0; i < 32u-cidr ; ++i) {
		broadcast += (1 << i);
	}
	
	uint32_t minhost= neta+1, maxhost = broadcast-1;

	printIp("IP ADDRESS", ip);
	printIp("NETWORK ADDRESS", neta);
	printIp("BROADCAST ADDRESS", broadcast);
	printIp("SUBNET MASK", mask);
	printIp("MIN HOST ADDRESS", minhost);
	printIp("MAX HOST ADDRESS", maxhost);
	std::cout << "NUMBER OF HOSTS: " << (maxhost-minhost+1) << '\n';
}
