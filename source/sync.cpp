#include "sync.h"
#include "settings/settings.h"

#include <chrono>

void Sync::Initialize(const Settings & settings) {
	ss_sync_mutx = std::vector<std::mutex>(GM_SS_SYNC_NSEMS);
	ss_sync_cond = std::vector<std::condition_variable>(GM_SS_SYNC_NSEMS);
	ss_board_row = std::vector<std::mutex>(settings.grid_height);
	ss_board_col = std::vector<std::mutex>(settings.grid_width);
}

void Sync::LockBoard(int row, int col) {
	ss_board_row[row].lock();
	ss_board_col[col].lock();
}

void Sync::UnlockBoard(int row, int col) {
	ss_board_col[col].unlock();
	ss_board_row[row].unlock();
}

void Sync::LockSync(int nsem) {
	ss_sync_mutx[nsem].lock();
}
void Sync::UnlockSync(int nsem) {
	ss_sync_mutx[nsem].unlock();
}

bool Sync::Wait(int nsem, long time) {
	std::unique_lock lock(ss_sync_mutx[nsem]);
	if (time <= 0) {
		ss_sync_cond[nsem].wait(lock);
		return true;
	} else {
		auto val = ss_sync_cond[nsem].wait_for(lock, std::chrono::duration<long, std::micro>(time));
		switch(val) {
			case std::cv_status::timeout: return false;
			case std::cv_status::no_timeout: return true;
			default: return false;
		}
	}
}

void Sync::Notify(int nsem) {
	std::lock_guard guard(ss_sync_mutx[nsem]);
	ss_sync_cond[nsem].notify_one();
}

void Sync::Broadcast(int nsem) {
	std::lock_guard guard(ss_sync_mutx[nsem]);
	ss_sync_cond[nsem].notify_all();
}
