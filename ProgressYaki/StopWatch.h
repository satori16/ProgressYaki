#pragma once
#include <chrono>

class StopWatch {
	std::chrono::high_resolution_clock::time_point S;
	std::chrono::high_resolution_clock::time_point E;
public:

	StopWatch() {
		Start();
	}

	bool Start() {
		S = std::chrono::high_resolution_clock::now();
		return true;
	}
	bool Restart() {
		return Start();
	}
	bool Stop() {
		E = std::chrono::high_resolution_clock::now();
		return true;
	}
	template<class T = std::chrono::milliseconds>
	T Result() {//not Real time Count.
		return std::chrono::duration_cast<T>(E - S);
	}
	template<class T = std::chrono::milliseconds>
	T Ellipse() {//Real Time Count
		return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - S);
	}
};