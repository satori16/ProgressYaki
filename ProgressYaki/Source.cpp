//**********************************************//
//*                                            *//
//*          Hyper Cosmic System               *//
//*                                            *//
//*                  UI                        *//
//*                                            *//
//*           copyrigh 2019/1/XX              *//
//*               Yakitori                     *//
//**********************************************//
#include <iostream>
#include <string>
#include <cstdint>
#include <random>
#include <algorithm>
#include <thread>

#include "StopWatch.h"
#include "WindowsMidiDevice.h"


std::int64_t Rand(std::int64_t min, std::int64_t max) {

	if (min > max) std::swap(min, max);

	std::random_device rd;
	std::uniform_int_distribution<std::uint64_t> ui(min, max);

	return ui(rd);

}

bool WriteLine(const std::string& S, const std::chrono::nanoseconds& ns,const std::uint64_t& cut) {
	std::string c = "/-\\-";
	std::string v;
	StopWatch SW;
	MIDIDevice MD;

	//auto MOC = EnumMidiDevice();

	MD.Initalize(MIDIMAPPER);
	MD.SendGMReset();

	auto Sender = MD.GetMidiSender();

	Sender->SendEvent(0xc3, 82, 0);
	std::chrono::nanoseconds L(ns / cut);
	
	for (std::uint64_t i = 0; i < cut; i++) {
		std::uint64_t j = 0;
		for (SW.Start(); std::chrono::duration_cast<std::chrono::nanoseconds>(SW.Ellipse<std::chrono::nanoseconds>()) < L;) {

			std::cout << S << v << c[j % 4] << ' ' << '\r';	
			std::this_thread::sleep_for(std::chrono::milliseconds(Rand(1,8)));
			j++;
		}
		v += '.';
		Sender->SendEvent(0x9A, 80, 127);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		Sender->SendEvent(0x8A, 80, 0);
	}
	std::cout << S << v << ' ' << "\r";
	std::this_thread::sleep_for(std::chrono::nanoseconds(std::min<std::chrono::nanoseconds>(L * 2, std::chrono::nanoseconds(1000 * 1000 * 1000))));
	std::cout << S << v << ' ' << "Done." << std::endl;
	Sender->SendEvent(0x9A, 81, 127);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	Sender->SendEvent(0x8A, 81, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(16));

	return true;
}

bool Rolling(const std::string& S,const std::string& M, const std::chrono::nanoseconds ns) {
	StopWatch SW;
	std::uint64_t j = 0;

	for (SW.Start(); std::chrono::duration_cast<std::chrono::nanoseconds>(SW.Ellipse<std::chrono::nanoseconds>()) < ns;) {
		std::cout <<S <<M[j % M.size()] << '\r';	
		std::this_thread::sleep_for(std::chrono::milliseconds(Rand(1,8)));
		j++;
	}
	std::cout << S;
	return true;
}
bool Rolling2(const std::string& S, const std::chrono::nanoseconds ns) {
	StopWatch SW;
	std::uint64_t j = 0;

	for (SW.Start(); std::chrono::duration_cast<std::chrono::nanoseconds>(SW.Ellipse<std::chrono::nanoseconds>()) < ns;) {
		std::cout << S[j % S.size()] << '\r';	
		std::this_thread::sleep_for(std::chrono::milliseconds(Rand(1,8)));
		j++;
	}

	return true;
}
bool StartHoge() {
	Rolling2("/-\\-", std::chrono::milliseconds(1500));
	std::cout<<"\r  " << std::endl;
	//std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	std::cout << "******************* Hello World!! ********************" << std::endl << std::endl << std::endl;;
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	std::cout << "Welcome to *Hyper Cosmic SYSTEM*" << std::endl << std::endl << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	//std::cout << "Start booting..." << std::endl;;
	std::cout << std::endl;
	std::chrono::nanoseconds v(Rand(3000, 10000) * 1000 * 1000 / 2);
	Rolling("Start booting...","/-\\-", v);

	std::cout << "\r " << std::endl;

	v = std::chrono::nanoseconds(Rand(3000, 4000) * 1000 * 1000);
	WriteLine("Start BasicProsess... ", v, Rand(8, 16));
	std::cout << std::endl;
	v = std::chrono::nanoseconds(Rand(1000, 3000) * 1000 * 1000);
	Rolling("*Checking something!*", "/-\\-", v);
	std::cout << std::endl;
	v = std::chrono::nanoseconds(Rand(800, 1000) * 1000 * 1000);
	WriteLine("CPU is?", v, Rand(2, 16));
	v = std::chrono::nanoseconds(Rand(2500, 5000) * 1000 * 1000);
	WriteLine("GPU is?", v, Rand(8, 16));
	v = std::chrono::nanoseconds(Rand(1000, 3000) * 1000 * 1000);
	WriteLine("Sound is?", v, Rand(3, 5));
	v = std::chrono::nanoseconds(Rand(1000, 3000) * 1000 * 1000);
	WriteLine("Network is?", v, Rand(4, 8));
	std::cout << std::endl;

	v = std::chrono::nanoseconds(Rand(10000, 15000) * 1000 * 1000);
	Rolling("Checking Network Connect...", "/-\\-", v);
	std::cout << std::endl;
	v = std::chrono::nanoseconds(Rand(8000, 18000) * 1000 * 1000);
	Rolling("booting ExtentionMode...", "/-\\-", v);

	std::cout << std::endl << std::endl << std::endl;
	std::cout << "Done!!" << std::endl << std::endl;
	v = std::chrono::nanoseconds(Rand(3000, 5000) * 1000 * 1000);	
	Rolling("!!Final Checking...","!-", v);
	std::cout <<"\r"<<"!!Final Checking...!"<< std::endl<< std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "Open Door...!!" << std::endl << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return true;
}

int main() {
	StartHoge();

	return 0;
}