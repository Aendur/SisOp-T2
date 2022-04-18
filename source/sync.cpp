#include "sync.h"
#include "settings/settings.h"

#include <chrono>

void Sync::Initialize(const Settings & settings) {
	Sync::instance = new Sync();
	instance->ss_sync_mutx = std::vector<std::mutex>(GM_SS_SYNC_NSEMS);
	instance->ss_sync_cond = std::vector<std::condition_variable>(GM_SS_SYNC_NSEMS);
	instance->ss_board_row = std::vector<std::mutex>(settings.grid_height);
	instance->ss_board_col = std::vector<std::mutex>(settings.grid_width);
}

void Sync::Dispose(void) {
	delete Sync::instance;
	Sync::instance = nullptr;
}

void Sync::LockBoard(int row, int col) {
	instance->ss_board_row[row].lock();
	instance->ss_board_col[col].lock();
}

void Sync::UnlockBoard(int row, int col) {
	instance->ss_board_col[col].unlock();
	instance->ss_board_row[row].unlock();
}

void Sync::LockSync(int nsem) {
	instance->ss_sync_mutx[nsem].lock();
}
void Sync::UnlockSync(int nsem) {
	instance->ss_sync_mutx[nsem].unlock();
}

bool Sync::Wait(int nsem, long time) {
	std::unique_lock lock(instance->ss_sync_mutx[nsem]);
	if (time <= 0) {
		instance->ss_sync_cond[nsem].wait(lock);
		return true;
	} else {
		auto val = instance->ss_sync_cond[nsem].wait_for(lock, std::chrono::duration<long, std::micro>(time));
		switch(val) {
			case std::cv_status::timeout: return false;
			case std::cv_status::no_timeout: return true;
			default: return false;
		}
	}
}

bool Sync::Wait(int nsem, long time, std::function<bool(void)> pred) {
	std::unique_lock lock(instance->ss_sync_mutx[nsem]);
	if (time <= 0) {
		instance->ss_sync_cond[nsem].wait(lock, pred);
		return true;
	} else {
		return instance->ss_sync_cond[nsem].wait_for(lock, std::chrono::duration<long, std::micro>(time), pred);
	}
}

void Sync::Notify(int nsem) {
	std::lock_guard guard(instance->ss_sync_mutx[nsem]);
	instance->ss_sync_cond[nsem].notify_one();
}

void Sync::Broadcast(int nsem) {
	std::lock_guard guard(instance->ss_sync_mutx[nsem]);
	instance->ss_sync_cond[nsem].notify_all();
}
