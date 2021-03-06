#ifndef POLYGONTEST_H
#define POLYGONTEST_H

#include <string>
#include <map>
#include <vector>
#include <LeEngine/ITask.h>
#include <LeEngine/Shapes.h>

#include <GL/glew.h>
#include <GL/glfw.h>

class PolygonTest : public ITask
{
public:
    PolygonTest() : ITask("PolygonTest") { }
    bool Start();
    void Update();
    void Render() const;
    void Stop();

    //! Called when the window is resized
    static void GLFWCALL WindowResizeHandler(int width, int height);

private:
    std::vector<Polygon<Vertex>> _polygons;
};

#endif // POLYGONTEST_H
