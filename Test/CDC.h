#pragma once

#include "framework.h"

// 출력에 필요한 정보를 가지는 데이터 구조체 , 좌표 ,색 ,굵기등 출력에 필요한 모든 정보를 담고있다

class CDC
{
public:
	void Render(HDC hdc);
};

