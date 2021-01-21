//
//  ViewControls.hpp
//  thesis
//
//  Created by Gary Rendle on 20/08/2019.
//

#ifndef ViewControls_h
#define ViewControls_h


#include <GLFW/glfw3.h>

#include <stddef.h>

namespace grt {
namespace gl{
    
    class ViewControls {
        
        float panSpeed = 0.001f;
        float mouseSpeed = 0.005f;

        //current positions
        float slide = 0.f;
        float rise = 0.f;
        float zoom = 5.f;
        float mouseX = 0.f;
        float mouseY = 0.f;
        double lastXPos, lastYPos;
        
        // Initial Field of View
        float initialFoV = 25.0f;

        glm::fvec3 centre;
        
        //calculated matrices
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::fmat4 m_view_transform = glm::fmat4{1.0};

        glm::mat4 forcedView;
        bool useForcedView = false;
        
        int levelChange = 0;
        bool stepChange = false;
        bool colourChange = false;
        float scaleFactor = 1.0;
        int timeStepChange = 0;
        bool reportNodeInfo = false;
        bool togglePointer = false;
        
        glm::ivec3 selectorChange = glm::ivec3(0);
        
        static ViewControls* instance;
        
        ViewControls() {}
        
    public:
        
        static ViewControls* getInstance() {
            if (!instance) {
                instance = new ViewControls();
            }
            return instance;
        }
        
        ~ViewControls() {}

        void setCentre(glm::vec3 _centre);
        
        void computeMatricesFromInputs(GLFWwindow* window);
        void computeMatricesFromInputs(GLFWwindow* window, const bool freezeMouse);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        
        void printViewMat();
        
        glm::vec3 getWorldSpacePointFromMousePointer(GLFWwindow* window, const GLuint WIDTH, const GLuint HEIGHT);

        glm::ivec3 getSelectorChange();
        bool shouldReportNodeInfo();
        
        float getScaleFactor();
        int shouldChangeLevel();
        bool shouldChangeStep();
        bool shouldChangeColouring();


        bool shouldTogglePointer();
        

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mods);
        
        int getTimeStepChange();
    };
    
}
}


#endif /* ViewControls_h */
