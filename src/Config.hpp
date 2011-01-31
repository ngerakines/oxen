#ifndef __OXEN_CONFIG_H__
#define __OXEN_CONFIG_H__

#include <string>

namespace oxen {

class Config {
	public:
		Config() : directory_("/tmp") { }
		~Config() { }

		void directory(std::string directory) {
			directory_ = directory;
		}
		std::string directory() {
			return directory_;
		}

	private:
		std::string directory_;
};

}

#endif /* __OXEN_CONFIG_H__ */

