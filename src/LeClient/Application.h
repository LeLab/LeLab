#ifndef APPLICATION_H
#define APPLICATION_H

#include <LeEngine/IApplication.h>

class Application : public IApplication
{
public:
	virtual ~Application() { }
    void Run(int argc, char* argv[]);
};

#endif // APPLICATION_H
