#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>


using namespace std;


struct Job {
	string type;
	int data;
};


typedef function<void(Job)> Handler;


map<string/*jobType*/, list<Handler>> handlers;


void logJob(const Job& job) {
	static int count = 0;
	cout << "[" << count++ << "] Received " << job.type << ", data: " << job.data << endl;
}


void doAuth(const Job& job) {
	cout << "Done auth " << job.type << endl;
}


void doPayment(const Job& job) {
	cout << "Done payment " << job.type << endl;
}


void addJobHandler(const string& jobType, const Handler& h) {
	handlers[jobType].push_back(h);
}


int main() {
	srand(time(NULL));

	// handler 를 사전에 등록한다.
	// addJobHandler("auth", logJob);
	addJobHandler("auth", doAuth);

	// addJobHandler("payment", logJob);
	addJobHandler("payment", doPayment);

	// 이제 랜덤하게 job 이 들어왔다고 가정한다.
	const int NUM_JOBS = 10;
	queue<Job> jobs;
	for (int i = 0; i < NUM_JOBS; ++i) {
		string type = rand() % 2 == 0 ? "auth" : "payment";
		int data = rand() % 1000;
		jobs.push({ type, data });
	}

	// job queue 를 처리한다.
	while (jobs.empty() == false) {
		const Job& job = jobs.front();
		for (const Handler& h : handlers[job.type]) {
			h(job);
		}
		jobs.pop();
	}
}