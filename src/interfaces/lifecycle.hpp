#pragma once

#include "../nodetypes/node.hpp"
#include <stardustxr/util/threadsafelist.hpp>

namespace StardustXRServer {

typedef struct {
	Client *client;
	std::string nodePath;
	std::string methodName;
} LifeCycleUpdateMethod;

class LifeCycleInterface : public Node {
public:
	LifeCycleInterface(Client *client);
	virtual ~LifeCycleInterface() {}

	static void sendLogicStepSignals();
	void handleMessengerDeletion(uint sessionID);

	std::vector<uint8_t> subscribeLogicStep(uint sessionID, flexbuffers::Reference data, bool returnValue);

protected:
	static void logicStepSignal(uint32_t index, LifeCycleUpdateMethod &updateMethod);
	static ThreadSafeList<LifeCycleUpdateMethod> lifeCycleUpdateMethodList;

	static double prevFrameTime;
	static double frameTime;
	static double delta;
};

} // namespace StardustXRServer
