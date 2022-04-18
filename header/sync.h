#ifndef SYNC_H
#define SYNC_H

#define GM_SS_SYNC_NSEMS 4
#define GM_SEM_RNG 0
#define GM_SEM_SYNC_BARRIER 1
#define GM_SEM_END_GAME 2
#define GM_SEM_GET_ID 3

#define GM_NO_DELAY -1

#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

class Settings;
class Sync {
private:
	inline static Sync * instance = nullptr;
	std::vector<std::condition_variable> ss_sync_cond;
	std::vector<std::mutex>              ss_sync_mutx;
	std::vector<std::mutex>              ss_board_row;
	std::vector<std::mutex>              ss_board_col;
public:
	static void Initialize(const Settings &);
	static void Dispose(void);

	static void LockBoard(int row, int col);
	static void UnlockBoard(int row, int col);
	static void LockSync(int nsem);
	static void UnlockSync(int nsem);
	static bool Wait(int nsem, long time);
	static bool Wait(int nsem, long time, std::function<bool (void)> pred);
	static void Notify(int nsem);
	static void Broadcast(int nsem);
};

#endif

