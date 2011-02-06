#ifndef __OXEN_CONFIG_H__
#define __OXEN_CONFIG_H__

#include <string>

namespace oxen {

class Config {
	public:
		Config() : directory_("/tmp"), maxMemory_(16), mode_(0), hours_(48), ratio_(1.5) { }
		~Config() { }

		void directory(std::string directory) {
			directory_ = directory;
		}

		std::string directory() { return directory_; }
		int maxMemory() { return maxMemory_; }
		int getMode() { return mode_; }
		int getHours() { return hours_; }
		double getRatio() { return ratio_; }

		void setMode(int mode) { mode_ = mode; }
		void setRatio(double ratio) { ratio_ = ratio; }
		void setHours(int hours) { hours_ = hours; }
		void setMaxMemory(int maxMemory) { maxMemory_ = maxMemory; }

	private:
		std::string directory_;
		int maxMemory_;
		int mode_;
		int hours_;
		double ratio_;
};

}

#endif /* __OXEN_CONFIG_H__ */

