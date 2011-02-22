#ifndef __OXEN_UTILS_H__
#define __OXEN_UTILS_H__

#define SLEEP(N) boost::this_thread::sleep(boost::posix_time::seconds(N));

#define LOG_INFO(X) cout << X;
#define LOG_ERROR(X) cerr << X;

#endif /* __OXEN_UTILS_H__ */

