#pragma once

class Scene {
public:
	Scene() {

	}

	virtual int UpdateLogic() = 0;
	virtual int DrawCall() = 0;


};
