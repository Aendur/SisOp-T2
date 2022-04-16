#ifndef UIEMPTY_H
#define UIEMPTY_H

#include "UI/UI.h"

class UIEmpty : public UI {
public:
	inline UIEmpty(void) { }
	inline void Initialize(const Board*) { }
	inline void Dispose(void) { }
	inline void Refresh(int) { }
	inline void Clear(void) { }
	inline bool Stop(void) { return false; }
};

#endif

