#pragma once

class Component {
public:

	Component(class Actor* owner, int updateOrder = 100) {
		mUpdateOrder = updateOrder;
		mOwner = owner;
	}

	virtual ~Component() {}

	virtual void Update(float delta) {}

	int GetUpdateOrder() const {
		return mUpdateOrder;
	}

protected:
	class Actor* mOwner;
	int mUpdateOrder;
};